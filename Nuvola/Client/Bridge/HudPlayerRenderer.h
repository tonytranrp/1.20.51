#include "../Signatures/SigManager.h"

class MinecraftUIRenderContext;
class ClientInstance;
class UIControl;
class Rect;

class HudPlayerRenderer
{
public:
	// string: variable.is_first_person
	// the function on client has a shit ton of switch statements so i guess that could help aswell
	unsigned __int64 render(MinecraftUIRenderContext* a1, ClientInstance* a2, UIControl* a3, int a4, Rect* a5)
	{
		auto func = (unsigned __int64(__thiscall*)(HudPlayerRenderer*, MinecraftUIRenderContext*, ClientInstance*, UIControl*, int, Rect*))SigRegistry::FindAddressOf(SigTable::HudPlayerRenderer_render);
		return func(this, a1, a2, a3, a4, a5);
	}
};