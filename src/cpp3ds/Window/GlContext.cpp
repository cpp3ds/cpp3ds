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
#include "../Graphics/CitroHelpers.hpp"

typedef cpp3ds::priv::GlContext ContextType;

namespace
{
    cpp3ds::priv::GlContext* currentContext(NULL);
}


namespace cpp3ds
{
namespace priv
{
////////////////////////////////////////////////////////////
void GlContext::globalInit()
{

}


////////////////////////////////////////////////////////////
void GlContext::globalCleanup()
{

}


////////////////////////////////////////////////////////////
void GlContext::ensureContext()
{
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create()
{
    // Create the context
    GlContext* context = new ContextType(NULL);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    // Create the context
    GlContext* context = new ContextType(NULL, settings, width, height);
    context->initialize();
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
}


////////////////////////////////////////////////////////////
bool GlContext::makeCurrent()
{
    return true;
}


////////////////////////////////////////////////////////////
void GlContext::setVerticalSyncEnabled(bool enabled)
{
}


////////////////////////////////////////////////////////////
void GlContext::display()
{
}


////////////////////////////////////////////////////////////
const ContextSettings& GlContext::getSettings() const
{
    return m_settings;
}


////////////////////////////////////////////////////////////
int GlContext::getHandle() const
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
        currentContext = nullptr;
        return true;
    }
}


////////////////////////////////////////////////////////////
GlContext::GlContext(GlContext* shared)
{
	m_settings = ContextSettings();
}

GlContext::GlContext(GlContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height)
{
	m_settings = settings;
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
    m_settings.attributeFlags = ContextSettings::Default;
}


////////////////////////////////////////////////////////////
void GlContext::checkSettings(const ContextSettings& requestedSettings)
{
    // Perform checks to inform the user if they are getting a context they might not have expected
}

} // namespace priv

} // namespace cpp3ds
