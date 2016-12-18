#pragma once
#include <string>
#include <iostream>
#include "ORMLite.h"
#include "Player.h"
using namespace BOT_ORM;
using namespace BOT_ORM::Expression;


namespace DataBase {

	struct PlayerInDB
	{
		std::string username;
		std::string psw;

		// [win_num]|[fail_num]|[monster_num]|[MSBDARET|MSBDARET|...]|
		std::string VSinfo;

		ORMAP("PlayerInDB", username, psw, VSinfo);
	};

	class DataBase
	{
	public: 
		static ORMapper&  Init()
		{
			static ORMapper UserInfo ("Player.db");
			try
			{
				UserInfo.CreateTbl(PlayerInDB{});
				std::cout << "用户信息数据库建立成功" << std::endl;
			}
			catch (const std::exception&)
			{
				std::cout << "用户信息数据库已存在" << std::endl;
			}
			return UserInfo;
		}

		static std::string Register(std::string s);
		static std::string Login(std::string s);
		static std::string WinNotGetMonster(std::string, std::string);
		static std::string WinGetMonster(std::string, std::string);
		static std::string FailLoseMonster(std::string, int, std::string);
		static std::string FailGive3new(std::string, std::string);
	};
}
