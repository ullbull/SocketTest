#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

void custom_exit(int exit_code) {
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
	SOCKET server_socket;
	SOCKET client_socket;
	struct sockaddr_in server_address;

	const char* ip_address = "127.0.0.1";
	int port = 9002;

	// Initialize Winsock
	i_result = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (i_result != NO_ERROR)
		custom_exit(i_result);

	// create socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		printf("Socket error %d\n", WSAGetLastError());
		custom_exit(WSAGetLastError());
	}

	BOOL b_opt_val = TRUE;
	i_result = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &b_opt_val, sizeof(b_opt_val));
	if (i_result == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		custom_exit(WSAGetLastError());
	}

	// define address
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	//server_address.sin_addr.s_addr = INADDR_ANY;
	i_result = inet_pton(server_address.sin_family, ip_address, &server_address.sin_addr.s_addr);
	if (i_result != 1) {
		printf("Invalid IP-address \"%s\"\n", ip_address);
		printf("Error code %d\n", WSAGetLastError());
		custom_exit(WSAGetLastError());
	}

	// bind the socket to our specified IP and port
	i_result = bind(server_socket, (SOCKADDR*)&server_address, sizeof(server_address));
	if (i_result == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		closesocket(server_socket);
		custom_exit(WSAGetLastError());
	}

	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	i_result = listen(server_socket, 1);
	if (i_result == SOCKET_ERROR) {
		printf("Socket error %d\n", WSAGetLastError());
		closesocket(server_socket);
		custom_exit(WSAGetLastError());
	}

	//----------------------
	// Accept the connection.	
	printf("Waiting for connection on %s:%d ...\n", ip_address, port);
	client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		printf("Socket error %d\n", WSAGetLastError());
		closesocket(server_socket);
		custom_exit(WSAGetLastError());
	}
	printf("Connected!\n");

	// No longer need server socket
	closesocket(server_socket);

	// Send a message to the connected client.
	char server_message[256] = { "You have reached the server!" };
	send(client_socket, server_message, sizeof(server_message), 0);

	// Receive data from the client
	char message[512];
	int n_bytes;
	int max_print = 10;
	long total_received = 0;
	while (1) {
		n_bytes = recv(client_socket, message, sizeof(message), 0);

		if (n_bytes == 0) {
			printf("\nClient disconnected.\n");
			break;
		}

		if (n_bytes) {
			total_received += n_bytes;
			printf("\rReceived bytes: %d", total_received);
			//for (int i = 0; i < n_bytes && i < max_print; i++) {
			//	putchar(message[i]);
			//}
			//printf("\n");
		}
	}

	// Close client socket
	shutdown(client_socket, SD_BOTH);
	closesocket(client_socket);

	custom_exit(0);
	return 0;
}