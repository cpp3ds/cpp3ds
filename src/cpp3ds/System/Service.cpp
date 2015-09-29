#include <cpp3ds/Config.hpp>
#include <cpp3ds/System/Service.hpp>
#include <3ds.h>
#include <malloc.h>
#include <sys/unistd.h>
#include <string.h>

namespace cpp3ds {

Uint16 Service::m_enabledServices = 0x0;
u32* Service::socBuffer = NULL;


bool Service::enable(ServiceName service) {
	if (isEnabled(service))
		return true;

	bool success = false;

	switch (service) {
		case ALL:
			return enable(NETWORK) && enable(AUDIO) &&
			       enable(CONFIG) && enable(ROMFS);
		case NETWORK:
			socBuffer = (u32*) memalign(0x1000, 0x100000);
			if (!socBuffer)
				break;
			if (SOC_Initialize(socBuffer, 0x100000) != 0) {
				free(socBuffer);
				socBuffer = NULL;
				break;
			}
			success = true;
			break;
		case AUDIO:
			success = csndInit() == 0;
			break;
		case CONFIG:
			initCfgu();
			success = true;
			break;
		case ROMFS:
			romfsInit();
			success = strcmp(getcwd(NULL,0), "romfs:/") == 0;
			break;
		default:
			break;
	}

	if (success)
		m_enabledServices |= service;
	return success;
}


bool Service::disable(ServiceName service) {
	if (service & ~m_enabledServices)
		return true;

	bool success = true;

	switch (service) {
		case ALL:
			return disable(NETWORK) && disable(AUDIO) &&
			       disable(CONFIG) && disable(ROMFS);
		case NETWORK:
			SOC_Shutdown();
			free(socBuffer);
			socBuffer = NULL;
			break;
		case AUDIO:
			// TODO: Stop all audio
			csndExecCmds(true);
			csndExit();
			break;
		case CONFIG:
			exitCfgu();
			break;
		case ROMFS:
			romfsExit();
			break;
		default:
			break;
	}

	if (success)
		m_enabledServices &= ~service;
	return success;
}


bool Service::isEnabled(ServiceName service) {
	return service & m_enabledServices;
}

} // namespace cpp3ds
