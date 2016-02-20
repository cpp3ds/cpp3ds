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

bool Console::m_enabled      = false;
bool Console::m_enabledBasic = false;

////////////////////////////////////////////////////////////
Console::Console()
: m_visible(true)
{
}


////////////////////////////////////////////////////////////
Console::~Console()
{
}


////////////////////////////////////////////////////////////
Console& Console::getInstance()
{
	static Console console;
	return console;
}


////////////////////////////////////////////////////////////
void Console::enable(Screen screen, Color color)
{
	if (m_enabledBasic)
		return;
	if (!m_enabled) {
		m_enabled = true;
#ifndef EMULATION
		gfxInitDefault();
		devoptab_list[STD_OUT] = &dotab_stdout;
		devoptab_list[STD_ERR] = &dotab_stdout;
		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);
#endif
		Console& console = getInstance();
		priv::ResourceInfo font = priv::core_resources["opensans.ttf"];
		console.m_font.loadFromMemory(font.data, font.size);

		console.m_memoryText.setFont(console.m_font);
		console.m_memoryText.setCharacterSize(12);

		console.m_screen = screen;
		console.m_limit = 1000;
		console.setColor(color);
	}
}


////////////////////////////////////////////////////////////
void Console::enableBasic(Screen screen)
{
	if (m_enabled)
		return;
	if (!m_enabledBasic) {
		m_enabledBasic = true;
		Console& console = getInstance();
		console.m_screen = screen;
#ifndef EMULATION
		gfxInitDefault();
		consoleInit(screen == TopScreen ? GFX_TOP : GFX_BOTTOM, nullptr);
#endif
	}
}


////////////////////////////////////////////////////////////
bool Console::isEnabled()
{
	return m_enabled;
}


////////////////////////////////////////////////////////////
bool Console::isEnabledBasic()
{
	return m_enabledBasic;
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
	m_memoryText.setPosition((m_screen == TopScreen ? 395 : 315) - m_memoryText.getGlobalBounds().width, 5);
#endif

	int h = 240;
	int i = m_lines.size();

	while (h > 0 && i > 0) {
		Text& text = m_lines[--i];
		h -= text.getGlobalBounds().height;
		text.setPosition(0, h);
	}
}


////////////////////////////////////////////////////////////
void Console::write(String text)
{
	Text line(text, m_font, 10);
	line.setFillColor(m_color);
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
void Console::setScreen(Screen screen)
{
	m_screen = screen;
}

////////////////////////////////////////////////////////////
Screen Console::getScreen()
{
	return m_screen;
}


////////////////////////////////////////////////////////////
void Console::draw(RenderTarget& target, RenderStates states) const
{
	if (!m_visible)
		return;

	int h = 240;
	int i = m_lines.size();

	while (h > 0 && i > 0) {
		const Text& text = m_lines[--i];
		h -= text.getGlobalBounds().height;
//		text.setPosition(0, h);
		target.draw(text);
	}

	target.draw(m_memoryText);
}


void Console::setColor(const Color& color)
{
	m_memoryText.setFillColor(color);
	m_color = color;
}


const Color& Console::getColor() const
{
	return m_color;
}


} // namespace cpp3ds
