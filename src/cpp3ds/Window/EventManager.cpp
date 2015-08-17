#include <cpp3ds/Window/EventManager.hpp>
#include <cpp3ds/Window/Keyboard.hpp>
#include <cpp3ds/System/Sleep.hpp>
#include <3ds.h>

namespace cpp3ds {

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
	// Check all inputs and push Events that are triggered
	Event e;
	int i;

	Keyboard::update();

	// 3D slider
	float slider3d = Keyboard::getSlider3D();
	if (slider3d != m_slider3d) {
		m_slider3d = slider3d;
		e.type = Event::Slider3DChanged;
		e.slider.value = slider3d;
		pushEvent(e);
	}

	// Volume slider
	float sliderVolume = Keyboard::getSliderVolume();
	if (sliderVolume != m_sliderVolume) {
		m_sliderVolume = sliderVolume;
		e.type = Event::SliderVolumeChanged;
		e.slider.value = sliderVolume;
		pushEvent(e);
	}

	// Circle pad
	const float circleRatio = 100.f / 156.f; // Ratio to convert to [-100, 100] range
	circlePosition circle;
	hidCircleRead(&circle);
	circle.dx = static_cast<s16>(circleRatio * circle.dx);
	circle.dy = static_cast<s16>(circleRatio * circle.dy);
	if (static_cast<float>(circle.dx) > -m_joystickThreshold && static_cast<float>(circle.dx) < m_joystickThreshold)
		circle.dx = 0;
	if (static_cast<float>(circle.dy) > -m_joystickThreshold && static_cast<float>(circle.dy) < m_joystickThreshold)
		circle.dy = 0;
	if (circle.dx != m_circle.dx || circle.dy != m_circle.dy) {
		m_circle = circle;
			e.type = Event::JoystickMoved;
			e.joystickMove.x = circle.dx;
			e.joystickMove.y = circle.dy;
			pushEvent(e);
	}

	// TODO: implement TouchSwiped event

	// Touch screen
	touchPosition touch;
	static bool isTouching;
	if (Keyboard::isKeyDown(Keyboard::Touchpad)) {
		hidTouchRead(&touch);
		if (touch.px != m_touch.px || touch.py != m_touch.py || !isTouching) {
			m_touch = touch;
			if (isTouching) {
				e.type = Event::TouchMoved;
			} else {
				e.type = Event::TouchBegan;
				isTouching = true;
			}
			e.touch.x = touch.px;
			e.touch.y = touch.py;
			pushEvent(e);
		}
	} else if (isTouching) {
		isTouching = false;
		e.type = Event::TouchEnded;
		e.touch.x = m_touch.px;
		e.touch.y = m_touch.py;
		pushEvent(e);
	}

	// Sensors (gyroscope & accelerometer)
	for (i = 0; i < Sensor::Count; ++i) {
		Sensor::Type type = static_cast<Sensor::Type>(i);
		if (Sensor::isEnabled(type)) {
			Vector3f val = Sensor::getValue(type);
			if (val != m_sensorValue[type]) {
				m_sensorValue[type] = val;
				e.type = Event::SensorChanged;
				e.sensor.type = type;
				e.sensor.x = val.x;
				e.sensor.y = val.y;
				e.sensor.z = val.z;
				pushEvent(e);
			}
		}
	}

	// Buttons
	for (i = 0; i < 32; ++i) {
		Keyboard::Key key = static_cast<Keyboard::Key>(1 << i);
		if (Keyboard::isKeyPressed(key)) {
			e.type = Event::KeyPressed;
			e.key.code = key;
			pushEvent(e);
		}
		if (Keyboard::isKeyReleased(key)) {
			e.type = Event::KeyReleased;
			e.key.code = key;
			pushEvent(e);
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
