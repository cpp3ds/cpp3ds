#ifndef CPP3DS_GAME_HPP
#define CPP3DS_GAME_HPP

#include <vector>
#include <cpp3ds/Window/Event.hpp>
#include <cpp3ds/Window/Window.hpp>
#include <cpp3ds/Graphics/Console.hpp>
#include <cpp3ds/Graphics/Shader.hpp>
#include <cpp3ds/Graphics/RenderTexture.hpp>
#ifdef EMULATION
    #include <SFML/Graphics.hpp>
#else
    #include <3ds.h>
#endif

namespace cpp3ds {

class Game {
public:
	virtual void update(float delta) = 0;
	virtual void processEvent(Event& event) = 0;
	virtual void renderTopScreen(Window& window) = 0;
    virtual void renderBottomScreen(Window& window) = 0;
	void console(Screen screen = BottomScreen, Color color = Color::White);
    void render();
	void run();
	void exit();
	Game();
	virtual ~Game();
protected:
    Window windowTop, windowBottom;
private:
	bool m_consoleEnabled;
	bool m_triggerExit;
	Console m_console;
#ifdef EMULATION
	sf::RenderTexture m_frameTextureTop, m_frameTextureBottom;
	sf::Sprite m_frameSpriteTop, m_frameSpriteBottom;
#else
	Shader m_shader;
#endif
};

}

#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Game
/// \ingroup window
///
/// cpp3ds::Game example goes here
///
////////////////////////////////////////////////////////////
