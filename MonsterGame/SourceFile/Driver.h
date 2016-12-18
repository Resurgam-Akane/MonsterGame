#pragma once
#include "Player.h"
#include <winsock2.h>
#include <map>
#include <mutex>

namespace Driver
{
	struct Room
	{
		std::string RoomNum;
		Monster::Monster *Server, *Client;
		bool ServerFirst;
	};

	struct OnlineUser
	{
		std::string Username;
		std::string MonsterList;
	};

	extern std::map<std::string, Room> RoomPool;
	extern std::map<SOCKET, OnlineUser> OnlineUserPool;
	extern Player user;
	extern std::string OnlineUserInfo;
	extern bool IsLogin;
	extern bool HasRequest;
	extern bool IsRequest;
	extern bool IsReady;
	extern bool IsWin;
	extern Monster::Monster *ClientWarMonster;
	extern Monster::Monster *ServerWarMonster;
	extern std::string RoomNum;
	extern std::mutex RoomPool_mtx;
	extern std::mutex G_var_mtx; // todo:: delete

	std::string RequestWar();
	std::string DealRequestWar(std::string);
	Monster::Monster* CreateMonsterFromInfo(std::string);
	std::string ReadyWar();
	std::string Round();
	std::string Round(std::string, std::string);
	std::string ServerChangeState(Monster::Monster *active, Monster::Monster *passive, std::string info);
	std::string ClientChangeState(std::string);
	std::string ServerFirst(Monster::Monster *active, Monster::Monster *passive);
	std::string ClientChangeStateForServerFirst(std::string);
	void DealOnlineUser(std::string, SOCKET);
	void DealOnlineUser(SOCKET);
	std::string ReturnOnlineUser();
}
