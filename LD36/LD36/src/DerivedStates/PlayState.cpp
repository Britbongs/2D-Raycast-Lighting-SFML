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
	Circle_ = CircleShape(10);
	Circle_.setFillColor(Color::Red);

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
	CalculateUniquePoints();

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
	sf::Vector2i MousePos = Mouse::getPosition(*GetRenderWindow());
	sf::Vector2f WorldMousePos = GetRenderTexture()->mapPixelToCoords(MousePos);
	Circle_.setPosition(WorldMousePos - Vector2f(Circle_.getRadius(), Circle_.getRadius()));
	DrawVisibilityPolygon(WorldMousePos);


}

void PlayState::DrawVisibilityPolygon(const Vector2f& Origin)
{
	auto& Colliders = World_->GetTileMeshCollidersBlocked();


	std::vector<float> UniqueAngles = CalculateUniqueAngles(Origin);

	std::vector<Vector3f> Intersections;

	Vector2f A(0.f, 0.f), B(0.f, 0.f);
	Vector3f Intersection(0.f, 0.f, 0.f);

	for (Int32 k{ 0 }; k < (Int32)UniqueAngles.size(); ++k)
	{
		Vector2f DirectionVector(cosf(UniqueAngles[k]), sinf(UniqueAngles[k]));
		Ray Ray{ Origin, Vector2f(Origin.x + DirectionVector.x, Origin.y + DirectionVector.y) };

		Vector3f ClosestIntersection{ -5,-5,-5 };
		for (Int32 i{ 0 }; i < (Int32)Colliders.size(); ++i)
		{
			TileCollisionData& Data = Colliders[i];
			//If the collider isn't blocked then continue the loop
			if (!Data.bIsBlockedTile)
				continue;

			//Loop through every point in the mesh collider
			for (Int32 j{ 0 }; j < Data.MCollider.GetPointCount(); ++j)
			{
				//Point a of segment
				A = Data.MCollider.GetTransformedPoint(j);
				//Point b of segment
				B = j + 1 < Data.MCollider.GetPointCount() ? Data.MCollider.GetTransformedPoint(j + 1) : Data.MCollider.GetTransformedPoint(0);

				//Create ray
				//Get the intersection point
				Intersection = RayTest_.GetIntersection(Ray, A, B);

				//If GetIntersection returned early then continue
				if (Intersection.z == -1)
					continue;

				if (ClosestIntersection.z == -5 || Intersection.z < ClosestIntersection.z)
				{
					ClosestIntersection = Intersection;
				}

			}
		}
		Intersections.push_back(Vector3f(ClosestIntersection.x, ClosestIntersection.y, UniqueAngles[k]));
	}


	//Sort intersections by angle
	sort(Intersections.begin(), Intersections.end(), [](Vector3f const& a, Vector3f const& b) { return a.z < b.z; });

	VisibilityPolygon_.resize(Intersections.size() + 2);
	VisibilityPolygon_.setPrimitiveType(TrianglesFan);

	for (Int32 i = 0; i < (Int32)Intersections.size(); ++i)
	{
		VisibilityPolygon_[i + 1].position = Vector2f(Intersections[i].x, Intersections[i].y);
		VisibilityPolygon_[i + 1].color = Color::Green;
	}

	VisibilityPolygon_[0].position = Origin;
	VisibilityPolygon_[0].color = Color::Green;
	VisibilityPolygon_[VisibilityPolygon_.getVertexCount() - 1].position = VisibilityPolygon_[1].position;
	VisibilityPolygon_[VisibilityPolygon_.getVertexCount() - 1].color = Color::Green;

}

void PlayState::Render() const
{
	GetRenderTexture()->draw(*TiledMap_);
	GetRenderTexture()->draw(VisibilityPolygon_);
	GetRenderTexture()->draw(Circle_);

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

void PlayState::CalculateUniquePoints()
{
	auto& Colliders = World_->GetTileMeshCollidersBlocked();

	std::vector<Vector2f> Points;
	for (int i{ 0 }; i < Colliders.size(); ++i)
	{
		for (int j{ 0 }; j < Colliders[i].MCollider.GetPointCount(); ++j)
		{
			Points.push_back(Colliders[i].MCollider.GetTransformedPoint(j));
		}
	}

	for (int i{ 0 }; i < Points.size(); ++i)
	{
		if (find(UniquePoints_.begin(), UniquePoints_.end(), Points[i]) != UniquePoints_.end())
		{

		}
		else
		{
			UniquePoints_.push_back(Points[i]);
		}
	}
}

std::vector<float> PlayState::CalculateUniqueAngles(const Vector2f origin)
{
	std::vector<float> UniqueAngles;

	for (int i{ 0 }; i < UniquePoints_.size(); ++i)
	{
		float angle = atan2(UniquePoints_[i].y - origin.y, UniquePoints_[i].x - origin.x);
		UniqueAngles.push_back(angle - 0.0001);
		UniqueAngles.push_back(angle);
		UniqueAngles.push_back(angle + 0.0001);
	}

	return UniqueAngles;
}

