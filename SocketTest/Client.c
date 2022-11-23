#include<Winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main_c()
{
	static WSADATA wsaData;
	int wsaerr = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (wsaerr)
		exit(wsaerr);

	int return_code;

	// create socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		exit(WSAGetLastError());
	}

	// define address
	const char *ip_address = "127.0.0.1";
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	inet_pton(server_address.sin_family, ip_address, &server_address.sin_addr.s_addr);

	// connect to server
	return_code = connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
	if (return_code == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		exit(WSAGetLastError());
	}

	// receive message
	char server_response[256];
	recv(server_socket, server_response, sizeof(server_response), 0);
	printf("Received data from server: %s\n", server_response);

	closesocket(server_socket);

	printf("Press enter to exit ... ");
	getchar();
	return 0;
}
