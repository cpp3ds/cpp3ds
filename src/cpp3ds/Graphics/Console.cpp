////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <vector>
#include <cpp3ds/Config.hpp>
#include <cpp3ds/Window/Event.hpp>
#include <cpp3ds/Graphics/Console.hpp>
#include <cpp3ds/Window/GlContext.hpp>
#include <cpp3ds/Graphics/RenderTarget.hpp>
#include <cpp3ds/Resources.hpp>
#include <stdio.h>
#ifndef EMULATION
#include <sys/iosupport.h>
#endif

namespace cpp3ds {
	std::vector<String> g_stdout;
}

extern "C" {

#ifndef EMULATION
ssize_t console_write(struct _reent *r, int fd, const char *ptr, size_t len) {
	cpp3ds::g_stdout.push_back(cpp3ds::String(ptr));
	return len;
}

static const devoptab_t dotab_stdout = {
	"con", 0, NULL, NULL, console_write, NULL, NULL, NULL
};
#endif

}

namespace cpp3ds
{
////////////////////////////////////////////////////////////
Console::Console()
{
	//
}


////////////////////////////////////////////////////////////
Console::~Console()
{
	if (m_initialized)
	{
		//
	}
}


////////////////////////////////////////////////////////////
void Console::create()
{
	if (!m_initialized) {
		m_initialized = true;
#ifndef EMULATION
		devoptab_list[STD_OUT] = &dotab_stdout;
		devoptab_list[STD_ERR] = &dotab_stdout;
		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);

		priv::ResourceInfo font = priv::core_resources["sansation.ttf"];
		m_font.loadFromMemory(font.data, font.size);
#endif

		m_limit = 1000;
	}
}


////////////////////////////////////////////////////////////
void Console::update(float delta)
{
	for (String& s : g_stdout)
		write(s);
	g_stdout.clear();

	if (m_lines.size() > m_limit)
		m_lines.erase(m_lines.begin(), m_lines.end() - m_limit);
}


////////////////////////////////////////////////////////////
void Console::write(String text)
{
	Text line(text, m_font, 12);
	m_lines.push_back(line);
}


////////////////////////////////////////////////////////////
bool Console::processEvent(Event& event)
{
	if (event.type == Event::KeyPressed) {
		if (event.key.code == Keyboard::A) {
			m_visible = !m_visible;
			return false;
		}
	}

	// Allow event to also be processed by the game
	return true;
}


////////////////////////////////////////////////////////////
void Console::setVisible(bool visible)
{
	m_visible = visible;
}


////////////////////////////////////////////////////////////
void Console::draw(RenderTarget& target, RenderStates states) const
{
	if (!m_visible)
		return;

	int h = 240;
	int i = m_lines.size();

	while (h > 0 && i > 0) {
		Text text = m_lines[--i];
		h -= text.getGlobalBounds().height;
		text.setPosition(0, h);
		target.draw(text);
	}
}


} // namespace cpp3ds
