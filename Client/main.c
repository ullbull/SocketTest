#include<Winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

void user_send(int socket) {
	char message[256];

	while (1) {
		printf("Enter a message or \"q\" to quit:\n");
		scanf_s("%s", message, sizeof(message));

		if (message[0] == 'q') return;

		send(socket, message, strlen(message), 0);
	}
}

int custom_exit(int exit_code) {
	WSACleanup();
	if (exit_code != NO_ERROR)
		printf("Error code = %d\n", exit_code);
	printf("Press any key to exit ... ");
	getch();
	exit(exit_code);
}

int main()
{
	WSADATA wsaData;
	int i_result;
	SOCKET my_socket;
	struct sockaddr_in server_address;

	const char* ip_address = "127.0.0.1";
	int port = 9002;

	// Initialize Winsock
	i_result = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (i_result != NO_ERROR)
		custom_exit(i_result);

	// create socket
	my_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (my_socket == INVALID_SOCKET) {
		printf("Socket error %d\n", WSAGetLastError());
		custom_exit(WSAGetLastError());
	}

	// define address
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	i_result = inet_pton(server_address.sin_family, ip_address, &server_address.sin_addr.s_addr);
	if (i_result != 1) {
		printf("Invalid IP-address \"%s\"\n", ip_address);
		printf("Error code %d\n", WSAGetLastError());
		custom_exit(WSAGetLastError());
	}
	// connect to server
	printf("Connecting to %s:%d ...\n", ip_address, port);
	i_result = connect(my_socket, (SOCKADDR*)&server_address, sizeof(server_address));
	if (i_result == SOCKET_ERROR) {
		wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
		i_result = closesocket(my_socket);
		if (i_result == SOCKET_ERROR)
			wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
		custom_exit(WSAGetLastError());
	}

	// receive message
	char server_response[256];
	recv(my_socket, server_response, sizeof(server_response), 0);
	printf("Received data from server: %s\n", server_response);

	user_send(my_socket);

	printf("Closing socket ...\n");
	shutdown(my_socket, SD_BOTH);
	i_result = closesocket(my_socket);
	if (i_result == SOCKET_ERROR) {
		wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
		custom_exit(WSAGetLastError());
	}

	custom_exit(0);
	return 0;
}