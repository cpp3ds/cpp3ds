#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Resources.hpp>
#include <cpp3ds/System/Service.hpp>
#include <cpp3ds/Window/Game.hpp>
#include <cpp3ds/System/I18n.hpp>
#include "../Graphics/CitroHelpers.hpp"


namespace cpp3ds {

// System font sheets to be fetched and released by Game
cpp3ds::Texture *system_font_textures = nullptr;

//Apt hook cookie
static aptHookCookie apt_hook_cookie;

static void apt_clock_hook(APT_HookType hook, void* param)
{
	if (hook == APTHOOK_ONRESTORE || hook == APTHOOK_ONWAKEUP) {
		Clock* clock = (Clock*) param;
		clock->restart();
	}
}


Game::Game(size_t gpuCommandBufSize)
: m_triggerExit(false)
{
	if (!Console::isEnabled() && !Console::isEnabledBasic())
		gfxInitDefault();
	CitroInit(gpuCommandBufSize);
	osSetSpeedupEnable(true);
	Service::enable(RomFS);
	I18n::getInstance();   // Init and load localization file(s)

	windowTop.create(ContextSettings(TopScreen));
	windowBottom.create(ContextSettings(BottomScreen));

	// Use default shader if one isn't provided by user.
	priv::ResourceInfo defaultShader = priv::core_resources["default_shader.vsh"];
	if (!Shader::Default.loadBinary(defaultShader.data, defaultShader.size, Shader::Vertex)){
		err() << "Failed to load default_shader.vsh from cpp3ds core.";
	}

	// Load system font texture sheets
	if (R_SUCCEEDED(fontEnsureMapped()))
	{
		TGLP_s* glyphInfo = fontGetGlyphInfo();
		system_font_textures = new cpp3ds::Texture[glyphInfo->nSheets];
		for (int i = 0; i < glyphInfo->nSheets; ++i)
		{
			system_font_textures[i].loadFromPreprocessedMemory(
				fontGetGlyphSheetTex(i), glyphInfo->sheetSize,
				glyphInfo->sheetWidth, glyphInfo->sheetHeight,
				(GPU_TEXCOLOR)glyphInfo->sheetFmt, false);
			system_font_textures[i].setSmooth(true);
		}
	}
}


Game::~Game()
{
	delete[] system_font_textures;
	Service::disable(All);
	CitroDestroy();
	gfxExit();
}


void Game::render()
{
	Console& console = Console::getInstance();

	if (!console.isEnabledBasic() || console.getScreen() != TopScreen) {
		C3D_RenderTarget* target = windowTop.getCitroTarget();
		C3D_RenderBufBind(&target->renderBuf);
		windowTop.resetGLStates();
		renderTopScreen(windowTop);
		if (console.isEnabled() && console.getScreen() == TopScreen) {
			windowTop.setView(windowTop.getDefaultView());
			windowTop.draw(console);
		}
		C3D_Flush();
		C3D_RenderBufTransfer(&target->renderBuf, (u32*)gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL), target->transferFlags);
	}

	if (!console.isEnabledBasic() || console.getScreen() != BottomScreen) {
		C3D_RenderTarget* target = windowBottom.getCitroTarget();
		C3D_RenderBufBind(&target->renderBuf);
		windowBottom.resetGLStates();
		renderBottomScreen(windowBottom);
		if (console.isEnabled() && console.getScreen() == BottomScreen) {
			windowBottom.setView(windowBottom.getDefaultView());
			windowBottom.draw(console);
		}
		C3D_Flush();
		C3D_RenderBufTransfer(&target->renderBuf, (u32*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL), target->transferFlags);
	}

	gfxSwapBuffersGpu();
	gspWaitForVBlank();

	// This currently is only use to properly use frameTimeLimit
	windowTop.display();
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
