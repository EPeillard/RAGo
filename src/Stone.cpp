/**
  * Implementation of the functions of the class Stone
  *
  * @author Nicolas David and Sylvain Palominos
  *
  **/

#include "Stone.hpp"

#include "define.hpp"

using namespace rago;

Stone::Stone()
{
    this->x = 0;
    this->y = 0;
    this->value = PLAYER_NONE;
}

Stone::~Stone()
{
}

void Stone::setDraw(int x, int y, int value)
{
    this->x = x;
    this->y = y;
    this->value = value;
}

void Stone::setPlayer(int i)
{
    value=i;
}


int Stone::getPlayer()
{
    return value;
}
