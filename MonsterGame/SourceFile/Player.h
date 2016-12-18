#pragma once
#include <string>
#include <vector>
#include "Monster.h"
#include <memory>

	class Player {
	public:
		enum class Status
		{
			Gold,
			Silver,
			Bronze,
		};
		Player() {}
		Player(std::string username, std::string psw);
		Player(std::string info);
		void GetMonster(Monster::Monster * p);
		void LostMonster(int i);
		void UpdateMilitaryExp(std::string);
		std::string MonsterList();
		std::string MonsterList(int);
		void Show();

		Status GetStatus() { return _status; }
		std::string GetUerName() { return _username; }
		std::string GetName() { return _name; }
		int GetMonsterNum() 
		{
			return _num_monster;
		}
		int GetWinNum()
		{
			return _num_win;
		}
		int GetFailureNum()
		{
			return _num_failure;
		}
		void GetMonsterInfo();
		std::string GetMonsterInfo(int);
		Monster::Monster* Get_A_Monster(int);
		double GetWinRate() { return _winrate; }
		std::vector<Monster::Monster *> GetMonsterVec() { return _MonsterVec; }
		void AddWinNum() { ++_num_win; _winrate = double(_num_win) / double(_num_win + _num_failure); }
		void AddFailNum() { ++_num_failure; _winrate = double(_num_win) / double(_num_win + _num_failure);}

	private:
		void _DistributeMonster();

		Status _status;
		std::string _username;
		std::string _name;
		std::string _psw;
		int _num_monster;
		int _num_win;
		int _num_failure;
		double _winrate;
		std::vector<Monster::Monster *> _MonsterVec;
	};
