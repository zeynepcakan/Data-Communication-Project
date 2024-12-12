#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib") // Winsock k�t�phanesini ba�la
#define PORT 8080
#define BUFFER_SIZE 1024

// Winsock'� ba�latma fonksiyonu
void initialize_winsock() {
    WSADATA wsa;
    printf("Winsock initializing...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock could not be initialized. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Winsock initialized.\n");
}

// Men� g�sterme fonksiyonu
void show_menu() {
    printf("\n******PASTRY STOCK INFORMATION******:\n");
    printf("1. For cake stock information\n");
    printf("2. For juice stock information\n");
    printf("3. For Pogaca stock information\n");
    printf("4. For Borek stock information\n");
    printf("5. For Birthday cake stock information\n");
    printf("6. Exit\n");
    printf("Make your choice: ");
}

int main() {
    initialize_winsock();

    SOCKET client_socket;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    int choice;

    // Soket olu�turuluyor
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Socket created.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // Sunucunun IP adresi
    server.sin_port = htons(PORT);

    // Sunucuya ba�lanma
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    // Men�den se�im yapma ve veriyi g�nderme
    while (1) {
        show_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: strcpy(buffer, "CAKE"); break; // Kek sto�u iste�i
            case 2: strcpy(buffer, "JUICE"); break; // Meyve suyu sto�u iste�i
            case 3: strcpy(buffer, "POGACA"); break; // Po�a�a sto�u iste�i
            case 4: strcpy(buffer, "BOREK"); break; // B�rek sto�u iste�i
            case 5: strcpy(buffer, "BIRTHDAY_CAKE"); break; // Do�um g�n� keki sto�u iste�i
            case 6: 
                printf("Exiting...\n");
                closesocket(client_socket);  // Soketi kapat
                WSACleanup();
                return 0;  // Programdan ��k
            default:
                printf("Invalid choice!\n");
                continue;  // Ge�ersiz se�imde d�ng� devam eder
        }

        // Sunucuya veri g�nderme
        send(client_socket, buffer, strlen(buffer), 0);
        
        // Sunucudan gelen cevab� alma ve ekrana yazd�rma
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) > 0) {
            buffer[strlen(buffer)] = '\0';  // Gelen veriyi sonland�r
            printf("%s\n", buffer);  // Sunucudan gelen yan�t� yazd�r
        }
    }
    closesocket(client_socket);  // Soket kapat�l�yor
    WSACleanup();  // Winsock temizli�i
    return 0;
}

