#pragma once

#include "Player.hpp"
#include "Enemy.hpp"

class Combat
{
public:

	//player attack enemy
	static void attack(Player& player, Enemy& enemy, Player::AttackType atkType)
	{
		int damage = player.getAtkDmg(atkType);
		enemy.reduceHP(damage);
		enemy.setState(Enemy::State::Hurt);
	}

	//enemy attack player
	static void attack(Enemy& enemy, Player& player)
	{
		int damage = enemy.getAtkDmg();
		player.reduceHP(damage);
		// Change player state to hurt (todo fn. in player)
	}
};