#pragma once
#include "DataBase.h"

namespace Protocol {
	/*
		00 - request a war
		01 - ready war or server monster attack first
		02 - client monster attack first
		99 - register
		98 - register success
		97 - register fail
		96 - login
		95 - login success
		94 - login fail
	*/
	std::string CommandDealForServer(std::string);
	std::string CommandDealForClient(std::string);
	std::string Register(std::string);
	std::string Login(std::string);
	std::string DealRequestWar(std::string);
	std::string Round(std::string , std::string);
	std::string WinNotGetMonster(std::string, std::string);
	std::string WinGetMonster(std::string, std::string);
	std::string FailLoseMonster(std::string, int, std::string);
	std::string FailGive3new(std::string, std::string);
	std::string ReturnOnlineUser();
}
