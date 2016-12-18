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
		std::cout << "连接成功！" << std::endl;
	}
}

MsgSocket::Client::~Client()
{
	closesocket(_ClientSocket);
}

std::string MsgSocket::Client::ClientWork(std::string content)
{
		strcpy(_SendBuffer, content.c_str());
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
			std::cout << "服务器挂了！" << std::endl;
			throw std::runtime_error("");
		}

		std::cout << "收到服务器消息：" << RecvBuffer << std::endl;
		return content = std::string{ RecvBuffer };
}

void MsgSocket::Client::PrintCommand()
{
	std::cout << "command line menu:" << std::endl
		<< "99 username password" << std::endl
		<< "96 username password" << std::endl
		<< "00 request a war" << std::endl
		<< "01 ready war" << std::endl
		<< "02 client attack" << std::endl;
}

bool MsgSocket::Client::InputFormatSuspect(std::string &s)
{
	if (s == "99") {
		std::pair<bool, std::string> username = UsernameSuspect();
		std::pair<bool, std::string> psw = PswSuspect();
		if (!username.first) {
			s = "Username has illegle char";
			return false;
		}
		else if (!psw.first) {
			s = "Password is at least 6 length.";
			return false;
		}
		else {
			s += " " + username.second + " " + psw.second;
			return true;
		}
	}
	else if (s == "96") {
		std::pair<bool, std::string> username = UsernameSuspect();
		std::pair<bool, std::string> psw = PswSuspect();
		if (!username.first) {
			s = "Username has illegle char";
			return false;
		}
		else if (!psw.first) {
			s = "Password is at least 6 length.";
			return false;
		}
		else {
			s += " " + username.second + " " + psw.second;
			return true;
		}
	}
	else if (s == "00") {
		if (!Driver::IsLogin) {
			s = "Please login first.";
			return false;
		}
		else {
			s += "|" + Driver::RequestWar(); //00|[monsterinfo]
			return true;
		}
	}
	else if (s == "01") {
		if (!Driver::IsLogin) {
			s = "Please login first.";
			return false;
		}
		else if (!Driver::IsRequest) {
			s = "Please request a war first.";
			return false;
		}
		else {
			s = Driver::ReadyWar();
			return true;
		}
	}
	else if (s == "02") {
		
		if (!Driver::IsLogin) {
			s = "Please login first.";
			return false;
		}
		else if (!Driver::IsRequest) {
			s = "Please request a war first.";
			return false;
		}
		else if (!Driver::IsReady) {
			s = "Please ready for the war first.";
			return false;
		}
		else {
			s = Driver::Round();
			return true;
		}
	}
	
}

std::pair<bool, std::string> UsernameSuspect()
{
	std::string username;
	bool b = true;
	std::cout << "Please input username(only number and alpha can use):";
	std::cin >> username;
	for (auto i : username)
		if (!isalnum(i)) {
			b = false;
			return std::make_pair(b, std::string{});
		}
	return std::make_pair(b, username);
}

std::pair<bool, std::string> PswSuspect()
{
	std::string psw;
	bool b = true;
	std::cout << "Please input password(at least 6 length):";
	std::cin >> psw;
	if (psw.length() < 6) {
		b = false;
		return std::make_pair(b, std::string{});
	}
	return std::make_pair(b, psw);
}
