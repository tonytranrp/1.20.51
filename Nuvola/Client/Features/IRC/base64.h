#pragma once

#include <vector>
#include <string>
#include <cstdint>

std::string base64_encode(uint8_t const* buf, unsigned int bufLen);
std::vector<uint8_t> base64_decode(std::string const&);