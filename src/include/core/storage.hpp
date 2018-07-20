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
        
        struct Info{
            //info: pos-posicion de la fila que contiene"luca en el archivo tabla
            //      values-vector de las key, entre ellas, "luca",
            //      i_value-key iesima del nodo hoja que contien "luca",
            //      registros-vector de registros (mejor dicho, de las posiciones donde inicia el siguiente registro que contiene luca),
            //      i_registro- posicion de cierto registro que contiene la posicion de key en el archivo
            //      regs-vector de vector de registros que va emparejado con values
            //      i_reg- iesimo vector de registros
            //      last, indica siguiente nodo hoja
            size_t pos; 
            vector<size_t> *values;            
            unsigned int i_value;
            vector<BPlus::type_reg> *registros;
            unsigned int i_registro;
            vector<vector<BPlus::type_reg>> *regs;
            unsigned int i_reg;
            Address<BPlus::node> last;
        };

    public:

        BPlus& get_bplus() { return bplus; }

        bool search(string key, Info &info)
        {            
            size_t valor_hash = hash<string>{}(key);
            if(bplus.Root.get_memory_address() != NULL){
                Address<BPlus::node> hoja = bplus.getTargetNode(bplus.Root, valor_hash);
                if(hoja.get_memory_address() != NULL){
                    int i;
                    for(i=0;i<hoja.get_memory_address()->value.size();i++){
                        if(hoja.get_memory_address()->value[i]>=valor_hash) break;
                    }
                    info.pos = hoja.get_memory_address()->regs[i][0].get_disk_address().value();
                    info.values = &hoja.get_memory_address()->value;
                    info.i_value = i;
                    info.regs = &hoja.get_memory_address()->regs;
                    info.i_reg = i;
                    info.registros = &hoja.get_memory_address()->regs[i];
                    info.i_registro = 0;
                    info.last = hoja.get_memory_address()->last;
                    return true;
                }
            }
            cerr << "El índice está vacio\n";
            return false;            
        }

        bool next_registro(Info &info)
        {
            if(info.i_registro < info.registros->size()-1){//legaría hasta el último registro
                info.pos = (*info.registros)[info.i_registro+1].get_disk_address().value();
                info.i_registro++;
                return true;
            }else{//está en el último registro
                cerr << "No hay siguiente registro.\n";
                //info sigue siendo el mismo que el anterior
            }
        }
        
        bool next_key(Info &info)
        {
            if(info.i_value < info.values->size()-1){//si hay una siguiente key en el nodo
                info.i_registro = 0;
                info.i_value++;
                info.i_reg++;
                info.registros = &(*info.regs)[info.i_reg];
                info.pos = (*info.registros)[0].get_disk_address().value();
                return true;
            }else 
            if(info.last.get_memory_address() != NULL){ //toma la key del siguiente nodo
                info.i_value = 0;
                info.i_reg = 0;
                info.values = &info.last.get_memory_address()->value;
                info.regs = &info.last.get_memory_address()->regs;
                info.registros = &(*info.regs)[0];
                info.i_registro = 0;
                info.last = info.last.get_memory_address()->last;
                info.pos = (*info.regs)[0][0].get_disk_address().value();
                return true;
            }else
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