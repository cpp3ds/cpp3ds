#include <cpp3ds/Emulator.hpp>
#include <cpp3ds/Window/EventManager.hpp>
#include <cpp3ds/System/Sleep.hpp>
#include <iostream>
#include <map>

namespace cpp3ds {

// TODO: configurable key-mapping
std::map<sf::Keyboard::Key, cpp3ds::Keyboard::Key> keyMap = {
	{sf::Keyboard::A, cpp3ds::Keyboard::A},
	{sf::Keyboard::B, cpp3ds::Keyboard::B}
};

EventManager::EventManager() {
	m_joystickThreshold = 10.f;
}

bool EventManager::pollEvent(Event& event) {
	if (popEvent(event, false)) {
		return filterEvent(event);
	} else {
		return false;
	}
}

bool EventManager::waitEvent(Event& event) {
	if (popEvent(event, true)) {
		return filterEvent(event);
	} else {
		return false;
	}
}

void EventManager::setJoystickThreshold(float threshold) {
	if (threshold >=0 && threshold <= 100)
		m_joystickThreshold = threshold;
}

void EventManager::pushEvent(const Event& event) {
	m_events.push(event);
}

bool EventManager::filterEvent(const Event& event) {
	return true;
}

////////////////////////////////////////////////////////////
void EventManager::processEvents() {
	int BOTTOM_X = 40,
		BOTTOM_Y = 240,
		BOTTOM_WIDTH = 320;
	// Check all inputs and push Events that are triggered
	sf::Event event;
	while (_emulator->screen->pollMouseEvent(event)) {
		switch (event.type) {
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.x > BOTTOM_X && event.mouseButton.x < BOTTOM_X+BOTTOM_WIDTH && event.mouseButton.y > BOTTOM_Y){
					cpp3ds::Event e;
					e.type = Event::TouchBegan;
					e.touch.x = event.mouseButton.x - BOTTOM_X;
					e.touch.y = event.mouseButton.y - BOTTOM_Y;
					pushEvent(e);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.x > BOTTOM_X && event.mouseButton.x < BOTTOM_X+BOTTOM_WIDTH && event.mouseButton.y > BOTTOM_Y){
					cpp3ds::Event e;
					e.type = Event::TouchEnded;
					e.touch.x = event.mouseButton.x - BOTTOM_X;
					e.touch.y = event.mouseButton.y - BOTTOM_Y;
					pushEvent(e);
				}
				break;
			case sf::Event::MouseMoved:
				if (event.mouseButton.x > BOTTOM_X && event.mouseButton.x < BOTTOM_X+BOTTOM_WIDTH && event.mouseButton.y > BOTTOM_Y){
					cpp3ds::Event e;
					e.type = Event::TouchMoved;
					e.touch.x = event.mouseButton.x - BOTTOM_X;
					e.touch.y = event.mouseButton.y - BOTTOM_Y;
					pushEvent(e);
				}
				break;
			default:
				break;
		}
	}
	while (_emulator->screen->pollEvent(event)) {
		std::cout << "EVENT: " << event.type << std::endl;
        cpp3ds::Event e;
		std::map<sf::Keyboard::Key, cpp3ds::Keyboard::Key>::iterator key;
		switch (event.type) {
			case sf::Event::KeyPressed:
			std::cout << "sf::KeyPressed";
				key = keyMap.find(event.key.code);
				if (key != keyMap.end()) {
					e.type = Event::KeyPressed;
					e.key.code = key->second;
					pushEvent(e);
				}
				break;
			case sf::Event::KeyReleased:
//				std::cout << "EVENT: " << event.type << std::endl;
                e.type = Event::KeyReleased;
                pushEvent(e);
				break;
			case sf::Event::JoystickMoved:
				e.type = Event::JoystickMoved;
				// TODO: implement joystick for emulator
				e.joystickMove.x = event.joystickMove.position;
//				pushEvent(e);
				break;
			default:
				break;
		}
	}
}

////////////////////////////////////////////////////////////
bool EventManager::popEvent(Event& event, bool block) {
    // If the event queue is empty, let's first check if new events are available from the OS
    if (m_events.empty()) {
        // Get events from the system
        processEvents();

        // In blocking mode, we must process events until one is triggered
        if (block) {
            // Here we use a manual wait loop instead of the optimized
            // wait-event provided by the OS, so that we don't skip joystick
            // events (which require polling)
            while (m_events.empty()) {
                sleep(milliseconds(10));
                processEvents();
            }
        }
    }

    // Pop the first event of the queue, if it is not empty
    if (!m_events.empty()) {
        event = m_events.front();
        m_events.pop();
        return true;
    }

    return false;
}

}
