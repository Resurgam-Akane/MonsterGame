#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Protocol.h"
#include <utility>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12306
#define IP_ADDRESS "10.201.12.49"
#define MAX_MSGCONTENT_LEN 1024

namespace MsgSocket
{
	class MsgSocketInit
	{
	public:
		MsgSocketInit();
		~MsgSocketInit();
	};

	class Server
	{
	public:
		Server();
		void ServerWork(std::string&);
		~Server();

	private:
		SOCKET _ServerSocket;
		struct sockaddr_in _LocalAddr, _ClientAddr;
		int _Ret = 0;
		int _AddrLen = 0;
		char _SendBuffer[MAX_MSGCONTENT_LEN];
	};

	class Client
	{
	public:
		Client();
		std::string ClientWork(std::string);
		void PrintCommand();
		bool InputFormatSuspect(std::string&);
		~Client();

	private:
		SOCKET _ClientSocket, _ServerSocket;
		struct sockaddr_in _ServerAddr;
		int _Ret = 0;
		int _AddrLen = 0;
		char _SendBuffer[MAX_MSGCONTENT_LEN];
	};
}

std::pair<bool, std::string> UsernameSuspect();
std::pair<bool, std::string> PswSuspect();
