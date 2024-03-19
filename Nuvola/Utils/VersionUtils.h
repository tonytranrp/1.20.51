#pragma once
#include <string>

enum SupportedVersion {
	MC_1_20_31_1,
	MC_UNSUPPORTED
};

class VersionUtils {
    static inline SupportedVersion theVersion;
public:
    static SupportedVersion retrieveVersion();
    static SupportedVersion getVersion();
    static SupportedVersion strToVer(const std::string&);
    static std::string verToStr(SupportedVersion);
};