#pragma once

#define STRING_EXPAND(s) #s
#define STRING(s) STRING_EXPAND(s)

//Define the build ver if CMake failed to or it wasn't build with it
#ifndef NUVOLA_BUILD_VERSION
#define NUVOLA_BUILD_VERSION Unknown
#endif	NUVOLA
#ifndef NUVOLA_BUILD_TAG
#define NUVOLA_BUILD_TAG 1.0.0
#endif

#define CLIENT_AUTH_DAT "texmap.bin"

#include <string>

namespace Utils {
	const std::string& GetRoamingState();
}