#define _WIN32_WINNT 0x501
//#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>
#include "mingw.thread.h"
 
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
 
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "4562"

std::string IP_ADDRESS = "127.0.0.1";
std::string NAME;

struct client_type
{
	SOCKET socket;
	int id;
	char received_message[DEFAULT_BUFLEN];
};
 
int process_client(client_type &new_client);
int main();

int process_client(client_type &new_client)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string msg;
	while (1)
	{
		memset(new_client.received_message, 0, DEFAULT_BUFLEN);
 
		if (new_client.socket != 0)
		{
			int iResult = recv(new_client.socket, new_client.received_message, DEFAULT_BUFLEN, 0);
 
			if (iResult != SOCKET_ERROR)
			{
				msg = string(new_client.received_message).c_str();
				SetConsoleTextAttribute(hConsole,atoi(msg.substr(msg.find("¹")+2,(msg.find("²")-4)-msg.find("¹")+2).c_str()));
				std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
				SetConsoleTextAttribute(hConsole,7);
				if (msg.find("˅") != string::npos)
				{
					std::cout << msg.substr(msg.find_last_of("²")+1,(msg.find("˄")-2)-msg.find_last_of("²")+1);
					SetConsoleTextAttribute(hConsole,atoi(msg.substr(msg.find("˄")+2,msg.find("˅")-msg.find("˄")+2).c_str()));
					std::cout << msg.substr(msg.find("˅")+2,(msg.find_last_of("˅")-5)-msg.find("˅")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("˅")+1) << std::endl;
				}
				else {std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;}
			}
			else
			{
				cout << "recv() failed: " << WSAGetLastError() << endl;
				break;
			}
		}
	}
 
	if (WSAGetLastError() == WSAECONNRESET)
		cout << "The server has been disconnected." << endl;
 
	return 0;
}
 
int main()
{
	start:
	WSAData wsa_data;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	string sent_message = "";
	client_type client = { INVALID_SOCKET, -1, "" };
	int iResult = 0;
	string message;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 
	cout << "Starting client...\nPlease enter the target address: ";
	getline(cin,IP_ADDRESS);
 
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		cout << "WSAStartup() failed with error: " << iResult << endl;
		return 1;
	}
 
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	 
	cout << "Connecting...\n";
 
	// Resolve the server address and port
	iResult = getaddrinfo(IP_ADDRESS.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		cout << "getaddrinfo() failed with error: " << iResult << endl;
		WSACleanup();
		system("pause");
		return 1;
	}
 
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
 
		// Create a SOCKET for connecting to server
		client.socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (client.socket == INVALID_SOCKET) {
			cout << "socket() failed with error: " << WSAGetLastError() << endl;
			WSACleanup();
			system("pause");
			return 1;
		}
 
		// Connect to server.
		iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(client.socket);
			client.socket = INVALID_SOCKET;
			continue;
		}
		break;
	}
 
	freeaddrinfo(result);
 
	if (client.socket == INVALID_SOCKET) {
		cout << "Unable to connect to server!" << endl;
		WSACleanup();
		system("pause");
		return 1;
	}
 
	cout << "Successfully Connected.\nPlease enter a username: ";
	getline(cin, NAME);
	iResult = send(client.socket, NAME.c_str(), strlen(NAME.c_str()), 0);
	
	//Obtain id from server for this client;
	recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
	message = string(client.received_message).c_str();
	SetConsoleTextAttribute(hConsole,atoi(message.substr(message.find("¹")+2,(message.find("²")-4)-message.find("¹")+2).c_str()));
	std::cout << message.substr(message.find("²")+2,(message.find_last_of("²")-5)-message.find("²")+2);
	SetConsoleTextAttribute(hConsole,7);
	std::cout << message.substr(message.find_last_of("²")+1) << std::endl;
	recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
	message = client.received_message;
 
	if (message != "Server is full.")
	{
		client.id = atoi(client.received_message);
 
		thread my_thread(process_client, client);
 
 
		while (1)
		{
			getline(cin, sent_message);
			if (sent_message != "") {iResult = send(client.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0);}
 
			if (iResult <= 0)
			{
				cout << "send() failed: " << WSAGetLastError() << endl;
				break;
			}
		}
 
		//Shutdown the connection since no more data will be sent
		my_thread.detach();
	}
	else
	{
		cout << client.received_message << endl;
	}
 
	cout << "Shutting down socket..." << endl;
	iResult = shutdown(client.socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown() failed with error: " << WSAGetLastError() << endl;
		closesocket(client.socket);
		WSACleanup();
		system("pause");
		return 1;
	}
 
	closesocket(client.socket);
	WSACleanup();
	system("pause");
	return 0;
}