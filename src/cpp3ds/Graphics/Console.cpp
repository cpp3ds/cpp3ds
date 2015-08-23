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
#include <sstream>
#ifndef EMULATION
#include <sys/iosupport.h>
extern u32 __linear_heap_size;
#endif

namespace cpp3ds {
	std::vector<String> g_stdout;
}

extern "C" {

#ifndef EMULATION
ssize_t console_write(struct _reent *r, int fd, const char *ptr, size_t len) {
	cpp3ds::String s;
	int i = 0;
	while (i < len) {
		s += ptr[i];
		i++;
	}
	cpp3ds::g_stdout.push_back(s);
	return len;
}

static const devoptab_t dotab_stdout = {
	"con", 0, NULL, NULL, console_write, NULL, NULL, NULL
};
#endif

}

namespace cpp3ds
{

bool Console::m_initialized = false;

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
void Console::initialize()
{
	if (!m_initialized) {
		m_initialized = true;
#ifndef EMULATION
		devoptab_list[STD_OUT] = &dotab_stdout;
		devoptab_list[STD_ERR] = &dotab_stdout;
		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);
#endif
	}
}


////////////////////////////////////////////////////////////
void Console::create()
{
	initialize();
	priv::ResourceInfo font = priv::core_resources["opensans.ttf"];
	m_font.loadFromMemory(font.data, font.size);

	m_memoryText.setFont(m_font);
	m_memoryText.setCharacterSize(12);

	m_limit = 1000;
}


////////////////////////////////////////////////////////////
void Console::update(float delta)
{
	for (String& s : g_stdout)
		write(s);
	g_stdout.clear();

	if (m_lines.size() > m_limit)
		m_lines.erase(m_lines.begin(), m_lines.end() - m_limit);

	#ifndef EMULATION
	std::ostringstream ss;
	ss << (__linear_heap_size - linearSpaceFree()) / 1024 << "kb / " << __linear_heap_size / 1024 << "kb";
	m_memoryText.setString(ss.str());
	m_memoryText.setPosition(395 - m_memoryText.getGlobalBounds().width, 5);
	#endif
}


////////////////////////////////////////////////////////////
void Console::write(String text)
{
	Text line(text, m_font, 10);
	m_lines.push_back(line);
}


////////////////////////////////////////////////////////////
bool Console::processEvent(Event& event)
{
	if (event.type == Event::KeyPressed) {
		if (event.key.code == Keyboard::DPadDown && Keyboard::isKeyDown(Keyboard::R)) {
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

	target.draw(m_memoryText);
}


} // namespace cpp3ds
