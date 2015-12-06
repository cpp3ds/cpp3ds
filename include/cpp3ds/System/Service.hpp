#ifndef CPP3DS_SERVICE_H
#define CPP3DS_SERVICE_H

#include <cpp3ds/Config.hpp>
#ifndef EMULATION
#include <3ds.h>
#endif

namespace cpp3ds {

enum ServiceName {
	NETWORK     = 1 << 0,
	AUDIO       = 1 << 1,
	CONFIG      = 1 << 2,
	ROMFS       = 1 << 3,
	WIFI_STATUS = 1 << 4,

	ALL         = 0xFFFF
};

class Service {
public:
	static bool enable(ServiceName service);
	static bool disable(ServiceName service);
	static bool isEnabled(ServiceName service);

private:
	static Uint16 m_enabledServices;
	#ifndef EMULATION
	static u32* m_socBuffer;
	#endif
};

} // namespace cpp3ds


#endif //CPP3DS_SERVICE_H
