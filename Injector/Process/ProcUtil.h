#pragma once

#include <functional>
#include <string>

namespace ProcUtil {
	void ForEachProcess(std::function<void(std::string procName, size_t procId)> callback);
	size_t FindProcID(std::string name);
};