#include "Framework.h"

#include "Scenes/GridScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ParticleScene.h"
#include "Scenes/ParticleConfigScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/MonsterScene.h"
#include "Scenes/PlayerScene.h"


GameManager::GameManager()
{
    Create();

    SceneManager::Get()->Create("Grid", new GridScene());
    //SceneManager::Get()->Create("ModelExport", new ModelExportScene());
    //SceneManager::Get()->Create("ParticleConfig", new ParticleConfigScene());
    //SceneManager::Get()->Create("Particle", new ParticleScene());
    SceneManager::Get()->Create("Terrain", new TerrainScene());
    SceneManager::Get()->Create("Monster", new MonsterScene());
    SceneManager::Get()->Create("Player", new PlayerScene());

    SceneManager::Get()->Add("Grid");
    //SceneManager::Get()->Add("ModelExport");     
    //SceneManager::Get()->Add("ParticleConfig");
    SceneManager::Get()->Add("Terrain");
    SceneManager::Get()->Add("Monster");
    SceneManager::Get()->Add("Player");

    Font::Get()->AddColor("Gray", 185.0f / 255.0f, 185.0f / 255.0f, 185.0f / 255.0f);
    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Futura", L"Futura", 20.0f);
    Font::Get()->AddStyle("Futura_big", L"Futura", 30.0f);
    Font::Get()->AddStyle("Futura_more_big", L"Futura", 40.0f);
    Font::Get()->AddStyle("Futura_outtro", L"Futura", 200.0f);

    Font::Get()->SetColor("Gray");
    Font::Get()->SetStyle("Futura");
}

GameManager::~GameManager()
{
    Delete();
}

void GameManager::Update()
{
    Keyboard::Get()->Update();
    Timer::Get()->Update();    

    SceneManager::Get()->Update();

    Environment::Get()->Update();
}

void GameManager::Render()
{
    SceneManager::Get()->PreRender();
    
    Device::Get()->Clear();
    Font::Get()->GetDC()->BeginDraw();
    
    Environment::Get()->Set();    
    SceneManager::Get()->Render();
    
    Environment::Get()->PostSet();
    SceneManager::Get()->PostRender();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    
    
    static bool isActive = true;

    if (isActive)
    {
        ImGui::Begin("Inspector", &isActive);
        Environment::Get()->GUIRender();
        SceneManager::Get()->GUIRender();
        ImGui::End();
    }    

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    Font::Get()->GetDC()->EndDraw();

    Device::Get()->Present();
}

void GameManager::Create()
{
    Keyboard::Get();
    Timer::Get();
    Device::Get();
    Environment::Get();
    Observer::Get();
    
    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"배달의민족 주아");
    
    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("Default");

    Texture::Add(L"Textures/Color/White.png");
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
    Keyboard::Delete();
    Timer::Delete();
    Device::Delete();
    Shader::Delete();
    Texture::Delete();
    Environment::Delete();    
    Observer::Delete();
    Font::Delete();
    
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    
    ImGui::DestroyContext();
}
