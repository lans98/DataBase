### Table
  en Table modificaría storage para que en lugar de manejar un único índice maneje varios pues varios campos pueden estar indizados map<Field, RecordStoragePtr> storage;//storage de índices
	así, para hacer la proyección se haría
		storage[Field]->begin();
	ojo, cada tabla tiene su storage, por lo tanto pueden existir tabla_alumnos.storage["nombre"]; y tabla_profesores.storage["nombre"];
