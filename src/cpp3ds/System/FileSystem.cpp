#include <cpp3ds/System/FileSystem.hpp>
#include <string>
#include <iostream>

namespace cpp3ds {

const std::string FileSystem::getFilePath(const std::string& filename)
{
#ifdef TEST
	std::string pathPrefix = "../res/test/";
#elif defined(EMULATION)
	std::string pathPrefix = "../res/";
#endif

#ifdef EMULATION
	std::string newpath;
	if (filename.find(pathPrefix + "romfs/") == 0)
		return filename;
	if (filename.find(pathPrefix + "sdmc/") == 0)
		return filename;
	if (filename.find("sdmc:/") == 0) {
		newpath = filename;
		newpath.erase(0, 5);
		return pathPrefix + "sdmc" + newpath;
	}
	newpath = pathPrefix + "romfs/" + filename;
	return newpath;
#else
	return filename;
#endif
}

} // namespace cpp3ds
