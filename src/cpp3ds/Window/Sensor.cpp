////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Window/Sensor.hpp>
#ifndef EMULATION
#include <3ds.h>
#endif

namespace cpp3ds
{

bool Sensor::m_accel_enabled = false;
bool Sensor::m_gyro_enabled = false;
Vector3f Sensor::m_gyro = {0,0,0};
Vector3f Sensor::m_accel = {0,0,0};

////////////////////////////////////////////////////////////
bool Sensor::isAvailable(Type sensor)
{
	switch(sensor) {
		case Sensor::Gyroscope:
		case Sensor::Accelerometer:
			return true;
		default:
			return false;
	}
}

////////////////////////////////////////////////////////////
void Sensor::setEnabled(Type sensor, bool enabled)
{
	#ifndef EMULATION
	if (sensor == Sensor::Gyroscope && enabled != m_gyro_enabled) {
		m_gyro_enabled = enabled;
		if (enabled)
			HIDUSER_EnableGyroscope();
		else
			HIDUSER_DisableGyroscope();
	}
	else if (sensor == Sensor::Accelerometer && enabled != m_accel_enabled) {
		m_accel_enabled = enabled;
		if (enabled)
			HIDUSER_EnableAccelerometer();
		else
			HIDUSER_DisableAccelerometer();
	}
	#endif
}

////////////////////////////////////////////////////////////
bool Sensor::isEnabled(Type sensor)
{
	switch(sensor) {
		case Sensor::Gyroscope:
			return m_gyro_enabled;
		case Sensor::Accelerometer:
			return m_accel_enabled;
		default:
			return false;
	}
}

////////////////////////////////////////////////////////////
Vector3f Sensor::getValue(Type sensor)
{
	switch(sensor) {
		case Sensor::Gyroscope:
			return m_gyro;
		case Sensor::Accelerometer:
			return m_accel;
		default:
			return Vector3f(0,0,0);

	}
}

////////////////////////////////////////////////////////////
void Sensor::update()
{
	#ifndef EMULATION
	if(m_gyro_enabled) {
		angularRate gyro;
		hidGyroRead(&gyro);
		m_gyro = Vector3f{gyro.x/100.f, gyro.y/100.f, gyro.z/100.f};
	}
	if(m_accel_enabled) {
		accelVector accel;
		hidAccelRead(&accel);
		m_accel = {accel.x/100.f, accel.y/100.f, accel.z/100.f};
	}
	#endif
}

} // namespace cpp3ds
