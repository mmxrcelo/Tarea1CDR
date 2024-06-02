1- El cliente se conecta a un servidor remoto utilizando el protocolo TCP/IP.

2- El cliente toma la dirección IP del servidor y el puerto como argumentos de la línea de comandos al ejecutarse.

3- Se crea un socket para la comunicación con el servidor.

4- Se configura la dirección del servidor utilizando la dirección IP y el puerto proporcionados.

5- El cliente intenta conectarse al servidor utilizando la función connect().

6- Una vez conectado, el cliente puede enviar mensajes al servidor y recibir respuestas.

7- En un bucle infinito, el cliente espera que el usuario ingrese su movimiento en el juego (por ejemplo, "C3").

8- El cliente envía el movimiento al servidor utilizando el socket.

9- Luego, espera y recibe la respuesta del servidor.

10- Si el servidor envía un mensaje para salir del juego, el cliente sale del bucle y finaliza la conexión con el servidor.

