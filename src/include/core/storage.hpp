#ifndef SCPPDB_STORAGE_HPP
#define SCPPDB_STORAGE_HPP

#include <config.hpp>
#include <core/bplus.hpp>
#include <core/address.hpp>

namespace storage {

    using namespace std;
    using namespace bplus;
    using namespace address;

    /*
    template <class T, size_t N>
    class BTree {
    private:

        struct Node;
            
        /**
         * Address type is just like a pointer type, 
         * but it also stores an address to disk
         
        struct Page {
            array<T, N>             page;
            array<Address<Node>, N> childs;
        };

        struct Node {
            Page page;
        };

    public:

        class Iterator {
        private:

        public:
            T& operator*() { /*return Record(); }
            T& operator->() { /*return Record(); }
           
            bool operator==(const Iterator& rhs) { return false; }
            bool operator!=(const Iterator& rhs) { return false; }
            
            Iterator operator++() { return *this; }
            Iterator operator++(int) { return *this; }
        };

        // TODO
        Iterator begin() {  return Iterator(); }

        // TODO
        Iterator end() { return Iterator(); }

        // TODO
        Iterator last() { return Iterator(); }

        // TODO 
        Iterator search(T data) { return Iterator(); }
    };
    */


    /*
    tabla.indexar_campo("apellido"){
        indice=tabla.storage["apellido"]; //acá se crea la clase RecordStorage (es el interfaz de la clase b+)
        indice->indexar_campo(this,"apellido");
    }
    tabla.storage["apellido"].search("Lux");
    tabla.storage["apellido"].insert("Lux");
    tabla.storage["apellido"].remove("Lux");
    tabla.storage["apellido"].is_empty("Lux");
    tabla.storage["apellido"].flush("Lux");
    --clase b+(RecordStorage) -- elemento de clase b+ --
    tabla.storage["apellido"].it.begin();
    tabla.storage["apellido"].it++;
    tabla.storage["apellido"].it.last();
    */

    // abstraction for manipulate rows in disk
    class RecordStorage {
    public:
        // B+ object (impl is in btree.hpp)
        // BTree btree;
        BPlus bplus;
        /*
        struct Info{//para search pos es posicion en disco del elemento hallado, i_ son los índices de los vectores
            size_t pos; 
            vector<size_t> values;            
            size_t i_value;
            vector<size_t> regs;
            size_t i_reg;
        }*/
    public:

        BPlus& get_bplus() { return bplus; }

        bool search(string key, size_t &r)
        {            
            size_t valor_hash = hash<string>{}(key);
            if(bplus.Root.get_memory_address() != NULL){
                Address<BPlus::node> hoja = bplus.getTargetNode(bplus.Root, valor_hash);
                int i;
                for(i=0;i<hoja.get_memory_address()->value.size();i++){
                    if(hoja.get_memory_address()->value[i]>=valor_hash) break;
                }
                r = hoja.get_memory_address()->regs[i][0].get_disk_address().value();
                return true;
            }
            cerr << "El índice está vacio\n";
            return false;
            
        }

        void mostrar()
        {
            bplus.bfsTraverse(bplus.Root);
        }

        //Iterator it; it.begin(): puntero a primer Pn de todas las hojas


        /*
        // TODO
        bool insert(Record) {
            return false;
        }

        // TODO
        bool remove(Record) {
            return false;
        }

        // TODO
        bool is_empty() {
            return true;
        }

        // TODO
        bool flush() {
            return true;
        }

        BTree<Record>::Iterator begin();
        BTree<Record>::Iterator last();
        BTree<Record>::Iterator end();
        BTree<Record>::Iterator search();
        */
    };
}

#endif