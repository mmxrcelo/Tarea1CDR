#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <IP servidor> <puerto>" << endl;
        exit(EXIT_FAILURE);
    }

    const char *ip_servidor = argv[1];
    int port = atoi(argv[2]);
    int socket_cliente;
    struct sockaddr_in direccionServidor;

    cout << "Creando socket del cliente ..." << endl;
    if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creando el socket del cliente" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Configurando estructura de dirección del servidor ..." << endl;
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_servidor, &direccionServidor.sin_addr) <= 0) {
        cout << "Dirección de servidor inválida" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Conectando al servidor ..." << endl;
    if (connect(socket_cliente, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Error de conexión" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Conexión exitosa. Puedes empezar a jugar." << endl;
    cout << "Envía 'Q' para salir del juego." << endl;

    char mensaje[1024];
    while (true) {
        cout << "Ingrese su movimiento (ej. C3): ";
        cin.getline(mensaje, 1024);

        if (send(socket_cliente, mensaje, strlen(mensaje), 0) < 0) {
            cout << "Error enviando el movimiento" << endl;
            exit(EXIT_FAILURE);
        }

        memset(mensaje, '\0', sizeof(char)*1024);

        if (recv(socket_cliente, mensaje, 1024, 0) < 0) {
            cout << "Error recibiendo la respuesta" << endl;
            exit(EXIT_FAILURE);
        }

        cout << "Respuesta del servidor: " << mensaje << endl;

        if (mensaje[0] == 'Q') {
            cout << "Saliendo del juego..." << endl;
            break;
        }
    }

    close(socket_cliente);

    return 0;
}

