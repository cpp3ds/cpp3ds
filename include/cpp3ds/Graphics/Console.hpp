#ifndef CPP3DS_CONSOLE_HPP
#define CPP3DS_CONSOLE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/System/NonCopyable.hpp>
#include <cpp3ds/Graphics/Drawable.hpp>
#include <cpp3ds/Graphics/Font.hpp>
#include <cpp3ds/Graphics/Text.hpp>

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
	/// \brief Default constructor
	///
	/// The constructor creates and activates the context
	///
	////////////////////////////////////////////////////////////
	Console();

	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// The destructor deactivates and destroys the context
	///
	////////////////////////////////////////////////////////////
	~Console();

public:

	void create();

	void update(float delta);

	void write(String text);

	bool processEvent(Event& event);

	void setVisible(bool visible);

	static void initialize();

private:

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
	std::vector<Text> m_lines;
	Text m_memoryText;
	unsigned int m_limit;
	static bool m_initialized;
	bool m_visible;
};

} // namespace cpp3ds


#endif // CPP3DS_CONSOLE_HPP
