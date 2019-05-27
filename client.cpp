#define _WIN32_WINNT 0x501
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <sstream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <Tchar.h>
#include "mingw.thread.h"
 
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
 
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "4562"

string LOCAL_IP = "127.0.0.1";
string NAME;

struct client_type
{
	SOCKET socket;
	int id;
	char received_message[DEFAULT_BUFLEN];
};
	
int process_client(client_type &new_client);
int main();

bool ccin;
bool connected;
vector<string> stmsg;

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
				if (!ccin)
				{
					SetConsoleTextAttribute(hConsole,atoi(msg.substr(msg.find("¹")+2,(msg.find("²")-4)-msg.find("¹")+2).c_str()));
					cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					if (msg.find("˅") != string::npos)
					{
						cout << msg.substr(msg.find_last_of("²")+1,(msg.find("˄")-2)-msg.find_last_of("²")+1);
						SetConsoleTextAttribute(hConsole,atoi(msg.substr(msg.find("˄")+2,msg.find("˅")-msg.find("˄")+2).c_str()));
						cout << msg.substr(msg.find("˅")+2,(msg.find_last_of("˅")-5)-msg.find("˅")+2);
						SetConsoleTextAttribute(hConsole,7);
						cout << msg.substr(msg.find_last_of("˅")+1) << endl;
					}
					else {cout << msg.substr(msg.find_last_of("²")+1) << endl;}
				}
				else
				{
					stmsg.push_back(msg);
				}
			}
			else
			{
				SetConsoleTextAttribute(hConsole,4);
				cout << "recv() failed: " << WSAGetLastError() << endl;
				SetConsoleTextAttribute(hConsole,7);
				break;
			}
		}
	}
 
	if (WSAGetLastError() == WSAECONNRESET)
		connected = false;
		SetConsoleTextAttribute(hConsole,76);
		cout << "Lost connection to host." << endl << endl;
		SetConsoleTextAttribute(hConsole,7);
		cout << "Would you like to reconnect (Y/n)? ";
	return 0;
}
 
int main()
{
	WSAData wsa_data;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	string sent_message = "";
	client_type client = { INVALID_SOCKET, -1, "" };
	int iResult = 0;
	string message;
	string lmsg;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	stringstream title;
 
	if (NAME.empty())
	{
		cout << "Starting client...\nPlease enter the target address: ";
		getline(cin,LOCAL_IP);
	}
 
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		SetConsoleTextAttribute(hConsole,4);
		cout << "WSAStartup() failed with error: " << iResult << endl;
		SetConsoleTextAttribute(hConsole,7);
		return 1;
	}
 
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	connect:
	cout << "Connecting..." << endl << endl;
 
	iResult = getaddrinfo(LOCAL_IP.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		SetConsoleTextAttribute(hConsole,4);
		cout << "getaddrinfo() failed with error: " << iResult << endl;
		SetConsoleTextAttribute(hConsole,76);
		cout << "Unable to resolve hostname." << endl << endl;
		SetConsoleTextAttribute(hConsole,7);
		WSACleanup();
		system("pause");
		return 1;
	}
 
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
 
		client.socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (client.socket == INVALID_SOCKET) {
			SetConsoleTextAttribute(hConsole,4);
			cout << "socket() failed with error: " << WSAGetLastError() << endl;
			SetConsoleTextAttribute(hConsole,7);
			WSACleanup();
			system("pause");
			return 1;
		}
 
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
		SetConsoleTextAttribute(hConsole,4);
		cout << "Unable to connect to server." << endl;
		SetConsoleTextAttribute(hConsole,76);
		cout << "Could not reach host." << endl << endl;
		SetConsoleTextAttribute(hConsole,7);
		WSACleanup();
		system("pause");
		return 1;
	}
 
	if (NAME.empty())
	{
		SetConsoleTextAttribute(hConsole,2);
		cout << "Successfully Connected." << endl;
		SetConsoleTextAttribute(hConsole,7);
		cout << "Please enter a username: ";
		getline(cin, NAME);
		cout << endl;
	}
	iResult = send(client.socket, NAME.c_str(), strlen(NAME.c_str()), 0);
	
	//Receive client ID
	recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
	message = string(client.received_message).c_str();
	SetConsoleTextAttribute(hConsole,atoi(message.substr(message.find("¹")+2,(message.find("²")-4)-message.find("¹")+2).c_str()));
	cout << message.substr(message.find("²")+2,(message.find_last_of("²")-5)-message.find("²")+2);
	SetConsoleTextAttribute(hConsole,7);
	cout << message.substr(message.find_last_of("²")+1) << endl;
	recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
	message = client.received_message;
 
	if (message != "Server is full.")
	{
		connected = true;
		client.id = atoi(client.received_message);
		thread my_thread(process_client, client);
		
		title.str("");
		title.clear();
		title << "dice_client: " << message.substr(message.find("²")+2,(message.find_last_of("²")-5)-message.find("²")+2) << " [" << client.id << "]";
		SetConsoleTitle(_T(title.str().c_str()));
 
		while (1)
		{
			HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
			DWORD mode = 0;
			GetConsoleMode(hStdin, &mode);
			SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
			//getline(cin, sent_message);
			char c = getch();
			if (!connected && (c == 'Y' || c == 'y'))
			{
				cout << c << endl;
				my_thread.detach();
				goto connect; 
			}
			else if (!connected)
			{
				cout << c << endl;
				break;
			}
			if (!ccin) {sent_message = "";}
			if (c != '\0' && c != '\r' && c != '\b' && isprint(c))
			{
				sent_message += c;
				ccin = true;
			}
			else if (c == '\r')
			{
				cout << endl;
				ccin = false;
			}
			else if (c == '\b')
			{
				ccin = true;
				if(sent_message.size() > 0) {sent_message.pop_back();}
				cout << c << " " << c;
			}
			if (c == -32)
			{
				c = getch();
				if (c == 72)
				{
					cout << "\r                                                       \r" << lmsg;
					sent_message = lmsg;
				}
				else if (c == 80) {cout << "\r                                                       \r";}
				c = '\0';
				ccin = true;
			}
			if (isprint(c)) {cout << c;}
			if (sent_message == "") {ccin = false;}
			
			SetConsoleMode(hStdin, mode);
		
			if (sent_message != "" && !ccin) {lmsg = sent_message; iResult = send(client.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0);}
 
			if (connected && iResult <= 0)
			{
				SetConsoleTextAttribute(hConsole,4);
				cout << "send() failed: " << WSAGetLastError() << endl;
				SetConsoleTextAttribute(hConsole,7);
				break;
			}
			if (stmsg.size() > 0 && !ccin)
			{
				for (string cur : stmsg)
				{
					SetConsoleTextAttribute(hConsole,atoi(cur.substr(cur.find("¹")+2,(cur.find("²")-4)-cur.find("¹")+2).c_str()));
					cout << cur.substr(cur.find("²")+2,(cur.find_last_of("²")-5)-cur.find("²")+2);
					SetConsoleTextAttribute(hConsole,7);
					if (cur.find("˅") != string::npos)
					{
						cout << cur.substr(cur.find_last_of("²")+1,(cur.find("˄")-2)-cur.find_last_of("²")+1);
						SetConsoleTextAttribute(hConsole,atoi(cur.substr(cur.find("˄")+2,cur.find("˅")-cur.find("˄")+2).c_str()));
						cout << cur.substr(cur.find("˅")+2,(cur.find_last_of("˅")-5)-cur.find("˅")+2);
						SetConsoleTextAttribute(hConsole,7);
						cout << cur.substr(cur.find_last_of("˅")+1) << endl;
					}
					else {cout << cur.substr(cur.find_last_of("²")+1) << endl;}
				}
				stmsg.clear();
			}
		}
 
		my_thread.detach();
	}
	else
	{
		SetConsoleTextAttribute(hConsole,4);
		cout << client.received_message << endl;
		SetConsoleTextAttribute(hConsole,7);
	}
 
	cout << endl << "Shutting down socket..." << endl;
	iResult = shutdown(client.socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		SetConsoleTextAttribute(hConsole,4);
		cout << "shutdown() failed with error: " << WSAGetLastError() << endl;
		SetConsoleTextAttribute(hConsole,7);
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