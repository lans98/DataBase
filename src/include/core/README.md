# Módulo core 

## Address
Es una clase especial para aplicar la persistencia en la base de datos, es casi
como un *smart pointer* guardando dos tipos de direcciones una en memoria RAM y otra en disco,
cuando no se esta usando la memoria RAM y se quiere acceder al objeto, se tiene que cargar
desde disco (si es que existe en disco), tambien se puede pasar una dirección recien
creada y luego con un método se puede escribir el objeto en disco y se guarda su posición.

## Storage 
`RecordStorage` es una interfaz para el B+ 

## B+ Tree 
	B+
		guarda un size_t(hash de dato) y dirección(dirección en buffer o en disco)
		Se tiene
			address(disk_address, memory_address)

	B+, construcción, usando el libro de silberschatz

	- Nodo [P1 K1|P2 K2|... |Pn-1 Kn-1| Pn]

	- En los nodos hoja Pn apunta a sig nodo.
	- En los internos Pn apunta a un hijo.

	-Por ahora nuestra BD solo manejará indices densos, y	la clave podrá ser cualquier campo. Si hay claves repetidas se agregarán al vector de punteros de esa key Ki, el cual es apuntado por Pi de las hojas.

	-En disco, los Pi tienen las posiciones en el mismo archivo de los bloques hijos. ojo, se len solo los bloques adecuados, no se carga todo a ram.

	- Cuando leo del disco:
		Levanto raiz.
		Cuando se levanta todo el bloque a ram, Pi es la posición en disco.	Detecto Kj comparando con las otras claves, y con Pj voy al disco y genero un nuevo nodo hijo pero cambio el Pj anterior con la dir de memoria de este nodo.

	-Se escribe en disco desde el nodo raíz hasta las hojas y el vector de estas hojas.

	-En disco:
		------------------
		- 0 P1 K1 P2 K2 ... Pn-1 Kn-1 Pn
		- 0 P1 K1 P2 K2 ... Pn-1 Kn-1 Pn
		- ...
		- 0 P1 K1 P2 K2 ... Pn-1 Kn-1 Pn
		- 1 P1 K1 P2 K2 ... Pn-1 Kn-1 Pn
		- cant_ptrs ptr_reg1 ptr_reg2 ptr_reg3
		- cant_ptrs ptr_reg1 ptr_reg2 
		- ...
		- cant_ptrs ptr_reg1 ptr_reg2 ptr_reg3 ptr_reg4
		----------------------

	-Para escribir los nodos a disco:
		Nodo interno:
		Si P1 apunta a algo, calcular la futurización de la posición en disco del nodo apuntado por P1, escribir este valor en archivo, escribir K1... hasta Pn.
			Futurización de Pi = (orden de aparición de Pi en todo el árbol != NULL)*tamaño de bloque; 1<i<n
		Nodo hoja:
		Si P1 apunta a algo, calcular la futurización Fp de su nodo, escribirla, luego escribir K1.... hasta Kn-1.
			ojo: Fp no va de 1 a n pues es una variable que recorre todas las ocurrencias de Futurizaciones de Pi's a través de todas las hojas, así puede ir de 1 a cualquier valor.
			F1 = (última Futurización en los nodos internos + 1)*tamaño de bloque
			Fp = Fp-1 + (size_t)(1+tamaño de vector de p-1); p>1
		Para los Pn de cada nodo hoja:
			Futurización de Pn:
			Dado que se está escribiendo los nodos según los punteros de los nodos padres, Pn tomará la misma futurización que ya tiene el padre para su hermano derecho.

	- Detalles, los nodos internos del B+ siempre van a tener un puntero a la derecha (en Pn+1), se cumple esta invariante ya que el árbol es balanceado.

	- Inserción en B+:
		- Aplicamos buscar key en el B+. Retorna las direcciones en disco donde están los registros que contienen la key, y la dirección en memoria donde está el puntero a la hoja que contiene el key, lo llamaremos puntero padre (Pp), el cual es un puntero doble.
		- Si la hoja está llena antes de insertar:
			- Tenemos Pp y hoja hija llena.
			- La hoja hija ahora se convertirá en dos hijos, que contendrán los keys antiguos y la nueva a insertar. Así tendremos un hijo mayor y uno menor.
			- Pero tenemos un padre que puede apuntar solo a uno de ellos, ¿a cuál apunta? al mayor, y que una copia de la primera key de este hijo mayor suba, con su puntero, a la izquierda de su padre pero que no descuide (que su puntero apunte) a su hermano menor. Tener en cuenta que no deben haber keys internas repetidas, y notamos que al subir un hijo se mantiene esto.
			- Recordar que Pi Ki significa que lo que apunta Pi es menor que Ki. El nodo es una concatenación de varios Ki Pi. Pensar en esto y NO que en | Pi Ki | Pi+1 Ki+1 | lo que apunta Pi+1 es mayor o igual a Ki.
			- Ahora, si al subir el hijo mayor no hay espacio para él (nodo interno lleno):
				- Es casi lo mismo de lo que se hizo si quien está llena es la hoja. Solo que el hijo mayor quita de su nodo la primera key, y su puntero asociado, quien es la que sube al nodo padre; esto porque se busca que no hayan nodos internos repetidos.
			- En lugar de punteros dobles creo que será menjor que los nodos guarden un puntero a su padre.



