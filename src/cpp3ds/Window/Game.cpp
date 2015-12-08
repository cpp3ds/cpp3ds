#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Resources.hpp>
#include <cpp3ds/System/Service.hpp>
#include <cpp3ds/Window/Game.hpp>
#include <cpp3ds/System/I18n.hpp>


namespace cpp3ds {


//Apt hook cookie
static aptHookCookie apt_hook_cookie;

static void apt_clock_hook(APT_HookType hook, void* param)
{
	if (hook == APTHOOK_ONRESTORE || hook == APTHOOK_ONWAKEUP) {
		Clock* clock = (Clock*) param;
		clock->restart();
	}
}


Game::Game()
: m_triggerExit(false)
, m_consoleEnabled(false)
{
	gfxInitDefault();
	Service::enable(RomFS);
	Console::initialize(); // Initialize console if it hasn't been already
	I18n::getInstance();   // Init and load localization file(s)

	windowTop.create(ContextSettings(TopScreen));
	windowBottom.create(ContextSettings(BottomScreen));

	// Use default shader if one isn't provided by user.
	priv::ResourceInfo defaultShader = priv::core_resources["default_shader.vsh"];
	if (!Shader::Default.loadBinary(defaultShader.data, defaultShader.size, Shader::Vertex)){
		err() << "Failed to load default_shader.vsh from cpp3ds core.";
	}
}


Game::~Game()
{
	Service::disable(All);
	gfxExit();
}


void Game::console(Screen screen, Color color)
{
	Console::initialize();
	m_consoleEnabled = true;
	m_console.setVisible(true);
	m_console.create(screen);
	m_console.setColor(color);
}


void Game::render()
{
	windowTop.setActive(true);
	renderTopScreen(windowTop);
	if (m_consoleEnabled && m_console.getScreen() == TopScreen)
		windowTop.draw(m_console);
	windowTop.display();

	windowBottom.setActive(true);
	renderBottomScreen(windowBottom);
	if (m_consoleEnabled && m_console.getScreen() == BottomScreen)
		windowBottom.draw(m_console);
	windowBottom.display();

	gl3ds_swapBuffers();
}


void Game::exit()
{
	m_triggerExit = true;
}


void Game::run()
{
	Event event;
	EventManager eventmanager;
	Clock clock;
	Time deltaTime;

	// Hook for clock
	aptHook(&apt_hook_cookie, apt_clock_hook, &clock);

	while (aptMainLoop())
	{
		// Update sensors only once outside of EventManager, they change too often
		Sensor::update();

		while (eventmanager.pollEvent(event)) {
			if (m_consoleEnabled) {
				if (!m_console.processEvent(event))
					continue;
			}
			processEvent(event);
		}
		deltaTime = clock.restart();

		if (m_triggerExit)
			break;

		if (m_consoleEnabled)
			m_console.update(deltaTime.asSeconds());

		update(deltaTime.asSeconds());
		render();
	}

	aptUnhook(&apt_hook_cookie);
}


}
