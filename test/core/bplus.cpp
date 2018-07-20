#include <core/bplus.hpp>
#include <entity/field.hpp>
#include <entity/table.hpp>

using namespace std;
using namespace field;
using namespace table;

int main(){
    /*
    bmas<int> indice;
    printf("How many values in each node?\n");
    FILE *fp;
    fp = fopen("input.txt","r");
    int w,v;
    fscanf(fp,"%d",&w);
    v = w-1;


    printf("nPointer = %d\n",indice.nPointer);
    int k; int x=1;
    while(fscanf(fp,"%d",&k)!=EOF){
        printf("val: %d\n",k);
        bmas<int>::type_reg reg(x++,NULL);
        indice.insert(k,reg);
    }
    while(true){
        printf("Action: \npress 1 to insert\npress 2 to print in tree structure\npress 3 for delete\npress 0 for exit\n");
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            bmas<int>::type_key value;
            scanf("%d",&value);            
            bmas<int>::type_reg reg2(x++,NULL);
            indice.insert(value,reg2);
        }else if(choice==2){
            printf("\n\n\n");
            indice.bfsTraverse(indice.Root);
            printf("\n\n\n");
        }else if(choice==3){
            bmas<int>::type_key delV;
            scanf("%d",&delV);
            indice.delet(delV,NULL);
        }else if(choice==0) break;
    }
    fclose(fp);
    */
    
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