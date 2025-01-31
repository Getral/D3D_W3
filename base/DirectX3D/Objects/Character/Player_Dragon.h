#pragma once

class Player_Dragon : public ModelAnimator
{
public:
	struct Status
	{
		float curHp = 100;
		float maxHp = 100;
		float curstamina = 10;
		float maxstamina = 100;
		float atk = 10;
		float def = 10;
		float speed = 1.5f;
	};

	enum ACTION
	{
		OHM_IDLE, OHM_HIT_LIGHT, OHM_HIT_MEDIUM, OHM_HIT_LARGE, OHM_HIT_LARGEST, OHM_HIT_BLOCK,
		OHM_WALK_F, OHM_WALK_FL, OHM_WALK_FR, OHM_WALK_B, OHM_WALK_BL, OHM_WALK_BR, OHM_WALK_L, OHM_WALK_R,
		OHM_RUN_F, OHM_RUN_FL, OHM_RUN_FR, OHM_RUN_B, OHM_RUN_BL, OHM_RUN_BR, OHM_RUN_L, OHM_RUN_R,
		CIDLE, CMOVE_F, CMOVE_FL, CMOVE_FR, CMOVE_B, CMOVE_BL, CMOVE_BR, CMOVE_L, CMOVE_R,
		OHM_CATK_R, OHM_CATK_L, OHM_CATK_P,
		OHM_ATK_R, OHM_ATK_L, OHM_ATK_P,
		OHM_BLOCK,
		OHM_WALK_FW_ATK, OHM_WALK_BW_ATK, OHM_WALK_L_ATK, OHM_WALK_R_ATK,
		OHM_RUN_FW_ATK, OHM_RUN_BW_ATK, OHM_RUN_L_ATK, OHM_RUN_R_ATK,

		THM_IDLE, THM_BLOCK, THM_ATK_L, THM_ATK_R, THM_ATK_P,
		THM_HIT_LIGHT, THM_HIT_MEDIUM, THM_HIT_LARGE, THM_HIT_LARGEST, THM_HIT_BLOCK,
		THM_WALK_F, THM_WALK_FL, THM_WALK_FR, THM_WALK_B, THM_WALK_BL, THM_WALK_BR, THM_WALK_L, THM_WALK_R,
		THM_RUN_F, THM_RUN_FL, THM_RUN_FR, THM_RUN_B, THM_RUN_BL, THM_RUN_BR, THM_RUN_L, THM_RUN_R,
		THM_WALK_FW_ATK, THM_WALK_BW_ATK, THM_WALK_L_ATK, THM_WALK_R_ATK,
		THM_RUN_FW_ATK, THM_RUN_BW_ATK, THM_RUN_L_ATK, THM_RUN_R_ATK,

		BOW_IDLE, BOW_DRAW_INTRO, BOW_DRAW_IDLE, BOW_RELEASE,
		BOW_WALK_F, BOW_WALK_FL, BOW_WALK_FR, BOW_WALK_B, BOW_WALK_BL, BOW_WALK_BR, BOW_WALK_L, BOW_WALK_R,
		BOW_RUN_F, BOW_RUN_FL, BOW_RUN_FR, BOW_RUN_B, BOW_RUN_BL, BOW_RUN_BR, BOW_RUN_L, BOW_RUN_R,
		BOW_DRAWN_WALK_F, BOW_DRAWN_WALK_FL, BOW_DRAWN_WALK_FR, BOW_DRAWN_WALK_B,
		BOW_DRAWN_WALK_BL, BOW_DRAWN_WALK_BR, BOW_DRAWN_WALK_L, BOW_DRAWN_WALK_R,

		OHM_EQUIP, OHM_UNEQUIP, THM_EQUIP, THM_UNEQUIP, BOW_EQUIP, BOW_UNEQUIP

		//,DEATH
	};

public:
	Player_Dragon();
	~Player_Dragon();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	Status& GetStatus() { return status; }
	CapsuleCollider* GetCollier() { return collider; }
	IronMace* GetMace() { return ironmace; }
	EbonyLongSword* GetSword() { return ebonylongsword; }
	IronArrow* GetArrow() { return ironarrow; }

	bool GetIsHit() { return isHit; }
	ACTION GetAction() { return curAction; }
	bool GetIsInvincible() { return isInvincible; }

	void SetHit() { return GetHit(); }
	void SetAction(ACTION action);
	void SetIsHit(bool value) { isHit = value; }
	void SetIsInvincible(bool value) { isInvincible = value; }
	bool& SetisCollRend() { return isCollRender; }

	bool& Getis1hm() { return is1hm; }
	bool& Getis2hm() { return is2hm; }
	bool& Getisbow() { return isbow; }

private:
	void Control();
	void Move();
	void Rotate();
	void Attack();
	void Block();
	void GetHit();

	void WeaponChange();

	void SetAnimation();

	void WeaponCollider();
	void EndAttack();
	void EndBlock();
	void EndHit();
	void EndBlockHit();

	void SetBowDrawn();
	void ShootArrow();
	void EndBowDrawn();
	void SetInvincible();
	void EndInvincible();

	void Change1hm();
	void Change2hm();
	void Changebow();
	void Set1hmIdle();
	void Set2hmIdle();
	void SetbowIdle();

	void DoNothing();

protected:
	BladeSword* bladeSword;
	Shield* shield;

	// Armor
	IronShield* ironshield;
	DragonShield* dragonshield;

	// Weapon
	//  Ebony
	EbonyBattleAxe* ebonybattleaxe;
	EbonyClaymore* ebonyclaymore;
	EbonyDagger* ebonydagger;
	EbonyLongSword* ebonylongsword;
	EbonyMace* ebonymace;
	EbonyWarAxe* ebonywaraxe;
	EbonyWarHammer* ebonywarhammer;

	EbonyBow* ebonybow;
	EbonyQuiver* ebonyquiver;

	// Iron
	IronBattleAxe* ironbattleaxe;
	IronClaymore* ironclaymore;
	IronDagger* irondagger;
	IronLongSword* ironlongsword;
	IronMace* ironmace;
	IronWarAxe* ironwaraxe;
	IronWarHammer* ironwarhammer;

	IronBow* ironbow;
	IronQuiver* ironquiver;
	IronArrow* ironarrow;



	POINT clientCenterPos = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };

	Transform* rightHand;
	Transform* leftHand;
	Transform* head;
	Transform* back;

	//Transform* body;
	//Transform* foot;
	//Transform* hands;
	CapsuleCollider* backCollider;
	CapsuleCollider* headCollider;

	Status status;

	CapsuleCollider* collider;

	ACTION action;

	ACTION curAction = OHM_IDLE;

	float rotSpeed = 1;
	float deceleration = 10;

	UINT nodeIndex1 = 1;
	UINT nodeIndex2 = 1;

	Vector3 velocity;

	Vector3 prevMousePos;

	float attackCharge = 1.0f;
	float invincibleCount = 0.0f; // 무적시간 재는 변수
	float delay = 0.5f;

	bool isInvincible = false; //무적인가?
	bool isBlock = false;
	bool isHit = false;
	bool isCombo = false;
	bool isPAttack = false;

	bool is1hm = true;
	bool is2hm = false;
	bool isbow = false;

	bool isbowdrawn = false;
	bool isCollRender = false;

	int weight;
	int value;
	int atk;
	int def;
};

