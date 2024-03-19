#pragma once

#include "CompoundTagVariant.h"
#include "Tag.h"

#include <map>
#include <string>

class CompoundTag
{
public:
	std::map<std::string, CompoundTagVariant> data;

	virtual ~CompoundTag();
    virtual void deleteChildren(void);
    virtual void write(class IDataOutput&);
    virtual void load(IDataInput&);
    virtual std::string toString() const;
    virtual Tag::Type getId() const;
    virtual bool equals(Tag const&) const;
    virtual void print(std::string const&, class PrintStream&) const;
    virtual std::unique_ptr<Tag> copy() const;
    virtual unsigned __int64 hash() const;
}; //Size: 0x0018
static_assert(sizeof(CompoundTag) == 0x18);