#include "DimensionSigProvider.h"

#include <xorstr.hpp>

DimensionSigProvider::DimensionSigProvider() : SigProvider("Dimension")
{
	this->addResult(SigTable::Dimension_getBrightnessDependentFogColor, std::string("41 0f 10 08 48 8b c2 0f 28 d3"));
	this->addResult(SigTable::Dimension_getCloudHeight, std::string("40 53 48 83 EC 20 48 8B 89 ? ? ? ? 48 8B 01 48 8B 80"));
}