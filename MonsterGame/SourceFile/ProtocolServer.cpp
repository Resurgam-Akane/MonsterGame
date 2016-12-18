#include "Protocol.h"
#include "Driver.h"


namespace Protocol
{
	std::string CommandDealForServer(std::string s)
	{
		if (s.substr(0, 2) == "99") { return Protocol::Register(s.substr(3, s.length() - 3)); }
		else if (s.substr(0, 2) == "96") { return Protocol::Login(s.substr(3, s.length() - 3)); }
		else if (s.substr(0, 2) == "00") { return Protocol::DealRequestWar(s.substr(3, s.length() - 3)); }
		else if (s.substr(0, 2) == "01") { 
			s = s.substr(3, s.length() - 3);
			int index = s.find_first_of('|');
			std::string RoomNum = s.substr(0, index);
			s = s.substr(index + 1, s.length() - index - 1);
			return Protocol::Round(RoomNum, s);
		}
		else if (s.substr(0, 2) == "02") {
			s = s.substr(3, s.length() - 3);
			int index = s.find_first_of('|');
			std::string RoomNum = s.substr(0, index);
			s = s.substr(index + 1, s.length() - index - 1);
			return Protocol::Round(RoomNum, s);
		}
		else if (s.substr(0, 2) == "91") { // win, not get a monster
			s = s.substr(3, s.length() - 3);
			int index = s.find_first_of('|');
			std::string RoomNum = s.substr(0, index);
			s = s.substr(index + 1, s.length() - index - 1);
			index = s.find_first_of('|');
			std::string username = s.substr(0, index);
			std::string info = s.substr(index + 1, s.length() - index - 1);

			{
				std::cout << "释放房间加锁" << std::endl;
				std::lock_guard<std::mutex> lck(Driver::RoomPool_mtx);
				Driver::RoomPool.erase(RoomNum);
				std::cout << "释放房间解锁" << std::endl;
			}
			return Protocol::WinGetMonster(username, info);
		}
		else if (s.substr(0, 2) == "90") { // win, get a monster
			s = s.substr(3, s.length() - 3);
			int index = s.find_first_of('|');
			std::string RoomNum = s.substr(0, index);
			s= s.substr(index + 1, s.length() - index - 1);
			index = s.find_first_of('|');
			std::string username = s.substr(0, index);
			std::string info = s.substr(index + 1, s.length() - index - 1);
			
			{
				std::cout << "释放房间加锁" << std::endl;
				std::lock_guard<std::mutex> lck(Driver::RoomPool_mtx);
				Driver::RoomPool.erase(RoomNum);
				std::cout << "释放房间解锁" << std::endl;
			}
			return Protocol::WinGetMonster(username, info);
		}
		else if (s.substr(0, 2) == "89") { // lose a monster 
			s = s.substr(3, s.length() - 3);
			int index = s.find_first_of('|');
			std::string RoomNum = s.substr(0, index);
			s = s.substr(index + 1, s.length() - index - 1);
			index = s.find_first_of('|');
			std::string username = s.substr(0, index);
			std::string info = s.substr(index + 1, s.length() - index - 1);

			{
				std::cout << "释放房间加锁" << std::endl;
				std::lock_guard<std::mutex> lck(Driver::RoomPool_mtx);
				Driver::RoomPool.erase(RoomNum);
				std::cout << "释放房间解锁" << std::endl;
			}
			return Protocol::WinGetMonster(username, info);
		}
		else if (s.substr(0, 2) == "88") { // lose the last one and the get 3 new
			s = s.substr(3, s.length() - 3);
			int index = s.find_first_of('|');
			std::string RoomNum = s.substr(0, index);
			s = s.substr(index + 1, s.length() - index - 1);
			index = s.find_first_of('|');
			std::string username = s.substr(0, index);
			s = s.substr(index + 1, s.length() - 1 - index);
			Player p("", "");
			std::string player_init = p.MonsterList();
			{
				std::cout << "释放房间加锁" << std::endl;
				std::lock_guard<std::mutex> lck(Driver::RoomPool_mtx);
				Driver::RoomPool.erase(RoomNum);
				std::cout << "释放房间解锁" << std::endl;
			}

			Protocol::WinGetMonster(username,  s + "|"+ player_init);
			return "88|" + username + "|" +s + "|" + player_init;
		}
		else if (s.substr(0, 2) == "85") {
			return Protocol::ReturnOnlineUser();
		}
	}

	std::string Protocol::Register(std::string s)
	{
		return DataBase::DataBase::Register(s);
	}

	std::string Protocol::Login(std::string s)
	{
		return DataBase::DataBase::Login(s);
	}

	std::string Protocol::DealRequestWar(std::string s)
	{
		return Driver::DealRequestWar(s);
	}

	std::string Protocol::Round(std::string RoomNum, std::string s)
	{
		return Driver::Round(RoomNum, s);
	}

	std::string Protocol::WinGetMonster(std::string username, std::string info)
	{
		return DataBase::DataBase::WinGetMonster(username, info);
	}

	std::string Protocol::WinNotGetMonster(std::string username, std::string RoomNum)
	{
		return DataBase::DataBase::WinNotGetMonster(username, RoomNum);
	}

	std::string Protocol::FailLoseMonster(std::string username, int whichMonster, std::string RoomNum)
	{
		return DataBase::DataBase::FailLoseMonster(username, whichMonster, RoomNum);
	}

	std::string Protocol::FailGive3new(std::string username, std::string RoomNum)
	{
		return DataBase::DataBase::FailGive3new(username, RoomNum);
	}

	std::string Protocol::ReturnOnlineUser()
	{
		return Driver::ReturnOnlineUser();
	}
}
