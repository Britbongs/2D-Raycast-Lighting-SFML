#include "stdafx.hpp"
#include <future>
#include "DerivedStates\PlayState.hpp"
using namespace sf;

PlayState::PlayState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture)
	: State("Play", ID, RWindow, RTexture)
{
	AssetMgr_ = AssetManager::GetInstance();
}

PlayState::~PlayState()
{
		
}

bool PlayState::Initialise()
{
	InitRand();

	auto AM = AssetManager::GetInstance();

	//TODO remove test circle 
	Circle_ = CircleShape(10);
	Circle_.setFillColor(Color::Red);

	//---

	World_ = new World(GameObjects_, GetRenderTexture());
	/* 
	if (!Loader_.LoadMap("res//movement_test_map.tmx"))
	{
		return false;
	}
	*/
	if (!Loader_.LoadMap("res//test_map.tmx"))
	{
		return false;
	}

	TiledMap_ = new TiledMap(64);

	Texture *T = AM->LoadTexture("res//textures//tilesheet.png");
	if (T != nullptr)
	{
		TiledMap_->SetTexture(T);
	}
	else
	{
		return false;
	}

	TiledMap_->SetupVetices(Loader_);

	World_->SetupTileMeshColliders(TiledMap_);
	GameObjects_.push_back(new GameObject());
	GameObjects_.back()->SetSize(Vector2f(64.f, 64.f));
	Player_ = new PlayerController(World_);
	Player_->SetGameObject(GameObjects_[0]);
	Player_->Initialise();
	
	World_->SetupTileMeshColliders(TiledMap_);
	
	LightMap_ = new LightMap(GetRenderTexture()->getSize(), World_);

	SceneRenderer_.create(STATIC_CAST(Uint32, TiledMap_->GetSize().x), STATIC_CAST(Uint32, TiledMap_->GetSize().y));
	
	AmbientShader_ = AM->LoadShader("res//shader//defaultVertShader.vert", "res//shader//ambientFragShader.frag");

	return true;
}

void PlayState::Deinitialise()
{
	Player_->Deinitialise();
	if (ProjectileMgr_)
	{
		delete ProjectileMgr_;
		ProjectileMgr_ = nullptr;
	}

	if (TiledMap_)
	{
		delete TiledMap_;
		TiledMap_ = nullptr;
	}

	if (Player_)
	{
		delete Player_;
		Player_ = nullptr;
	}

	for (Int32 i = 0; i < STATIC_CAST(Int32, GameObjects_.size()); ++i)
	{
		if (GameObjects_[i] != nullptr)
		{
			delete GameObjects_[i];
			GameObjects_[i] = nullptr;
		}
	}
	
	if (World_)
	{
		delete World_;
		World_ = nullptr;
	}

	if (LightMap_)
	{
		delete LightMap_;
		LightMap_ = nullptr;
	}
}

void PlayState::Update(float Delta)
{
	for (const auto& GO : GameObjects_)
	{
		GO->UpdateCollider();
	}
	Player_->Update(Delta);

	sf::Vector2i MousePos = Mouse::getPosition(*GetRenderWindow());
	sf::Vector2f WorldMousePos = GetRenderTexture()->mapPixelToCoords(MousePos);
	Circle_.setPosition(WorldMousePos - Vector2f(Circle_.getRadius(), Circle_.getRadius()));
}

void PlayState::Render()
{

	Sprite Empty;
	RenderStates RStates;
	RStates.shader = AmbientShader_;
	RStates.blendMode = BlendAlpha;

	SceneRenderer_.clear(Color::Black);
	SceneRenderer_.draw(*TiledMap_);
	SceneRenderer_.draw(Circle_);
	SceneRenderer_.display();

	Sprite S(SceneRenderer_.getTexture());
	LightMap_->Draw();
	if (AmbientShader_)
	{
		AmbientShader_->setUniform("ambientColour", Glsl::Vec4(0.27f, 0.15f, 0.3f, 0.6f));
		AmbientShader_->setUniform("lightMapTexture", LightMap_->GetTexture());
		AmbientShader_->setUniform("resolution", Glsl::Vec2(GetRenderTexture()->getSize()));
	}
	GetRenderTexture()->draw(S, RStates);

	for (const auto& GO : GameObjects_)
	{
		if (GO->IsActive() == true)
		{
			GetRenderTexture()->draw(*GO);
		}
	}
}

void PlayState::PostRender() const
{
	RenderStates Blend;
}

void PlayState::HandleEvents(sf::Event& Evnt, float Delta)
{
	if (Evnt.type == sf::Event::KeyPressed)
	{
		if (Evnt.key.code == Keyboard::X)
		{
			Image Img = LightMap_->GetTexture().copyToImage();
			Img.saveToFile("Rendered_Light_Map.png");
		}
	}

	if (Evnt.type == Event::MouseButtonPressed)
	{
		if (Evnt.mouseButton.button == Mouse::Left)
		{
			sf::Vector2i MousePos = Mouse::getPosition(*GetRenderWindow());
			sf::Vector2f WorldMousePos = GetRenderTexture()->mapPixelToCoords(MousePos);

#pragma region VisibilityAsync
			sf::Clock C;
			C.restart();
			
			LightMap_->AddLight(WorldMousePos);

			Time T = C.getElapsedTime();
			float Seconds = T.asSeconds();
			Int32 Milliseconds = T.asMilliseconds();
			Int64 Microseconds = T.asMicroseconds();
			DebugPrintF(DebugLog, L"Time taken to execute: %f Seconds \n%i Milliseconds \n%li Microseconds", Seconds, Milliseconds, Microseconds);

#pragma endregion VisibilityAsync
		}

	}
}