# Módulo Entity

## Entity
Sirve para aplicar polimorfismo a las demás clases: `Field, Record, Table, DataBase` que
son *entidades*, lo especial de `Entity` es que tienen su propio identificador (que sea 
generan aleatoriamente la primera vez que se crea cualquier entidad, y luego cuando se lee
de disco, esta labor es realizada por `EntityIDManager`). Estos requerimientos son necesarios 
para implementar el `LockManager`. 

## Entity ID Manager
Es un objeto *singleton* usado para guardar los identificadores únicos de las entidades
existentes hasta el momento, junto con sus tipo de entidad (si es que es `Field, Record,
Table, DataBase`) y el identificador de su padre.

Padres de las entidades:

```
Field, Record: Table
Table: DataBase
DataBase: Unknown
```

## Field 
Representa los posibles campos en un Tabla (i.e. columnas), y guarda el nombre del campo 
junto con el tipo de dato de ese campo:

FirstName | LastName | Phone
------------------------------
   ...    |   ...    |  ...

Donde `FirstName, LastName, Phone` podrian guardar un tipo de dato `STRING` (vease el
código para ver todos los posibles tipos).

## Record 
Representa una fila, guardando Tipos de Datos (`DataType`) que es una variante de
diferentes tipos de datos como ints, doubles, strings, etc..

## Table
Representación de una Tabla, con un vector de Campos (`vector<Field>`), un nombre
carácteristico y un mapa de `map<string, RecordStoragePtr>` que asocia nombres de campos
con `RecordStoragePtr`, que son una interfaz para el árbol B+.

Tabla tiene varios metodos para realizar queries (`selection, projection, union,
etc...`).

Esta clase también permite indexar el campo que se  quiere; por ejemplo, al hacer ```Table machine_attributes(name,fields)``` se indexaría el campo "value" haciendo ```machine_attributes.index_field("value")```. Y podríamos indexar primary keys simples o compuestas 
```
vector<string> primary_key;
 
primary_key.push_back("timestamp");
primary_key.push_back("id");
    
tabla_persona.set_primary_key(primary_key); //compuesta 
```
Indexaríamos la pk con `tabla_persona.index_pk();`
Ojo, cada tabla tiene su storage(su índice), por lo tanto pueden existir `tabla_alumnos.storage["nombre"];` y `tabla_profesores.storage["nombre"];`

Además nos da los métodos para haccer los queries como projection, selection, intersectionTable, insert, delete, update (estos últimos aún incompletos).

### Algebra Relacional:
- Projection: Es la función de proyección de la tabla que imita a la operación homónima en el algebra relacional.
En el parámetro necesita un vector de los campos (Field) que van a retornar en la nueva tabla result. `Table projection(vector<Field>)`
- Selection: Existen dos maneras de seleccionar Records (el conjunto de Records forma una tabla) de una Tabla:
`selection(Field, Field, tipo de comparacion)` -> Existen (Field) "dato" comparables con (Field) "otro dato" ambos del mismo tipo?

La función selection hace una búsqueda de ocurrencias de datos del primer campo en el segundo, en la misma tupla. Para las ocurrencias, se guardaran las tuplas del registro obtenido en tal iteración.
`template<algun tipo T>selection(Field, T,tipo de comparacion)` -> Existen (Field) "dato" comparables con "otro dato" ambos del mismo tipo?

La función de selection recorre el campo Field comparándolo con una constante. El registro donde se encuentra el dato i del campo selecionado se guarda en la nueva tabla.
 Selection puede anidar otras "selections" con las funciones Union y Intersection. En el algebra relacional son comparables con OR y AND. Como ejemplos:

      `Table Intersection(Table)`
                ´´´SELECT EDAD
                FROM TABLA PERSONA
                WHERE EDAD < 100 && EDAD > 10´´´
  en dos selects: `Select(EDAD,10,>).Intersection(Select(EDAD,100,<))`.

      `Table Union(Table)`
                ´´´SELECT EDAD
                FROM TABLA PERSONA
                WHERE EDAD > 50 || EDAD < 10´´´
  en dos selects: `Select(EDAD,10,>).Union(Select(EDAD,100,<))`.

- Insert: Inserta una tupla nueva en la tabla, consultando si la primary key de la nueva tupla ya existe en la tabla, entonces no podría. `bool insert(Record, pk)`

- Update: Usa el método delete, buscando con la primary key y si la encuentra, la configura como "borrada" e inserta la nueva tupla. `bool update(Record, pk)`

- Delete: Si la primary key del la tupla es encontrado, configura ese Record como "borrado" `delete(Record, pk)`
