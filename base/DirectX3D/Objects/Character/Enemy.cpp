#include "Framework.h"

Enemy::Enemy(string name, UINT index, ModelAnimatorInstancing* modelAnimatorInstancing, Transform* transform, Vector3 spawnPos, float trackRange)
	: Character(transform, name, spawnPos), index(index), instancing(modelAnimatorInstancing), trackRange(trackRange)
{
	trackCollider = new SphereCollider();
	trackCollider->Scale() *= trackRange;
	trackCollider->SetParent(transform);

	attackRange = trackRange * 0.35f;
	attackCollider = new SphereCollider();
	attackCollider->Scale() *= attackRange;
	attackCollider->Pos().SetY(attackRange * 0.5f);
	attackCollider->SetParent(transform);

	//instancing->GetClip(ATTACK)->SetEvent(bind(&Enemy::Attack, this), 0.0f);
}

Enemy::~Enemy()
{
	delete trackCollider;
	delete attackCollider;
	for (Transform* colliderTransform : colliderTransforms)
		delete colliderTransform;
	for (CapsuleCollider* collider : colliders)
		delete collider;
}

void Enemy::Update()
{
	Character::Update();
	trackCollider->UpdateWorld();
	attackCollider->UpdateWorld();
	for (Transform* t : colliderTransforms)
		t->UpdateWorld();

	//for (CapsuleCollider* collider : colliders)
	//{
	//	if (playerData->GetCollier()->IsCollision(collider))
	//	{
	//		playerData->SetAction(Player::HIT_MEDIUM);
	//		playerData->SetIsHit(true);
	//	}
	//	else
	//	{
	//		playerData->SetIsHit(false);
	//	}
	//}
	Track();

	for (CapsuleCollider* collider : colliders)
		collider->UpdateWorld();
}

void Enemy::Render()
{
	Character::Render();
	//trackCollider->Render();
	//attackCollider->Render();
	//for (CapsuleCollider* collider : colliders)
		//collider->Render();
}

void Enemy::GUIRender()
{
	Character::GUIRender();
}

void Enemy::Track()
{
}

void Enemy::SetCollidersParent()
{
	FOR(colliders.size())
	{
		colliderTransforms.push_back(new Transform());
		colliderTransforms[i]->SetParent(transform);
		colliders[i]->SetParent(colliderTransforms[i]);
	}
}
