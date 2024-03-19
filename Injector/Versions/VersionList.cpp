#include "VersionList.h"

#include <Web/Requests.h>

#include <nlohmann/json.hpp>

#if defined DEBUG_WEB and defined _DEBUG
#define ROOT_URL API_HOST
#else
#define ROOT_URL API_HOST
#endif

#define VERSION_ROUTE "/versions"
#define API_ROUTE "/api"

#define API_LIST_VERSION_ROUTE "/listclientversions"

VersionList::VersionList() :
	latestVersion("latest")
{
	std::string response;
	Requests::MakeRequest(ROOT_URL, API_ROUTE API_LIST_VERSION_ROUTE, { }, &response);
	nlohmann::json response_json = nlohmann::json::parse(response);

	//Throw exception if an error occours
	if (!response_json["error"].is_null()) {
		throw std::exception(response_json["error"].dump().c_str());
	}

	nlohmann::json tagsList = response_json["tags"];
	for (std::string tag : tagsList) {
		this->versions.emplace_back(tag);
	}
}

static VersionList* instance;
VersionList* VersionList::GetList() {
	if (instance == nullptr) {
		instance = new VersionList;
	}
	return instance;
}
void VersionList::Refresh() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

const Version& VersionList::GetLatest() {
	return this->latestVersion;
}

const std::vector<Version>& VersionList::GetVersions() {
	return this->versions;
}