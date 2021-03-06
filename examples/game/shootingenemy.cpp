#include "shootingenemy.h"
#include "enemybullet.h"
#include "game.h"
#include "automaticgun.h"

DECLARE_SOBJ_INHERITANCE(ShootingEnemy, GameObject)

ShootingEnemy::ShootingEnemy()
{
    this->initFromConstants<ShootingEnemy>();
    this->addGun(new AutomaticGun<EnemyBullet>() );
    this->setAngularVelocity(1.0);
}
