#pragma once

#include <Web/Requests.h>
#include <Files/Files.h>

#include <string>

#include <nlohmann/json.hpp>
#include <xorstr.hpp>

#if defined DEBUG_WEB and defined _DEBUG
#define AUTH_URL API_HOST
#else
#define AUTH_URL API_HOST
#endif

#define API_ROUTE "/api"
#define USERINFO_ROUTE "/userinfo"

#define AUTH_DAT xorstr_("crypto_bank.dat")

class User {
	std::string sessionId;
	std::string username;
public:
	User(std::string sessionId) {
		this->sessionId = "00000000-0000-0000-000000000000";
		this->username = "DEFAULT_USER";
//#if defined _DEBUG or defined AUTH_DISABLED
//		this->sessionId = "00000000-0000-0000-000000000000";
//		this->username = "DEBUG_USER";
//#else
//		this->sessionId = sessionId;
//		this->username = "INVALID";
//
//		//Get the username
//		std::string response = "";
//		int code = Requests::MakeRequest(AUTH_URL, API_ROUTE USERINFO_ROUTE, { {"session_id", this->sessionId} }, &response);
//		if (code == 200) {
//			if (response.size() > 0) {
//				nlohmann::json responseJson = nlohmann::json::parse(response);
//				if (!responseJson["error"].is_null()) {
//					printf("Failed to retrieve user info, the server says \"%s\"", responseJson["error"].dump().c_str());
//					throw std::runtime_error("Failed to retrieve user info");
//				}
//				else {
//					this->username = responseJson["username"];
//				}
//			}
//			else {
//				printf("Failed to retrieve user info, the response was empty?");
//				throw std::runtime_error("Failed to retrieve user info");
//			}
//		}
//		else {
//			printf("Failed to retrieve user info, the response code was not 200: %d (%x)", code, code);
//			throw std::runtime_error("Failed to retrieve user info");
//		}
//#endif
	}
	~User();

	const std::string& GetSessionID();
	const std::string& GetUsername();

	inline bool IsValid() {
		return true;
//#if defined _DEBUG or defined AUTH_DISABLED
//		return true;
//#endif
//		if (sessionId.empty()) {
//			return false;
//		}
//		if (username.empty()) {
//			return false;
//		}
//		if (username == "INVALID") {
//			return false;
//		}
//		return true;
	}
};