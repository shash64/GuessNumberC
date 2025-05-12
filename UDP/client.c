#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7777
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char server_ip_str[20];
    int nombre_entre;
    char nombre_str[10];
    socklen_t serv_addr_len = sizeof(serv_addr);

    printf("Entrez l'adresse IP du serveur (par défaut 127.0.0.1):  ");
    if (fgets(server_ip_str, sizeof(server_ip_str), stdin) == NULL) {
        printf("Erreur lors de la lecture de l'IP\n");
        exit(EXIT_FAILURE);
    }
    server_ip_str[strcspn(server_ip_str, "\n")] = 0;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip_str, &serv_addr.sin_addr) <= 0) {
        printf("Adresse IP invalide %s\n", server_ip_str);
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Devinez le nombre entre 0 et 100\n");

    while (1) {
        if (scanf("%d", &nombre_entre) != 1) {
            printf("Erreur: Veuillez entrer un nombre valide\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        snprintf(nombre_str, sizeof(nombre_str), "%d", nombre_entre);

        if (sendto(sock, nombre_str, strlen(nombre_str), 0, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("Erreur lors de l'envoi");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recvfrom(sock, buffer, BUFFER_SIZE -1, 0, (struct sockaddr *)&serv_addr, &serv_addr_len);

        if (bytes_received < 0) {
            perror("Erreur lors de la réception");
            break;
        }
        buffer[bytes_received] = '\0';

        printf("%s\n", buffer);

        if (strcmp(buffer, "gagne") == 0) {
            printf("Bravo, vous avez trouvé !\n");
            break;
        }
    }

    close(sock);
    return 0;
}
