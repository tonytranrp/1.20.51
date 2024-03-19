#pragma once

#include "../Signatures/SigManager.h"

#include "HashedString.h"
#include "Macro.h"

#include <memory>
#include <stdint.h>
#include <vector>

enum class EnchantFrequency
{
    VeryRare = 1,
    Rare = 3,
    Uncommon = 10,
    Common = 30
};

enum class EnchantSlot
{
    ALL = -1,
    NONE = 0,
    ARMOR_HEAD = 1,
    ARMOR_TORSO = 2,
    ARMOR_FEET = 4,
    ARMOR_LEGS = 8,
    G_ARMOR = 15,
    SWORD = 16,
    BOW = 32,
    HOE = 64,
    SHEARS = 128,
    FLINTSTEEL = 256,
    AXE = 512,
    PICKAXE = 1024,
    SHOVEL = 2048,
    G_DIGGING = 3648,
    FISHING_ROD = 4096,
    CARROT_STICK = 8192,
    ELYTRA = 16384,
    SPEAR = 32768,
    CROSSBOW = 65536,
    SHIELD = 131072,
    G_TOOL = 131520,
    COSMETIC_HEAD = 262144,
    COMPASS = 524288,
    MUSHROOM_STICK = 1048576
};

#undef INFINITY
enum class EnchantType : uint8_t
{
    PROTECTION = 0,
    FIRE_PROTECTION = 1,
    FEATHER_FALLING = 2,
    BLAST_PROTECTION = 3,
    PROJECTILE_PROTECTION = 4,
    THORNS = 5,
    RESPIRATION = 6,
    DEPTH_STRIDER = 7,
    AQUA_AFFINITY = 8,
    SHARPNESS = 9,
    SMITE = 10,
    BANE_OF_ARTHROPODS = 11,
    KNOCKBACK = 12,
    FIRE_ASPECT = 13,
    LOOTING = 14,
    EFFICIENCY = 15,
    SILK_TOUCH = 16,
    UNBREAKING = 17,
    FORTUNE = 18,
    POWER = 19,
    PUNCH = 20,
    FLAME = 21,
    INFINITY = 22,
    LUCK_OF_THE_SEA = 23,
    LURE = 24,
    FROST_WALKER = 25,
    MENDING = 26,
    CURSE_OF_BINDING = 27,
    CURSE_OF_VANISHING = 28,
    IMPALING = 29,
    RIPTIDE = 30,
    LOYALTY = 31,
    CHANNELING = 32,
    MULTISHOT = 33,
    PIERCING = 34,
    QUICK_CHARGE = 35,
    SOUL_SPEED = 36,
    SWIFT_SNEAK = 37
};

class Enchant
{
public:
    char pad_0008[24]; //0x0008
    std::string translationKey; //0x0020
    HashedString identifier; //0x0040
    char pad_0070[16]; //0x0070

private:
    static inline std::vector<std::unique_ptr<Enchant>>& get_mEnchants()
    {
        auto addr = systems->getSigManager()->findAddressOf(SigTable::Enchant_mEnchants);
        auto& enchants = *reinterpret_cast<std::vector<std::unique_ptr<Enchant>>*>(addr);
        return enchants;
    }
public:
    __declspec(property(get = get_mEnchants)) std::vector<std::unique_ptr<Enchant>>& mEnchants;

    virtual ~Enchant();
    virtual bool isCompatibleWith(enum class EnchantType) const;
    virtual int getMinCost(int) const;
    virtual int getMaxCost(int) const;
    virtual int getMinLevel() const;
    virtual int getMaxLevel() const;
    virtual int getDamageProtection(int, class ActorDamageSource const&) const;
    virtual float getDamageBonus(int, class Actor const&) const;
    virtual void doPostAttack(class Actor&, class Actor&, int) const;
    virtual void doPostHurt(class ItemInstance&, class Actor&, class Actor&, int) const;
    virtual bool isMeleeDamageEnchant(void);
    virtual bool isProtectionEnchant(void);
    virtual bool isTreasureOnly(void);
    virtual bool isDiscoverable(void);
    virtual bool _isValidEnchantmentTypeForCategory(enum class EnchantType) const;
};
static_assert(sizeof(Enchant) == 0x80);