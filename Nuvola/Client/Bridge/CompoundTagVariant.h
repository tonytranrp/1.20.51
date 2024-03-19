#pragma once

#include "Tag.h"
#include "EndTag.h"
#include "ByteTag.h"
#include "ShortTag.h"
#include "IntTag.h"
#include "Int64Tag.h"
#include "FloatTag.h"
#include "DoubleTag.h"
#include "StringTag.h"
#include "ListTag.h"
#include "ByteArrayTag.h"
#include "IntArrayTag.h"

class CompoundTag;
#pragma pack(push, 1)
class CompoundTagVariant 
{
    char pad_0000[40]; //0x0000
    Tag::Type type; //0x0028
    char pad_002C[7]; //0x002C
public:
    inline Tag::Type getTagType() {
        return this->type;
    }
    inline Tag* asTag() {
        return (Tag*)this;
    }
    inline EndTag* asEndTag() {
        return (EndTag*)this;
    }
    inline ByteTag* asByteTag() {
        return (ByteTag*)this;
    }
    inline ShortTag* asShortTag() {
        return (ShortTag*)this;
    }
    inline IntTag* asIntTag() {
        return (IntTag*)this;
    }
    inline Int64Tag* asInt64Tag() {
        return (Int64Tag*)this;
    }
    inline FloatTag* asFloatTag() {
        return (FloatTag*)this;
    }
    inline DoubleTag* asDoubleTag() {
        return (DoubleTag*)this;
    }
    inline StringTag* asStringTag() {
        return (StringTag*)this;
    }
    inline ListTag* asListTag() {
        return (ListTag*)this;
    }
    inline CompoundTag* asCompoundTag() {
        return (CompoundTag*)this;
    }
    inline ByteArrayTag* asByteArrayTag() {
        return (ByteArrayTag*)this;
    }
    inline IntArrayTag* asIntArrayTag() {
        return (IntArrayTag*)this;
    }
};
#pragma pack(pop)

static_assert(sizeof(CompoundTagVariant) == 0x30);