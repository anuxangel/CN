#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    int array_size;
    int* arr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Creating socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Server address information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(43451);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connecting to server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        printf("Connection failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("Connected to server.\n");

    // Input size of the array
    printf("Enter the size of the array: ");
    scanf("%d", &array_size);

    // Allocate memory for the array
    arr = (int*)malloc(array_size * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Input array elements from the user
    printf("Enter %d integers:\n", array_size);
    for (int i = 0; i < array_size; i++) {
        printf("Element %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    // Send the size of the array to the server
    send(sockfd, (char*)&array_size, sizeof(array_size), 0);

    // Send the array to the server
    send(sockfd, (char*)arr, array_size * sizeof(int), 0);

    // Receive the sorted array from the server
    recv(sockfd, (char*)arr, array_size * sizeof(int), 0);

    // Print the sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < array_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Cleanup
    free(arr);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
