#pragma once

class Enemy : public Character
{
public:
	Enemy(string name, UINT index = 0, ModelAnimatorInstancing* modelAnimatorInstancing = new ModelAnimatorInstancing(""),
		Transform* transform = new Transform(), Vector3 spawnPos = { 0, 0, 0 });
	~Enemy();

	virtual void Update();
	virtual void Render();
	virtual void GUIRender();

	void SetTarget(Transform* target) { if(this->target != target) this->target = target; }
	void SetPlayerData(Player* player) { if (this->playerData != player) playerData = player; }

	SphereCollider* GetTrackCollider() { return trackCollider; }
	SphereCollider* GetAttackCollider() { return attackCollider; }
	vector<CapsuleCollider*> GetColliders() { return colliders; }

protected:
	void Init();
	void Death();

	virtual void Track();
	virtual void SetCollidersParent();
	virtual void SetColliderByNode();

protected:
	ModelAnimatorInstancing* instancing;
	ModelAnimatorInstancing::Motion* motion;

	float trackTime = 0.0f;
	float hitDelay = 0.0f;
	float trackRange = 0.0f;
	float attackRange = 0.0f;
	float gravity = 50.0f;

	vector<map<float, Event>> totalEvent;
	vector<map<float, Event>::iterator> eventIters;

	UINT index;
	UINT node;

	BoxCollider* rigidbody;
	SphereCollider* trackCollider;
	SphereCollider* attackCollider;
	vector<CapsuleCollider*> colliders;
	vector<Transform*> colliderTransforms;

	Transform* target;
	Vector3 velocity;

	Player* playerData;
};

