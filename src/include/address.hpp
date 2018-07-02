#ifndef SCPPDB_ADDRESS_HPP
#define SCPPDB_ADDRESS_HPP

namespace address {

    template <class T>
    class Address {
    private:
        size_t disk_offset;
        size_t disk_address;
        T*     memory_address;
        bool   using_memory;

    public:
        Address(size_t disk_address, size_t disk_offset = 0): 
            disk_offset(disk_offset),
            disk_address(disk_address),
            memory_address(nullptr),
            using_memory(false) {}

        Address(T* memory_address):
            disk_offset(0UL),
            disk_address(0UL),
            memory_address(memory_address),
            using_memory(true) {}

        Address(size_t disk_address, size_t disk_offset, T* memory_address):
            disk_offset(disk_offset),
            disk_address(disk_address),
            memory_address(memory_address),
            using_memory(true) {}

        bool using_disk_address() { return !using_memory; }
        bool using_memory_address() { return using_memory; }

        T* get(const char* file = "") {
            if (using_memory)        
                return memory_address;
            
            // parse type T
            // TODO
        }

    };
}

#endif
