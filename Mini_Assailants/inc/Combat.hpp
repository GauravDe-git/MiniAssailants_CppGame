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
		if (enemy.getHp() <= 0)
		{
			enemy.setHp(0);
			enemy.setState(Enemy::State::Dead);
		}
	}

	//enemy attack player
	inline void attack(Enemy& enemy, Player& player)
	{
		if (!player.isHurt() && !player.isAttacking())
		{
			int damage = enemy.getAtkDmg();
			player.reduceHP(damage);
			player.setState(Player::State::Hurt);
		}
	}
};