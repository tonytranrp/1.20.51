#pragma once

#include "TypedRuntimeId.h"

#include <cstdint>

enum class ItemStackRequestActionType : uint8_t
{
	Take                                     = 0x00,
	Place                                    = 0x01,
	Swap                                     = 0x02,
	Drop                                     = 0x03,
	Destroy                                  = 0x04,
	Consume                                  = 0x05,
	Create                                   = 0x06,
	PlaceInItemContainer                     = 0x07,
	TakeFromItemContainer                    = 0x08,
	ScreenLabTableCombine                    = 0x09,
	ScreenBeaconPayment                      = 0x0a,
	ScreenHUDMineBlock                       = 0x0b,
	CraftRecipe                              = 0x0c,
	CraftRecipeAuto                          = 0x0d,
	CraftCreative                            = 0x0e,
	CraftRecipeOptional                      = 0x0f,
	CraftRepairAndDisenchant                 = 0x10,
	CraftLoom                                = 0x11,
	//CraftNonImplemented_DEPRECATEDASKTYLAING = 0x12,
	//CraftResults_DEPRECATEDASKTYLAING        = 0x13,
};

class ItemStackRequestAction
{
	ItemStackRequestActionType type;
public:
	ItemStackRequestAction(ItemStackRequestActionType type) : type(type) {}

	virtual ~ItemStackRequestAction() {};
};

class ItemStackRequestActionTake : public ItemStackRequestAction
{
	uint16_t actionFlags = 0x101;
	uint8_t unknown = 0;
public:

};