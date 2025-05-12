#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>    
#include <unistd.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <time.h>       

#define PORT 7777        
#define BUFFER_SIZE 1024  

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0}; 
    int nombre_a_deviner;
    int nombre_client;
    char reponse[BUFFER_SIZE];

    srand(time(NULL));
    nombre_a_deviner = rand() % (100 + 1);
    printf("Le nombre à deviner est %d\n", nombre_a_deviner);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur socket"); 
        exit(EXIT_FAILURE);     
    }
    address.sin_family = AF_INET;         
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);       

    int bindtest = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    if (bindtest < 0) {
        perror("Erreur bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("Erreur listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion sur le port %d\n", PORT);

    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    if (new_socket < 0) {
        perror("Erreur accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connecté\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        int bytes_read = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Client déconnecté\n");
            } else {
                perror("Erreur recv");
            }
            break; 
        }

        nombre_client = atoi(buffer);
        printf("Client: %d\n", nombre_client);

        memset(reponse, 0, BUFFER_SIZE); 
        if (nombre_client < nombre_a_deviner) {
            strcpy(reponse, "plus grand");
        } else if (nombre_client > nombre_a_deviner) {
            strcpy(reponse, "plus petit");
        } else {
            strcpy(reponse, "gagne"); 
        }

        int bytes_send = send(new_socket, reponse, strlen(reponse), 0);
        if ( bytes_send < 0) {
            perror("Erreur send");
            break; 
        }
        printf("Réponse envoyée: %s\n", reponse);

        if (strcmp(reponse, "gagne") == 0) {
            printf("Nombre trouvé par le client\n");
            break;
        }
    }

    printf("Fermeture de la connexion\n");
    close(new_socket); 
    close(server_fd);  

    return 0;
}
