#include "MsgSocket.h"

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

	std::cout << "服务端启动" << std::endl;
}

void MsgSocket::Server::ServerReceive(std::string &content)
{
	while (true)
	{
		_AddrLen = sizeof(_ClientAddr);
		_ClientSocket = accept(_ServerSocket, (struct sockaddr*)&_ClientAddr, &_AddrLen);
		if (_ClientSocket == INVALID_SOCKET)
		{
			std::cout << "Accept Failed::" << GetLastError() << std::endl;
			throw std::runtime_error("");
		}

		std::thread([=, &content]()
		{
			int _Ret = 0;
			char RecvBuffer[MAX_MSGCONTENT_LEN];

			while (true)
			{
				memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
				_Ret = recv(_ClientSocket, RecvBuffer, MAX_MSGCONTENT_LEN, 0);
				if (_Ret == 0 || _Ret == SOCKET_ERROR)
				{
					std::cout << "客户端退出!" << std::endl;
					break;
				}
				content = RecvBuffer;
				std::cout << "接收到客户信息为:" << content << std::endl;
			}
		} 
		).detach();
	}
}

void MsgSocket::Server::ServerSend(std::string &content)
{
//	while (true)
//	{
		_AddrLen = sizeof(_ClientAddr);
		_ClientSocket = accept(_ServerSocket, (struct sockaddr*)&_ClientAddr, &_AddrLen);
		if (_ClientSocket == INVALID_SOCKET)
		{
			std::cout << "Accept Failed::" << GetLastError() << std::endl;
			throw std::runtime_error("");
		}


		strcpy(_SendBuffer, content.c_str());
		_Ret = send(_ClientSocket, _SendBuffer, (int)strlen(_SendBuffer), 0);
		if (_Ret == SOCKET_ERROR)
		{
			std::cout << "Send Info Error::" << GetLastError() << std::endl;
			throw std::runtime_error("");
		}
//	}
}

MsgSocket::Server::~Server()
{
	closesocket(_ServerSocket);
	closesocket(_ClientSocket);
}

MsgSocket::Client::Client()
{
	_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_ClientSocket == INVALID_SOCKET)
	{
		std::cout << "Create Socket Failed::" << GetLastError() << std::endl;
		throw std::runtime_error("");
	}

	_ServerAddr.sin_family = AF_INET;
	//_ServerAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	inet_pton(AF_INET, IP_ADDRESS, (void *)&(_ServerAddr.sin_addr.s_addr));
	_ServerAddr.sin_port = htons(PORT);
	memset(_ServerAddr.sin_zero, 0x00, 8);

	_Ret = connect(_ClientSocket, (struct sockaddr*)&_ServerAddr, sizeof(_ServerAddr));
	if (_Ret == SOCKET_ERROR)
	{
		std::cout << "Connect Error::" << GetLastError() << std::endl;
		throw std::runtime_error("");
	}
	else
	{
		std::cout << "连接成功!" << std::endl;
	}
}

void MsgSocket::Client::ClientReceive(std::string &content)
{
	while (true)
	{
		int _Ret = 0;
		char RecvBuffer[MAX_MSGCONTENT_LEN];

		while (true)
		{
			memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
			_Ret = recv(_ClientSocket, RecvBuffer, MAX_MSGCONTENT_LEN, 0);
			if (_Ret == 0 || _Ret == SOCKET_ERROR)
			{
				std::cout << "服务器挂了!" << std::endl;
				throw std::runtime_error("");
			}
			content = RecvBuffer;
			std::cout << "接收到服务器信息为:" << content << std::endl;
		}
	}
	
}

void MsgSocket::Client::ClientSend()
{
	while (true)
	{
		std::cin.getline(_SendBuffer, sizeof(_SendBuffer));
		_Ret = send(_ClientSocket, _SendBuffer, (int)strlen(_SendBuffer), 0);
		if (_Ret == SOCKET_ERROR)
		{
			std::cout << "Send Info Error::" << GetLastError() << std::endl;
			throw std::runtime_error("");
		}
	}
}

MsgSocket::Client::~Client()
{
	closesocket(_ClientSocket);
}

void MsgSocket::Server::ServerWork(std::string& content)
{
	while (true)
	{
		_AddrLen = sizeof(_ClientAddr);
		_ClientSocket = accept(_ServerSocket, (struct sockaddr*)&_ClientAddr, &_AddrLen);
		if (_ClientSocket == INVALID_SOCKET)
		{
			std::cout << "Accept Failed::" << GetLastError() << std::endl;
			throw std::runtime_error("");
		}

		std::thread([=, &content]()
		{
			int _Ret = 0;
			char RecvBuffer[MAX_MSGCONTENT_LEN];

			while (true)
			{
				//todo:
				memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
				_Ret = recv(_ClientSocket, RecvBuffer, MAX_MSGCONTENT_LEN, 0);
				if (_Ret == 0 || _Ret == SOCKET_ERROR)
				{
					std::cout << "客户端退出!" << std::endl;
					break;
				}
				content = RecvBuffer;
				std::cout << "接收到客户信息为:" << content << std::endl;
				
				content = Protocol::CommandDeal(content);
				strcpy(_SendBuffer, content.c_str());
//				std::cin.getline(_SendBuffer, sizeof(_SendBuffer));
				_Ret = send(_ClientSocket, _SendBuffer, (int)strlen(_SendBuffer), 0);
				if (_Ret == SOCKET_ERROR)
				{
					std::cout << "Send Info Error::" << GetLastError() << std::endl;
					throw std::runtime_error("");
				}
			}
		}
		).detach();
	}
}

void MsgSocket::Client::ClientWork(std::string &content)
{
	while (true)
	{
		PrintCommand();
		std::cin.getline(_SendBuffer, sizeof(_SendBuffer));
		_Ret = send(_ClientSocket, _SendBuffer, (int)strlen(_SendBuffer), 0);
		if (_Ret == SOCKET_ERROR)
		{
			std::cout << "Send Info Error::" << GetLastError() << std::endl;
			throw std::runtime_error("");
		}

		int _Ret = 0;
		char RecvBuffer[MAX_MSGCONTENT_LEN];

		memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
		_Ret = recv(_ClientSocket, RecvBuffer, MAX_MSGCONTENT_LEN, 0);
		if (_Ret == 0 || _Ret == SOCKET_ERROR)
		{
			std::cout << "服务器挂了!" << std::endl;
			throw std::runtime_error("");
		}
		
		content = Protocol::CommandDeal(RecvBuffer);
		std::cout << "接收到服务器信息为:" << content << std::endl;
	}
}

void MsgSocket::Client::PrintCommand()
{
	std::cout << "command line menu:" << std::endl
		<< "99 username password" << std::endl
		<< "00 request a war -n" << std::endl; // the number of the player's monster
}