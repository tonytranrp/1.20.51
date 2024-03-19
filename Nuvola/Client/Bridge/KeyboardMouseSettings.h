#pragma once

#include "MCKeyBind.h"

#include <vector>

class KeyboardMouseSettings
{
public:
	char pad_0000[8]; //0x0000
	std::vector<MCKeyBind> keyboard_type_0; //0x0008
	std::vector<MCKeyBind> keyboard_type_1; //0x0020

public:
	int GetMCKey(std::string keyName)
	{
		const std::vector<MCKeyBind>& keybinds = this->keyboard_type_0;
		for (auto& bind : keybinds) {
			if (bind.bindName == keyName) {
				//The key is not bound
				if (bind.bindKey.size() == 0) {
					return 0;
				}

				return bind.bindKey[0];
			}
		}
		return 0;
	}

	static bool isMouseButton(int key)
	{
		return key < 0;
	}

	//Util function to convert from a Minecraft mouse button ID to a standard mouse button ID (Eg. 1 = LMB, 2 = RMB, 3 = Wheel, etc)
	static int keyIdToMB(int key)
	{
		return key + 100;
	}
}; //Size: 0x0028