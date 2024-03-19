#pragma once
class alduin : public ModelAnimator
{ 
	
private:
	enum State
	{
		IDLE,
		TAKEOFF,
		HOVER,
		HOVER_HIT,
		FORWARD,
		BACKWARD,
		BLEED,
		APROACH,
		FLIGHT_FOR,
		ATTACK_F, //����, (����) ����
		ATTACK_L, //���� �ֵθ��� ����
		ATTACK_R, //���� �ֵθ��� ������
		ATTACK_B, // ���� �ֵθ���
		INHALE,
		BREATH, //inhale -> exhale_breath
		FIREBALL, //inhale -> exhale_fireball
		PAIN, // pain,pain2  ���� ���
		PAIN2,
		LANDING,
		LANDING_SOFT,
		LANDING_HARD,
		LAND,
		LAND_VERTICAL,
		DIVE,
		HIT,
		DEATH,
		TURN_L,
		TURN_R
	};
public:

	alduin();
	~alduin();

	void Update();
	void Render();
	void GUIRender();

	void SetTarget(Player* target);
	float GetHP() { return curHp; }

private:

	//void SetAnimation();
	void SetState(State state);
	void Move();

	void Inhale();
	void InhaleStart();
	void BreathAttack();

	void Attacking();
	void Attacking2();
	void beginTakeoff();
	void EndTakeoff();
	void beginAproach();
	void EndAction();
	void HitDelayEnd();

	void hit();
	void Dying();
	void Dead();

	void Patterns();
	void PatternFire();
	void PatternsAir();

	State curState = IDLE;
	CapsuleCollider* alduinCollider2;

	CapsuleCollider* HeadCollider;
	CapsuleCollider* BodyCollider;
	CapsuleCollider* TailCollider;

	CapsuleCollider* collider_F;
	CapsuleCollider* collider_R;
	CapsuleCollider* collider_L;
	CapsuleCollider* collider_B;

	CapsuleCollider* Acollider_F;
	CapsuleCollider* Acollider_R;
	CapsuleCollider* Acollider_L;
	CapsuleCollider* Acollider_B;
	BoxCollider* breathCollider;

	Transform* transform;
	Transform* FireAttackTransform;
	Player* target;
	Vector3 tempPos;
	Vector3 tempRot;
	ParticleSystem* DeathParticle;
	ParticleSystem* BreathParticle;

	UINT nodeIndex = 0;

	float moveSpeed = 15.0f;
	float rotSpeed = 0.25;
	float altitude = 0.0f;
	float deceleration = 5; //����
	float CoolingTime = 1.0f;
	float HitDelay = 1.0f;
	float fireBallDir = 0.0f;

	bool isAscending = false;
	bool isDescending = false;
	bool isAttacking = false;
	bool isFireAttack = false;


	Vector3 velocity;

	vector<map<float, Event>> totalEvent;
	vector<map<float, Event>::iterator> eventIters;

	float maxHp = 3000.0f;
	float curHp = 3000.0f;


};

