#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesini baðla
#define PORT 8080
#define BUFFER_SIZE 1024 //BUFFER_SIZE, genellikle bir veri tamponunun (buffer) boyutunu belirlemek için kullanýlýr.

// Stok verileri
int stock_cake = 60, stock_pogaca = 40, stock_borek = 30, stock_juice = 65, stock_birthday_cake = 12;

// Winsock'ý baþlatma fonksiyonu
void initialize_winsock() {
    WSADATA wsa;
    printf("Winsock initializing...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock could not be initialized. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    
    printf("Winsock initialized.\n");
}

int main() {
	//STOCK GÜNCELLEME
    int choice, new_value;
    printf("\nStock Update Menu:\n");
    printf("1. Cake\n");
    printf("2. Juice\n");
    printf("3. Pogaca\n");
    printf("4. Borek\n");
    printf("5. Birthday Cake\n");
    printf("0. Exit\n");

    while (1) {
        printf("Which stock should be updated? (Enter 0 to exit): ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Exiting stock update process.\n");
            break;
        }

        printf("Enter the new stock value: ");
        scanf("%d", &new_value);

        switch (choice) {
            case 1:
                stock_cake = new_value;
                printf("Cake stock updated: %d\n", stock_cake);
                break;
            case 2:
                stock_juice = new_value;
                printf("Juice stock updated: %d\n", stock_juice);
                break;
            case 3:
                stock_pogaca = new_value;
                printf("Pogaca stock updated: %d\n", stock_pogaca);
                break;
            case 4:
                stock_borek = new_value;
                printf("Borek stock updated: %d\n", stock_borek);
                break;
            case 5:
                stock_birthday_cake = new_value;
                printf("Birthday cake stock updated: %d\n", stock_birthday_cake);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    initialize_winsock();

    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client; 
    int client_len, recv_size;
    char buffer[BUFFER_SIZE];

    // Sunucu soketi oluþturuluyor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket could not be created. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Socket created.\n");

    // Sunucu yapýlandýrmasý
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Soket baðlanýyor
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Bind successful.\n");

    // Dinlemeye baþlama
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Waiting for connections...\n");

    // Müþteri baðlantýsý kabul ediliyor
    client_len = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Connection could not be accepted. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted.\n");
    
	//
    // Veri alýþveriþi döngüsü
    while ((recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[recv_size] = '\0'; // Gelen veriyi sonlandýr
        printf("Message from client: %s\n", buffer);

        if (strcmp(buffer, "CAKE") == 0) {
            sprintf(buffer, "Cake stock: %d", stock_cake);
        } else if (strcmp(buffer, "JUICE") == 0) {
            sprintf(buffer, "Juice stock: %d", stock_juice);
        } else if (strcmp(buffer, "POGACA") == 0) {
            sprintf(buffer, "Pogaca stock: %d", stock_pogaca);
        } else if (strcmp(buffer, "BOREK") == 0) {
            sprintf(buffer, "Brek stock: %d", stock_borek);
        } else if (strcmp(buffer, "BIRTHDAY_CAKE") == 0) {
            sprintf(buffer, "Birthday cake stock: %d", stock_birthday_cake);
        }else {
            sprintf(buffer, "Unknown request: %s", buffer);
        }

        // Yanýtý müþteriye gönder
        send(client_socket, buffer, strlen(buffer), 0);
    }
    if (recv_size == 0) {
        printf("Client disconnected.\n");
    } else if (recv_size == SOCKET_ERROR) {
        printf("recv error. Error Code: %d\n", WSAGetLastError());
    }

    // Soketler kapatýlýyor
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}

