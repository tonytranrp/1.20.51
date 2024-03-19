#pragma once

#include "ClientScreen.h"

//A class for a screen with a window as its main focus
//Examples of this include the ClickGui, and other windows within that
class WindowScreen : public ClientScreen
{
protected:
	float fontScale = 1.0;
	float windowOpacity = 0.0f;
	float blurValue = 0.0f;
	Vector2<float> guiSize = { 800, 500 };
	bool movable = true;
public:
	WindowScreen(std::string name);

	virtual void initialize(ScreenData* data) override;
	virtual void render(float deltaTime) override;
	virtual void renderWindowContent(float deltaTime) = 0;
	virtual void cleanup() override;
	virtual void handleInput(KeyEvent& event) override;
	virtual void handleInput(MouseEvent& event) override;
	virtual void closeBehavior();
};