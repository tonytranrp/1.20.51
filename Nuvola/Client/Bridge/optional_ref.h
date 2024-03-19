#pragma once

//Sourced from LiteBDS
//https://cpp.docs.litebds.com/api/d5/d34/Types_8hpp_source.html

template <typename T>
class optional_ref {
    T* value;

public:
    inline T* get() const {
        if (*this)
            return value;
        return nullptr;
    }
    // inline T* set(T const& val)
    //{
    //     *value = &val;
    // }
    inline T& operator*() const {
        return *value;
    }
    inline T* operator->() const {
        return value;
    }
    inline operator bool() const {
        return value != nullptr;
    }
};