#pragma once

class ClientInputMappingFactory
{
public:
	char pad_0000[320]; //0x0000
	class KeyboardMouseSettings* keyboardMouseControls; //0x0140
	char pad_0148[840]; //0x0148
}; //Size: 0x0490