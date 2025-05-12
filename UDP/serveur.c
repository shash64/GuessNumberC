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
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    int nombre_a_deviner;
    int nombre_client;
    char reponse[BUFFER_SIZE];

    srand(time(NULL));
    nombre_a_deviner = rand() % (100 + 1);
    printf("Le nombre à deviner est %d\n", nombre_a_deviner);

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur dans le socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur dubind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur UDP en attente de messages sur le port %d\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE -1, 0, (struct sockaddr *)&client_addr, &client_addr_len);

        if (bytes_received < 0) {
            perror("Erreur lors de la récéption");
            continue;
        }
        buffer[bytes_received] = '\0';

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Message reçu de %s:%d\n", client_ip, ntohs(client_addr.sin_port));

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

        if (sendto(server_fd, reponse, strlen(reponse), 0, (const struct sockaddr *)&client_addr, client_addr_len) < 0) {
            perror("sendto");
        }
        printf("Réponse envoyée: %s\n", reponse);

        if (strcmp(reponse, "gagne") == 0) {
            printf("Le client a trouvé le nombre, le serveur va choisir un nouveau nombre\n");
            nombre_a_deviner = rand() % (100 + 1);
            printf("Nouveau nombre à deviner: %d\n", nombre_a_deviner);
        }
    }

    close(server_fd);
    return 0;
}
