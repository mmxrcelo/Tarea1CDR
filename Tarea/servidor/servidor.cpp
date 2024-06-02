#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

mutex mtx;

void jugar(int socket_cliente, struct sockaddr_in direccionCliente) {
    char buffer[1024];
    memset(buffer, '\0', sizeof(char)*1024);
    int n_bytes = 0;

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(direccionCliente.sin_addr), ip, INET_ADDRSTRLEN);

    cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Nuevo jugador." << endl;

    while ((n_bytes = recv(socket_cliente, buffer, 1024, 0))) {
        buffer[n_bytes] = '\0';

        if (buffer[0] == 'Q') {
            cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Sale del juego." << endl;
            close(socket_cliente);
            break;
        }

        switch (buffer[0]) {
            case 'C':
                {
                    string line(&buffer[0]);
                    cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Columna: " << line[2] << endl;
                    send(socket_cliente, "ok\n", 3, 0);
                    break;
                }
            default:
                send(socket_cliente, "error\n", 6, 0);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <puerto>" << endl;
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int socket_server = 0;
    struct sockaddr_in direccionServidor, direccionCliente;

    cout << "Creando socket de escucha ..." << endl;
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creando el socket de escucha" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Configurando estructura de dirección del socket ..." << endl;
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port = htons(port);

    cout << "Enlazando el socket ..." << endl;
    if (bind(socket_server, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Error en bind()" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Haciendo que el socket esté escuchando ..." << endl;
    if (listen(socket_server, 1024) < 0) {
        cout << "Error en listen()" << endl;
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size;
    addr_size = sizeof(struct sockaddr_in);

    cout << "Esperando solicitudes de clientes ..." << endl;

    vector<thread> threads;

    while (true) {
        int socket_cliente;

        if ((socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size)) < 0) {
            cout << "Error en accept()" << endl;
            exit(EXIT_FAILURE);
        }

        mtx.lock();
        threads.emplace_back(jugar, socket_cliente, direccionCliente);
        mtx.unlock();
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}

