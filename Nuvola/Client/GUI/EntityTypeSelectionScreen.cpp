#include "EntityTypeSelectionScreen.h"

#include "../Bridge/ActorFactory.h"
#include "../Bridge/ClientInstance.h"
#include "../Bridge/Level.h"
#include "../Bridge/LocalPlayer.h"

EntityTypeSelectionScreenData::EntityTypeSelectionScreenData(std::shared_ptr<Setting> settingPtr)
{
	std::shared_ptr<EntityTypeListSetting> castedSetting = std::dynamic_pointer_cast<EntityTypeListSetting>(settingPtr);
	assert(castedSetting != nullptr);
	this->settingPtr = castedSetting;
	this->initializer = [&](GroupedType<EntityTypeGroup, std::string>* groups) -> void {
		ClientInstance* ci = Bridge::getClientInstance();
		LocalPlayer* lp = ci->getClientPlayer();
		if (lp == nullptr) return;
		Level* level = lp->level;
		if (level == nullptr) return;

		const auto* factory = level->mActorFactory;
		for (const auto& [key, identifier] : factory->actorIdentifiers)
		{
			const NamespacedKey& nskey = identifier.namespacedKey;
			const std::string& canonicalName = identifier.canonicalName.get();
			EntityTypeGroup group = this->getGroupFor(nskey);

			groups->registerType(canonicalName, group);
		}

		//Register player in case the factory doesnt have it (since its only for actors)
		//If it was already registered this call does nothing
		groups->registerType("minecraft:player", EntityTypeGroup::Misc);
	};
}

EntityTypeGroup EntityTypeSelectionScreenData::getGroupFor(NamespacedKey id)
{
	std::map<std::string, EntityTypeGroup> nameToGroup = {
		{ "minecraft:bat", EntityTypeGroup::Animal },
		{ "minecraft:bee", EntityTypeGroup::Animal },
		{ "minecraft:cat", EntityTypeGroup::Animal },
		{ "minecraft:chicken", EntityTypeGroup::Animal },
		{ "minecraft:cow", EntityTypeGroup::Animal },
		{ "minecraft:donkey", EntityTypeGroup::Animal },
		{ "minecraft:fox", EntityTypeGroup::Animal },
		{ "minecraft:frog", EntityTypeGroup::Animal },
		{ "minecraft:goat", EntityTypeGroup::Animal },
		{ "minecraft:horse", EntityTypeGroup::Animal },
		{ "minecraft:llama", EntityTypeGroup::Animal },
		{ "minecraft:mooshroom", EntityTypeGroup::Animal },
		{ "minecraft:mule", EntityTypeGroup::Animal },
		{ "minecraft:ocelot", EntityTypeGroup::Animal },
		{ "minecraft:panda", EntityTypeGroup::Animal },
		{ "minecraft:parrot", EntityTypeGroup::Animal },
		{ "minecraft:pig", EntityTypeGroup::Animal },
		{ "minecraft:polar_bear", EntityTypeGroup::Animal },
		{ "minecraft:rabbit", EntityTypeGroup::Animal },
		{ "minecraft:sheep", EntityTypeGroup::Animal },
		{ "minecraft:strider", EntityTypeGroup::Animal },
		{ "minecraft:trader_llama", EntityTypeGroup::Animal },
		{ "minecraft:turtle", EntityTypeGroup::Animal },
		{ "minecraft:wolf", EntityTypeGroup::Animal },

		{ "minecraft:axolotl", EntityTypeGroup::Sea },
		{ "minecraft:cod", EntityTypeGroup::Sea },
		{ "minecraft:dolphin", EntityTypeGroup::Sea },
		{ "minecraft:glow_squid", EntityTypeGroup::Sea },
		{ "minecraft:tropicalfish", EntityTypeGroup::Sea },
		{ "minecraft:pufferfish", EntityTypeGroup::Sea },
		{ "minecraft:salmon", EntityTypeGroup::Sea },
		{ "minecraft:squid", EntityTypeGroup::Sea },

		{ "minecraft:blaze", EntityTypeGroup::Monster },
		{ "minecraft:cave_spider", EntityTypeGroup::Monster },
		{ "minecraft:creeper", EntityTypeGroup::Monster },
		{ "minecraft:drowned", EntityTypeGroup::Monster },
		{ "minecraft:elder_guardian", EntityTypeGroup::Monster },
		{ "minecraft:enderman", EntityTypeGroup::Monster },
		{ "minecraft:endermite", EntityTypeGroup::Monster },
		{ "minecraft:ender_dragon", EntityTypeGroup::Monster },
		{ "minecraft:evocation_illager", EntityTypeGroup::Monster },
		{ "minecraft:ghast", EntityTypeGroup::Monster },
		{ "minecraft:guardian", EntityTypeGroup::Monster },
		{ "minecraft:hoglin", EntityTypeGroup::Monster },
		{ "minecraft:husk", EntityTypeGroup::Monster },
		{ "minecraft:magma_cube", EntityTypeGroup::Monster },
		{ "minecraft:phantom", EntityTypeGroup::Monster },
		{ "minecraft:piglin", EntityTypeGroup::Monster },
		{ "minecraft:piglin_brute", EntityTypeGroup::Monster },
		{ "minecraft:pillager", EntityTypeGroup::Monster },
		{ "minecraft:ravager", EntityTypeGroup::Monster },
		{ "minecraft:shulker", EntityTypeGroup::Monster },
		{ "minecraft:skeleton", EntityTypeGroup::Monster },
		{ "minecraft:skeleton_horse", EntityTypeGroup::Monster },
		{ "minecraft:silverfish", EntityTypeGroup::Monster },
		{ "minecraft:slime", EntityTypeGroup::Monster },
		{ "minecraft:stray", EntityTypeGroup::Monster },
		{ "minecraft:spider", EntityTypeGroup::Monster },
		{ "minecraft:vex", EntityTypeGroup::Monster },
		{ "minecraft:vindicator", EntityTypeGroup::Monster },
		{ "minecraft:warden", EntityTypeGroup::Monster },
		{ "minecraft:witch", EntityTypeGroup::Monster },
		{ "minecraft:wither", EntityTypeGroup::Monster },
		{ "minecraft:wither_skeleton", EntityTypeGroup::Monster },
		{ "minecraft:zombie", EntityTypeGroup::Monster },
		{ "minecraft:zombie_horse", EntityTypeGroup::Monster },
		{ "minecraft:zombie_pigman", EntityTypeGroup::Monster },
		{ "minecraft:zombie_villager", EntityTypeGroup::Monster },
		{ "minecraft:zombie_villager_v2", EntityTypeGroup::Monster },
		{ "minecraft:zoglin", EntityTypeGroup::Monster },

		{ "minecraft:agent", EntityTypeGroup::Misc },
		{ "minecraft:area_effect_cloud", EntityTypeGroup::Misc },
		{ "minecraft:armor_stand", EntityTypeGroup::Misc },
		{ "minecraft:arrow", EntityTypeGroup::Misc },
		{ "minecraft:boat", EntityTypeGroup::Misc },
		{ "minecraft:chest_minecart", EntityTypeGroup::Misc },
		{ "minecraft:command_block_minecart", EntityTypeGroup::Misc },
		{ "minecraft:egg", EntityTypeGroup::Misc },
		{ "minecraft:elder_guardian_ghost", EntityTypeGroup::Misc },
		{ "minecraft:ender_crystal", EntityTypeGroup::Misc },
		{ "minecraft:ender_pearl", EntityTypeGroup::Misc },
		{ "minecraft:evocation_fang", EntityTypeGroup::Misc },
		{ "minecraft:eye_of_ender_signal", EntityTypeGroup::Misc },
		{ "minecraft:falling_block", EntityTypeGroup::Misc },
		{ "minecraft:fireball", EntityTypeGroup::Misc },
		{ "minecraft:fireworks_rocket", EntityTypeGroup::Misc },
		{ "minecraft:fishing_hook", EntityTypeGroup::Misc },
		{ "minecraft:ice_bomb", EntityTypeGroup::Misc },
		{ "minecraft:iron_golem", EntityTypeGroup::Misc },
		{ "minecraft:item", EntityTypeGroup::Misc },
		{ "minecraft:leash_knot", EntityTypeGroup::Misc },
		{ "minecraft:lightning_bolt", EntityTypeGroup::Misc },
		{ "minecraft:lingering_potion", EntityTypeGroup::Misc },
		{ "minecraft:llama_spit", EntityTypeGroup::Misc },
		{ "minecraft:minecart", EntityTypeGroup::Misc },
		{ "minecraft:npc", EntityTypeGroup::Misc },
		{ "minecraft:painting", EntityTypeGroup::Misc },
		{ "minecraft:player", EntityTypeGroup::Misc },
		{ "minecraft:shulker_bullet", EntityTypeGroup::Misc },
		{ "minecraft:snowball", EntityTypeGroup::Misc },
		{ "minecraft:snow_golem", EntityTypeGroup::Misc },
		{ "minecraft:splash_potion", EntityTypeGroup::Misc },
		{ "minecraft:thrown_trident", EntityTypeGroup::Misc },
		{ "minecraft:tnt", EntityTypeGroup::Misc },
		{ "minecraft:tnt_minecart", EntityTypeGroup::Misc },
		{ "minecraft:villager", EntityTypeGroup::Misc },
		{ "minecraft:wandering_trader", EntityTypeGroup::Misc },
		{ "minecraft:wither_skull", EntityTypeGroup::Misc },
		{ "minecraft:wither_skull_dangerous", EntityTypeGroup::Misc },
		{ "minecraft:xp_bottle", EntityTypeGroup::Misc },
		{ "minecraft:xp_orb", EntityTypeGroup::Misc },

		{ "minecraft:balloon", EntityTypeGroup::Education },
		{ "minecraft:tripod_camera", EntityTypeGroup::Education }
	};

	std::string canonicalName = id.asString();
	auto it = nameToGroup.find(canonicalName);
	if (it != nameToGroup.end()) {
		return it->second;
	}

	if (id.name != "minecraft")
	{
		return EntityTypeGroup::Custom;
	}

	return EntityTypeGroup::Misc;
}

EntityTypeSelectionScreen::EntityTypeSelectionScreen() : TypeSelectionScreen<EntityTypeGroup, std::string>("EntityTypeSelectionScreen")
{
}

void EntityTypeSelectionScreen::initialize(ScreenData* data)
{
	auto* castedData = dynamic_cast<EntityTypeSelectionScreenData*>(data);
	assert(castedData != nullptr);

	this->settingPtr = castedData->settingPtr;

	TypeSelectionScreen<EntityTypeGroup, std::string>::initialize(data);
}

void EntityTypeSelectionScreen::render(float deltaTime)
{
	this->settingPtr->setSelectedTypes(this->selected);

	TypeSelectionScreen<EntityTypeGroup, std::string>::render(deltaTime);
}
