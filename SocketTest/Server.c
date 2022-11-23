#include<Winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main_s()
{
	static WSADATA wsaData;
 	int wsaerr = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (wsaerr)
		exit(wsaerr);

	char server_message[256] = { "You have reached the server!" };
	int return_code;

	// create socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		exit(WSAGetLastError());
	}

	// define address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to our specified IP and port
	return_code = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
	if (return_code == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		exit(WSAGetLastError());
	}

	return_code = listen(server_socket, 1);
	if (return_code == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		exit(WSAGetLastError());
	}

	printf("Waiting for connection ...");
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	printf("here!\n");

	send(client_socket, server_message, sizeof(server_message), 0);

	closesocket(server_socket);

	printf("Press enter to exit ... ");
	getchar();
	return 0;
}