#include <cpp3ds/Simulator.hpp>
#include <cpp3ds/Window/EventManager.hpp>
#include <cpp3ds/Window/BottomScreen.hpp>
#include <cpp3ds/System/Sleep.hpp>
#include <iostream>

namespace cpp3ds {

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
	//
}

void EventManager::pushEvent(const Event& event) {
	m_events.push(event);
}

bool EventManager::filterEvent(const Event& event) {
	return true;
}

////////////////////////////////////////////////////////////
void EventManager::processEvents() {
	// Check all inputs and push Events that are triggered
	sf::Event event;
	while (_simulator->screen->pollEvent(event)) {
		switch (event.type) {
			case sf::Event::KeyPressed:
				break;
			case sf::Event::KeyReleased:
				break;
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
				break;
			case sf::Event::JoystickMoved:
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
