#ifndef SCPPDB_ADDRESS_HPP
#define SCPPDB_ADDRESS_HPP

#include <cstring>
#include <fstream>

namespace address {

    using namespace std;

    /**
     * @field disk_offset, si el archivo es muy grande, representa un offset de 
     *                     ULONG::MAX * disk_offset
     * @field disk_address, la posición hasta la que tenemos que mover el cursor
     *                      en el archivo
     * @field memory_address, dirección en memoria
     * @field using_memory, sirve para saber si estamos usando la dirección en 
     *                      memoria o en disco
     */
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

        T* get(string file_name = "") {
            if (using_memory)        
                return memory_address;
            
            // TODO: podemos usar sizeof(T) para reconocer el objeto 
            // en disco, pero tendria que estar en binario en el disco
            if (file_name == "") 
                return nullptr;

            ifstream file(file_name, ios::binary);
            T* data; 
            // TODO: Read binary info an put it in data
            T* newt = new T();
            memcpy(newt, data, sizeof(T));
            memory_address = newt;
        }

    };
}

#endif
