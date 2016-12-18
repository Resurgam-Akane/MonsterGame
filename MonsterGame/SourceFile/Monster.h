#pragma once
#include <string>
#include <vector>
#define TOTAL_MONSTER 9

namespace Monster {
	enum class Phyle
	{
		Strength,	//P1
		Blood,		//P2
		Defense,	//P3
		Agile,		//P4
	};

	class Monster {
	public:
		const std::vector<std::string> SkillTable{ "BiLiBiLi", "Push Gas Through Body", "Sudden Dodge", "Code Geasse", "Excalibur", "Invisible_Air", "Rho_Aias", "Caladbolg", "Enuma Elish", "Gate_of_Babylon", "The art of fox", "Pure Yang fire", "Fox read" };

		Monster(Phyle phyle, std::string name, int, int, int, int, int);
		Monster(Phyle phyle, std::string name, int, int, int, int, int, int, int, int);

		virtual std::string Attack(int) { return std::string{ "At" }; }
		virtual std::string Defense() { return std::string{ "Ds" }; }

		void Levelup(int exp);
		//add strength, add blood, add defense, add agile
		void AttrPointDistribution();
		void Show();
		void AddEnergy();
		void SubEnergy(int);
		void SetEnergy(int j = 0) { _energy = j; }
		void SetBlood(int i) { _current_blood = i; }

		Phyle GetPhyle() { return _phyle; };
		std::string GetName() { return _name; }
		int GetStrength() { return _strength; }
		int GetCalStrength() { return _cal_strength; }
		int GetBlood() { return _blood; }
		int GetCalBlood() { return _cal_blood; }
		int GetDefense() { return _defense; }
		int GetCalDefense() { return _cal_defense; }
		int GetAgile() { return _agile; }
		int GetRank() { return _rank; }
		int GetExp() { return _exp; }
		int SubBlood(int a);
		int GetCurBlood() { return _current_blood; }
		int GetPoint() { return _attribute_point; }
		int GetMonsterID() { return _MonsterID; }
		int GetEnergy() { return _energy; }
		std::string GetMonsterInfo();
		
		std::vector<std::string> GetSkill() { return _skill; }
	protected:
		void SetSkill(std::string, std::string);
	private:
		int _MonsterID;
		Phyle _phyle;		//P[parameter]
		int _strength;		//S[value]
		int _blood;			//B[value]
		int _defense;		//D[value]
		int _agile;			//A[value]

		int _cal_strength;
		int _cal_blood;
		int _cal_defense;
		int _cal_agile;

		int _current_blood;

		std::string _name;
		int _rank;			//R[value]
		int _exp;		//E[value]
		int _energy;
		int _attribute_point;	//T[value]
		std::vector<std::string> _skill;
	};

	// http://baike.baidu.com/view/1902239.htm
	// strength  skill:1.5 attack
	class Misaka_Mikoto : public Monster
	{
	public:
		Misaka_Mikoto(int s=7, int b=2, int d=5, int a=1, int id = 1) : Monster(Phyle::Strength, "Misaka Mikoto", s, b, d, a, id) 
		{ 
			SetSkill("BiLiBiLi", ""); 
		}
		Misaka_Mikoto(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Strength, "Misaka Mikoto", s, b, d, a, 1, r, e, t) 
		{ 
			SetSkill("BiLiBiLi", ""); 
		}
		std::string Attack(int) override;
	private:
	};

	// http://baike.baidu.com/view/3055367.htm
	// blood skill: Push gas add blood
	class Saten_Ruiko : public Monster
	{
	public:
		Saten_Ruiko(int s=5, int b=5, int d=5, int a=5, int id = 2) : Monster(Phyle::Blood, "Saten Ruiko", s, b, d, a, id) 
		{ 
			SetSkill("Push Gas Through Body", "");
		}
		Saten_Ruiko(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Blood, "Saten Ruiko", s, b, d, a, 2, r, e, t)
		{
			SetSkill("Push Gas Through Body", "");
		}
		virtual std::string Defense();
	private:
		int _prob_use_defense_skill = 4;
	};

	// http://baike.baidu.com/item/%E7%99%BD%E4%BA%95%E9%BB%91%E5%AD%90/1061890
	// agile  skill:sudden dodge
	class Shirai_Kuroko : public Monster
	{
	public:
		Shirai_Kuroko(int s = 5, int b = 5, int d = 5, int a = 5, int id = 3) : Monster(Phyle::Agile, "Shirai Kuroko", s, b, d, a, id) 
		{ 
			SetSkill("Sudden Dodge", ""); 

		}
		Shirai_Kuroko(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Agile, "Shirai Kuroko", s, b, d, a, 3, r, e, t)
		{
			SetSkill("Sudden Dodge", "");

		}
		virtual std::string Defense();

	private:
		int _prob_use_defense_skill = 4;

	};

	// http://baike.baidu.com/view/3056510.htm#4
	// defense skill: Code Geasse 0.8 be attacked
	class Uiharu_Kazari : public Monster
	{
	public:
		Uiharu_Kazari(int s = 5, int b = 5, int d = 5, int a = 5, int id = 4) : Monster(Phyle::Defense, "Uiharu Kazari", s, b, d, a, id) 
		{ 
			SetSkill("Code Geasse", ""); 

		}
		Uiharu_Kazari(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Defense, "Uiharu Kazari", s, b, d, a, 4, r, e, t)
		{
			SetSkill("Code Geasse", "");

		}
		virtual std::string Defense();

	private:
		int _prob_use_defense_skill = 4;

	};

	// http://baike.baidu.com/item/%E9%98%BF%E5%B0%94%E6%89%98%E5%88%A9%E4%BA%9A%C2%B7%E6%BD%98%E5%BE%B7%E6%8B%89%E8%B4%A1/10500553?title=jump&oldlemmaid=8420590&fromtitle=%E9%98%BF%E5%B0%94%E6%89%98%E8%8E%89%E9%9B%85%C2%B7%E6%BD%98%E5%BE%B7%E6%8B%89%E8%B4%A1&fromid=10430732
	// strength skill: Excalibur 1.4 attack, Invisible_Air 0.8 be attacked
	class Saber : public Monster
	{
	public:
		Saber(int s = 5, int b = 5, int d = 5, int a = 5, int id = 5) : Monster(Phyle::Strength, "Saber", s, b, d, a, id)
		{ 
			SetSkill("Excalibur", "Invisible_Air"); 

		}
		Saber(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Strength, "Saber", s, b, d, a, 5, r, e, t)
		{
			SetSkill("Excalibur", "Invisible_Air");

		}
		virtual std::string Attack(int);
		virtual std::string Defense();
	private:
		int _prob_use_defense_skill = 4;

	};

	// http://baike.baidu.com/item/%E8%8B%B1%E7%81%B5%E5%8D%AB%E5%AE%AB/20123130#2_6
	// defense skill:Rho_Aias 0.5 be attacked, Caladbolg 1.1 attack
	class Emiya : public Monster
	{
	public:
		Emiya(int s = 5, int b = 5, int d = 5, int a = 5, int id = 6) : Monster(Phyle::Defense, "Emiya", s, b, d, a, id)
		{ 
			SetSkill("Caladbolg", "Rho_Aias"); 

		}
		Emiya(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Defense, "Emiya", s, b, d, a, 6, r, e, t)
		{
			SetSkill("Caladbolg", "Rho_Aias");

		}
		virtual std::string Attack(int);
		virtual std::string Defense();
	private:
		int _prob_use_defense_skill = 4;

	};

	// http://baike.baidu.com/subview/44324/7381356.htm
	// strength skill: Enuma Elish 3.0 attacked, Gate_of_Babylon 1.5 attack
	// the strongest strength monster!!!
	class Gilgamesh : public Monster
	{
	public:
		Gilgamesh(int s = 5, int b = 5, int d = 5, int a = 5, int id = 7) : Monster(Phyle::Strength, "Gilgamesh", s, b, d, a, id)
		{ 
			SetSkill("Enuma Elish", "Gate_of_Babylon"); 

		}
		Gilgamesh(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Strength, "Gilgamesh", s, b, d, a, 7, r, e, t)
		{
			SetSkill("Enuma Elish", "Gate_of_Babylon");

		}
		virtual std::string Attack(int);

	private:

	};


	// http://baike.baidu.com/item/%E4%B8%9C%E6%96%B9%E6%9C%88%E5%88%9D
	// blood skill: The art of fox add all blood, Pure Yang fire 1.2 attack
	// the strongest blood monster!!!
	class EastMoon : public Monster
	{
	public:
		EastMoon(int s = 5, int b = 5, int d = 5, int a = 5, int id = 8) : Monster(Phyle::Blood, "EastMoon", s, b, d, a, id)
		{
			SetSkill("Pure Yang fire", "The art of fox");

		}
		EastMoon(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Blood, "EastMoon", s, b, d, a, 8, r, e, t)
		{
			SetSkill("Pure Yang fire", "The art of fox");

		}
		virtual std::string Attack(int);
		virtual std::string Defense();

	private:
		int _prob_use_defense_skill = 4;

	};

	//http://baike.baidu.com/link?url=KkkJsyoY0DjFKk7qBkYO5Oxklo8VqkyiIG1OUHkz6XNOHW-NHLbqlf4lWyMXKXJIHNe00B0eOqy6Cu7ClhyTKGf7zXyf9q3qm2aAU0wosYQD2u-lV2dZ6_Bn-2bnALP4#3
	// agile skill: Fox read 1.5 attack, sudden dodge 0 be attacked
	// the strongest agile monster!!!
	class HongHong : public Monster
	{
	public:
		HongHong(int s = 5, int b = 5, int d = 5, int a = 5, int id = 9) : Monster(Phyle::Agile, "HongHong", s, b, d, a, id)
		{
			SetSkill("Fox Read", "Sudden Dodge"); 

		}
		HongHong(int s, int b, int d, int a, int r, int e, int t) : Monster(Phyle::Blood, "EastMoon", s, b, d, a, 9, r, e, t)
		{
			SetSkill("Fox Read", "Sudden Dodge");

		}
		virtual std::string Attack(int);
		virtual std::string Defense();

	private:
		int _prob_use_defense_skill = 4;

	};
}