#pragma once

#include "Version.h"

#include <vector>

class VersionList {
	Version latestVersion;
	std::vector<Version> versions;
	VersionList();
public:
	static VersionList* GetList();
	static void Refresh();

	const Version& GetLatest();
	const std::vector<Version>& GetVersions();
};