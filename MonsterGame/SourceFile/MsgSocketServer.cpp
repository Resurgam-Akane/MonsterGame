#include "MsgSocket.h"
#include "Driver.h"
#include <iostream>
#include <exception>
#include <thread>



MsgSocket::MsgSocketInit::MsgSocketInit()
{
	WSADATA  Ws;
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0) {
		std::cout << "Init Windows Socket Failed::" << GetLastError() << std::endl;
		throw std::runtime_error("");
	}
}

MsgSocket::MsgSocketInit::~MsgSocketInit()
{
	WSACleanup();
}

MsgSocket::Server::Server()
{
	_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_ServerSocket == INVALID_SOCKET)
	{
		std::cout << "Create Socket Failed::" << GetLastError() << std::endl;
		throw std::runtime_error("");
	}

	_LocalAddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP_ADDRESS, (void *)&(_LocalAddr.sin_addr.s_addr));
	_LocalAddr.sin_port = htons(PORT);
	memset(_LocalAddr.sin_zero, 0x00, 8);

	//Bind Socket
	_Ret = bind(_ServerSocket, (struct sockaddr*)&_LocalAddr, sizeof(_LocalAddr));
	if (_Ret != 0)
	{
		std::cout << "Bind Socket Failed::" << GetLastError() << std::endl;
		throw std::runtime_error("");
	}
	//listen
	_Ret = listen(_ServerSocket, 10);
	if (_Ret != 0)
	{
		std::cout << "listen Socket Failed::" << GetLastError() << std::endl;
		throw std::runtime_error("");
	}

	std::cout << "服务器端启动" << std::endl;
}

MsgSocket::Server::~Server()
{
	closesocket(_ServerSocket);
}

void MsgSocket::Server::ServerWork(std::string& content)
{
	while (true)
	{
		_AddrLen = sizeof(_ClientAddr);
		auto _ClientSocket = accept(_ServerSocket, (struct sockaddr*)&_ClientAddr, &_AddrLen);
		if (_ClientSocket == INVALID_SOCKET)
		{
			std::cout << "Accept Failed::" << GetLastError() << std::endl;
			throw std::runtime_error("");
		}

		std::thread([=]()
		{
			int _Ret = 0;
			char RecvBuffer[MAX_MSGCONTENT_LEN];
			char SendBuffer[MAX_MSGCONTENT_LEN];
			std::string c;
			

			while (true)
			{
				//todo:
				memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
				_Ret = recv(_ClientSocket, RecvBuffer, MAX_MSGCONTENT_LEN, 0);
				if (_Ret == 0 || _Ret == SOCKET_ERROR)
				{
					Driver::DealOnlineUser(_ClientSocket);
					std::cout << "客户端退出" << std::endl;
					break;
				}
				c = RecvBuffer;
				std::cout << "接收客户端信息为：" << c << std::endl;

				c = Protocol::CommandDealForServer(c);
				strcpy(SendBuffer, c.c_str());
				Driver::DealOnlineUser(c, _ClientSocket);
				_Ret = send(_ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
				if (_Ret == SOCKET_ERROR)
				{
					std::cout << "Send Info Error::" << GetLastError() << std::endl;
					throw std::runtime_error("");
				}
			}

			closesocket(_ClientSocket);
		}
		).detach();
	}
}