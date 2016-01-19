#ifndef CPP3DS_CONSOLE_HPP
#define CPP3DS_CONSOLE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/System/NonCopyable.hpp>
#include <cpp3ds/Graphics/Drawable.hpp>
#include <cpp3ds/Graphics/Font.hpp>
#include <cpp3ds/Graphics/Text.hpp>
#include <cpp3ds/Window/ContextSettings.hpp>

namespace cpp3ds
{

extern std::vector<String> g_stdout;

////////////////////////////////////////////////////////////
/// \brief Class holding a valid drawing context
///
////////////////////////////////////////////////////////////
class Console : public Drawable
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// The destructor deactivates and destroys the console
	///
	////////////////////////////////////////////////////////////
	~Console();

	void update(float delta);

	void write(String text);

	bool processEvent(Event& event);

	void setVisible(bool visible);

	static Console& getInstance();

	static void enable(Screen screen, Color color = Color::White);
	static void enableBasic(Screen screen);
	static bool isEnabled();
	static bool isEnabledBasic();

	void setScreen(Screen screen);
	Screen getScreen();

	void setColor(const Color& color);
	const Color& getColor() const;

private:

	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// The constructor creates and activates the context
	///
	////////////////////////////////////////////////////////////
	Console();

	////////////////////////////////////////////////////////////
	/// \brief Draw the console to a render target
	///
	/// \param target Render target to draw to
	/// \param states Current render states
	///
	////////////////////////////////////////////////////////////
	virtual void draw(RenderTarget& target, RenderStates states) const;

	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
	Font  m_font;
	Color m_color;
	std::vector<Text> m_lines;
	Text m_memoryText;
	unsigned int m_limit;
	static bool m_enabled;
	static bool m_enabledBasic;
	bool m_visible;
	Screen m_screen;
};

} // namespace cpp3ds


#endif // CPP3DS_CONSOLE_HPP
