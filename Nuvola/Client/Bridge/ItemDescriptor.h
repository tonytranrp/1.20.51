#pragma once

class ItemDescriptor
{
public:
	char pad_0008[8]; //0x0008

	virtual ~ItemDescriptor() {};
	//virtual void serialize();
	//virtual void serialize();
}; //Size: 0x0010
static_assert(sizeof(ItemDescriptor) == 0x10);
