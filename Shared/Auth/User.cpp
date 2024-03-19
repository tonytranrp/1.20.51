#include "User.h"

User::~User() {}

const std::string& User::GetSessionID() {
	return this->sessionId;
}

const std::string& User::GetUsername() {
	return this->username;
}