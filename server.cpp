#define _WIN32_WINNT 0x501
//#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include <cfloat>
#include <fstream>
#include <windows.h>
#include <Tchar.h>
#include "mingw.thread.h"
 
#pragma comment (lib, "Ws2_32.lib")
 
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "4562"
 
struct client_type
{
	int id;
	int colour;
	std::string name;
	SOCKET socket;
};
 
const char OPTION_VALUE = 1;
int MAX_CLIENTS = 6;
//std::string PORT = "4562";
std::string PORT = DEFAULT_PORT;
std::string IP_ADDRESS = "127.0.0.1";
 
//Function Prototypes
int process_client(client_type &new_client, std::vector<client_type> &client_array, std::thread &thread);
int main();
 
int process_client(client_type &new_client, std::vector<client_type> &client_array, std::thread &thread)
{	
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string msg = "";
	char tempmsg[DEFAULT_BUFLEN] = "";
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(double(0.0), std::nextafter(double(1.0), DBL_MAX));
	double rn;
 
	//Session
	while (1)
	{
		memset(tempmsg, 0, DEFAULT_BUFLEN);
 
		if (new_client.socket != 0)
		{
			int iResult = recv(new_client.socket, tempmsg, DEFAULT_BUFLEN, 0);
 
			if (iResult != SOCKET_ERROR)
			{
				SetConsoleTextAttribute(hConsole,7);
				if (strcmp("", tempmsg)) {msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + "): " + tempmsg;}
				SetConsoleTextAttribute(hConsole,new_client.colour);
				std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
				SetConsoleTextAttribute(hConsole,7);
				std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;

				//Broadcast that message to the other clients
				if (std::string(tempmsg) == "/d2")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D2 and got: " + std::to_string(round(rn+1)).erase(std::to_string(round(rn+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn+1) + ")";
					msg.erase(msg.find_last_not_of('0') + 1, std::string::npos);
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/d4")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D4 and got: " + std::to_string(round(rn*3+1)).erase(std::to_string(round(rn*3+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn*3+1) + ")";
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/d6")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D6 and got: " + std::to_string(round(rn*5+1)).erase(std::to_string(round(rn*5+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn*5+1) + ")";
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/d8")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D8 and got: " + std::to_string(round(rn*7+1)).erase(std::to_string(round(rn*7+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn*7+1) + ")";
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/d10")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D10 and got: " + std::to_string(round(rn*9+1)).erase(std::to_string(round(rn*9+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn*9+1) + ")";
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/d12")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D12 and got: " + std::to_string(round(rn*11+1)).erase(std::to_string(round(rn*11+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn*11+1) + ")";
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/d20")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D20 and got: " + std::to_string(round(rn*19+1)).erase(std::to_string(round(rn*19+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn*19+1) + ")";
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/d100")
				{
					rn = dist(mt);
					msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Rolled a D100 and got: " + std::to_string(round(rn*99+1)).erase(std::to_string(round(rn*99+1)).find_last_not_of('0') + 1, std::string::npos) + "\t\t(" + std::to_string(rn*99+1) + ")";
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
				}
				else if (std::string(tempmsg) == "/g")
				{
					rn = dist(mt);
					if (rn < 0.05) {msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Took their chances: ˄76˅CRITICAL FAILURE˅" + "\t\t(" + std::to_string(rn) + ")";}
					else if (rn >= 0.05 && rn < 0.5) {msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Took their chances: ˄4˅FAILURE˅" + "\t\t(" + std::to_string(rn) + ")";}
					else if (rn >= 0.5 && rn < 0.95) {msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Took their chances: ˄2˅SUCCESS˅" + "\t\t(" + std::to_string(rn) + ")";}
					else if (rn >= 0.95) {msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Took their chances: ˄42˅CRITICAL SUCCESS˅" + "\t\t(" + std::to_string(rn) + ")";}
					SetConsoleTextAttribute(hConsole,new_client.colour);
					std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2).c_str();
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("²")+1,(msg.find("˄")-2)-msg.find_last_of("²")+1);
					SetConsoleTextAttribute(hConsole,atoi(msg.substr(msg.find("˄")+2,msg.find("˅")-msg.find("˄")+2).c_str()));
					std::cout << msg.substr(msg.find("˅")+2,(msg.find_last_of("˅")-5)-msg.find("˅")+2);
					SetConsoleTextAttribute(hConsole,7);
					std::cout << msg.substr(msg.find_last_of("˅")+1) << std::endl;
				}
				for (int i = 0; i < MAX_CLIENTS; i++)
				{
					if (client_array[i].socket != INVALID_SOCKET)
					{
						if (new_client.id != i || std::string(tempmsg).find("/") != std::string::npos) {iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);}
					}
				}
			}
			else
			{
				msg = "¹" + std::to_string(new_client.colour) + "²" + new_client.name + "² (" + std::to_string(new_client.id) + ") Disconnected";
				SetConsoleTextAttribute(hConsole,new_client.colour);
				std::cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
				SetConsoleTextAttribute(hConsole,7);
				std::cout << msg.substr(msg.find_last_of("²")+1) << std::endl;
 
				closesocket(new_client.socket);
				closesocket(client_array[new_client.id].socket);
				client_array[new_client.id].socket = INVALID_SOCKET;
 
				//Broadcast the disconnection message to the other clients
				for (int i = 0; i < MAX_CLIENTS; i++)
				{
					if (client_array[i].socket != INVALID_SOCKET)
						iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
				}
 
				break;
			}
		}
	} //end while
 
	thread.detach();
 
	return 0;
}
 
int main()
{
	std::string inp;
	/*std::cout << "Please enter the server port (leave blank for default [4562]): ";
	getline(std::cin,inp);
	if (inp == "") {PORT = "4562";}
	else {PORT = inp;}*/
	
	std::cout << "Please enter the client capacity (leave blank for default [6]): ";
	getline(std::cin,inp);
	if (inp == "") {MAX_CLIENTS = 6;}
	else {MAX_CLIENTS = atoi(inp.c_str());}
	
	WSADATA wsaData;
	struct addrinfo hints;
	struct addrinfo *server = NULL;
	SOCKET server_socket = INVALID_SOCKET;
	std::string msg = "";
	std::vector<client_type> client(MAX_CLIENTS);
	int num_clients = 0;
	int temp_id = -1;
	srand(time(NULL));
	std::thread my_thread[MAX_CLIENTS];
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//Initialize Winsock
	std::cout << "Intializing Winsock..." << std::endl;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
 
	//Setup hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
 
	//Setup Server
	std::cout << "Configuring server..." << std::endl;
	std::ifstream IPFile;
	int offset; 

	system("ipconfig > ipconfig.txt");
	IPFile.open ("ipconfig.txt"); 
	if(IPFile.is_open())
	{
	   while(!IPFile.eof())
		{
			getline(IPFile,IP_ADDRESS);
			if ((offset = IP_ADDRESS.find("IPv4 Address. . . . . . . . . . . :", 0)) != std::string::npos)
			{ 
				IP_ADDRESS.erase(0,39);
				IPFile.close();
			}
		}
	}
	getaddrinfo(IP_ADDRESS.c_str(), PORT.c_str(), &hints, &server);
 
	//Create a listening socket for connecting to server
	std::cout << "Creating server socket..." << std::endl;
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
 
	//Setup socket options
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs
 
	//Assign an address to the server socket.
	std::cout << "Binding socket..." << std::endl;
	bind(server_socket, server->ai_addr, (int)server->ai_addrlen);
 
	//Listen for incoming connections.
	std::cout << "Awaiting clients, listening..." << std::endl;
	listen(server_socket, SOMAXCONN);
 
	//Initialize the client list
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		client[i] = { -1, 7, "", INVALID_SOCKET };
	}
 
	while (1)
	{
		SOCKET incoming = INVALID_SOCKET;
		incoming = accept(server_socket, NULL, NULL);
 
		if (incoming == INVALID_SOCKET) continue;
 
		//Reset the number of clients
		num_clients = -1;
 
		//Create a temporary id for the next client
		temp_id = -1;
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			if (client[i].socket == INVALID_SOCKET && temp_id == -1)
			{
				client[i].socket = incoming;
				client[i].id = i;
				temp_id = i;
			}
 
			if (client[i].socket != INVALID_SOCKET)
				num_clients++;
 
			//std::cout << client[i].socket << std::endl;
		}
 
		if (temp_id != -1)
		{	
			char tempname[DEFAULT_BUFLEN] = "";
			memset(tempname, 0, DEFAULT_BUFLEN);
			client[temp_id].colour = rand()%15+1;
			int iResult = recv(client[temp_id].socket, tempname, DEFAULT_BUFLEN, 0);
			client[temp_id].name = tempname;
			//Send the id to that client
			SetConsoleTextAttribute(hConsole,client[temp_id].colour);
			std::cout << client[temp_id].name;
			SetConsoleTextAttribute(hConsole,7);
			std::cout << " (" << client[temp_id].id << ") Connected." << std::endl;
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (client[i].socket != INVALID_SOCKET)
				{
					msg = "¹" + std::to_string(client[temp_id].colour) + "²" + client[temp_id].name + "² (" + std::to_string(client[temp_id].id) + ") Accepted";
					send(client[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
				}
			}
			msg = std::to_string(client[temp_id].id);
			send(client[temp_id].socket, msg.c_str(), strlen(msg.c_str()), 0);
 
			//Create a thread process for that client
			my_thread[temp_id] = std::thread(process_client, std::ref(client[temp_id]), std::ref(client), std::ref(my_thread[temp_id]));
		}
		else
		{
			msg = "Server is full.";
			send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << msg << std::endl;
		}

		//SetConsoleTitle(_T("dice_server ("+std::to_string(num_clients)+" client(s) connected)");
		SetConsoleTitle(_T(std::to_string(num_clients).c_str()));
	} //end while
 
 
	//Close listening socket
	closesocket(server_socket);
 
	//Close client socket
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		my_thread[i].detach();
		closesocket(client[i].socket);
	}
 
	//Clean up Winsock
	WSACleanup();
	std::cout << "Server has closed successfully." << std::endl;
 
	system("pause");
	return 0;
}