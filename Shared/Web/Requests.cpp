#include "Requests.h"

#include <sstream>

std::string Requests::FormArgs(std::map<std::string, std::string> args)
{
	std::stringstream arg_stream;
	int counter = 0;
	for (auto const& [name, value] : args) {
		if (counter == 0) {
			arg_stream << "?";
		}
		else {
			arg_stream << "&";
		}
		arg_stream << name;
		arg_stream << "=";
		arg_stream << value;
		counter++;
	}
	return arg_stream.str();
}