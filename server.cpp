#define _WIN32_WINNT 0x501
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include <cfloat>
#include <sstream>
#include <windows.h>
#include <wininet.h>
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
std::string LOCAL_IP = "127.0.0.1";
std::string EXT_IP = "127.0.0.1";
int CLIENT_COUNT = 0;
 
using namespace std;
 
//Function Prototypes
string ext_ip();
int process_client(client_type &new_client, vector<client_type> &client_array, thread &thread);
int main();
 
string ext_ip()
{ 
    HINTERNET net = InternetOpen("IP retriever",INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL,0);
    HINTERNET conn = InternetOpenUrl(net,"http://myexternalip.com/raw",NULL,0,INTERNET_FLAG_RELOAD,0);

    char buffer[4096];
    DWORD read;

    InternetReadFile(conn, buffer, sizeof(buffer)/sizeof(buffer[0]), &read);
    InternetCloseHandle(net);    

    return string(buffer, read);
}
 
int process_client(client_type &new_client, vector<client_type> &client_array, thread &thread)
{	
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string msg = "";
	char tempmsg[DEFAULT_BUFLEN] = "";
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(double(0.0), nextafter(double(1.0), DBL_MAX));
	double rn;
	stringstream title;
 
	while (1)
	{
		memset(tempmsg, 0, DEFAULT_BUFLEN);
 
		if (new_client.socket != 0)
		{
			int iResult = recv(new_client.socket, tempmsg, DEFAULT_BUFLEN, 0);
 
			if (iResult != SOCKET_ERROR)
			{
				SetConsoleTextAttribute(hConsole,7);
				if (strcmp("", tempmsg)) {msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + "): " + tempmsg;}
				SetConsoleTextAttribute(hConsole,new_client.colour);
				cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
				SetConsoleTextAttribute(hConsole,7);
				cout << msg.substr(msg.find_last_of("²")+1) << endl;
				
				if (tempmsg[0] == '/')
				{
					int co = 0;
					for (size_t ofs = string(tempmsg).find(","); ofs != string::npos; ofs = string(tempmsg).find(",",ofs+string(",").length()))
					{
						++co;
					}
					++co;
					int c;
					string dm;
					for (c=1; c<=co; ++c)
					{
						rn = dist(mt);
						int mod;
						string modm;
						if (c == 1 && co == 1) {dm = string(tempmsg).substr(string(tempmsg).find_last_of("/")+1);}
						else if (c == 1 && co > 1) {dm =  string(tempmsg).substr(string(tempmsg).find("/")+1,(string(tempmsg).find_first_of(",")-2)-string(tempmsg).find("/")+1);}
						else if (c > 1 && co > 1)
						{
							size_t fpos,lpos,from=0;
							int i=0;
							while (i<c-1)
							{
								fpos = string(tempmsg).find(",",from);
								if (fpos == string::npos) {break;}
								from = fpos+=1;
								++i;
							}
							from=0;
							i=0;
							while (i<c)
							{
								lpos = string(tempmsg).find(",",from);
								if (lpos == string::npos) {break;}
								from = lpos+=1;
								++i;
							}
							if (string(tempmsg).substr(fpos,(lpos-1)-fpos).length() > 0) {dm = string(tempmsg).substr(fpos,(lpos-1)-fpos);}
						}
						else if (c == co && co > 1) {dm = string(tempmsg).substr(string(tempmsg).find_last_of(",")+1);}
						else {dm = "ERROR: Invalid command.";}
	
						if (dm.find("+") != string::npos) {mod = atoi((dm.substr(dm.find("+")+1)).c_str()); modm = dm.substr(dm.find("+"));}
						else if (dm.find("-") != string::npos) {mod = atoi((dm.substr(dm.find("-"))).c_str()); modm = dm.substr(dm.find("-"));}
						else {mod = 0; modm = "";}
						
						if (dm.find("d2") != string::npos && dm.find("d20") == string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D2" + modm + " and got: " + to_string(round(rn+1)).erase(to_string(round(rn+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn+1) + ")";
						}
						else if (dm.find("d4") != string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D4" + modm + " and got: " + to_string(round(rn*3+1)+mod).erase(to_string(round(rn*3+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn*3+1) + ")";
						}
						else if (dm.find("d6") != string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D6" + modm + " and got: " + to_string(round(rn*5+1)+mod).erase(to_string(round(rn*5+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn*5+1) + ")";
						}
						else if (dm.find("d8") != string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D8" + modm + " and got: " + to_string(round(rn*7+1)+mod).erase(to_string(round(rn*7+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn*7+1) + ")";
						}
						else if (dm.find("d10") != string::npos && dm.find("d100") == string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D10" + modm + " and got: " + to_string(round(rn*9+1)+mod).erase(to_string(round(rn*9+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn*9+1) + ")";
						}
						else if (dm.find("d12") != string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D12" + modm + " and got: " + to_string(round(rn*11+1)+mod).erase(to_string(round(rn*11+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn*11+1) + ")";
						}
						else if (dm.find("d20") != string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D20" + modm + " and got: " + to_string(round(rn*19+1)+mod).erase(to_string(round(rn*19+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn*19+1) + ")";
						}
						else if (dm.find("d100") != string::npos)
						{
							msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Rolled a D100" + modm + " and got: " + to_string(round(rn*99+1)+mod).erase(to_string(round(rn*99+1)+mod).find_last_not_of('0'), string::npos) + "\t\t(" + to_string(rn*99+1) + ")";
						}
						SetConsoleTextAttribute(hConsole,new_client.colour);
						cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2).c_str();
						SetConsoleTextAttribute(hConsole,7);
						if (dm.find("g") != string::npos)
						{
							if (rn < 0.05) {msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Took their chances: ˄76˅CRITICAL FAILURE˅" + "\t(" + to_string(rn) + ")";}
							else if (rn >= 0.05 && rn < 0.5) {msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Took their chances: ˄4˅FAILURE˅" + "\t\t(" + to_string(rn) + ")";}
							else if (rn >= 0.5 && rn < 0.95) {msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Took their chances: ˄2˅SUCCESS˅" + "\t\t(" + to_string(rn) + ")";}
							else if (rn >= 0.95) {msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Took their chances: ˄42˅CRITICAL SUCCESS˅" + "\t(" + to_string(rn) + ")";}
							cout << msg.substr(msg.find_last_of("²")+1,(msg.find("˄")-2)-msg.find_last_of("²")+1);
							SetConsoleTextAttribute(hConsole,atoi(msg.substr(msg.find("˄")+2,msg.find("˅")-msg.find("˄")+2).c_str()));
							cout << msg.substr(msg.find("˅")+2,(msg.find_last_of("˅")-5)-msg.find("˅")+2);
							SetConsoleTextAttribute(hConsole,7);
							cout << msg.substr(msg.find_last_of("˅")+1) << endl;
						}
						else {cout << msg.substr(msg.find_last_of("²")+1) << endl;}
						for (int i = 0; i < MAX_CLIENTS; i++)
						{
							if (client_array[i].socket != INVALID_SOCKET)
							{
								if (new_client.id != i || tempmsg[0] == '/') 
								{
									iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
								}
							}
						}
					}
				}
				else
				{
					for (int i = 0; i < MAX_CLIENTS; i++)
					{
						if (client_array[i].socket != INVALID_SOCKET)
						{
							if (new_client.id != i) 
							{
								iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
							}
						}
					}
				}
			}
			else
			{
				msg = "¹" + to_string(new_client.colour) + "²" + new_client.name + "² (" + to_string(new_client.id) + ") Disconnected.";
				SetConsoleTextAttribute(hConsole,new_client.colour);
				cout << msg.substr(msg.find("²")+2,(msg.find_last_of("²")-5)-msg.find("²")+2);
				SetConsoleTextAttribute(hConsole,7);
				cout << msg.substr(msg.find_last_of("²")+1) << endl;
 
				closesocket(new_client.socket);
				closesocket(client_array[new_client.id].socket);
				client_array[new_client.id].socket = INVALID_SOCKET;
 
				for (int i = 0; i < MAX_CLIENTS; i++)
				{
					if (client_array[i].socket != INVALID_SOCKET)
						iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
				}
				
				--CLIENT_COUNT;
				title.str("");
				title.clear();
				title << "dice_server: " << to_string(CLIENT_COUNT) << " client(s) connected";
				SetConsoleTitle(_T(title.str().c_str()));
 
				break;
			}
		}
	}
 
	thread.detach();
 
	return 0;
}
 
int main()
{
	string inp;
	/*cout << "Please enter the server port (leave blank for default [4562]): ";
	getline(cin,inp);
	if (inp.empty()) {PORT = "4562";}
	else {PORT = inp;}*/
	
	cout << "Please enter the client capacity (leave blank for default [6]): ";
	getline(cin,inp);
	if (inp.empty()) {MAX_CLIENTS = 6;}
	else {MAX_CLIENTS = atoi(inp.c_str());}
	
	WSADATA wsaData;
	struct addrinfo hints;
	struct addrinfo *server = NULL;
	char host_name[255];
	struct hostent *host_entry;
	SOCKET server_socket = INVALID_SOCKET;
	string msg = "";
	vector<client_type> client(MAX_CLIENTS);
	int num_clients = 0;
	int temp_id = -1;
	srand(time(NULL));
	thread my_thread[MAX_CLIENTS];
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	stringstream title;

	cout << "Intializing Winsock..." << endl;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
 
	//Setup hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
 
	cout << "Configuring server..." << endl;
	gethostname(host_name,255);
	host_entry = gethostbyname(host_name);
	LOCAL_IP = string(inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list));
	getaddrinfo(LOCAL_IP.c_str(), PORT.c_str(), &hints, &server);
 
	cout << "Creating server socket..." << endl;
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
 
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs
 
	cout << "Binding socket..." << endl;
	bind(server_socket, server->ai_addr, (int)server->ai_addrlen);
 
	EXT_IP = ext_ip();
	if (EXT_IP == "") EXT_IP = "Not Found";
	SetConsoleTextAttribute(hConsole,112);
	cout << "\n\t\t\t\t\t  \n  ";
	SetConsoleTextAttribute(hConsole,7);
	cout << "\t\t\t\t\t";
	SetConsoleTextAttribute(hConsole,112);
	cout << "  \n  ";
	SetConsoleTextAttribute(hConsole,7);
	cout << "  Started server:\t\t\t";
	SetConsoleTextAttribute(hConsole,112);
	cout << "  \n  ";
	SetConsoleTextAttribute(hConsole,7);
	cout << "\t\t\t\t\t";
	SetConsoleTextAttribute(hConsole,112);
	cout << "  \n  ";
	SetConsoleTextAttribute(hConsole,7);
	cout << "  Port:\t" << PORT << "\t\t\t";
	SetConsoleTextAttribute(hConsole,112);
	cout << "  \n  ";
	SetConsoleTextAttribute(hConsole,7);
	cout << "  Local IP:\t" << LOCAL_IP << "\t\t";
	SetConsoleTextAttribute(hConsole,112);
	cout << "  \n  ";
	SetConsoleTextAttribute(hConsole,7);
	cout << "  Public IP:\t" << EXT_IP << "\t\t";
	SetConsoleTextAttribute(hConsole,112);
	cout << "  \n  ";
	SetConsoleTextAttribute(hConsole,7);
	cout << "\t\t\t\t\t";
	SetConsoleTextAttribute(hConsole,112);
	cout << "  \n  ";
	cout << "\t\t\t\t\t  \n";
	SetConsoleTextAttribute(hConsole,7);
	
	cout << endl << "Awaiting clients, listening..." << endl << endl;
	listen(server_socket, SOMAXCONN);
 
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		client[i] = { -1, 7, "", INVALID_SOCKET };
	}
 
	while (1)
	{
		SOCKET incoming = INVALID_SOCKET;
		incoming = accept(server_socket, NULL, NULL);
 
		if (incoming == INVALID_SOCKET) continue;
 
		//Reset client count
		num_clients = -1;
 
		//Create temporary client ID
		temp_id = -1;
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			if (client[i].socket == INVALID_SOCKET && temp_id == -1)
			{
				client[i].socket = incoming;
				client[i].id = i;
				temp_id = i;
			}
 
			if (client[i].socket != INVALID_SOCKET) num_clients++;
		}
 
		if (temp_id != -1)
		{	
			char tempname[DEFAULT_BUFLEN] = "";
			memset(tempname, 0, DEFAULT_BUFLEN);
			client[temp_id].colour = rand()%15+1;
			int iResult = recv(client[temp_id].socket, tempname, DEFAULT_BUFLEN, 0);
			client[temp_id].name = tempname;
			//Send client ID
			SetConsoleTextAttribute(hConsole,client[temp_id].colour);
			cout << client[temp_id].name;
			SetConsoleTextAttribute(hConsole,7);
			cout << " (" << client[temp_id].id << ") Connected." << endl;
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (client[i].socket != INVALID_SOCKET)
				{
					msg = "¹" + to_string(client[temp_id].colour) + "²" + client[temp_id].name + "² (" + to_string(client[temp_id].id) + ") Accepted.";
					send(client[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
				}
			}
			msg = to_string(client[temp_id].id);
			send(client[temp_id].socket, msg.c_str(), strlen(msg.c_str()), 0);
 
			//Create client thread
			my_thread[temp_id] = thread(process_client, ref(client[temp_id]), ref(client), ref(my_thread[temp_id]));
			++CLIENT_COUNT;
		}
		else
		{
			msg = "Server is full.";
			send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
			cout << msg << endl;
		}
		title.str("");
		title.clear();
		title << "dice_server: " << to_string(CLIENT_COUNT) << " client(s) connected";
		SetConsoleTitle(_T(title.str().c_str()));
	}
 
	closesocket(server_socket);
 
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		my_thread[i].detach();
		closesocket(client[i].socket);
	}
 
	WSACleanup();
	cout << endl << "Server has closed successfully." << endl;
 
	system("pause");
	return 0;
}