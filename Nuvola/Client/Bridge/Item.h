#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Components/IFoodItemComponent.h"

#include "BlockLegacy.h"
#include "Enchant.h"
#include "Macro.h"
#include "HashedString.h"
#include "WeakPtr.h"

enum class ItemGroup {
	Anvil,
	Arrow,
	Axe,
	Banner,
	BannerPattern,
	Bed,
	Boat,
	Boots,
	Buttons,
	Candles,
	Chalkboard,
	Chest,
	ChestBoat,
	Chestplate,
	Concrete,
	ConcretePowder,
	CookedFood,
	Copper,
	Coral,
	CoralDecorations,
	Crop,
	Door,
	Dye,
	EnchantedBook,
	Fence,
	FenceGate,
	Firework,
	FireworkStars,
	Flower,
	Glass,
	GlassPane,
	GlazedTerracotta,
	GoatHorn,
	Grass,
	HangingSign,
	Helmet,
	Hoe,
	HorseArmor,
	Leaves,
	Leggings,
	LingeringPotion,
	Log,
	Minecart,
	MiscFood,
	MobEgg,
	MonsterStoneEgg,
	Mushroom,
	NetherWartBlock,
	Ore,
	Permission,
	Pickaxe,
	Planks,
	Potion,
	PotterySherds,
	PressurePlate,
	Rail,
	RawFood,
	Record,
	Sandstone,
	Sapling,
	Sculk,
	Seed,
	Shovel,
	ShulkerBox,
	Sign,
	Skull,
	Slab,
	SmithingTemplates,
	SplashPotion,
	StainedClay,
	Stairs,
	Stone,
	StoneBrick,
	Sword,
	Trapdoor,
	Walls,
	Wood,
	Wool,
	WoolCarpet
};

#pragma pack(push, 1)
class Item
{
public:
    std::string texture_atlas; //0x0008
    char pad_0028[56]; //0x0028
    std::string texture_name; //0x0060
    char pad_0080[40]; //0x0080
    std::string locale; //0x00A8
    HashedString key; //0x00C8
    std::string _namespace; //0x00F8
    HashedString item_id; //0x0118
    int16_t maxDurability; //0x0148
    char pad_014A[78]; //0x014A
    std::string versionAdded; //0x0198
    char pad_01B8[16]; //0x01B8
    WeakPtr<BlockLegacy> blockLegacy; //0x01C8
    char pad_01D0[64]; //0x01D0
    IFoodItemComponent* foodComponent; //0x0210
    char pad_0218[40]; //0x0218
    std::vector<HashedString> recipetags; //0x0240

public:
    virtual ~Item();
    bool isArmor() const
    {
        return vftCall<12, bool, const Item*>(this);
    }
    IFoodItemComponent* getFood() const
    {
        return vftCall<27, IFoodItemComponent*, const Item*>(this);
    }
    int getAttackDamage() const
    {
        return vftCall<48, int, const Item*>(this);
    }

    bool hasRecipeTag(const std::string& tag) const
    {
        for (const std::string& rtag : this->recipetags)
        {
            if (rtag == tag)
            {
                return true;
            }
        }
        return false;
    }
};
#pragma pack(pop)

static_assert(sizeof(Item) == 0x258);