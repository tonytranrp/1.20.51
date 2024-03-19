#pragma once

#include "Option.h"

#include <cstdint>

struct IntOption : public Option {
	/* Fields */
	char padding_16[16];
	int32_t maximum;
	char padding_24[4];
	int32_t value;
	char padding_32[4];
	int32_t minimum;
	/* Virtuals */
	/* Functions */
};