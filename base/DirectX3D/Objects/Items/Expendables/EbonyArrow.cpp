#include "Framework.h"
#include "EbonyArrow.h"

EbonyArrow::EbonyArrow(string name, int type, int weight,
	int value, int arrow_type, int atk)
	: Arrow(name, type, weight, value, arrow_type, atk)
{
	SetTag("EbonyArrow");

	Rot().x += 0;
	Rot().y += 0;
	Rot().z += XM_PI;


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


	collider->SetTag("EbonyArrowCollider");
	collider->SetParent(this);
	collider->Load();
}

EbonyArrow::~EbonyArrow()
{
	delete collider;
}

void EbonyArrow::Update()
{
	time += DELTA;

	if (time > LIFE_SPAN)
		transform->SetActive(false);

	transform->Pos() += direction * speed * DELTA;

	UpdateWorld();
	collider->UpdateWorld();
}

void EbonyArrow::Render()
{
	Model::Render();
	collider->Render();
}

void EbonyArrow::GUIRender()
{
	Model::GUIRender();
	collider->GUIRender();
}

void EbonyArrow::ColliderManager(bool isBowCollider)
{
	if (isBowCollider)
		collider->SetActive(true);

	else
		collider->SetActive(false);
}

void EbonyArrow::Shoot(Vector3 pos, Vector3 dir)
{
	transform->SetActive(true);

	transform->Pos() = pos;
	direction = dir;

	transform->Rot().y = atan2(dir.x, dir.z);
	   
	time = 0;
}
