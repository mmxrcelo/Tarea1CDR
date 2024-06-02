#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    // Comprobar si se proporcionan suficientes argumentos en la línea de comandos
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <IP servidor> <puerto>" << endl;
        exit(EXIT_FAILURE);
    }

    // Obtener la dirección IP del servidor y el puerto del argumento de la línea de comandos
    const char *ip_servidor = argv[1];
    int port = atoi(argv[2]);
    int socket_cliente;
    struct sockaddr_in direccionServidor;

    // Crear un socket para el cliente
    cout << "Creando socket del cliente ..." << endl;
    if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creando el socket del cliente" << endl;
        exit(EXIT_FAILURE);
    }

    // Configurar la estructura de dirección del servidor
    cout << "Configurando estructura de dirección del servidor ..." << endl;
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(port);

    // Convertir la dirección IP de formato de cadena a binario
    if (inet_pton(AF_INET, ip_servidor, &direccionServidor.sin_addr) <= 0) {
        cout << "Dirección de servidor inválida" << endl;
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    cout << "Conectando al servidor ..." << endl;
    if (connect(socket_cliente, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Error de conexión" << endl;
        exit(EXIT_FAILURE);
    }

    // Indicar que la conexión se realizó correctamente y el cliente puede comenzar a interactuar con el servidor
    cout << "Conexión exitosa. Puedes empezar a jugar." << endl;
    cout << "Envía 'Q' para salir del juego." << endl;

    // Bucle principal para recibir y enviar mensajes al servidor
    char mensaje[1024];
    while (true) {
        cout << "Ingrese su movimiento (ej. C3): ";
        cin.getline(mensaje, 1024);

        // Enviar el mensaje al servidor
        if (send(socket_cliente, mensaje, strlen(mensaje), 0) < 0) {
            cout << "Error enviando el movimiento" << endl;
            exit(EXIT_FAILURE);
        }

        // Limpiar el búfer de mensaje
        memset(mensaje, '\0', sizeof(char)*1024);

        // Recibir la respuesta del servidor
        if (recv(socket_cliente, mensaje, 1024, 0) < 0) {
            cout << "Error recibiendo la respuesta" << endl;
            exit(EXIT_FAILURE);
        }

        // Mostrar la respuesta del servidor
        cout << "Respuesta del servidor: " << mensaje << endl;

        // Comprobar si el servidor envía un mensaje para salir del juego
        if (mensaje[0] == 'Q') {
            cout << "Saliendo del juego..." << endl;
            break;
        }
    }

    // Cerrar el socket del cliente
    close(socket_cliente);

    return 0;
}
