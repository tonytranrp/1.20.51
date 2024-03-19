#pragma once

template<typename T>
class OwnerPtrT {
	T* value;
public:
	size_t unknown;
private:
	bool valid;

public:
    inline T* get() {
        if (value)
            return value;
        return nullptr;
    }
    inline T const* get() const {
        if (value)
            return value;
        return nullptr;
    }
    inline T& operator*() {
        return get();
    }
    inline T const& operator*() const {
        return get();
    }
    inline operator bool() const {
        return get() != nullptr;
    }

    bool isValid() const {
        return this->valid;
    }
};