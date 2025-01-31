#include "Framework.h"
#include "IronArrow.h"

IronArrow::IronArrow(string name, int type, int weight,
	int value, int arrow_type, int atk)
	: Arrow(name, type, weight, value, arrow_type, atk)
{
	SetTag("IronArrow");

	Pos().x += 0.0f;
	Pos().y += 41.0f;
	Pos().z += 0.0f;

	Rot().x += 0;
	Rot().y += 0;
	Rot().z += XM_PI;

	Scale().x *= 1.0f;
	Scale().y *= 1.0f;
	Scale().z *= 1.0f;

	collider = new BoxCollider();
	collider->Pos().x += 1.0f;
	collider->Pos().y += 25.0;
	collider->Pos().z += 2.0f;

	collider->Rot().x += 0.0f;
	collider->Rot().y += 0.0f;
	collider->Rot().z += 0.0f;

	collider->Scale().x *= 4.0f;
	collider->Scale().y *= 65.0f;
	collider->Scale().z *= 5.0f;


	collider->SetTag("IronArrowCollider");
	collider->SetParent(this);
	collider->Load();
}

IronArrow::~IronArrow()
{
	delete collider;
}

void IronArrow::Update()
{
	UpdateWorld();
	collider->UpdateWorld();
}

void IronArrow::Render()
{
	Model::Render();
	collider->Render();
}

void IronArrow::GUIRender()
{
	Model::GUIRender();
	collider->GUIRender();
}

void IronArrow::ColliderManager(bool isBowCollider)
{
	if (isBowCollider)
		collider->SetActive(true);

	else
		collider->SetActive(false);
}

void IronArrow::Shoot(Vector3 pos, Vector3 dir)
{
	transform->SetActive(true);

	transform->Pos() = pos;
	direction = dir;

	transform->Rot().y = atan2(dir.x, dir.z);

	time = 0;
}
