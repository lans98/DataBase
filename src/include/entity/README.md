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

Esta clase también permite indexar el campo que se  quiere; por ejemplo, al hacer ---Table machine_attributes(name,fields) se indexaría el campo "value" haciendo machine_attributes.index_field("value". Y podríamos indexar la )---
ojo, cada tabla tiene su storage, por lo tanto pueden existir tabla_alumnos.storage["nombre"]; y tabla_profesores.storage["nombre"];

