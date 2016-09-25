#include "stdafx.h"
#include "DerivedStates\PlayState.hpp"

using namespace sf;

PlayState::PlayState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture)
	: State("Play", ID, RWindow, RTexture)
{
	AssetMgr_ = AssetManager::GetInstance();
}

PlayState::~PlayState()
{
	delete BG_;
	BG_ = nullptr;

	delete ProjectileMgr_;
	ProjectileMgr_ = nullptr;

	delete Player_;
	Player_ = nullptr;

	for (auto& B : GameObjects_)
	{
		delete B;
		B = nullptr;
	}

}

bool PlayState::Initialise()
{
	InitRand();

	auto AM = AssetManager::GetInstance();

	World_ = new World(GameObjects_, GetRenderTexture());

	ProjectileMgr_ = new ProjectileManager(World_);

	for (Int32 i = 0; i < PROJECTILE_PER_TYPE * PROJECTILE_TYPE_COUNT; ++i)
	{
		GameObjects_.push_back(new Projectile);
		ProjectileMgr_->AddProjectile(dynamic_cast<Projectile*> (GameObjects_.back()));
	}

	ProjectileMgr_->SetupProjectiles();

	Player_ = new PlayerController(ProjectileMgr_, World_, Vector2f(GetRenderTexture()->getSize()));

#if IN_DEVELOPMENT_BUILD
	assert(ProjectileMgr_);
	assert(World_);
	assert(Player_);
#endif 

	//Implement other player boats after another when ready
	GameObjects_.push_back(new Boat(None, Raft));

	Player_->AddPlayerBoats(dynamic_cast<Boat*>(GameObjects_.back()));

	//AI boat manager
	AIBoatMgr_ = new AIBoatManager(ProjectileMgr_);
	//todo add boats then call initialise
	for (Int32 i = 0; i < AI_BOAT_COUNT; ++i)
	{
		GameObjects_.push_back(new Boat());
		AIBoatMgr_->AddBoat(dynamic_cast<Boat*>(GameObjects_.back()));
	}

	AIBoatMgr_->InitialiseAI(GetRenderTexture()->getView());

	//Instructions text
	sf::Font* f = AM->GetDefaultFont();
	if (f == nullptr)
	{
		return false;
	}

	//Initialise game texts 
	Instructions_.setFont(*f);
	Instructions_.setString("Tap X To Begin");
	Instructions_.setCharacterSize(48u);

	RespawnText_.setFont(*f);
	RespawnText_.setCharacterSize(36u);

	LivesText_.setFont(*f);
	LivesText_.setCharacterSize(28u);
	LivesText_.setPosition(10.f, LivesText_.getGlobalBounds().height * 2.5f);
	LivesText_.setString(L"Lives: " + std::to_wstring(PlayerData_.Lives));

	sf::Vector2f Centre = sf::Vector2f(GetRenderWindow()->getSize())*0.5f;
	Centre -= sf::Vector2f(Instructions_.getGlobalBounds().width, Instructions_.getGlobalBounds().height) * 0.5f;
	Instructions_.setPosition(Centre);

	FinishedSplash_ = sf::Sprite(*AssetMgr_->LoadTexture("res//textures//win_screen.png"));

	//Game background init 
	sf::Texture* T = AM->LoadTexture("res//textures//water.png");
	if (T != nullptr)
	{
#ifndef PLAYABLE_BUILD
		DebugPrintF(AssetLog, L"Failed to load texture at %s", L"res//textures//water.png");
#endif
		return (false);
	}

	T->setRepeated(true);

	Int32 ScreenScale = (Int32)GetRenderWindow()->getSize().y * 100u;
	Int32 BGHeight = (GetRenderWindow()->getSize().y * 100u) / 128u;

	BG_ = new TiledBackground(128u, Vector2u(GetRenderWindow()->getSize().x / 128u, BGHeight));
	BG_->SetTexture(T);
	BG_->setPosition(0, (float)GetRenderWindow()->getSize().y - ScreenScale);

	SpawnObstacles();

	View V = GetRenderTexture()->getView();
	//V.zoom(10.f);
	GetRenderTexture()->setView(V);

	return false;
}

void PlayState::Update(float Delta)
{
	if (!GameStarted_ || Respawning_)
	{
		//Early out if the game isn't started
		return;
	}

	LivesText_.setString(L"Lives: " + std::to_wstring(PlayerData_.Lives) + L"\nKills: " + std::to_wstring(PlayerData_.KillCount));

	PlayerMoveDirection Direc;

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		Direc = UP_LEFT;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		Direc = UP_RIGHT;
	}
	else
	{
		Direc = UP;
	}

	Player_->SetDirection(Direc);
	//todo: collision detection with sides of map to player controller

	View V(GetRenderTexture()->getView());
	V.move(0.f, Player_->GetMoveVector().y* Delta);
	GetRenderTexture()->setView(V);

	Player_->UpdatePlayer(Delta);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		Player_->Fire();
	}

	AIBoatMgr_->Update(Delta, GetRenderTexture()->getView());

	ProjectileMgr_->ProjectileUpdate(Delta);

	//Todo: add in player health mechanic 
}

void PlayState::Render() const
{
	if (!ShowFinished_)
	{
		GetRenderTexture()->draw(*BG_);
		for (const auto& R : Obstacles_)
		{
			GetRenderTexture()->draw(R);
		}

		for (const auto& GO : GameObjects_)
		{
			if (GO->IsActive() == true)
			{
				GetRenderTexture()->draw(*GO);
			}
		}

		if (GameStarted_ == false)
		{
			GetRenderTexture()->draw(Instructions_);
		}

		if (Respawning_ == true)
		{
			GetRenderTexture()->draw(RespawnText_);
		}
	}

}

void PlayState::PostRender() const
{
	if (ShowFinished_ == false)
	{
		GetRenderWindow()->draw(LivesText_);
	}
	else
	{
		GetRenderWindow()->draw(FinishedSplash_);
	}
}

void PlayState::HandleEvents(sf::Event& Evnt, float Delta)
{
	if (Evnt.type == sf::Event::KeyPressed)
	{
		if (Evnt.key.code == Keyboard::X && !GameStarted_)
		{
			GameStarted_ = true;
		}

		if (Evnt.key.code == Keyboard::Space && Respawning_)
		{
			Respawning_ = false;
		}
	}
}

void PlayState::SpawnObstacles()
{
	//Spawn obstacles
	Obstacles_.resize(ObstacleCount_);
	Vector2f Pos;
	const float MinSpawnDist(powf(100.f, 2));

	float MinSpawnY = BG_->getPosition().y / 2.f;
	float MaxSpawnY = 0.f;

	for (Int32 i = 0; i < (signed)Obstacles_.size(); ++i)
	{
		if ((i + 1) > (signed) Obstacles_.size() / 2)
		{
			MaxSpawnY = BG_->getPosition().y / 2.f;
			MinSpawnY = BG_->getPosition().y;
		}

		auto& R = Obstacles_[i];

		Texture* T = AssetMgr_->LoadTexture("res//textures//rock.png");
		if (T != nullptr)
		{
			R.setTexture(T);
		}

		R.setSize(Vector2f(32, 32));
		Pos.x = RandFloat(0, (float)GetRenderWindow()->getSize().x);
		Pos.y = RandFloat(fabs(MaxSpawnY), fabs(MinSpawnY))*-1;
		R.setPosition(Pos);

		for (int j(0); i < 4; ++i)
		{
			for (RectangleShape& R2 : Obstacles_)
			{
				if (&R == &R2)
				{
					continue;
				}

				if (GetSquareLength(R2.getPosition() - R.getPosition()) < MinSpawnDist)
				{
					Pos.x = RandFloat(0, (float)GetRenderWindow()->getSize().x);
					Pos.y = RandFloat(fabs(MaxSpawnY), fabs(MinSpawnY))*-1;
					R.setPosition(Pos);
				}
			}
		}
	}
}
