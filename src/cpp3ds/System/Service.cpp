#include <cpp3ds/Config.hpp>
#include <cpp3ds/System/Service.hpp>
#include <cpp3ds/System/Err.hpp>
#include <3ds.h>
#include <malloc.h>
#include <sys/unistd.h>
#include <string.h>

namespace cpp3ds {

Uint16 Service::m_enabledServices = 0x0;
u32*   Service::m_socBuffer       = nullptr;


bool Service::enable(ServiceName service) {
	if (isEnabled(service))
		return true;

	bool success = false;
	Result result;

	switch (service) {
		case ALL:
			return enable(NETWORK) && enable(AUDIO) &&
			       enable(CONFIG) && enable(ROMFS);
		case NETWORK:
			m_socBuffer = (u32*) memalign(0x1000, 0x100000);
			if (m_socBuffer == nullptr)
				break;
			result = socInit(m_socBuffer, 0x100000);
			if (result != 0) {
				err() << "Network service (soc:U) failed to initialize: " << std::hex << result;
				socExit();
				free(m_socBuffer);
				break;
			}
			success = true;
			break;
		case AUDIO:
			result = ndspInit();
			success = R_SUCCEEDED(result);
			if (!success)
				err() << "Audio service (dsp) failed to initialize: " << std::hex << result;
			break;
		case CONFIG:
			success = R_SUCCEEDED(cfguInit());
			break;
		case ROMFS:
			success = R_SUCCEEDED(romfsInit());
			break;
		case WIFI_STATUS:
			success = R_SUCCEEDED(acInit());
			break;
		default:
			break;
	}

	if (success)
		m_enabledServices |= service;
	return success;
}


bool Service::disable(ServiceName service) {
	if (service == ALL)
		return disable(NETWORK) && disable(AUDIO) &&
			   disable(CONFIG) && disable(ROMFS);

	if (!isEnabled(service))
		return true;

	bool success = true;

	switch (service) {
		case NETWORK:
			socExit();
			free(m_socBuffer);
			break;
		case AUDIO:
			ndspExit();
			break;
		case CONFIG:
			cfguExit();
			break;
		case ROMFS:
			romfsExit();
			break;
		case WIFI_STATUS:
			acExit();
			break;
		default:
			break;
	}

	if (success)
		m_enabledServices &= ~service;
	return success;
}


bool Service::isEnabled(ServiceName service) {
	if (service == NETWORK) {
		if (!enable(WIFI_STATUS))
			return false;
		u32 status;
		Result result = ACU_GetWifiStatus(&status);
		if (result != 0 || status == 0)
			return false;
	}
	return service & m_enabledServices;
}

} // namespace cpp3ds
