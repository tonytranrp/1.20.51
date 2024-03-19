#pragma once

#include "SharedCounter.h"

//Source: https://gist.githubusercontent.com/MCMrARM/180c743f7882f40ad1b993bdc78dca28/raw/3236a6637b0070789d0cb4543200675633bc6b81/Memory.h

template<typename T>
struct SharedPtr {
    SharedCounter<T>* counter = nullptr;

    SharedPtr(T* val = nullptr) {
        if (val) {
            counter = new SharedCounter<T>(val);
            counter->addSharedRef();
        }
    }

    SharedPtr(SharedPtr const& a) : counter(a.counter) {
        if (counter)
            counter->addSharedRef();
    }

    void reset() {
        if (counter && counter->releaseSharedRef())
            delete counter;
        counter = nullptr;
    }

    SharedPtr& operator=(SharedPtr const& ptr) {
        reset();
        this->counter = ptr.counter;
        if (counter)
            counter->addSharedRef();
        return *this;
    }

    ~SharedPtr() {
        reset();
    }

    template <typename... Args>
    static SharedPtr<T> make(Args&&... args) {
        return SharedPtr<T>(new T(std::forward(args...)));
    }

    T& operator*() {
        return *counter->value;
    }

    T* operator->() {
        return counter->value;
    }

    T* get() {
        if (!counter)
            return nullptr;
        return counter->value;
    }
};