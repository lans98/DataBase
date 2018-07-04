#pragma once

#include <cstring>
#include <fstream>
#include <optional>

namespace address {

    using namespace std;

    /**
     * @field disk_address, la posición hasta la que tenemos que mover el cursor
     *                      en el archivo
     * @field memory_address, dirección en memoria
     * @field using_memory, sirve para saber si estamos usando la dirección en 
     *                      memoria o en disco
     */
    template <class T>
    class Address {
    public:
        using DiskAddress = optional<size_t>;
        using MemoryAddress = T*;

    private:
        DiskAddress    disk_address;
        MemoryAddress  memory_address;
        bool           using_memory;

    public:
        Address(DiskAddress disk_address): 
            disk_address(disk_address),
            memory_address(nullptr),
            using_memory(false) {}

        Address(MemoryAddress memory_address):
            disk_address(nullopt),
            memory_address(memory_address),
            using_memory(true) {}

        Address(DiskAddress disk_address, MemoryAddress memory_address):
            disk_address(disk_address),
            memory_address(memory_address),
            using_memory(true) {}

        bool using_disk_address() { return !using_memory; }
        bool using_memory_address() { return using_memory; }
        bool exists_on_disk() { return disk_address.has_value(); }

        T* get(string file_name = "") {
            if (using_memory)        
                return memory_address;
            
            if (file_name == "") 
                return nullptr;

            if (!exists_on_disk())
                return nullptr;

            // try to open file
            ifstream file(file_name, ios::binary);
            if (!file) 
                return nullptr;

            // allocate space for a new T
            T* data = new T(); 

            // seek and read data
            file.seekg(*disk_address);
            file.read(reinterpret_cast<char*>(data), sizeof(T));

            // assign memory address and return it
            memory_address = data;
            return memory_address;
        }

        bool write(string file_name = "") {
            if (exists_on_disk())
                return true;

            if (file_name == "")
                return false;
            
            // try to open file at the end of file
            ofstream file(file_name, ios::binary | ios::ate);
            if (!file)
                return false;

            // write object to file
            file.write(reinterpret_cast<char*>(memory_address), sizeof(T));
            return true;
        }
    };
}
