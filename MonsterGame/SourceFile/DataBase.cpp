#include "DataBase.h"
#include <mutex>
#include "Driver.h"

namespace DataBase {
	std::string DataBase::Register(std::string s)
	{
		std::string username = s.substr(0, s.find('|'));
		std::string psw = s.substr(s.find('|') + 1, s.length() - s.find('|') - 1);
		Player p(username, psw);
		std::string player_init = p.MonsterList();
		PlayerInDB player{ username, psw, "0|0|" + player_init };
		
		try
		{
			auto &UserInfo = DataBase::Init();
			UserInfo.Insert(player);
		}
		catch (const std::exception&)
		{
			std::cout << "注册失败，用户名已存在" << std::endl;
			return "97";
		}
		
		std::cout << "注册成功！" << std::endl;
		return "98";
	}

	std::string DataBase::Login(std::string s)
	{
		std::string username = s.substr(0, s.find('|'));
		std::string psw = s.substr(s.find('|') + 1, s.length() - s.find('|') - 1);
		PlayerInDB helper;
		FieldExtractor field{ helper };

		auto &UserInfo = DataBase::Init();
		auto user = UserInfo.Query(PlayerInDB{}).Where(field(helper.username) == username).ToVector();
		if (user.empty()) { 
			std::cout << "用户名错误" << std::endl;
			return "94|2"; 
		}
		if (user.front().psw == psw) {
			std::cout << "登录成功" << std::endl;
			return "95|" + user.front().username + "|" + user.front().VSinfo;
		}
		else {
			std::cout << "密码错误" << std::endl;
			return "94|1";
		}
	}

	std::string DataBase::WinNotGetMonster(std::string username, std::string RoomNum)
	{
		PlayerInDB helper;
		FieldExtractor field{ helper };

		auto &UserInfo = DataBase::Init();
		auto user = UserInfo.Query(PlayerInDB{}).Where(field(helper.username) == username).ToVector();
		auto userinfo = user.front().VSinfo;
		int index = userinfo.find_first_of('|');
		int winNum = std::stoi(userinfo.substr(0, index));
		userinfo = userinfo.substr(index, userinfo.length() - index);
		++winNum;
		userinfo = std::to_string(winNum) + userinfo;
		UserInfo.Update(user.front());

		{
			std::cout << "释放房间加锁" << std::endl;
			std::lock_guard<std::mutex> lck(Driver::RoomPool_mtx);
			Driver::RoomPool.erase(RoomNum);
			std::cout << "释放房间解锁" << std::endl;
		}
		return "91";
	}

	std::string DataBase::WinGetMonster(std::string username, std::string info)
	{
		PlayerInDB helper;
		FieldExtractor field{ helper };

		auto &UserInfo = DataBase::Init();
		auto user = UserInfo.Query(PlayerInDB{}).Where(field(helper.username) == username).ToVector();
		user.front().VSinfo = info;
		UserInfo.Update(user.front());

			
		return "90";
	}

	std::string DataBase::FailLoseMonster(std::string username, int whichMonster, std::string RoomNum)
	{
		PlayerInDB helper;
		FieldExtractor field{ helper };

		auto &UserInfo = DataBase::Init();
		auto user = UserInfo.Query(PlayerInDB{}).Where(field(helper.username) == username).ToVector();
		auto userinfo = user.front().VSinfo;
		int index = userinfo.find_first_of('|');
		int winNum = std::stoi(userinfo.substr(0, index));
		userinfo = userinfo.substr(index + 1, userinfo.length() - index - 1);
		index = userinfo.find_first_of('|');
		int failNum = std::stoi(userinfo.substr(0, index));
		userinfo = userinfo.substr(index, userinfo.length() - index);
		std::string a, b;
		int i = 0;
		for (i = 0; i != userinfo.size(); ++i) {
			if (userinfo[i] == 'M')
				--whichMonster;
			if (whichMonster == -1) break;
		}
		a = userinfo.substr(0, i);
		index = userinfo.find_first_of('|', i);
		b = userinfo.substr(index + 1, userinfo.length() - index - 1);
		user.front().VSinfo = std::to_string(winNum) + "|" + std::to_string(failNum) + "|" + a + b;

		UserInfo.Update(user.front());
		return "89";
	}

	std::string DataBase::FailGive3new(std::string username, std::string RoomNum)
	{
		PlayerInDB helper;
		FieldExtractor field{ helper };

		auto &UserInfo = DataBase::Init();
		auto user = UserInfo.Query(PlayerInDB{}).Where(field(helper.username) == username).ToVector();
		auto userinfo = user.front().VSinfo;
		int index = userinfo.find_first_of('|');
		int winNum = std::stoi(userinfo.substr(0, index));
		userinfo = userinfo.substr(index + 1, userinfo.length() - index - 1);
		index = userinfo.find_first_of('|');
		int failNum = std::stoi(userinfo.substr(0, index));

		Player p("", "");
		std::string player_init = p.MonsterList();
		
		user.front().VSinfo = std::to_string(winNum) + "|" + std::to_string(failNum) + "|" + player_init;
		UserInfo.Update(user.front());
		return "88";
	}
}