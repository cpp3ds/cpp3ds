#include <cpp3ds/Window/EventManager.hpp>
#include <cpp3ds/System/Sleep.hpp>

namespace cpp3ds {

bool EventManager::pollEvent(Event& event) {
	return false;
}

bool EventManager::waitEvent(Event& event) {
	return false;
}

void EventManager::setJoystickThreshold(float threshold) {
	//
}

void EventManager::pushEvent(const Event& event) {
	//
}

////////////////////////////////////////////////////////////
void EventManager::processEvents() {
	//
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
