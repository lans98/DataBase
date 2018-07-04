#pragma once

#include <core/address.hpp>

#include <set>

namespace buffer {

    using namespace std;
    using namespace address;

    template <class T>
    class Buffer {
    private:
        set<Address<T>*> addresses;

    public:
        Buffer() = default;
        Buffer(const Buffer&) = delete;

        void push(Address<T>* address) {
        }
    };
}
