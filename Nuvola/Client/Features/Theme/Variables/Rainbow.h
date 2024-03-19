#pragma once

#include "../Theme.h"

class Rainbow : public ThemeVariable
{
	float progress = 0.0f;
	float speed = 1.0f;
	float offset = 0.0f;
	float saturation = 1.0f;
	float brightness = 1.0f;
	float alfuh = 1.0f;
public:
	Rainbow();

	virtual std::shared_ptr<ThemeVariable> buildNew(const std::vector<std::string>& argTokens) override;
	virtual void nextFrame() override;
	virtual std::string getName() override;
	virtual ColorF getColor() override;
};