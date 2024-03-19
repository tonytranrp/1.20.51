#ifndef GUARD_MinecraftUIRenderContext
#define GUARD_MinecraftUIRenderContext
#ifndef SCANSIG
#define SCANSIG Mem::FindSig
#endif
#ifndef WRITEOUT
#define WRITEOUT printf
#endif
#ifndef PREINIT
#define PREINIT Utils::PreInit
#endif
#include <Math/Math.h>
#include "../../Utils/Utils.h"
#include "../../Mem/Mem.h"
#include "string"
#include "functional"
struct MinecraftUIRenderContext {
	/* Fields */
	class ClientInstance* clientInstance;
	class ScreenContext* screenContext;
	char padding_40[16];
	float textAlpha;
	char padding_240[196];
	class UIScene* uiScene;
	/* Virtuals */
	virtual void virt_pad_0() {};
	virtual auto getLineLength(class BitmapFont* font, std::string* text, float scale) -> float {};
	virtual void virt_pad_2() {};
	virtual void virt_pad_3() {};
	virtual void virt_pad_4() {};
	virtual auto drawText(class BitmapFont* font, class RectangleArea* rectArea, std::string* text, class Color* color, float param_5, class TextAlignment* textAlignment, class TextMeasureData* textMeasureData, class CaretMeasureData* caretMeasureData) -> void {};
	virtual auto flushText(float param_1) -> void {};
	virtual void virt_pad_7() {};
	virtual void drawImage() {};
	virtual void virt_pad_9() {};
	virtual void virt_pad_10() {};
	virtual void virt_pad_11() {};
	virtual void virt_pad_12() {};
	virtual auto fillRectangle(float* rect, float* color, float alpha) -> void {};
	/* Functions */
};
#endif