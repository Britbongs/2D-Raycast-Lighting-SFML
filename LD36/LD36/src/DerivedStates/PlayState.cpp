#include "stdafx.h"
#include "DerivedStates\PlayState.hpp"
#include "Game\Game.hpp"
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

	for (auto& B : Boats_)
	{
		delete B;
		B = nullptr;
	}

}

bool PlayState::Initialise()
{
	InitRand();

	auto AM = AssetManager::GetInstance();

	ProjectileMgr_ = new ProjectileManager(GetRenderTexture());
	ProjectileMgr_->SetupProjectiles();

	assert(ProjectileMgr_);

	Player_ = new PlayerController(ProjectileMgr_, Vector2f(GetRenderTexture()->getSize()));

	assert(Player_);

	//Implement other player boats after another when ready
	Boats_.push_back(new Boat(ProjectileMgr_, None, Raft));

	Player_->AddPlayerBoats(Boats_);

	//AI boat manager
	AIBoatMgr_ = new AIBoatManager(ProjectileMgr_);
	//todo add boats then call initialise
	for (Int32 i = 0; i < AI_BOAT_COUNT; ++i)
	{
		Boats_.push_back(new Boat());
		AIBoatMgr_->AddBoat(Boats_.back());
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
	if (T == nullptr)
	{
		PrintToDebug("Error", "Failed to load water texture");
		return false;
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

	/*
	TODO:
	- Player controls with new player controller
	- Moving the view by players move vector * delta
	*/

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

	/*for (auto B : AIBoats_)
	{
		View V(GetRenderTexture()->getView());
		float MaxY = V.getCenter().y + V.getSize().y / 2.f;

		if (MaxY < B->getPosition().y)
		{
			SetAIBoatPosition(*B);
		}

		if (B->getPosition().y < V.getCenter().y + V.getSize().y / 2.f &&
			B->getPosition().y > V.getCenter().y - V.getSize().y / 2.f)
		{
			B->Update(Delta);
		}

		if (B->GetHealthPoints() <= 0 && B->IsBoatAlive())
		{
			B->SetBoatDead();
			++PlayerData_.KillCount;

			if (PlayerData_.KillCount > MaxKills_)
			{
				if (++PlayerData_.Type < (Int32)BoatType::Warship)
				{
					EvolvePlayerAndAI((BoatType)PlayerData_.Type);
					CentreViewOnPlayer();
					PlayerData_.KillCount = 0;
				}
				else
				{
					ShowFinished_ = true;
				}
			}
		}
	}

	Boats_[ActiveBoat_].Update(Delta);*/

	Player_->UpdatePlayer(Delta);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		Player_->Fire();
	}
	//if (PlayerData_.FireTimer == 0.f)
	//{
	//	const Vector2f MousePos(GetRenderTexture()->mapPixelToCoords(sf::Mouse::getPosition(*GetRenderWindow())));
	//	Vector2f Direc = (MousePos - Boats_[ActiveBoat_].getPosition());
	//	Direc = Normalise(Direc);

	//	ProjectileFireData Data;
	//	Data.BoatType = Raft;
	//	Data.Direction = Direc;
	//	Data.FiredBy = Player;
	//	Data.StartPos = Boats_[ActiveBoat_].getPosition();

	//	ProjectileMgr_->FireProjectile(Data);

	//	PlayerData_.FireTimer += Delta;
	//	//Boats_[ActiveBoat_].Fire(Direc);
	//}
	//else
	//{
	//	PlayerData_.FireTimer += Delta;
	//	if (PlayerData_.FireTimer > 0.75f)
	//	{
	//		PlayerData_.FireTimer = 0.f;
	//	}
	//}

	ProjectileMgr_->ProjectileUpdate(Delta, Boats_);

	//Todo: allow player to query a database of world data that contains all objects data for collisions  
	//Todo: add in player health mechanic 

	/*
	if (PlayerCrashed())
	{
		--PlayerData_.Lives;

		if (PlayerData_.Lives > 0)
		{
			RespawnText_.setString(L"You failed to avoid the rocks! \nLives remaining: " + std::to_wstring(PlayerData_.Lives));
		}
		Respawning_ = true;
		Vector2f Size(RespawnText_.getGlobalBounds().width / 2.f, RespawnText_.getGlobalBounds().height / 2.f);
		RespawnText_.setPosition(GetRenderTexture()->getView().getCenter() - Size);
	}
	*/
	/*
	if (PlayerData_.Lives <= 0)
	{
		Boats_[ActiveBoat_].SetBoatDead();
		RespawnText_.setString(L"Erm... Sorry, you died! \nLives remaining: " + std::to_wstring(PlayerData_.Lives));

		Respawning_ = true;
		Vector2f Size(RespawnText_.getGlobalBounds().width / 2.f, RespawnText_.getGlobalBounds().height / 2.f);
		RespawnText_.setPosition(GetRenderTexture()->getView().getCenter() - Size);
	}
	*/
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

		for (const auto& B : Boats_)
		{
			if (B->GetControlState() != BoatControlState::None)
			{
				GetRenderTexture()->draw(*B);
			}
		}

		//Render projectiles here 
		GetRenderTexture()->draw(*ProjectileMgr_);
		if (!GameStarted_)
		{
			GetRenderTexture()->draw(Instructions_);
		}

		if (Respawning_)
		{
			GetRenderTexture()->draw(RespawnText_);
		}
	}

}

void PlayState::PostRender() const
{
	if (!ShowFinished_)
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
			Respawn();
		}
	}
}

void PlayState::SetAIBoatPosition(Boat & B)
{
	const float SpawnDist(800);

	float BeforePoint(Boats_[ActiveBoat_]->getPosition().y);
	float XSpawn = RandFloat(0.f, (float)GetRenderTexture()->getSize().x - B.GetGlobalBounds().width);

	BeforePoint -= GetRenderTexture()->getView().getSize().y;
	BeforePoint = fabs(BeforePoint);

	B.setPosition(XSpawn, RandFloat(BeforePoint, BeforePoint + SpawnDist) * -1);
	B.ResetFireTimer();
	B.SetHealthPoints(100);
	B.SetBoatAlive();

	for (auto B2 : AIBoats_)
	{
		if (B2 != &B)
		{
			if (GetSquareLength(B2->getPosition() - B.getPosition()) <= pow(SpawnDist, 2))
			{
				XSpawn = RandFloat(0.f, (float)GetRenderTexture()->getSize().x - B.GetGlobalBounds().width);
				B.setPosition(XSpawn, RandFloat(BeforePoint, BeforePoint + SpawnDist) * -1);
			}
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
		if (i + 1 > (signed) Obstacles_.size() / 2)
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

		for (int i(0); i < 4; ++i)
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

void PlayState::Respawn()
{
	if (PlayerData_.Lives > 0)
	{

		Vector2f Pos = Boats_[ActiveBoat_]->getPosition();

		Pos.y += Boats_[ActiveBoat_]->GetGlobalBounds().height * 4.0f;
		Boats_[ActiveBoat_]->setPosition(Pos);


		CentreViewOnPlayer();
		return;
	}

	PlayerData_.Lives = 5;

	sf::Vector2f Pos;
	Pos.x = ((float)GetRenderWindow()->getSize().x / 2.f) - (Boats_[ActiveBoat_]->GetSize().x / 2.f);
	Pos.y = ((float)GetRenderWindow()->getSize().y / 2.f) - (Boats_[ActiveBoat_]->GetSize().y / 2.f);
	Boats_[ActiveBoat_]->setPosition(Pos);
	Boats_[ActiveBoat_]->setOrigin(Boats_[ActiveBoat_]->GetGlobalBounds().width / 2.f, Boats_[ActiveBoat_]->GetGlobalBounds().height / 2.f);

	CentreViewOnPlayer();

	EvolvePlayerAndAI(BoatType::Raft);

	for (auto B : AIBoats_)
	{
		SetAIBoatPosition(*B);
	}
}

bool PlayState::PlayerCrashed()
{
	/*  
	Boat* PBoat = Boats_[ActiveBoat_];

	for (auto& B2 : Boats_)
	{
		{
			if (PBoat == &B2)
			{
				continue;
			}

			if (!B2.IsBoatAlive() || !PBoat->IsBoatAlive() || B2.GetControlState() == BoatControlState::None)
			{
				continue;
			}

			if (PBoat->GetGlobalBounds().intersects(B2.GetGlobalBounds()))
			{
				PBoat->TakenDamage();
				return true;
			}
		}
	}

	for (auto& O : Obstacles_)
	{
		if (GetSquareLength(O.getPosition() - PBoat->getPosition()) > 100 * 100)
		{
			continue;
		}
		if (O.getGlobalBounds().intersects(PBoat->GetGlobalBounds()))
		{
			return true;
		}
	}
	*/
	return false;
}

void PlayState::CentreViewOnPlayer()
{
	/*View V = GetRenderTexture()->getView();
	if (!BG_->GetGlobalBounds().contains(V.getCenter() - sf::Vector2f(0.f, (float)V.getSize().y)))
	{
		return;
	}
	float HalfHeight;

	HalfHeight = Boats_[ActiveBoat_].GetGlobalBounds().height / 2.f;


	V.setCenter(V.getCenter().x, Boats_[ActiveBoat_].getPosition().y);

	GetRenderTexture()->setView(V);*/
}

void PlayState::EvolvePlayerAndAI(BoatType Type)
{
	//for (auto& B : Boats_)
	//{
	//	B.SetControlState(BoatControlState::None);
	//	B.SetBoatAlive();
	//	B.SetHealthPoints(100);
	//}

	//const Int32 BoatTypeCount = 5;
	//Int32 Index = 0;
	//Int32 AICount = 0;
	//bool AISet(false);
	//bool PlayerSet(false);

	//while (Index < (signed)Boats_.size())
	//{
	//	auto& B = Boats_[Index];

	//	if (B.GetBoatType() != Type)
	//	{
	//		++Index;
	//		continue;
	//	}

	//	if (PlayerSet && !AISet)
	//	{
	//		B.SetControlState(BoatControlState::AI);
	//		AIBoats_[AICount] = &B;
	//		++AICount;
	//		if (AICount >= (BoatCount_ / BoatTypeCount) - 1)
	//		{
	//			AISet = true;
	//		}
	//	}

	//	if (!PlayerSet)
	//	{
	//		B.SetControlState(BoatControlState::Player);
	//		PlayerSet = true;
	//		ActiveBoat_ = Index;


	//		Vector2f Pos;
	//		Pos.x = ((float)GetRenderWindow()->getSize().x / 2.f);
	//		Pos.y = ((float)GetRenderWindow()->getSize().y / 2.f);
	//		Boats_[ActiveBoat_].setPosition(Pos);
	//		Boats_[ActiveBoat_].setOrigin(Boats_[ActiveBoat_].GetGlobalBounds().width / 2.f, Boats_[ActiveBoat_].GetGlobalBounds().height / 2.f);

	//		PlayerData_.KillCount = 0;
	//		PlayerData_.Lives = 10;
	//		B.SetBoatAlive();
	//	}
	//	++Index;
	//}
}
