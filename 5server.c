#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> // For Windows systems

#pragma comment(lib, "Ws2_32.lib")

void sortArray(int* arr, int size);

int main() {
    WSADATA wsa;
    SOCKET server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    int addr_len, recv_len;
    int array_size;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    // Prepare sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(43451);

    // Bind
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }
    printf("Bind done.\n");

    // Listen for incoming connections
    listen(server_sock, 3);
    printf("Waiting for incoming connections...\n");

    addr_len = sizeof(struct sockaddr_in);

    // Accept incoming connection
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
    if (client_sock == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }
    printf("Connection accepted.\n");

    // Receive the size of the array from the client
    recv(client_sock, (char*)&array_size, sizeof(array_size), 0);

    // Allocate memory for the array
    int* arr = (int*)malloc(array_size * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        closesocket(client_sock);
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }

    // Receive the array from the client
    recv(client_sock, (char*)arr, array_size * sizeof(int), 0);

    // Sort the array
    sortArray(arr, array_size);

    // Send the sorted array back to the client
    send(client_sock, (char*)arr, array_size * sizeof(int), 0);

    // Cleanup
    free(arr);
    closesocket(client_sock);
    closesocket(server_sock);
    WSACleanup();

    return 0;
}

// Function to sort the array using Bubble Sort
void sortArray(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
