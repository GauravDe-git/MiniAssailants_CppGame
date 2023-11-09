#pragma once

#include "Player.hpp"
#include "Enemy.hpp"

namespace Combat
{
	//player attack enemy
	inline void attack(Player& player, Enemy& enemy, Player::AttackType atkType)
	{
		int damage = player.getAtkDmg(atkType);
		enemy.reduceHP(damage);
		enemy.setState(Enemy::State::Hurt);
	}

	//enemy attack player
	inline void attack(Enemy& enemy, Player& player)
	{
		int damage = enemy.getAtkDmg();
		player.reduceHP(damage);
		// Change player state to hurt (todo fn. in player)
	}
};