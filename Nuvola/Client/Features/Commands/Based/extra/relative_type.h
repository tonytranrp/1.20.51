#pragma once

#include "../bcs_internal.h"

#include <istream>

namespace bcs
{
	//Enables relativity for a given type (eg. relative coordinates for tp)
	template<typename type_t>
	struct relative
	{
		relative() : val(), is_rel(false) {};

		operator type_t() const
		{
			return val;
		}

		operator bool() const
		{
			return is_rel;
		}

		type_t val;
		bool is_rel;
	};

	template<typename type_t>
	std::istream& operator>>(std::istream& stream, relative<type_t>& obj)
	{
		obj.is_rel = false;
		char next = stream.peek();
		if (next == '~')
		{
			obj.is_rel = true;
			stream.seekg(1, stream.cur); //Seek 1 cuz of ~
		}

		//Read next as string
		std::string remaining;
		stream >> remaining;
		//Reset the fail bit in case this failed (we dont care lol)
		stream.clear();
		//If we read something from the string
		if (!remaining.empty())
		{
			//Convert the remaining value
			type_t value;
			//Need to specify its a template cuz of weird compiler specifics
			if (bcs::internal::template fromString<type_t>(remaining, value))
			{
				obj.val = value;
			}
		}

		return stream;
	}

	namespace internal
	{
		template<typename type_t>
		struct type_string<bcs::relative<type_t>>
		{
			static const char* str()
			{
				return typeid(type_t).name();
			}
		};
	}
}