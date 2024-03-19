#pragma once

#include <algorithm>
#include <map>
#include <vector>

template<typename groups_t, typename type_t>
class GroupedType
{
	// Redundant data for quick lookups
	std::map<groups_t, std::vector<type_t>> groupedTypes;
	std::map<type_t, groups_t> typesToGroup;
	std::vector<type_t> allTypes;
public:
	GroupedType() = default;

	void registerType(const type_t& type, const groups_t& group)
	{
		if (std::find(allTypes.begin(), allTypes.end(), type) != allTypes.end())
		{
			// We dont want to register duplicates
			return;
		}

		groupedTypes[group].push_back(type);
		typesToGroup[type] = group;
		allTypes.push_back(type);

		// Sort the data
		std::sort(groupedTypes[group].begin(), groupedTypes[group].end());
		std::sort(allTypes.begin(), allTypes.end());
	}

	const std::vector<type_t>& getAllTypes()
	{
		return this->allTypes;
	}

	const std::vector<type_t>& getTypesFor(groups_t group)
	{
		return this->groupedTypes[group];
	}

	groups_t getGroupOf(const type_t& type) {
		auto it = typesToGroup.find(type);
		if (it != typesToGroup.end()) {
			return it->second;
		}
		// Return a default-constructed instance of G if type is not found
		return groups_t();
	}
};