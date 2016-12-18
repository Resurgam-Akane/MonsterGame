#include "LocalVersionDriver.h"
#include <thread>

int main()
{
	Monster::Misaka_Mikoto Paojie;
	Monster::Emiya Weigong;
	Monster::Monster *p1, *p2;

	if (Paojie.GetAgile() > Weigong.GetAgile()) { p1 = &Paojie; p2 = &Weigong; }
	else { p1 = &Weigong; p2 = &Paojie; }
	Driver::Fight(p1, p2);
	return 0;
}