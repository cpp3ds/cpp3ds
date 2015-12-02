#ifndef CPP3DS_FILESYSTEM_H
#define CPP3DS_FILESYSTEM_H

#include <cpp3ds/Config.hpp>
#include <string>
#ifndef EMULATION
#include <3ds.h>
#endif

namespace cpp3ds {

class FileSystem {
public:
	static const std::string getFilePath(const std::string& filename);
};

} // namespace cpp3ds


#endif //CPP3DS_FILESYSTEM_H


////////////////////////////////////////////////////////////
/// \class cpp3ds::FileSystem
/// \ingroup system
///
////////////////////////////////////////////////////////////