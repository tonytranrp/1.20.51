#include "VersionUtils.h"
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define LMC_EXE L"Minecraft.Windows.exe"
#define MC_EXE "Minecraft.Windows.exe"

SupportedVersion VersionUtils::strToVer(const std::string& str) {

    if(str == "1.20.31.1") {
        return MC_1_20_31_1;
    }

    return MC_UNSUPPORTED;
}
std::string VersionUtils::verToStr(SupportedVersion ver) {
    switch(ver) {
    case MC_1_20_31_1:
        return "1.20.31.1";
    default:
        return "Unsupported";
    }
}


SupportedVersion VersionUtils::getVersion() {
    if(!theVersion)
        theVersion = retrieveVersion();
    return theVersion;
}

SupportedVersion VersionUtils::retrieveVersion() {
    DWORD verHandle = 0;
    UINT size = 0;
    LPBYTE lpBuffer = nullptr;
    DWORD verSize = GetFileVersionInfoSizeA(MC_EXE, &verHandle);

    if (verSize != NULL)
    {
        auto verData = new char[verSize];

        if (GetFileVersionInfoA(MC_EXE, verHandle, verSize, verData))
        {
            if (VerQueryValue(verData, TEXT("\\"), (VOID FAR* FAR*)&lpBuffer, &size))
            {
                if (size)
                {
                    auto* verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {

                        // Doesn't matter if you are on 32 bit or 64 bit,
                        // DWORD is always 32 bits, so first two revision numbers
                        // come from dwFileVersionMS, last two come from dwFileVersionLS
                        int verMS_Top = ( verInfo->dwFileVersionMS >> 16 ) & 0xffff;
                        int verMS_Bottom = ( verInfo->dwFileVersionMS >> 0 ) & 0xffff;
                        int verLS_Top = ( verInfo->dwFileVersionLS >> 16 ) & 0xffff;
                        int verLS_Bottom = ( verInfo->dwFileVersionLS >> 0 ) & 0xffff;


                        std::stringstream ss;
                        ss << verMS_Top << "." << verMS_Bottom << "." << verLS_Top << "." << verLS_Bottom;
                        std::string version = ss.str();

                        //Log::getLogger()->write("Got MC Version: ")->writeLine(version);

                        delete[] verData;

                        return strToVer(version);
                    }
                }
            }
        }
        delete[] verData;
    }
    return MC_UNSUPPORTED;
};