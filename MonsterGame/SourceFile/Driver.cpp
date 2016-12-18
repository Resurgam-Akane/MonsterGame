#include "Driver.h"
#include <iostream>
#include <string>

namespace Driver
{
	std::map<SOCKET, OnlineUser> OnlineUserPool;
	std::map<std::string, Room> RoomPool;
	Player user;
	std::string OnlineUserInfo;
	bool IsLogin = false;
	bool HasRequest = false;
	bool IsRequest = false;
	bool IsReady = false;
	bool IsWin = false;
	Monster::Monster *ClientWarMonster;
	Monster::Monster *ServerWarMonster;
	std::string RoomNum;
	std::mutex RoomPool_mtx;
	std::mutex OnlineUserPool_mtx;
}

namespace Driver
{
	std::string RequestWar()
	{
		int choice;
		user.GetMonsterInfo();
		std::cout << "Please choose a Monster:";
		std::cin >> choice;
		ClientWarMonster = user.Get_A_Monster(choice);
		ClientWarMonster->SetEnergy(0);

		
		return user.GetMonsterInfo(choice);
	}

	std::string DealRequestWar(std::string s)
	{
		Room room;
		char RoomNum_str[10];
		srand((unsigned int)time(NULL));
		int i = rand() % 23;
		switch (i)
		{
		case 0:
			room.Server = new Monster::Misaka_Mikoto();
			break;
		case 1:
			room.Server = new Monster::Saten_Ruiko();
			break;
		case 2:
			room.Server = new Monster::Shirai_Kuroko();
			break;
		case 3:
			room.Server = new Monster::Uiharu_Kazari();
			break;
		case 4:
			room.Server = new Monster::Saber();
			break;
		case 5:
			room.Server = new Monster::Emiya();
			break;
		case 6:
			room.Server = new Monster::Gilgamesh();
			break;
		case 7:
			room.Server = new Monster::EastMoon();
			break;
		case 8:
			room.Server = new Monster::HongHong();
			break;
		case 9:
			room.Server = new Monster::Gilgamesh(10,10,10,10,7);
			break;
		case 10:
			room.Server = new Monster::Gilgamesh(20, 5, 6, 2, 7);
			break;
		case 11:
			room.Server = new Monster::EastMoon(7, 20, 2, 5, 8);
			break;
		case 12:
			room.Server = new Monster::Saber(13, 20, 6, 10, 5);
			break;
		case 13:
			room.Server = new Monster::HongHong(6, 5, 5, 4, 9);
			break;
		case 14:
			room.Server = new Monster::HongHong(7, 5, 5, 5, 9);
			break;
		case 15:
			room.Server = new Monster::Saber(7, 6, 5, 5, 5);
			break;
		case 16:
			room.Server = new Monster::Misaka_Mikoto(9, 5, 5, 1, 1);
			break;
		case 17:
			room.Server = new Monster::Misaka_Mikoto(8, 13, 5, 3, 1);
			break;
		case 18:
			room.Server = new Monster::Saten_Ruiko(4, 4, 4, 9, 2);
			break;
		case 19:
			room.Server = new Monster::Saten_Ruiko(9, 5, 4, 2, 2);
			break;
		case 20:
			room.Server = new Monster::Saten_Ruiko(7, 20, 4, 9, 2);
			break;
		case 21:
			room.Server = new Monster::Saten_Ruiko(6, 6, 6, 6, 2);
			break;
		case 22:
			room.Server = new Monster::Uiharu_Kazari(10, 11, 6, 1, 4);;
			break;
		case 23:
			break;
		case 24:
			break;
		case 25:
			break;
		case 26:
			break;
		case 27:
			break;
		case 28:
			break;
		case 29:
			break;
		default:
			break;
		}
		room.Client = CreateMonsterFromInfo(s);

		if (room.Server->GetAgile() > room.Client->GetAgile()) {
			room.ServerFirst = true;
		}
		else {
			room.ServerFirst = false;
		}


		{
			std::cout << "创建房间加锁" << std::endl;
			std::lock_guard<std::mutex> lck(RoomPool_mtx);
			int RoomNum = RoomPool.size();
			room.RoomNum = std::string{ itoa(RoomNum, RoomNum_str, 10) };
			RoomPool.emplace(std::make_pair(room.RoomNum, room));
			std::cout << "创建房间解锁" << std::endl;
		}

		// "00|房间号|服务器monster的info"
		return "00|" + room.RoomNum + "|" + room.Server->GetMonsterInfo();
	}

	void DealOnlineUser(std::string c, SOCKET a)
	{
		if (c.substr(0, 2) == "95")
		{
			OnlineUser temp;
			c = c.substr(3, c.length() - 3);
			int index = c.find_first_of('|');
			temp.Username = c.substr(0, index);
			temp.MonsterList = "";
			for (int i = 0; i != c.length(); ++i)
				if (c[i] == 'M')
					temp.MonsterList += c[i + 1];
			{
				std::lock_guard<std::mutex> lck(OnlineUserPool_mtx);
				OnlineUserPool.emplace(std::make_pair(a, temp));
			}
		}
	}

	void DealOnlineUser(SOCKET a)
	{
		{
			std::lock_guard<std::mutex> lck(OnlineUserPool_mtx);
			OnlineUserPool.erase(a);
		}
	}

	std::string ReadyWar()
	{
		std::string s;

		while (s != "ready") {
			std::cout << "Please input ready:";
			std::cin >> s;
		}
		return "01|" + RoomNum + "|";
		
	}

	Monster::Monster* CreateMonsterFromInfo(std::string info)
	{
		int index1 = 0;

		index1 = info.find('S');
		std::string MonsterID_str = info.substr(0, index1);
		info = info.substr(index1, info.length() - index1);
		index1 = info.find('B');
		std::string Strength_str = info.substr(0, index1);
		info = info.substr(index1, info.length() - index1);
		index1 = info.find('D');
		std::string Blood_str = info.substr(0, index1);
		info = info.substr(index1, info.length() - index1);
		index1 = info.find('A');
		std::string Defense_str = info.substr(0, index1);			
		info = info.substr(index1, info.length() - index1);
		index1 = info.find('R');
		std::string Agile_str = info.substr(0, index1);
		info = info.substr(index1, info.length() - index1);
		index1 = info.find('E');
		std::string Rank_str = info.substr(0, index1);
		info = info.substr(index1, info.length() - index1);
		index1 = info.find('T');
		std::string Exp_str = info.substr(0, index1);
		info = info.substr(index1, info.length() - index1);
		index1 = info.find_first_of('|');
		std::string AttriPoint_str = info.substr(0, index1);
		info = info.substr(index1 + 1, info.length() - index1);
		int MonsterID = std::stoi(MonsterID_str.substr(1, MonsterID_str.length() - 1));
		int strength = std::stoi(Strength_str.substr(1, Strength_str.length() - 1));
		int blood = std::stoi(Blood_str.substr(1, Blood_str.length() - 1));
		int defense = std::stoi(Defense_str.substr(1, Defense_str.length() - 1));
		int agile = std::stoi(Agile_str.substr(1, Agile_str.length() - 1));
		int rank = std::stoi(Rank_str.substr(1, Rank_str.length() - 1));
		int exp = std::stoi(Exp_str.substr(1, Exp_str.length() - 1));
		int attriPoint = std::stoi(AttriPoint_str.substr(1, AttriPoint_str.length() - 1));


		switch (MonsterID)
		{
		case 1:
			return new Monster::Misaka_Mikoto(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 2:
			return new Monster::Saten_Ruiko(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 3:
			return new Monster::Shirai_Kuroko(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 4:
			return new Monster::Uiharu_Kazari(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 5:
			return new Monster::Saber(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 6:
			return new Monster::Emiya(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 7:
			return new Monster::Gilgamesh(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 8:
			return new Monster::EastMoon(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		case 9:
			return new Monster::HongHong(strength, blood, defense, agile, rank, exp, attriPoint);
			break;
		default:
			break;
		}

		return NULL;
	}

	std::string Round()
	{
		int useWhichSkill = 0;
		char Energy_str[10];
		//std::cout << "Server Monster:" << std::endl;
		//Driver::ServerWarMonster->Show();
		//std::cout << "Client Monster:" << std::endl;
		//Driver::ClientWarMonster->Show();
		std::cout << "Please input which attack:" << std::endl;
		std::cin >> useWhichSkill;
		return "02|" + Driver::RoomNum + "|" + Driver::ClientWarMonster->Attack(useWhichSkill) + "|" + std::string{ itoa(Driver::ClientWarMonster->GetEnergy(), Energy_str, 10) };
	}

	std::string Round(std::string RoomNum, std::string s)
	{
		std::string result;
		if (s == "") {
			{
				std::cout << "初始回合加锁" << std::endl;
				std::lock_guard<std::mutex> lck(Driver::RoomPool_mtx);
				if (Driver::RoomPool.at(RoomNum).ServerFirst) {
					std::cout << "初始回合解锁" << std::endl;
					return "01|" + ServerFirst(RoomPool.at(RoomNum).Server, RoomPool.at(RoomNum).Client);
				}
				else {
					std::cout << "初始回合解锁" << std::endl;
					return "02";
				}
			}
		}
		else {
			{
				std::cout << "后继回合加锁" << std::endl;
				std::lock_guard<std::mutex> lck(Driver::RoomPool_mtx);
				result = ServerChangeState(RoomPool.at(RoomNum).Client, RoomPool.at(RoomNum).Server, s);
				std::cout << "后继回合解锁" << std::endl;
			}
			return "04|" + result;  //reuslt中包括 服务器端monster的动作和状态改变结果，格式为[server monster action]|[change state]
		}
	}

	std::string ServerChangeState(Monster::Monster *active, Monster::Monster *passive, std::string info)  //服务器被动，客户端主动, info的格式:客户端Monster的动作|客户端Monster的能量
	{
		int BaseAttack;
		int ratio = 20;
		std::string PassiveDefense;
		std::string PassiveAttack;
		std::string ActiveAttack;
		std::string ActiveDefense;
		std::string FirstRound;
		std::string SecondRound;
		int PassiveBlood;
		int ActiveBlood;
		int ActiveEnergy;
		int PassiveEnergy;
		char PassiveEnergy_str[10];

		ActiveEnergy = std::stoi(info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1));
		active->SetEnergy(ActiveEnergy);
		info = info.substr(0, info.find_first_of('|'));
		BaseAttack = ratio * active->GetStrength() / passive->GetDefense();
		if (info != "At") {
			ActiveAttack = (active->GetSkill())[info[1] - '1'];
			if (ActiveAttack == "BiLiBiLi") {
				active->Attack(1);
				BaseAttack *= 1.5;
			}
			else if (ActiveAttack == "Excalibur") {
				active->Attack(1);
				BaseAttack *= 1.4;
			}
			else if (ActiveAttack == "Caladbolg") {
				active->Attack(1);
				BaseAttack *= 1.1;
			}
			else if (ActiveAttack == "Enuma Elish") {
				active->Attack(1);
				BaseAttack *= 3.0;
			}
			else if (ActiveAttack == "Gate_of_Babylon") {
				active->Attack(1);
				BaseAttack *= 1.5;
			}
			else if (ActiveAttack == "Pure Yang fire") {
				active->Attack(1);
				BaseAttack *= 1.2;
			}
			else if (ActiveAttack == "Fox read") {
				active->Attack(2);
				BaseAttack *= 1.5;
			}
		}

		PassiveDefense = passive->Defense();
		if (PassiveDefense != "Ds") {
			PassiveDefense = (passive->GetSkill())[PassiveDefense[PassiveDefense.length() - 1] - '1'];
			if (PassiveDefense == "Sudden Dodge") BaseAttack = 0;
			else if (PassiveDefense == "Push Gas Through Body") BaseAttack = 0 - int(0.66 * BaseAttack); // recover 2/3 blood
			else if (PassiveDefense == "Code Geasse") BaseAttack = int(0.8 * BaseAttack);
			else if (PassiveDefense == "Invisible_Air") BaseAttack = int(0.8 * BaseAttack);
			else if (PassiveDefense == "Rho_Aias") BaseAttack = int(0.5 * BaseAttack);
			else if (PassiveDefense == "The art of fox") BaseAttack = passive->GetCurBlood() - passive->GetCalBlood();
		}

		PassiveBlood = passive->SubBlood(BaseAttack);

		if (PassiveBlood == 0) return std::string{ "ServerDead" };
		else {
			char PassiveBlood_str[10];
			FirstRound =  PassiveDefense + "|" + std::string{ itoa(PassiveBlood, PassiveBlood_str, 10) };
		}

		BaseAttack = ratio * passive->GetStrength() / active->GetDefense();
		int SkillSize = passive->GetSkill().size(), j;
		srand((unsigned int)time(NULL));
		j = rand() % (SkillSize + 1);
		PassiveAttack = passive->Attack(j);
		if (PassiveAttack != "At") {
			PassiveAttack = (passive->GetSkill())[PassiveAttack[PassiveAttack.length() - 1] - '1'];
			if (PassiveAttack == "BiLiBiLi") BaseAttack *= 1.5;
			else if (PassiveAttack == "Excalibur") BaseAttack *= 1.4;
			else if (PassiveAttack == "Caladbolg") BaseAttack *= 1.1;
			else if (PassiveAttack == "Enuma Elish") BaseAttack *= 3.0;
			else if (PassiveAttack == "Gate_of_Babylon") BaseAttack *= 1.5;
			else if (PassiveAttack == "Pure Yang fire") BaseAttack *= 1.2;
			else if (PassiveAttack == "Fox read") BaseAttack *= 1.5;
		}

		ActiveDefense = active->Defense();
		if (ActiveDefense != "Ds") {
			ActiveDefense = (active->GetSkill())[ActiveDefense[ActiveDefense.length() - 1] - '1'];
			if (ActiveDefense == "Sudden Dodge") BaseAttack = 0;
			else if (ActiveDefense == "Push Gas Through Body") BaseAttack = 0 - int(0.66 * BaseAttack); // recover 2/3 blood
			else if (ActiveDefense == "Code Geasse") BaseAttack = int(0.8 * BaseAttack);
			else if (ActiveDefense == "Invisible_Air") BaseAttack = int(0.8 * BaseAttack);
			else if (ActiveDefense == "Rho_Aias") BaseAttack = int(0.5 * BaseAttack);
			else if (ActiveDefense == "The art of fox") BaseAttack = active->GetCurBlood() - active->GetCalBlood();
		}

		ActiveBlood = active->SubBlood(BaseAttack);
		if (ActiveBlood == 0) return std::string{ "ClientDead" };
		else {
			char ActiveBlood_str[10];
			SecondRound = PassiveAttack + "|" + ActiveDefense + "|" + std::string{ itoa(ActiveBlood, ActiveBlood_str, 10) };
		}

		active->AddEnergy();
		passive->AddEnergy();

		return FirstRound + "|" + SecondRound + "|" + std::string{ itoa(passive->GetEnergy(), PassiveEnergy_str, 10) }+"|" + std::string{ itoa(active->GetEnergy(), PassiveEnergy_str, 10) };
	}

	std::string ClientChangeState(std::string info) //格式："Ds|480|At|Rho_Aias|493|1|1" [服务器monster防守动作]|[服务器monster剩余气血]|[服务器monster攻击动作]|[客户端monster防守动作]|[客户端monster剩余气血]|[服务器Monster能量]|[客户端Monster能量]
	{
		std::string ServerMonsterDefense, ServerMonsterBlood, ServerMonsterAttack, ClientMonsterDefense, ClientMonsterBlood, ServerMonsterEnergy, ClientMonsterEnergy;

		if (info == "ServerDead") return "胜利";
		else if (info == "ClientDead") return "失败";
		ServerMonsterDefense = info.substr(0, info.find_first_of('|'));
		info = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		ServerMonsterBlood = info.substr(0, info.find_first_of('|'));
		info = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		ServerMonsterAttack = info.substr(0, info.find_first_of('|'));
		info = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		ClientMonsterDefense = info.substr(0, info.find_first_of('|'));
		info = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		ClientMonsterBlood = info.substr(0, info.find_first_of('|'));
		info = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		ServerMonsterEnergy = info.substr(0, info.find_first_of('|'));
		ClientMonsterEnergy = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		 

		Driver::ServerWarMonster->SetBlood(std::stoi(ServerMonsterBlood));
		Driver::ClientWarMonster->SetBlood(std::stoi(ClientMonsterBlood));
		Driver::ServerWarMonster->SetEnergy(std::stoi(ServerMonsterEnergy));
		Driver::ClientWarMonster->SetEnergy(std::stoi(ClientMonsterEnergy));

		std::cout << "服务器Monster防守:" << ServerMonsterDefense << std::endl;
		std::cout << "服务器Monster气血:" << ServerMonsterBlood << std::endl;
		std::cout << "服务器Monster能量:" << ServerMonsterEnergy << std::endl << std::endl;
		std::cout << "服务器Monster攻击:" << ServerMonsterAttack << std::endl;
		std::cout << "客户端Monster防守:" << ClientMonsterDefense << std::endl;
		std::cout << "客户端Monster气血:" << ClientMonsterBlood << std::endl;
		std::cout << "客户端Monster能量:" << ClientMonsterEnergy << std::endl << std::endl;

		return "继续对战！！！";
	}

	std::string ServerFirst(Monster::Monster *active, Monster::Monster *passive)  //[服务器Monster信息]|[服务器monster攻击动作] | [客户端monster防守动作] | [客户端monster剩余气血] | [服务器Monster能量] | [客户端Monster能量]
	{
		int BaseAttack;
		int ratio = 20;
		std::string PassiveDefense;
		std::string FirstRound;
		int PassiveBlood;
		char ActiveBlood_str[10];
		char PassiveBlood_str[10];

		BaseAttack = ratio * active->GetStrength() / passive->GetDefense();
		PassiveDefense = passive->Defense();
		if (PassiveDefense != "Ds") {
			PassiveDefense = (passive->GetSkill())[PassiveDefense[PassiveDefense.length() - 1] - '1'];
			if (PassiveDefense == "Sudden Dodge") BaseAttack = 0;
			else if (PassiveDefense == "Push Gas Through Body") BaseAttack = 0 - int(0.66 * BaseAttack); // recover 2/3 blood
			else if (PassiveDefense == "Code Geasse") BaseAttack = int(0.8 * BaseAttack);
			else if (PassiveDefense == "Invisible_Air") BaseAttack = int(0.8 * BaseAttack);
			else if (PassiveDefense == "Rho_Aias") BaseAttack = int(0.5 * BaseAttack);
			else if (PassiveDefense == "The art of fox") BaseAttack = passive->GetCurBlood() - passive->GetCalBlood();
		}

		PassiveBlood = passive->SubBlood(BaseAttack);

		return "At|" + PassiveDefense + "|" + std::string{ itoa(PassiveBlood, PassiveBlood_str,10) } +"|" + "0|0";

	}

	std::string ClientChangeStateForServerFirst(std::string info) //[服务器Monster信息]|[服务器monster攻击动作] | [客户端monster防守动作] | [客户端monster剩余气血] | [服务器Monster能量] | [客户端Monster能量]
	{
		std::string ServerMonsterAttack, ClientMonsterDefense, ClientMonsterBlood;

		ServerMonsterAttack = info.substr(0, info.find_first_of('|'));
		info = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		ClientMonsterDefense = info.substr(0, info.find_first_of('|'));
		info = info.substr(info.find_first_of('|') + 1, info.length() - info.find_first_of('|') - 1);
		ClientMonsterBlood = info.substr(0, info.find_first_of('|'));

		std::cout << "服务器Monster气血:" << Driver::ServerWarMonster->GetCurBlood() << std::endl;
		std::cout << "服务器Monster能量:" << "0" << std::endl << std::endl;
		std::cout << "服务器Monster攻击:" << ServerMonsterAttack << std::endl;
		std::cout << "客户端Monster防守:" << ClientMonsterDefense << std::endl;
		std::cout << "客户端Monster气血:" << ClientMonsterBlood << std::endl;
		std::cout << "客户端Monster能量:" << "0" << std::endl << std::endl;

		return "继续对战！！！";
	}

	std::string ReturnOnlineUser()
	{
		std::lock_guard<std::mutex> lck(Driver::OnlineUserPool_mtx);
		std::string s = "85|" + std::to_string(OnlineUserPool.size()) + "|";
		int i = 0;
		for (auto itr : OnlineUserPool) {
			if (i > 9) break;
			else {
				s += itr.second.Username + "|";
				s += itr.second.MonsterList + "|";
				++i;
			}
		}

		return s;
	}
}
