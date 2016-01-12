////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Window/GlContext.hpp>
#include <cpp3ds/System/ThreadLocalPtr.hpp>
#include <cpp3ds/System/Mutex.hpp>
#include <cpp3ds/System/Lock.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <set>
#include <cstdlib>
#include <cstring>

typedef cpp3ds::priv::GlContext ContextType;

namespace
{
    // AMD drivers have issues with internal synchronization
    // We need to make sure that no operating system context
    // or pixel format operations are performed simultaneously
    cpp3ds::Mutex mutex;

    // This per-thread variable holds the current context for each thread
    cpp3ds::ThreadLocalPtr<cpp3ds::priv::GlContext> currentContext(NULL);

    // The hidden, inactive context that will be shared with all other contexts
    ContextType* sharedContext = NULL;

    // Internal contexts
    cpp3ds::ThreadLocalPtr<cpp3ds::priv::GlContext> internalContext(NULL);
    std::set<cpp3ds::priv::GlContext*> internalContexts;
    cpp3ds::Mutex internalContextsMutex;

    // Check if the internal context of the current thread is valid
    bool hasInternalContext()
    {
        // The internal context can be null...
        if (!internalContext)
            return false;

        // ... or non-null but deleted from the list of internal contexts
        cpp3ds::Lock lock(internalContextsMutex);
        return internalContexts.find(internalContext) != internalContexts.end();
    }

    // Retrieve the internal context for the current thread
    cpp3ds::priv::GlContext* getInternalContext()
    {
        if (!hasInternalContext())
        {
            internalContext = cpp3ds::priv::GlContext::create();
            cpp3ds::Lock lock(internalContextsMutex);
            internalContexts.insert(internalContext);
        }

        return internalContext;
    }
}


namespace cpp3ds
{
namespace priv
{
////////////////////////////////////////////////////////////
void GlContext::globalInit()
{
    Lock lock(mutex);

    // Create the shared context
    sharedContext = new ContextType(NULL);
    sharedContext->initialize();

    // This call makes sure that:
    // - the shared context is inactive (it must never be)
    // - another valid context is activated in the current thread
    sharedContext->setActive(false);
}


////////////////////////////////////////////////////////////
void GlContext::globalCleanup()
{
    Lock lock(mutex);

    // Destroy the shared context
    delete sharedContext;
    sharedContext = NULL;

    // Destroy the internal contexts
    Lock internalContextsLock(internalContextsMutex);
    for (std::set<GlContext*>::iterator it = internalContexts.begin(); it != internalContexts.end(); ++it)
        delete *it;
    internalContexts.clear();
}


////////////////////////////////////////////////////////////
void GlContext::ensureContext()
{
    // If there's no active context on the current thread, activate an internal one
    if (!currentContext)
        getInternalContext()->setActive(true);
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create()
{
    Lock lock(mutex);

    // Create the context
    GlContext* context = new ContextType(sharedContext);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    ensureContext();

    Lock lock(mutex);

    // Create the context
    GlContext* context = new ContextType(sharedContext, settings, width, height);
    context->initialize();
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    // Deactivate the context before killing it, unless we're inside Cleanup()
    if (sharedContext)
        setActive(false);

#ifndef EMULATION
	C3D_DestroyContext(m_context);
#endif
}


////////////////////////////////////////////////////////////
bool GlContext::makeCurrent()
{
#ifdef EMULATION
    return true;
#else
    return C3D_MakeCurrent(m_context);
#endif
}


////////////////////////////////////////////////////////////
void GlContext::setVerticalSyncEnabled(bool enabled)
{
	// TODO: add vsync function to gl3ds
}


////////////////////////////////////////////////////////////
void GlContext::display() {
#ifndef EMULATION
    gl3ds_flushContext(m_context);
#endif
}


////////////////////////////////////////////////////////////
const ContextSettings& GlContext::getSettings() const
{
    return m_settings;
}


////////////////////////////////////////////////////////////
GLuint GlContext::getHandle() const
{
	return m_context;
}


////////////////////////////////////////////////////////////
bool GlContext::setActive(bool active)
{
    if (active)
    {
        if (this != currentContext)
        {
            Lock lock(mutex);

            // Activate the context
            if (makeCurrent())
            {
                // Set it as the new current context for this thread
                currentContext = this;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            // This context is already the active one on this thread, don't do anything
            return true;
        }
    }
    else
    {
        if (this == currentContext)
        {
            // To deactivate the context, we actually activate another one so that we make
            // sure that there is always an active context for subsequent graphics operations
            return getInternalContext()->setActive(true);
        }
        else
        {
            // This context is not the active one on this thread, don't do anything
            return true;
        }
    }
}


////////////////////////////////////////////////////////////
GlContext::GlContext(GlContext* shared)
{
	m_settings = ContextSettings();
#ifndef EMULATION
	C3D_CreateContext(m_context, C3D_DEFAULT_CMDBUF_SIZE);
#endif
}

GlContext::GlContext(GlContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height)
{
	m_settings = settings;
#ifndef EMULATION
    C3D_CreateContext(m_context, C3D_DEFAULT_CMDBUF_SIZE);
#endif
}


////////////////////////////////////////////////////////////
int GlContext::evaluateFormat(unsigned int bitsPerPixel, const ContextSettings& settings, int colorBits, int depthBits, int stencilBits, int antialiasing, bool accelerated)
{
    int colorDiff        = static_cast<int>(bitsPerPixel)               - colorBits;
    int depthDiff        = static_cast<int>(settings.depthBits)         - depthBits;
    int stencilDiff      = static_cast<int>(settings.stencilBits)       - stencilBits;
    int antialiasingDiff = static_cast<int>(settings.antialiasingLevel) - antialiasing;

    // Weight sub-scores so that better settings don't score equally as bad as worse settings
    colorDiff        *= ((colorDiff        > 0) ? 100000 : 1);
    depthDiff        *= ((depthDiff        > 0) ? 100000 : 1);
    stencilDiff      *= ((stencilDiff      > 0) ? 100000 : 1);
    antialiasingDiff *= ((antialiasingDiff > 0) ? 100000 : 1);

    // Aggregate the scores
    int score = std::abs(colorDiff) + std::abs(depthDiff) + std::abs(stencilDiff) + std::abs(antialiasingDiff);

    // Make sure we prefer hardware acceleration over features
    if (!accelerated)
        score += 100000000;

    return score;
}


////////////////////////////////////////////////////////////
void GlContext::initialize()
{
    // Activate the context
    setActive(true);

    m_settings.attributeFlags = ContextSettings::Default;
/*
	// Retrieve the context flags
	int flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		m_settings.attributeFlags |= ContextSettings::Debug;

	// Retrieve the context profile
	int profile = 0;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

	if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
		m_settings.attributeFlags |= ContextSettings::Core;
*/
    // Enable antialiasing if needed
//    if (m_settings.antialiasingLevel > 0)
//        glEnable(GL_MULTISAMPLE);
}


////////////////////////////////////////////////////////////
void GlContext::checkSettings(const ContextSettings& requestedSettings)
{
    // Perform checks to inform the user if they are getting a context they might not have expected
}

} // namespace priv

} // namespace cpp3ds
