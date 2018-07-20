### Table
Esta clase también permite indexar el campo que se  quiere; por ejemplo, al hacer ```Table machine_attributes(name,fields)``` se indexaría el campo "value" haciendo ```machine_attributes.index_field("value")```. Y podríamos indexar primary keys simples o compuestas 
 ```vector<string> primary_key;
 
    primary_key.push_back("timestamp");
    primary_key.push_back("id");
    
    tabla_persona.set_primary_key(primary_key); //compuesta 
 ```
indexaríamos la pk con ```tabla_persona.index_pk();```

ojo, cada tabla tiene su storage(su índice), por lo tanto pueden existir ```tabla_alumnos.storage["nombre"];``` y ```tabla_profesores.storage["nombre"];```

Además nos da los métodos para haccer los queries como projection, selection, intersectionTable, insert, delete, update (estos últimos aún incompletos). 
