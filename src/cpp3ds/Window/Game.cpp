#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Resources.hpp>
#include <cpp3ds/System/Service.hpp>
#include <cpp3ds/Window/Game.hpp>


namespace cpp3ds {


//Apt hook cookie
static aptHookCookie apt_hook_cookie;

static void apt_clock_hook(int hook, void* param)
{
	if (hook == APTHOOK_ONRESTORE) {
		Clock* clock = (Clock*) param;
		clock->restart();
	}
}


Game::Game()
{
	gfxInitDefault();
	initCfgu();
	Console::initialize();

	windowTop.create(ContextSettings(TopScreen));
	windowBottom.create(ContextSettings(BottomScreen));
}


Game::~Game()
{
	Service::disable(ALL);
	gfxExit();
}


void Game::console(Screen screen, bool enable, bool visible)
{
	m_consoleEnabled = enable;
	m_consoleScreen = screen;
	m_console.setVisible(visible);
	if (enable)
		m_console.create();
}


void Game::render()
{
	windowTop.setActive(true);
	renderTopScreen(windowTop);
	if (m_consoleEnabled && m_consoleScreen == TopScreen)
		windowTop.draw(m_console);
	windowTop.display();

	windowBottom.setActive(true);
	renderBottomScreen(windowBottom);
	if (m_consoleEnabled && m_consoleScreen == BottomScreen)
		windowBottom.draw(m_console);
	windowBottom.display();

	gl3ds_swapBuffers();
}


void Game::run()
{
	Event event;
	EventManager eventmanager;
	Clock clock;
	Time deltaTime;

	// Hook for clock
	aptHook(&apt_hook_cookie, apt_clock_hook, &clock);

	// Use default shader if one isn't provided by user
	if (m_shader.getNativeHandle() == NULL) {
		priv::ResourceInfo defaultShader = priv::core_resources["default_shader.vsh"];
		if (!m_shader.loadBinary(defaultShader.data, defaultShader.size)){
			// No shader, just give up! :(
			return;
		}
		glCheck(glUseProgram(m_shader.getNativeHandle())); // TODO: use it differently
	}

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

		if (m_consoleEnabled)
			m_console.update(deltaTime.asSeconds());

		update(deltaTime.asSeconds());
		render();
	}

	aptUnhook(&apt_hook_cookie);
}


}
