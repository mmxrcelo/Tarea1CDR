1- Configuración del servidor: El servidor crea un socket y lo enlaza a una dirección IP y puerto específicos para escuchar las conexiones entrantes de los clientes.

2- Espera de conexiones: Una vez que el servidor está configurado, entra en un bucle infinito para esperar las solicitudes de conexión de los clientes.

3- Manejo de clientes: Cuando un cliente se conecta, se crea un hilo para manejar la comunicación con ese cliente. Esto permite que el servidor atienda a múltiples clientes de manera simultánea.

4- Interacción con los clientes: El servidor recibe los mensajes enviados por los clientes, los procesa según el protocolo del juego y envía respuestas apropiadas. En este caso, el servidor espera comandos de los clientes, como "C" para indicar un movimiento en el juego o "Q" para salir del juego.

5- Finalización de la conexión: Cuando un cliente envía el comando para salir del juego, el servidor cierra la conexión con ese cliente y espera nuevas conexiones entrantes.
