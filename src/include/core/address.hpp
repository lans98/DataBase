#pragma once

#include <cstring>
#include <fstream>
#include <optional>
#include <iostream>

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
        Address(): 
            disk_address(nullopt),
            memory_address(nullptr),
            using_memory(false) {}

        Address(DiskAddress disk_address): 
            disk_address(disk_address),
            memory_address(nullptr),
            using_memory(false) {}

        Address(MemoryAddress memory_address):
            disk_address(nullopt),
            memory_address(memory_address),
            using_memory(memory_address) {}

        Address(DiskAddress disk_address, MemoryAddress memory_address):
            disk_address(disk_address),
            memory_address(memory_address),
            using_memory(memory_address) {}

        bool using_disk_address() { return !using_memory; }
        bool using_memory_address() { return using_memory; }
        bool exists_on_disk() { return disk_address.has_value(); }
        bool exists_on_memory() { return memory_address; }

        DiskAddress get_disk_address() {
            return disk_address;
        }

        void set_disk_address(DiskAddress disk_address) {
            this->disk_address = disk_address;
        }

        MemoryAddress get_memory_address() {
            return memory_address;
        }

        void set_memory_address(MemoryAddress memory_address) {
            this->memory_address = memory_address;
        }

        T* read(string file_name = "") {
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
            ofstream file(file_name, ios::app);
            if (!file)
                return false;
            
            // write object to file
            disk_address = file.tellp();
            file.write(reinterpret_cast<char*>(memory_address), sizeof(T));
            return true;
        }
    };
}
