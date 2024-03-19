#include "ScreenViewSigProvider.h"

ScreenViewSigProvider::ScreenViewSigProvider() : SigProvider("ScreenView")
{
	this->addResult(SigTable::ScreenView_setupAndRender, std::string("48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 8b fa 48 89 54 24 ? 4c 8b e9 48 89 4c 24"));
}
