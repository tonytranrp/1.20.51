#pragma once

class MoveInputHandler
{
public:
	char pad_0000[24]; //0x0000
	class ControlsSettings* controlsSettings; //0x0018
	char pad_0020[64]; //0x0020
}; //Size: 0x0060