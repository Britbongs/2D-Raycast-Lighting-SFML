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

	SceneRenderer_.create((Uint32)TiledMap_->GetSize().x, (Uint32)TiledMap_->GetSize().y);
	LightMap_.create(GetRenderTexture()->getSize().x, GetRenderTexture()->getSize().y);

	LoadShaders();
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
	Color Col = Color::White;
	Col.a = 85;
	sort(Intersections.begin(), Intersections.end(), [](Vector3f const& a, Vector3f const& b) { return a.z < b.z; });

	VisibilityPolygon_.resize(Intersections.size() + 2);
	VisibilityPolygon_.setPrimitiveType(TrianglesFan);

	for (Int32 i = 0; i < (Int32)Intersections.size(); ++i)
	{
		VisibilityPolygon_[i + 1].position = Vector2f(Intersections[i].x, Intersections[i].y);
		VisibilityPolygon_[i + 1].color = Col;
	}

	VisibilityPolygon_[0].position = Origin;
	VisibilityPolygon_[0].color = Col;
	VisibilityPolygon_[VisibilityPolygon_.getVertexCount() - 1].position = VisibilityPolygon_[1].position;
	VisibilityPolygon_[VisibilityPolygon_.getVertexCount() - 1].color = Col;

}

void PlayState::LoadShaders()
{
	if (!Shader::isAvailable())
	{
		DebugPrintF(DebugLog, L"Shaders not available!");
	}

	if (!AmbientShader_.loadFromFile("res//shader//defaultVertShader.vert", "res//shader/ambientFragShader.frag"))
	{
		DebugPrintF(DebugLog, L"Failed to load ambient shader");
	}

	if (!AttenuationShader_.loadFromFile("res//shader//defaultVertShader.vert", "res//shader//attenuationFragShader.frag"))
	{
		DebugPrintF(DebugLog, L"Failed to load attenuation shader");
	}
}

void PlayState::Render()
{

	Sprite Empty;
	RenderStates RStates;
	RStates.shader = &AmbientShader_;
	RStates.blendMode = BlendAlpha;

	SceneRenderer_.clear(Color::Black);
	SceneRenderer_.draw(*TiledMap_);
	SceneRenderer_.draw(Circle_);
	SceneRenderer_.display();

	Sprite S(SceneRenderer_.getTexture());
	RenderStates LightRenderState;

	AttenuationShader_.setParameter("texture", Shader::CurrentTexture);
	float Radius = Circle_.getRadius();
	Vector2f Pos = Vector2f(GetRenderTexture()->mapCoordsToPixel(Circle_.getPosition()));
	Pos.y = LightMap_.getSize().y - Pos.y + Radius; //Fix for inverted texture pos
	Pos.x += Radius;
	AttenuationShader_.setParameter("point", Pos);
	AttenuationShader_.setParameter("attenuationConstant", 45.f);

	LightRenderState.blendMode = BlendAdd;
	LightRenderState.shader = &AttenuationShader_;
	LightMap_.clear();
	LightMap_.setView(GetRenderTexture()->getView());
	LightMap_.draw(VisibilityPolygon_, LightRenderState);
	LightMap_.display();

	AmbientShader_.setParameter("ambientColour", 0.27f, 0.15f, 0.3f, 0.6f);
	AmbientShader_.setParameter("lightMapTexture", LightMap_.getTexture());
	AmbientShader_.setParameter("resolution", (float)GetRenderTexture()->getSize().x, (float)GetRenderTexture()->getSize().y);

	GetRenderTexture()->draw(S, RStates);//, RStates);

	/*for (const auto& GO : GameObjects_)
	{
		if (GO->IsActive() == true)
		{
			GetRenderTexture()->draw(*GO);
		}
	}*/

}

void PlayState::PostRender() const
{
	RenderStates Blend;
	//Blend.blendMode = BlendAlpha;
	//Blend.shader = &AmbientShader_;
	//GetRenderWindow()->draw(VisibilityPolygon_, Blend);
}

void PlayState::HandleEvents(sf::Event& Evnt, float Delta)
{
	if (Evnt.type == sf::Event::KeyPressed)
	{
		if (Evnt.key.code == sf::Keyboard::R)
		{
			LoadShaders();
		}
	}
}

void PlayState::CalculateUniquePoints()
{
	auto& Colliders = World_->GetTileMeshCollidersBlocked();

	std::vector<Vector2f> Points;
	for (int i{ 0 }; i < (Int32)Colliders.size(); ++i)
	{
		for (int j{ 0 }; j < Colliders[i].MCollider.GetPointCount(); ++j)
		{
			Points.push_back(Colliders[i].MCollider.GetTransformedPoint(j));
		}
	}

	for (int i{ 0 }; i < (Int32)Points.size(); ++i)
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

	for (int i{ 0 }; i < (Int32)UniquePoints_.size(); ++i)
	{
		float angle = atan2(UniquePoints_[i].y - origin.y, UniquePoints_[i].x - origin.x);
		UniqueAngles.push_back(angle - 0.0001f);
		UniqueAngles.push_back(angle);
		UniqueAngles.push_back(angle + 0.0001f);
	}

	return UniqueAngles;
}

