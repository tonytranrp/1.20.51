#pragma once

template<typename tag_t, typename id_t, id_t defaultVal = 0>
struct TypedRuntimeId
{
	id_t tag = defaultVal;
};