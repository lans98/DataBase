# Módulo Concurrency

## Concurrency manager
Pensado para ser un *singleton* donde se puedan preparar nuevas transacciones y pedir
permisos al momento de ejecutar transacciones en paralelo. *unimplemented*

## Lock manager
Es un administrador básico de bloqueos, puede bloquear cualquier entidad por su
identificador único, ya que `EntityIDManager` ofrece herramientas para saber el tipo de
entidad de un identificador, y su padre. El `LockManager` usa módelo de árbol y bloqueos 
implícitos para saber si una nueva petición puede ser aceptada o rechazada y puesta en
espera hasta que los bloqueos en conflicto sean liberados.

