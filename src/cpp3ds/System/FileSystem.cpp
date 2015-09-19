#include <cpp3ds/System/FileSystem.hpp>
#include <string>
#include <iostream>

namespace cpp3ds {

const std::string FileSystem::getFilePath(const std::string& filename)
{
#ifdef EMULATION
	if (filename.find("../res/romfs/") == 0)
		return filename;
	std::string newpath;
	newpath = "../res/romfs/" + filename;
	return newpath;
#else
	return filename;
#endif
}

} // namespace cpp3ds
