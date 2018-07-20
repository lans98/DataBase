#include <core/bplus.hpp>
#include <entity/field.hpp>
#include <entity/table.hpp>

using namespace std;
using namespace field;
using namespace table;

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
    //indexar_pk;
    tabla_persona.index_field("apellido");
    //tabla_persona.storage["apellido"]->search ..->begin()
    //tabla_persona.storage["apellido"]->mostrar();
    vector<size_t> pos_disco;
    size_t pos;/*
    Address<BPlus::nodo>  ram_hoja;
    Address<BPlus::nodo>  ram_i;
    Info info;//pos, values, regs, i_values, i_regs
    tabla_persona.storage["apellido"]->search("luca",info);//pos: posición en el archivo del apellido, row: 

    tabla_persona.storage["apellido"]->next_row(info);
    tabla_persona.storage["apellido"]->next_value(info);*/
    
    return 0;
}



