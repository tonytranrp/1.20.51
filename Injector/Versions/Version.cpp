#include <Auth/User.h>

#include "Version.h"

#include <nlohmann/json.hpp>

#include <xorstr.hpp>

#if defined DEBUG_WEB and defined _DEBUG
#define ROOT_URL xorstr_(API_HOST)
#else
#define ROOT_URL xorstr_(API_HOST)
#endif

#define VERSION_ROUTE xorstr_("/versions")
#define API_ROUTE "/api"

#define API_GET_VERSION_ROUTE "/getclientversion"

extern User* currentUser;

Version::Version(std::string tag) {
	std::string response;
	Requests::MakeRequest(ROOT_URL, xorstr_(API_ROUTE API_GET_VERSION_ROUTE), { {xorstr_("tag"), tag} }, &response);
	nlohmann::json response_json = nlohmann::json::parse(response);

	//Throw exception if an error occours
	if (!response_json[xorstr_("error")].is_null()) {
		throw std::exception(response_json[xorstr_("error")].dump().c_str());
	}

	std::string final_tag = response_json[xorstr_("tag")];
	std::string download_route = response_json[xorstr_("download")];
	bool auth = response_json[xorstr_("auth")];
	std::string mcVer = response_json[xorstr_("mc_ver")];

	this->tag = final_tag;
	this->downloadRoute = download_route;
	this->auth = auth;
	this->mcVer = mcVer;
}


const std::string& Version::GetTag() const {
	return this->tag;
}
bool Version::RequiresAuth() const {
	return this->auth;
}
const std::string& Version::GetDownloadRoute() const {
	return this->downloadRoute;
}


void Version::DownloadAsync(std::function<void(std::vector<uint8_t>)> callback, std::function<bool(uint64_t current, uint64_t total)> progress) const {
	std::thread asyncWorker([callback, progress, this]() {
		
#ifdef _DEBUG
		MessageBoxA(nullptr, "You are logged in as DEBUG_USER, meaning the download will fail. Please run the release version of Nuvola.", "Download Error", MB_OK);
		return;
#endif

		const std::string& downloadRoute = this->GetDownloadRoute();

		std::string response;
		int return_code = Requests::MakeRequest(ROOT_URL, downloadRoute, { { xorstr_("session_id"), currentUser->GetSessionID() } }, &response, progress);
		if (return_code != 200) {
			char buffer[256];
			sprintf(buffer, xorstr_("Non-200 code whilst downloading! %d (%x)"), return_code, return_code);
			throw std::exception(buffer);
		}

		//Store the response appropriately
		std::vector<uint8_t> fileData(response.begin(), response.end());

		//Invoke the callback
		callback(fileData);
	});
	asyncWorker.detach();
}