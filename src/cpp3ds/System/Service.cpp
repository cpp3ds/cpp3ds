#include <cpp3ds/Config.hpp>
#include <cpp3ds/System/Service.hpp>
#include <3ds.h>
#include <malloc.h>

namespace cpp3ds {

Uint16 Service::m_enabledServices = 0x0;
u32* Service::socBuffer = NULL;


bool Service::enable(ServiceName service) {
	if (isEnabled(service))
		return true;

	if (service == ALL)
		return enable(NETWORK) && enable(AUDIO);

	if (service == NETWORK) {
		socBuffer = (u32*) memalign(0x1000, 0x100000);
		if (!socBuffer)
			return false;
		Result socResult = SOC_Initialize(socBuffer, 0x100000);
		if (socResult != 0) {
			free(socBuffer);
			socBuffer = NULL;
			return false;
		}
		m_enabledServices |= service;
		return true;
	}

	if (service == AUDIO) {
		return csndInit() == 0;
	}

	return false;
}


bool Service::disable(ServiceName service) {
	if (service & ~m_enabledServices)
		return true;

	if (service == ALL)
		return disable(NETWORK);

	if (service == NETWORK) {
		SOC_Shutdown();
		free(socBuffer);
		socBuffer = NULL;
	}

	if (service == AUDIO) {
		// Stop all audio
//		CSND_SetPlayStateR(1, 0);
//		CSND_SetPlayStateR(2, 0);
		csndExecCmds(true);
		csndExit();
	}
}


bool Service::isEnabled(ServiceName service) {
	return service & m_enabledServices;
}

} // namespace cpp3ds
