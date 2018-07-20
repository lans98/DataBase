#include <core/bplus.hpp>
#include <entity/field.hpp>
#include <entity/table.hpp>

using namespace std;
using namespace field;
using namespace table;
using namespace bplus;
using namespace storage;

int main(){
    
    string name="persona";
    vector<Field> fields;
    fields.push_back(Field("timestamp",STRING,true));
    fields.push_back(Field("id",UINT,true));
    fields.push_back(Field("apellido",STRING,true));
    fields.push_back(Field("género",STRING,true));
    fields.push_back(Field("eliminado",BOOL,true));
    vector<string> primary_key;
    primary_key.push_back("timestamp");
    primary_key.push_back("id");
    Table tabla_persona(name, fields);
    tabla_persona.set_primary_key(primary_key);
    tabla_persona.index_pk();
    //indexar_pk;
    string pk("");
    for(int i=0; i<primary_key.size();i++){
        pk += primary_key[i];
    }
    //tabla_persona.storage[pk]->mostrar();
    //
    tabla_persona.index_field("apellido");
    vector<size_t> pos_disco;
    size_t pos;
    cerr<<"------------------------------\n";
    RecordStorage::Info info;
            //info: pos-posicion de la fila que contiene"luca en el archivo tabla
            //      values-vector de las key, entre ellas, "luca",
            //      i_value-key iesima del nodo hoja que contien "luca",
            //      registros-vector de registros (mejor dicho, de las posiciones donde inicia el siguiente registro que contiene luca),
            //      i_registro- posicion de cierto registro que contiene la posicion de key en el archivo
            //      regs-vector de vector de registros que va emparejado con values
            //      i_reg- iesimo vector de registros
            //      last, indica siguiente nodo hoja
    if(tabla_persona.storage["apellido"]->search("luca",info)){
        cerr <<"Posición de 'luca', "<<info.pos << "\n";
        if(tabla_persona.storage["apellido"]->next_registro(info)){
            cerr <<"Posición de siguiente 'luca', "<<info.pos << "\n";
        }
        tabla_persona.storage["apellido"]->next_key(info);
        cerr <<"Detalle, "<<info.pos << "\n";
    }
    cerr<<"------------------------------\n";
    string i_pk("2222222222222,0812308123");
    if(tabla_persona.storage[pk]->search(i_pk,info)){
        cerr <<"Posición de "<<i_pk<<": "<<info.pos << "\n";
        if(tabla_persona.storage[pk]->next_registro(info)){
            cerr <<"Posición de siguiente "<<i_pk<<": "<<info.pos << "\n";
        }
        tabla_persona.storage[pk]->next_key(info);
        cerr <<"Detalle, "<<info.pos << "\n";
    }

    return 0;
}



