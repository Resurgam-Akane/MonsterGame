#include "LocalVersionDriver.h"
#include "LocalVersionFomula.h"

namespace Driver
{
	bool Round(Monster::Monster *active, Monster::Monster *passive)
	{
		std::cout << active->GetName() << ":" << active->GetEnergy() << std::endl;
		std::cout << passive->GetName() << ":" << passive->GetEnergy() << std::endl;
		std::string as = active->Attack();
		std::string ps = passive->Defense();
		double base_attack =  BASE_ATTACK_VALUE(active->GetCalStrength(), passive->GetCalDefense());
		active->AddEnergy();
		passive->AddEnergy();

		if (as[as.length() - 1] != 't' || ps[ps.length() - 1] != 's') {
			if (as[as.length() - 1] != 't') as = (active->GetSkill())[as[as.length() - 1] - '1'];
			if (ps[ps.length() - 1] != 's') ps = (passive->GetSkill())[ps[ps.length() - 1] - '1'];
			if (as == "BiLiBiLi") base_attack *= 1.5;
			else if (as == "Excalibur") base_attack *= 1.4;
			else if (as == "Caladbolg") base_attack *= 1.1;
			else if (as == "Enuma Elish") base_attack *= 3.0;
			else if (as == "Gate_of_Babylon") base_attack *= 1.5;
			else if (as == "Pure Yang fire") base_attack *= 1.2;
			else if (as == "Fox read") base_attack *= 1.5;

			
			if (ps == "Sudden Dodge") base_attack = 0;
			else if (ps == "Push Gas Through Body") base_attack = 0 - 0.66 * base_attack; // recover 2/3 blood
			else if (ps == "Code Geasse") base_attack = 0.8 * base_attack;
			else if (ps == "Invisible_Air") base_attack = 0.8 * base_attack;
			else if (ps == "Rho_Aias") base_attack = 0.5 * base_attack;
			else if (ps == "The art of fox") base_attack = passive->GetCurBlood() - passive->GetCalBlood();
		}
		
		std::cout << active->GetName() << " use " << as << std::endl;
		std::cout << passive->GetName() << " use " << ps << std::endl;

		if (passive->SubBlood((int)base_attack) == 0) {
			std::cout << active->GetName() << " current blood:" << active->GetCurBlood() << std::endl;
			std::cout << passive->GetName() << " current blood: " << passive->GetCurBlood() << std::endl << std::endl;
			return true; // passive death
		}
		else {
			std::cout << active->GetName() << " current blood:" << active->GetCurBlood() << std::endl;
			std::cout << passive->GetName() << " current blood: " << passive->GetCurBlood() << std::endl << std::endl;
			return false;
		}

	}
	void Fight(Monster::Monster *p1, Monster::Monster *p2)
	{
		std::timed_mutex Round_Time;
		int NumRound = 0;
		Monster::Monster* term;
		
		p1->Show();
		p2->Show();

		while (!Round(p1, p2)) {
			++NumRound;
			term = p1;
			p1 = p2;
			p2 = term;
		}
		std::cout << p2->GetName() << " is dead." << std::endl;
	}
}