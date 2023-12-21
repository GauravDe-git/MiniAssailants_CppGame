#pragma once

#include "Player.hpp"
#include "Enemy.hpp"

namespace Combat
{
	//player attack enemy
	inline void attack(const Player& player, Enemy& enemy, Player::AttackType atkType)
	{
		const int damage = player.getAtkDmg(atkType);
		enemy.reduceHP(damage);
		enemy.setState(Enemy::State::Hurt);
		if (enemy.getHp() <= 0)
		{
			enemy.setHp(0);
			enemy.setState(Enemy::State::Dead);
		}
	}

	//enemy attack player
	inline void attack(const Enemy& enemy, Player& player)
	{
		if (!player.isHurt() && !player.isAttacking())
		{
			const int damage = enemy.getAtkDmg();
			//player.reduceHP(damage);
			player.setHP(player.getHP() - damage);
			player.setState(Player::State::Hurt);
		}
	}
};