#include "stdafx.hpp"
#include "DerivedStates\PlayState.hpp"

using namespace sf;

PlayState::PlayState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture)
	: State("Play", ID, RWindow, RTexture)
{
	AssetMgr_ = AssetManager::GetInstance();
}

PlayState::~PlayState()
{
	if (World_)
	{
		delete World_;
	}
}

bool PlayState::Initialise()
{
	InitRand();

	auto AM = AssetManager::GetInstance();

	//My test shit I need to remove
	Circle_ = CircleShape(5, 30);
	Circle_.setFillColor(Color::Red);


	RayLines_.resize(50);
	for (int i = 0; i < RayLines_.size(); ++i)
	{
		RayLines_.at(i).resize(2);
		RayLines_.at(i).setPrimitiveType(Lines);
		RayLines_.at(i)[0].color = Color::Red;
		RayLines_.at(i)[1].color = Color::Red;
	}


	//---

	World_ = new World(GameObjects_, GetRenderTexture());

	Loader_.LoadMap("res//test_map.tmx");
	
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

	//Player_ = new PlayerController(ProjectileMgr_, World_, Vector2f(GetRenderTexture()->getSize()));

#ifndef PLAYABLE_BUILD
	assert(World_);
#endif 

	return false;
}

void PlayState::Deinitialise()
{
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

	for (Int32 i = 0; i < (Int32)GameObjects_.size(); ++i)
	{
		if (GameObjects_[i] != nullptr)
		{
			delete GameObjects_[i];
			GameObjects_[i] = nullptr;
		}
	}
}

void PlayState::Update(float Delta)
{
	sf::Vector2i MousePos = Mouse::getPosition();
	sf::Vector2f WorldMousePos = GetRenderTexture()->mapPixelToCoords(MousePos);
	std::vector<TileCollisionData>* Colliders = World_->GetTileMeshCollidersBlocked();

	Circle_.setPosition(WorldMousePos);


	std::vector<Vector2f> Intersections;
	for (int angle = 0; angle < PI*2; angle+=(PI*2)/7)
	{
		float dx = cos(angle);
		float dy = sin(angle);
		Ray Ray{ WorldMousePos, Vector2f(WorldMousePos.x + dx, WorldMousePos.y + dy )};

		Vector3f ClosestIntersection{ -5,-5,-5 };
		for (int i{ 0 }; i < Colliders->size(); ++i)
		{
			TileCollisionData Data = Colliders->at(i);
			//If the collider isn't blocked then continue the loop
			if (!Data.bIsBlockedTile)
				continue;

			//Loop through every point in the mesh collider
			for (int j{ 0 }; j < Data.MCollider.GetPointCount(); ++j)
			{
				//Point a of segment
				Vector2f a = Data.MCollider.GetTransformedPoint(j);
				//Point b of segment
				Vector2f b = j + 1 < Data.MCollider.GetPointCount() ? Data.MCollider.GetTransformedPoint(j + 1) : Data.MCollider.GetTransformedPoint(0);

				//Create ray
				//Get the intersection point
				Vector3f intersection = RayTest_.GetIntersection(Ray, a, b);

				//If GetIntersection returned early then continue
				if (intersection.z == -1)
					continue;

				if (ClosestIntersection.z == -5 || intersection.z < ClosestIntersection.z)
				{
					ClosestIntersection = intersection;
				}

			}
		}
		Intersections.push_back(Vector2f(ClosestIntersection.x, ClosestIntersection.y));
	}

	for (int i = 0; i < Intersections.size(); ++i)
	{
		RayLines_.at(i)[0].position = WorldMousePos;
		RayLines_.at(i)[1].position = Intersections[i];
	}
}

void PlayState::Render() const
{
	GetRenderTexture()->draw(*TiledMap_);
	GetRenderTexture()->draw(Circle_);

	for (int i = 0; i < RayLines_.size(); ++i)
	{
		GetRenderTexture()->draw(RayLines_[i]);
	}

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

}

void PlayState::HandleEvents(sf::Event& Evnt, float Delta)
{
	if (Evnt.type == sf::Event::KeyPressed)
	{
	}
}
