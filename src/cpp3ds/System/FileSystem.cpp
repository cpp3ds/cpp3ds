#include <cpp3ds/System/FileSystem.hpp>
#include <string>
#include <iostream>

namespace cpp3ds {

const std::string FileSystem::getFilePath(const std::string& filename)
{
#ifdef EMULATION
	std::string newpath;
	if (filename.find("../res/romfs/") == 0) {
		return filename;
	}
	if (filename.find("sdmc:/") == 0) {
		newpath = filename;
		newpath.erase(0, 5);
		return "../res/sdmc" + newpath;
	}
	newpath = "../res/romfs/" + filename;
	return newpath;
#else
	return filename;
#endif
}

} // namespace cpp3ds
