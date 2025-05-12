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
    char server_ip[20];             
    int nombre_entre;
    char nombre_str[10];            

    printf("Entrez l'adresse IP du serveur: ");
    if (fgets(server_ip, sizeof(server_ip), stdin) == NULL) {
        printf("Erreur IP\n");
        exit(EXIT_FAILURE);
    }
    server_ip[strcspn(server_ip, "\n")] = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur création socket client");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;            
    serv_addr.sin_port = htons(PORT);      

    int binaryip = inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);

    if (binaryip <= 0) {
        printf("Adresse IP invalide  %s\n", server_ip);
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    int serverconnect = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (serverconnect < 0) {
        perror("Erreur de connexion");
        printf("Le serveur n'est pas démarré \n");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur %s sur le port %d\n", server_ip, PORT);
    printf("Devinez un nombre entre 0 et 100\n");

    while (1) {

        if (scanf("%d", &nombre_entre) != 1) {
            printf("Erreur: Veuillez entrer un nombre\n");
            while (getchar() != '\n');
            continue; 
        }
        while (getchar() != '\n');

        snprintf(nombre_str, sizeof(nombre_str), "%d", nombre_entre);

        if (send(sock, nombre_str, strlen(nombre_str), 0) < 0) {
            perror("Erreur send");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);

        int bytes_read = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Le serveur arreté\n");
            } else {
                perror("Erreur recv");
            }
            break; 
        }

        printf("%s\n", buffer);

        if (strcmp(buffer, "gagne") == 0) {
            printf("Bravo, vous avez trouvé !\n");
            break; 
        }
    }

    printf("Fermeture de la connexion\n");
    close(sock);

    return 0;
}
