-
#include <arpa/inet.h>      
#include <string.h>         
#include <unistd.h>         
#include <iostream>
#include <thread>           
#include <vector>
#include <mutex>

using namespace std;

mutex mtx; 

// Función que maneja la conexión con un cliente
void jugar(int socket_cliente, struct sockaddr_in direccionCliente) {
    char buffer[1024]; 
    memset(buffer, '\0', sizeof(char)*1024); 
    int n_bytes = 0; 

    char ip[INET_ADDRSTRLEN]; 
    inet_ntop(AF_INET, &(direccionCliente.sin_addr), ip, INET_ADDRSTRLEN); 

    cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Nuevo jugador." << endl; 

    // Bucle para recibir mensajes del cliente
    while ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0) {
        buffer[n_bytes] = '\0'; 
                
        if (buffer[0] == 'Q') { // Si el cliente envía 'Q', termina la conexión
            cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Sale del juego." << endl;
            close(socket_cliente); // Cerrar el socket del cliente
            break; // Salir del bucle
        }

        switch (buffer[0]) {
            case 'C': 
                {
                    string line(&buffer[0]); 
                    cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Columna: " << line[2] << endl; 
                    send(socket_cliente, "ok\n", 3, 0); 
                    break; // Salir del switch
                }
            default:
                send(socket_cliente, "error\n", 6, 0); 
        }
    }

    
    close(socket_cliente);
}

int main(int argc, char **argv) {
    // Comprobar si se proporciona el puerto como argumento en la línea de comandos
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <puerto>" << endl;
        exit(EXIT_FAILURE); // Salir del programa con un código de error
    }

    int port = atoi(argv[1]); // Convertir el argumento del puerto a un entero
    int socket_server = 0; 
    struct sockaddr_in direccionServidor, direccionCliente; // Estructuras para las direcciones del servidor y cliente
    
    // Crear un socket para el servidor
    cout << "Creando socket de escucha ..." << endl;
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creando el socket de escucha" << endl;
        exit(EXIT_FAILURE); 
    }
    
    // Configurar la dirección del servidor
    cout << "Configurando estructura de dirección del socket ..." << endl;
    memset(&direccionServidor, 0, sizeof(direccionServidor)); // Inicializar la estructura a ceros
    direccionServidor.sin_family = AF_INET; // Familia de direcciones (IPv4)
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY); // Dirección IP del servidor (cualquier interfaz de red)
    direccionServidor.sin_port = htons(port); // Puerto del servidor
    
    // Enlazar el socket a la dirección del servidor
    cout << "Enlazando el socket ..." << endl;
    if (bind(socket_server, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Error en bind()" << endl;
        exit(EXIT_FAILURE); // Salir del programa con un código de error
    }
    
    // Establecer el socket en modo de escucha
    cout << "Haciendo que el socket esté escuchando ..." << endl;
    if (listen(socket_server, 1024) < 0) {
        cout << "Error en listen()" << endl;
        exit(EXIT_FAILURE); // Salir del programa con un código de error
    }
    
    socklen_t addr_size; // Tamaño de la estructura de dirección del cliente
    addr_size = sizeof(struct sockaddr_in); // Obtener el tamaño de la estructura

    cout << "Esperando solicitudes de clientes ..." << endl;

    vector<thread> threads; // Vector para almacenar los hilos para cada cliente

    // Bucle principal para aceptar conexiones de clientes
    while (true) {
        int socket_cliente;

        // Aceptar una conexión
        if ((socket_cliente = accept(socket_server, (struct sockaddr *) &direccionCliente, &addr_size)) < 0) {
            cout << "Error al aceptar la conexión del cliente" << endl;
            continue; // Si hay un error, continuar esperando la próxima conexión
        }

        // Una vez aceptada la conexión, iniciar un nuevo hilo para manejar al cliente
        threads.emplace_back(jugar, socket_cliente, direccionCliente);
    }

    // Unirse a todos los hilos antes de terminar el programa
    for (auto& thread : threads) {
        thread.join();
    }

    
    close(socket_server);

    return 0;
}
