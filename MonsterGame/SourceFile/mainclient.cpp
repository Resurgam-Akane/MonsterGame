// Client.cpp

#ifdef CLIENT

#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT 4000
#define IP_ADDRESS "127.0.0.1"

int main(int argc, char * argv[])
{
	WSADATA Ws;
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;
	int Ret = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;
	char SendBuffer[MAX_PATH];

	//Init Windows Socket
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		cout << "Init Windows Socket Failed::" << GetLastError() << endl;
		return -1;
	}
	//Create Socket
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Create Socket Failed::" << GetLastError() << endl;
		return -1;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	ServerAddr.sin_port = htons(PORT);
	memset(ServerAddr.sin_zero, 0x00, 8);

	Ret = connect(ClientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if (Ret == SOCKET_ERROR)
	{
		cout << "Connect Error::" << GetLastError() << endl;
		return -1;
	}
	else
	{
		cout << "连接成功!" << endl;
	}

	while (true)
	{
		cin.getline(SendBuffer, sizeof(SendBuffer));
		Ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
		if (Ret == SOCKET_ERROR)
		{
			cout << "Send Info Error::" << GetLastError() << endl;
			break;
		}
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}
#endif // CLIENT

#include "MsgSocket.h"
#include "GUIGame.h"
int main()
{
	int a,b;

	GUI::ClientGUI Gui;
	
	if (!Gui.Login()) {
		EggAche::MsgBox("Thank you for your playing.", "MonsterGame");
		return 0;
	}
lable1:
	do {
		b = Gui.UserInfo();
		if (b != 0 && b != 1) {
			EggAche::MsgBox("Thank you for your playing.", "MonsterGame");
			return 0;
		}
		else if (b == 0)
			a = Gui.SelectMonster();
		else if (b == 1)
			a = Gui.OnlineUser();
	} while (a != 1);	
	
	if (a == 1) {
		Gui.War();		
		if (Driver::IsWin)
			Gui.WinSettleAccount();
		else Gui.FailSettleAccount();
		goto lable1;
	}

	return 0;
}
