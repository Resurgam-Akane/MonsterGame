#include "Monster.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

#define STRENGTH_TO_CAL 100
#define BLOOD_TO_CAL 100
#define DEFENSE_TO_CAL 100
#define AGILE_TO_CAL 1
namespace Monster {

	const int GradeTable[14] = { 50,100,150,200,250,300,350,400,450,500,550,600,650,700 };

	Monster::Monster(Phyle phyle, std::string name, int s, int b, int d, int a, int id)
	{
		_phyle = phyle;
		_MonsterID = id;

		_strength = s;
		_blood = b;
		_defense = d;
		_agile = a;

		_cal_strength = 0;
		_cal_blood = 0;
		_cal_defense = 0;
		_cal_agile = 0;

		switch (_phyle)
		{
		case Phyle::Strength:
			_strength += 5;
			break;
		case Phyle::Blood:
			_blood += 5;
			break;
		case Phyle::Defense:
			_defense += 5;
			break;
		case Phyle::Agile:
			_agile += 5;
			break;
		default:
			break;
		}

		_cal_strength = STRENGTH_TO_CAL * _strength;
		_cal_blood = BLOOD_TO_CAL * _blood;
		_cal_defense = DEFENSE_TO_CAL * _defense;
		_cal_agile = AGILE_TO_CAL * _agile;

		_current_blood = _cal_blood;
		_name = name;
		_rank = 0;
		_exp = 0;
		_energy = 0;
		_attribute_point = 0;

	}

	Monster::Monster(Phyle phyle, std::string name, int s, int b, int d, int a, int id, int r, int e, int t)
	{
		_phyle = phyle;
		_MonsterID = id;

		_strength = s;
		_blood = b;
		_defense = d;
		_agile = a;

		_cal_strength = 0;
		_cal_blood = 0;
		_cal_defense = 0;
		_cal_agile = 0;

		_cal_strength = STRENGTH_TO_CAL * _strength;
		_cal_blood = BLOOD_TO_CAL * _blood;
		_cal_defense = DEFENSE_TO_CAL * _defense;
		_cal_agile = AGILE_TO_CAL * _agile;

		_current_blood = _cal_blood;
		_name = name;
		_rank = r;
		_exp = e;
		_energy = 0;
		_attribute_point = t;
	}

	void Monster::AddEnergy()
	{
		if (_current_blood >= int(0.3*_cal_blood)) {
			if (_energy + 1 >= 8) _energy = 8;
			else _energy += 1;
		}
		else {
			if (_energy + 2 >= 8) _energy = 8;
			else _energy += 2;
		}
	}

	void Monster::SubEnergy(int j)
	{
		if (_energy - j <= 0) _energy = 0;
		else _energy -= j;
	}

	int Monster::SubBlood(int a)
	{
		if (a < 0) {
			_current_blood -= a;
			return _current_blood;
		}
		else if (_current_blood <= a) {
			_current_blood = 0;
			return 0;
		}
		else {
			_current_blood -= a;
			return _current_blood;
		}
	}

	void Monster::Levelup(int exp)
	{
		_exp += exp;
		if (_rank < 15 && _exp >= GradeTable[_rank]) {
			_exp -= GradeTable[_rank++];
			_attribute_point += 3;
			++_strength;
			++_blood;
			++_defense;
			++_agile;

			switch (_phyle)
			{
			case Phyle::Strength:
				++_strength;
				break;
			case Phyle::Blood:
				++_blood;
				break;
			case Phyle::Defense:
				++_defense;
				break;
			case Phyle::Agile:
				++_agile;
				break;
			default:
				break;
			}

			_cal_strength = STRENGTH_TO_CAL * _strength;
			_cal_blood = BLOOD_TO_CAL * _blood;
			_cal_defense = DEFENSE_TO_CAL * _defense;
			_cal_agile = AGILE_TO_CAL * _agile;
			_current_blood = _cal_blood;
		}
	}

	void Monster::AttrPointDistribution()
	{
		int addStrength = 0, addBlood = 0, addDefense = 0, addAgile = 0;

		if (_attribute_point == 0) return;
		
		std::cout << "Please add point:";
		std::cin >> addStrength >> addBlood >> addDefense >> addAgile;
		if (addStrength + addBlood + addDefense + addAgile <= _attribute_point) {

			_attribute_point -= addStrength + addBlood + addDefense + addAgile;
			_strength += addStrength;
			_blood += addBlood;
			_defense += addDefense;
			_agile += addAgile;

			_cal_strength = STRENGTH_TO_CAL * _strength;
			_cal_blood = BLOOD_TO_CAL * _blood;
			_cal_defense = DEFENSE_TO_CAL * _defense;
			_cal_agile = AGILE_TO_CAL * _agile;
			_current_blood = _cal_blood;
		}
		else std::cout << "add Fail." << std::endl << std::endl;

	}

	void Monster::Show()
	{
		std::cout << "name: " << _name << std::endl;
		switch (_phyle)
		{
		case Phyle::Strength:
			std::cout << "Phyle: Strength" << std::endl;
			break;
		case Phyle::Blood:
			std::cout << "Phyle: Blood" << std::endl;
			break;
		case Phyle::Defense:
			std::cout << "Phyle: Defense" << std::endl;
			break;
		case Phyle::Agile:
			std::cout << "Phyle: Agile" << std::endl;
			break;
		default:
			break;
		}

		std::cout << "Rank: " << _rank << std::endl;;
		std::cout << "Strength: " << _strength << "  " << _cal_strength << std::endl;
		std::cout << "Blood: " << _blood << "  " << _current_blood << std::endl;
		std::cout << "Defense: " << _defense << "  " << _cal_defense << std::endl;
		std::cout << "Agile: " << _agile << "  " << _cal_agile << std::endl;
		std::cout << "Skill: ";
		for (auto i : _skill)
			std::cout << i << " ";
		std::cout << std::endl;
		std::cout << "exp: " << _exp << std::endl;
		std::cout << "AttributePoint: " << _attribute_point << std::endl << std::endl;
	}

	void Monster::SetSkill(std::string a, std::string b)
	{
		if (a != "") _skill.push_back(a);
		if (b != "") _skill.push_back(b);
	}

	std::string Monster::GetMonsterInfo()
	{
		std::string s;
		char monster_id[20];
		char string_stength[20];
		char string_blood[20];
		char string_defense[20];
		char string_agile[20];
		char string_rank[20];
		char string_exp[30];
		char string_point[20];

		itoa(GetMonsterID(), monster_id, 10);
		itoa(GetStrength(), string_stength, 10);
		itoa(GetBlood(), string_blood, 10);
		itoa(GetDefense(), string_defense, 10);
		itoa(GetAgile(), string_agile, 10);
		itoa(GetRank(), string_rank, 10);
		itoa(GetExp(), string_exp, 10);
		itoa(GetPoint(), string_point, 10);

		s = s + "M" + std::string{ monster_id };
		s = s + "S" + std::string{ string_stength };
		s = s + "B" + std::string{ string_blood };
		s = s + "D" + std::string{ string_defense };
		s = s + "A" + std::string{ string_agile };
		s = s + "R" + std::string{ string_rank };
		s = s + "E" + std::string{ string_exp };
		s = s + "T" + std::string{ string_point };
		
		return s;
	}

	//格式:01|At ... 普通攻击
	//	  01|S1 ... 使用技能
	std::string Misaka_Mikoto::Attack(int j)
	{
		if (GetEnergy() >= 4 && j == 1) {
			SubEnergy(4);
			return std::string{ "S1" };
		}
		else {
			return std::string{ "At" };
		}
	}

	std::string Saten_Ruiko::Defense()
	{
		srand((unsigned int)time(NULL));
		int i = rand() % 10;
		if (i < _prob_use_defense_skill)
		{
			return std::string{ "S1" };
		}
		else {
			return std::string{ "Ds" };
		}
	}

	std::string Shirai_Kuroko::Defense()
	{
		srand((unsigned int)time(NULL));
		int i = rand() % 10;
		if (i < _prob_use_defense_skill)
		{
			return std::string{ "S1" };
		}
		else {
			return std::string{ "Ds" };
		}
	}

	std::string Uiharu_Kazari::Defense()
	{
		srand((unsigned int)time(NULL));
		int i = rand() % 10;
		if (i < _prob_use_defense_skill)
		{
			return std::string{ "S1" };
		}
		else {
			return std::string{ "Ds" };
		}
	}

	std::string Saber::Attack(int j)
	{
		if (GetEnergy() >= 4 && j == 1) {
			SubEnergy(4);
			return std::string{ "S1" };
		}
		else {
			return std::string{ "At" };
		}
	}

	std::string Saber::Defense()
	{
		srand((unsigned int)time(NULL));
		int i = rand() % 10;
		if (i < _prob_use_defense_skill) {
			return std::string{ "S2" };
		}
		else {
			return std::string{ "Ds" };
		}
	}

	std::string Emiya::Attack(int j)
	{
		if (GetEnergy() >= 2 && j == 1) {
			SubEnergy(2);
			return std::string{ "S1" };
		}
		else {
			return std::string{ "At" };
		}
	}

	std::string Emiya::Defense()
	{
		srand((unsigned int)time(NULL));
		int i = rand() % 10;
		if (i < _prob_use_defense_skill) {
			return std::string{ "S2" };
		}
		else {
			return std::string{ "Ds" };
		}
	}

	std::string Gilgamesh::Attack(int j)
	{
		if (GetEnergy() >= 6 && j == 1)
		{
			SubEnergy(6);
			return std::string{ "S1" };
		}
		else if (GetEnergy() >= 5 && j == 2)
		{
			SubEnergy(5);
			return std::string{ "S2" };
		}
		else {
			return std::string{ "At" };
		}
	}

	std::string EastMoon::Attack(int j)
	{

		if (GetEnergy() >= 3 && j == 1)
		{
			SubEnergy(3);
			return std::string{ "S1" };
		}
		else {
			return std::string{ "At" };
		}
	}

	std::string EastMoon::Defense()
	{
		srand((unsigned int)time(NULL));
		int i = rand() % 10;
		if (i < _prob_use_defense_skill)
		{
			return std::string{ "S2" };
		}
		else {
			return std::string{ "Ds" };
		}
	}

	std::string HongHong::Attack(int j)
	{
		if (GetEnergy() >= 4 && j == 1)
		{
			SubEnergy(4);
			return std::string{ "S1" };
		}
		else {
			return std::string{ "|At" };
		}
	}

	std::string HongHong::Defense()
	{
		srand((unsigned int)time(NULL));
		int i = rand() % 10;
		if (i < _prob_use_defense_skill) {
			return std::string{ "S2" };
		}
		else {
			return std::string{ "Ds" };
		}
	}
}