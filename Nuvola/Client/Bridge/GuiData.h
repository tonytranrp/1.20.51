#pragma once

#include "../Signatures/SigManager.h"
#include "../SystemManager.h"

#include <Math/Math.h>

#include <cstdint>
#include <unordered_map>
#include <string>

static const std::unordered_map<char, std::string> formatCodes = {
        {'0', "§0"},  // Black
        {'1', "§1"},  // Dark blue
        {'2', "§2"},  // Dark green
        {'3', "§3"},  // Dark aqua
        {'4', "§4"},  // Dark red
        {'5', "§5"},  // Dark purple
        {'6', "§6"},  // Gold
        {'7', "§7"},  // Gray
        {'8', "§8"},  // Dark gray
        {'9', "§9"},  // Blue
        {'a', "§a"},  // Green
        {'b', "§b"},  // Aqua
        {'c', "§c"},  // Red
        {'d', "§d"},  // Light purple
        {'e', "§e"},  // Yellow
        {'f', "§f"},  // White
        {'g', "§g"},  // Minecoin gold
        {'h', "§h"},  // Material quartz
        {'i', "§i"},  // Material iron
        {'j', "§j"},  // Material netherite
        {'k', "§k"},  // Obfuscated
        {'l', "§l"},  // Bold
        {'m', "§m"},  // Strikethrough
        {'n', "§n"},  // Underline
        {'o', "§o"},  // Italic
        {'p', "§p"},  // Material gold
        {'q', "§q"},  // Material emerald
        {'r', "§r"},   // Reset
        {'s', "§s"},  // Material diamond
        {'t', "§t"},  // Material lapiz
        {'u', "§u"},  // Material amethyst
};

struct GuiData {
	/* Fields */
	char pad_0000[48]; //0x0000
	Vector2<float> resolution; //0x0030
	Vector2<float> lastResolution; //0x0038
	Vector2<float> scaledResolution; //0x0040
	int32_t N00001B67; //0x0048
	float scale; //0x004C
	float scaleInverse; //0x0050
	/* Virtuals */
	/* Functions */
	static inline uintptr_t holder_displayClientMessage;
    void displayClientMessage(const std::string& message) {
        std::string newMessage;

        bool checkFormat = false;
        bool escapeNext = false;
        for (const char currentChar : message) {
            if (escapeNext) {
                newMessage += currentChar;
                escapeNext = false;
                continue;
            }
            if (currentChar == '\\') {
                escapeNext = true;
                continue;
            }
            if (currentChar == '&') {
                if (checkFormat) {
                    newMessage += currentChar;
                    checkFormat = false;
                    continue;
                }
                checkFormat = true;
                continue;
            }
            if (checkFormat) {
                const auto formatCodeIt = formatCodes.find(currentChar);
                if (formatCodeIt != formatCodes.end()) {
                    newMessage += formatCodeIt->second;
                    checkFormat = false;
                    continue;
                }
                checkFormat = false;
            }
            newMessage += currentChar;
        }

        const auto func = systems->getSigManager()->findAddressAs<void (__fastcall *)(GuiData *, std::string, bool)>(
                SigTable::GuiData_displayMessage);
        func(this, newMessage, false);
    }
};