#include "Framework.h"
#include "TerrainManager.h"

TerrainManager::TerrainManager()
{
	wstring heightMap = L"Textures/HeightMaps/Tamriel.jpg";
	//wstring heightMap = L"Textures/HeightMaps/TestMap.png";
	//wstring heightMap = L"Textures/Color/Black.png";
	terrain = new Terrain(L"Textures/Landscape/tamriel.4.0.0.dds", heightMap);

	//float size = 2.0f;
	//terrain->Scale() *= size;
	//terrain->Pos().SetX(-terrain->GetSize().x * size * 0.5f);
	//terrain->Pos().SetZ(-terrain->GetSize().y * size * 0.5f);
	//terrain->Pos().y -= 100.0f;
	terrain->UpdateWorld();
}

TerrainManager::~TerrainManager()
{
	delete terrain;
}

void TerrainManager::Update()
{
	playerData->Pos().y += -gravity * DELTA;
	terrain->ComputePicking(playerData->Pos(), playerData);
	if (playerData->Pos().y <= -0.5f)
		playerData->Pos().y = 0.5f;

	//if (playerData->Pos().y <= 0.0f)
	//	playerData->Pos().y = 200.0f;
}

void TerrainManager::Render()
{
	terrain->Render();
}

void TerrainManager::GUIRender()
{
	ImGui::Text("Test : %d", test);
}
