#pragma once
#include "Monster.h"
#include "Player.h"
#include <chrono>
#include <mutex>
#include <iostream>
#include <memory>

namespace Driver
{

	void Fight(Monster::Monster *p1, Monster::Monster *p2);

}