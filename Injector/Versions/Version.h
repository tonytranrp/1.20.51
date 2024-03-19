#pragma once

#include <functional>
#include <string>

class Version {
	std::string tag = "";
	bool auth = false;
	std::string downloadRoute = "";
	std::string mcVer = "";
public:
	Version(std::string tag);

	const std::string& GetTag() const;
	bool RequiresAuth() const;
	const std::string& GetDownloadRoute() const;

	void DownloadAsync(std::function<void(std::vector<uint8_t>)> callback, std::function<bool(uint64_t current, uint64_t total)> progress = [](uint64_t progress, uint64_t total) { return true; }) const;
};