#pragma once

#include "Macro.h"

#include <memory>
#include <string>
#include <stdint.h>

class Tag
{
public:
    enum class Type : uint8_t {
        End,
        Byte,
        Short,
        Int,
        Int64,
        Float,
        Double,
        ByteArray,
        String,
        List,
        Compound,
        IntArray,
    };

    virtual ~Tag();
    virtual void deleteChildren();
    VIRT_PAD;
    virtual void load(class IDataInput&);
    virtual std::string toString();
    virtual Type getId();
    virtual bool equals(const Tag&) const;
    virtual void print(class PrintStream&) const;
    virtual void print(std::string const&, class PrintStream&) const;
    virtual std::unique_ptr<Tag> copy() const = 0;
    VIRT_PAD; //virtual unsigned int64_t hash() const = 0;
};