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
    //indexar_pk;
    tabla_persona.index_field("apellido");
    //tabla_persona.storage["apellido"]->search ..->begin()
    //tabla_persona.storage["apellido"]->mostrar();
    vector<size_t> pos_disco;
    size_t pos;
    RecordStorage::Info info;
    //info: pos-posicion de la fila que contiene"luca en el archivo tabla
    //      values-vector de las key, entre ellas, "luca",
    //      i_value-key iesima del nodo hoja que contien "luca",
    //      regs-vector de registros (mejor dicho, de las posiciones donde inicia el siguiente registro que contiene luca),
    //      i_reg- registro iesimo que contiene "luca"
    tabla_persona.storage["apellido"]->search("luca",info);
    //tabla_persona.storage["apellido"]->next_registro(info);
    //tabla_persona.storage["apellido"]->next_key(info);
    
    
    return 0;
}



