#include "Player.h"
#include "Monster.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#define random(x) (rand()%x)

	Player::Player(std::string username, std::string psw) : _username(username), _name(username), _num_monster(3), _psw(psw), _status(Status::Bronze) 
	{ 
		_num_failure = 0;
		_num_win = 0;
		_winrate = 0;
		_DistributeMonster(); 
	}

	void Player::_DistributeMonster()
	{
		int t = 3;
		srand((unsigned int)time(NULL));


		while (t--) {
			int i = rand() % TOTAL_MONSTER;

			switch (i)
			{
			case 0:
				_MonsterVec.push_back((new Monster::Misaka_Mikoto));
				break;
			case 1:
				_MonsterVec.push_back((new Monster::Saten_Ruiko));
				break;
			case 2:
				_MonsterVec.push_back((new Monster::Shirai_Kuroko));
				break;
			case 3:
				_MonsterVec.push_back((new Monster::Uiharu_Kazari));
				break;
			case 4:
				_MonsterVec.push_back((new Monster::Saber));
				break;
			case 5:
				_MonsterVec.push_back((new Monster::Emiya));
				break;
			case 6:
				_MonsterVec.push_back((new Monster::Gilgamesh));
				break;
			case 7:
				_MonsterVec.push_back((new Monster::EastMoon));
				break;
			case 8:
				_MonsterVec.push_back((new Monster::HongHong));
				break;
			default:
				break;
			}
		}
	}

	// todo::maybe change style
	void Player::UpdateMilitaryExp(std::string a)
	{
		if (a == "W") ++_num_win;
		else ++_num_failure;

		_winrate = (double)_num_win / (_num_failure + _num_win);
	}

	void Player::GetMonster(Monster::Monster * p)
	{
		_MonsterVec.emplace_back(p);
		_num_monster = _MonsterVec.size();
		if (_MonsterVec.size() > 20) _status = Status::Gold;
		else if (_MonsterVec.size() > 10) _status = Status::Silver;
	}

	void Player::LostMonster(int i)
	{
		std::vector<Monster::Monster *>::iterator itr= _MonsterVec.begin();
		for (auto j = 0; j != i; ++j, ++itr);

		_MonsterVec.erase(itr);

		if (_MonsterVec.size() <= 3) _status = Status::Bronze;
		else if (_MonsterVec.size() <= 6) _status = Status::Silver;

		_num_monster = _MonsterVec.size();
	}

	// [monster num][MONSTER ID][S][B][D][A][R][E][T]
	std::string Player::MonsterList()
	{
		std::string s;
		int monster_num = _MonsterVec.size();
		char a[5];
		itoa(monster_num, a, 10);
		s = std::string{ a };
		s += std::string{ "|" };
		for (auto i : _MonsterVec) {
			s += (i->GetMonsterInfo());
			s += std::string{ "|" };
		}
		
		return s;
	}

	std::string Player::MonsterList(int i)
	{
		std::string s;
		int num = _MonsterVec.size();
		s = std::to_string(num) + "|";
		for (int j = 0; j != num; ++j) {
			if (j != i) {
				s += (_MonsterVec.at(j)->GetMonsterInfo());
				s += std::string{ "|" };
			}
		}
		return s;
	}

	void Player::Show()
	{
		std::cout << "GameName: " << _name << std::endl;
		std::cout << "Num of Monster: " << _num_monster << std::endl;
		std::cout << "Win: " << _num_win << " Failure: " << _num_failure << " WinRate: " << _winrate << std::endl;
		switch (_status)
		{
		case Player::Status::Gold:
			std::cout << "Rank: " << "Gold" << std::endl;
			break;
		case Player::Status::Silver:
			std::cout << "Rank: " << "Silver" << std::endl;
			break;
		case Player::Status::Bronze:
			std::cout << "Rank: " << "Bronze" << std::endl;
			break;
		default:
			break;
		}
	}

	// win_num|fail_num|monster_num|MSBDARET|MSBDARET|...
	// example: "0|0|3|M3S5B5D5A10R0E0T0|M3S5B5D5A10R0E0T0|M3S5B5D5A10R0E0T0|"
	Player::Player(std::string info)
	{
		int index1 = 0;
		int index2 = 0;
		std::string win_num_str;
		std::string fail_num_str;
		std::string monster_num_str;

		index1 = info.find_first_of('|');
		_username = info.substr(0, index1);
		info = info.substr(index1 + 1, info.length() - index1 - 1);
		index1 = info.find_first_of('|');
		win_num_str = info.substr(0, index1);
		_num_win = std::stoi(win_num_str);
		info = info.substr(index1 + 1, info.length() - index1 - 1);
		index1 = info.find_first_of('|');
		fail_num_str = info.substr(0, index1);
		_num_failure = std::stoi(fail_num_str);
		info = info.substr(index1 + 1, info.length() - index1 - 1);
		index1 = info.find_first_of('|');
		monster_num_str = info.substr(0, index1);
		_num_monster = std::stoi(monster_num_str);
		info = info.substr(index1 + 1, info.length() - index1 - 1);

		if (_num_monster > 6) _status = Status::Gold;
		else if (_num_monster > 3) _status = Status::Silver;
		else _status = Status::Bronze;

		//todo:: ½âÎömonster
		for (int i = 0; i != _num_monster; ++i) {
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

			//todo:: switch 
			switch (MonsterID)
			{
			case 1:
				_MonsterVec.push_back(new Monster::Misaka_Mikoto(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 2:
				_MonsterVec.push_back(new Monster::Saten_Ruiko(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 3:
				_MonsterVec.push_back(new Monster::Shirai_Kuroko(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 4:
				_MonsterVec.push_back(new Monster::Uiharu_Kazari(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 5:
				_MonsterVec.push_back(new Monster::Saber(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 6:
				_MonsterVec.push_back(new Monster::Emiya(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 7:
				_MonsterVec.push_back(new Monster::Gilgamesh(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 8:
				_MonsterVec.push_back(new Monster::EastMoon(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			case 9:
				_MonsterVec.push_back(new Monster::HongHong(strength, blood, defense, agile, rank, exp, attriPoint));
				break;
			default:
				break;
			}
		}

		if (_num_failure + _num_win == 0) _winrate = 0;
		else _winrate = double(_num_win) / double(_num_win + _num_failure);
		
	}

	void Player::GetMonsterInfo()
	{
		int j = 1;
		for (auto i : _MonsterVec) {
			std::cout << "±àºÅ£º" << j++ << std::endl;
			i->Show();
		}
	}

	std::string Player::GetMonsterInfo(int j)
	{
		auto i = _MonsterVec[j - 1];
		i->Show();
		return i->GetMonsterInfo();
	}

	Monster::Monster* Player::Get_A_Monster(int j)
	{
		return _MonsterVec[j - 1];
	}
