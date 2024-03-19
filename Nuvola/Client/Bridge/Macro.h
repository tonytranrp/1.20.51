#pragma once

#include "../SystemManager.h"
#include "../Offsets/OffsetManager.h"

#define VIRT_PAD_EX(ID) virtual void PaddingFunc_##ID() {}
#define VIRT_PAD_X(ID) VIRT_PAD_EX(ID)
#define VIRT_PAD VIRT_PAD_X(__LINE__)

template<int index, typename RetT, typename ThisT, typename... ArgsT>
RetT vftCall(const void* selfPtr, ArgsT... args)
{
	const void** vftable = *((const void***)selfPtr);
	assert(vftable != nullptr);
	const void* vftfunc = vftable[index];
	assert(vftfunc != nullptr);

	return ((RetT(__thiscall*)(const void*, ArgsT...))vftfunc)(selfPtr, args...);
}

template<OffsetTable offsetId, typename RetT, typename ThisT, typename... ArgsT>
RetT vftCall(const void* selfPtr, ArgsT... args)
{
	const uint64_t vftable = *reinterpret_cast<const uint64_t*>(selfPtr);
	assert(vftable != 0);
	int32_t offset = systems->getOffsetManager()->findOffsetOf(offsetId);
	uint64_t vftfunc = vftable + offset;
	assert(vftfunc != 0);

	//Call and return like a REAL man
	return ((RetT(__thiscall*)(const void*, ArgsT...))*reinterpret_cast<void**>(vftfunc))(selfPtr, args...);
}

//Class to define the size of a class
template<size_t size> class Sizer { uint8_t _[size]; };
template<> class Sizer<0> {};

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name

#define DEFINE_MEMBER(type, name, offsetId) \
AS_FIELD(type, name, get##name); \
type& get##name##() const \
{ \
	static int32_t offset = systems->getOffsetManager()->findOffsetOf(offsetId); \
	uintptr_t cursor = reinterpret_cast<uintptr_t>(this); \
	cursor += offset; \
	type& memberPtr = *reinterpret_cast<type*>(cursor); \
	return memberPtr; \
} \
void set##name##(type value) const \
{ \
    static int32_t offset = systems->getOffsetManager()->findOffsetOf(offsetId); \
    uintptr_t cursor = reinterpret_cast<uintptr_t>(this); \
    cursor += offset; \
    type& memberPtr = *reinterpret_cast<type*>(cursor); \
    memberPtr = value; \
}