#include <cpp3ds/OpenGL.hpp>
#include <cpp3ds/Emulator.hpp>
#include <cpp3ds/Window/Game.hpp>
#include <cpp3ds/Window/EventManager.hpp>
#include <cpp3ds/System/Clock.hpp>
#include <cpp3ds/Window/Keyboard.hpp>
#include <cpp3ds/Graphics/Sprite.hpp>
#include "../Audio/AudioDevice.hpp"

namespace cpp3ds {

Game::Game(size_t gpuCommandBufSize)
: m_triggerExit(false)
{
	priv::ensureExtensionsInit();

	windowTop.create(ContextSettings(TopScreen));
	windowBottom.create(ContextSettings(BottomScreen));

	m_frameTextureTop.create(400, 240);
	m_frameTextureBottom.create(320, 240);

	m_frameSpriteTop.setPosition(0, 0);
	m_frameSpriteBottom.setPosition(40, 240);
}


Game::~Game()
{
	//
}


void Game::exit()
{
	m_triggerExit = true;
}


void Game::render()
{
#ifndef TEST
	_emulator->screen->clear();

	// Top Screen
	m_frameTextureTop.setActive(true);
	renderTopScreen(windowTop);
	m_frameTextureTop.display();
	m_frameSpriteTop.setTexture(m_frameTextureTop.getTexture());
	_emulator->screen->draw(m_frameSpriteTop);

	// Bottom Screen
	m_frameTextureBottom.setActive(true);
	renderBottomScreen(windowBottom);
	m_frameTextureBottom.display();
	m_frameSpriteBottom.setTexture(m_frameTextureBottom.getTexture());
	_emulator->screen->draw(m_frameSpriteBottom);
#endif
}


void Game::run()
{
	Event event;
	EventManager eventmanager;
	Clock clock;
	Time deltaTime;

#ifndef TEST
	_emulator->screen->setFramerateLimit(60);
#endif

	while (windowTop.isOpen())
	{
		while (eventmanager.pollEvent(event)) {
			processEvent(event);
		}
		deltaTime = clock.restart();

		if (m_triggerExit)
			break;

		Keyboard::update();
		update(deltaTime.asSeconds());

		render();

#ifndef TEST
		_emulator->screen->display();
		// TODO: pause non-drawing services (sound, networking, etc.)
		if (_emulator->getState() == EMU_PAUSED){
			priv::AudioDevice::suspend();
			_emulator->screen->setActive(false);
			_emulator->updatePausedFrame();
			while (_emulator->getState() == EMU_PAUSED)
				sf::sleep(sf::milliseconds(100));
			// Restart clock and purge event queue
			clock.restart();
			while (eventmanager.pollEvent(event)) {}
			priv::AudioDevice::resume();
		}
#endif
	}
}

}
