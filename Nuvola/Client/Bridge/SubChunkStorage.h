#pragma once

//TODO: Figure this shit out

template<typename T>
class SubChunkStorage
{
	uint16_t elements[1368]; //0x0008
	T* types; //0x0AB8
};