#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Resources.hpp>
#include <cpp3ds/System/Service.hpp>
#include <cpp3ds/Window/Game.hpp>
#include <cpp3ds/System/I18n.hpp>
#include "../Graphics/CitroHelpers.hpp"


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
{
	if (!Console::isEnabled() && !Console::isEnabledBasic())
		gfxInitDefault();
	CitroInit();
	Service::enable(RomFS);
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
	CitroDestroy();
	gfxExit();
}


void Game::render()
{
	Console& console = Console::getInstance();

	if (!console.isEnabledBasic() || console.getScreen() != TopScreen) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C3D_FrameDrawOn(windowTop.getCitroTarget());
		windowTop.setView(windowTop.getDefaultView());
		renderTopScreen(windowTop);
		if (console.isEnabled() && console.getScreen() == TopScreen) {
			windowTop.setView(windowTop.getDefaultView());
			windowTop.draw(Console::getInstance());
		}
		C3D_FrameEnd(0);
	}

	if (!console.isEnabledBasic() || console.getScreen() != BottomScreen) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C3D_FrameDrawOn(windowBottom.getCitroTarget());
		windowBottom.setView(windowBottom.getDefaultView());
		renderBottomScreen(windowBottom);
		if (console.isEnabled() && console.getScreen() == BottomScreen) {
			windowBottom.setView(windowBottom.getDefaultView());
			windowBottom.draw(Console::getInstance());
		}
		C3D_FrameEnd(0);
	}
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

	Console& console = Console::getInstance();

	// Hook for clock
	aptHook(&apt_hook_cookie, apt_clock_hook, &clock);

	while (aptMainLoop())
	{
		// Update sensors only once outside of EventManager, they change too often
		Sensor::update();

		while (eventmanager.pollEvent(event)) {
			if (console.isEnabled()) {
				if (!console.processEvent(event))
					continue;
			}
			processEvent(event);
		}
		deltaTime = clock.restart();

		if (m_triggerExit)
			break;

		if (console.isEnabled())
			console.update(deltaTime.asSeconds());

		update(deltaTime.asSeconds());
		render();
	}

	aptUnhook(&apt_hook_cookie);
}


}
