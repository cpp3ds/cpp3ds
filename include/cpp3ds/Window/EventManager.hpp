#ifndef CPP3DS_EVENTMANAGER_HPP
#define CPP3DS_EVENTMANAGER_HPP

#include <queue>
#include <cpp3ds/Window/Event.hpp>

namespace cpp3ds {

class EventManager {
public:
	////////////////////////////////////////////////////////////
	/// \brief Pop the event on top of the event queue, if any, and return it
	///
	/// This function is not blocking: if there's no pending event then
	/// it will return false and leave \a event unmodified.
	/// Note that more than one event may be present in the event queue,
	/// thus you should always call this function in a loop
	/// to make sure that you process every pending event.
	/// \code
	/// cpp3ds::Event event;
	/// while (cpp3ds::pollEvent(event))
	/// {
	///    // process event...
	/// }
	/// \endcode
	///
	/// \param event Event to be returned
	///
	/// \return True if an event was returned, or false if the event queue was empty
	///
	/// \see cpp3ds::waitEvent
	///
	////////////////////////////////////////////////////////////
	bool pollEvent(Event& event);

	////////////////////////////////////////////////////////////
	/// \brief Wait for an event and return it
	///
	/// This function is blocking: if there's no pending event then
	/// it will wait until an event is received.
	/// After this function returns (and no error occured),
	/// the \a event object is always valid and filled properly.
	/// This function is typically used when you have a thread that
	/// is dedicated to events handling: you want to make this thread
	/// sleep as long as no new event is received.
	/// \code
	/// cpp3ds::Event event;
	/// if (cpp3ds::waitEvent(event))
	/// {
	///    // process event...
	/// }
	/// \endcode
	///
	/// \param event Event to be returned
	///
	/// \return False if any error occured
	///
	/// \see cpp3ds::pollEvent
	///
	////////////////////////////////////////////////////////////
	bool waitEvent(Event& event);

	////////////////////////////////////////////////////////////
	/// \brief Change the joystick threshold, ie. the value below which
	///        no move event will be generated
	///
	/// \param threshold : New threshold, in range [0, 100]
	///
	////////////////////////////////////////////////////////////
	void setJoystickThreshold(float threshold);

private:

	////////////////////////////////////////////////////////////
	/// \brief Return the next window event available
	///
	/// If there's no event available, this function calls the
	/// window's internal event processing function.
	/// The \a block parameter controls the behaviour of the function
	/// if no event is available: if it is true then the function
	/// doesn't return until a new event is triggered; otherwise it
	/// returns false to indicate that no event is available.
	///
	/// \param event Event to be returned
	/// \param block Use true to block the thread until an event arrives
	///
	////////////////////////////////////////////////////////////
	bool popEvent(Event& event, bool block);

	////////////////////////////////////////////////////////////
	/// \brief Push a new event into the event queue
	///
	/// This function is to be used by derived classes, to
	/// notify the SFML window that a new event was triggered
	/// by the system.
	///
	/// \param event Event to push
	///
	////////////////////////////////////////////////////////////
	void pushEvent(const Event& event);

	////////////////////////////////////////////////////////////
	/// \brief Read current inputs and push the relevant Events
	///
	////////////////////////////////////////////////////////////
	void processEvents();

	std::queue<Event> m_events;                          ///< Queue of available events
//	JoystickState     m_joystickStates[Joystick::Count]; ///< Previous state of the joysticks
	Vector3f          m_sensorValue[Sensor::Count];      ///< Previous value of the sensors
	float             m_joystickThreshold;               ///< Joystick threshold (minimum motion for "move" event to be generated)

};

}


#endif
