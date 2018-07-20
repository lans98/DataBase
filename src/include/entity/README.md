## TABLE

  - ### Algebra Relacional:
- projection: Es la función de proyección de la tabla que imita a la operación homónima en el algebra relacional.
En el parámetro necesita un vector de los campos (Field) que van a retornar en la nueva tabla result. `Table projection(vector<Field>)`
- selection: Existen dos maneras de seleccionar Records (el conjunto de Records forma una tabla) de una Tabla:
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

 - insert: Inserta una tupla nueva en la tabla, consultando si la primary key de la nueva tupla ya existe en la tabla, entonces no podría. `bool insert(Record, pk)`

 - update: Usa el método delete, buscando con la primary key y si la encuentra, la configura como "borrada" e inserta la nueva tupla. `bool update(Record, pk)`

 - delete: Si la primary key del la tupla es encontrado, configura ese Record como "borrado" `delete(Record, pk)`
