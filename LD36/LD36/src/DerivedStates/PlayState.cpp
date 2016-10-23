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
	LightMap_.setSmooth(true);
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
				Intersection = Ray.GetIntersection(A, B);

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
	Color Col = Color::Cyan;
	//Col.a = 10;
	Col.a = (rand() % 255) + 10;
	DebugPrintF(DebugLog, L"Col - %i", Col.a);
	sort(Intersections.begin(), Intersections.end(), [](Vector3f const& a, Vector3f const& b) { return a.z < b.z; });


	VertexArray VisibilityPolygon = VertexArray(TrianglesFan, Intersections.size() + 2);

	for (Int32 i = 0; i < (Int32)Intersections.size(); ++i)
	{
		VisibilityPolygon[i + 1].position = Vector2f(Intersections[i].x, Intersections[i].y);
		VisibilityPolygon[i + 1].color = Col;
	}

	VisibilityPolygon[0].position = Origin;
	VisibilityPolygon[0].color = Col;
	VisibilityPolygon[VisibilityPolygon.getVertexCount() - 1].position = VisibilityPolygon[1].position;
	VisibilityPolygon[VisibilityPolygon.getVertexCount() - 1].color = Col;


	VisibilityPolygons_.push_back(VisibilityPolygon);

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

	if (!AttenuationShader_.loadFromFile("res//shader//attenuationVertexShader.vert", "res//shader//attenuationFragShader.frag"))
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

#pragma region LightRender
	RenderStates LightRenderState;

	float Radius = Circle_.getRadius();
	Vector2f Pos = Vector2f(GetRenderTexture()->mapCoordsToPixel(Circle_.getPosition()));
	Pos.y = LightMap_.getSize().y - Pos.y + Radius; //Fix for inverted texture pos
	Pos.x += Radius;
	AttenuationShader_.setUniform("point", Pos);
	AttenuationShader_.setUniform("lightIntensity", 100.f);

	LightRenderState.blendMode = BlendAlpha;
	LightRenderState.shader = &AttenuationShader_;
	LightMap_.clear();
	LightMap_.setView(GetRenderTexture()->getView());

	for (Int32 i = 0; i < (Int32)VisibilityPolygons_.size(); ++i)
	{
#pragma region FragmentShaderAttenuation 
		AttenuationShader_.setUniform("texture", LightMap_.getTexture());
		Vector2f Pos = Vector2f(GetRenderTexture()->mapCoordsToPixel(VisibilityPolygons_[i][0].position));
		Pos.y = LightMap_.getSize().y - Pos.y + Radius; //Fix for inverted texture pos
		AttenuationShader_.setUniform("point", Pos);
#pragma endregion Fragment Shader Attenuation 

#pragma region VertexShaderAttenuation
		//AttenuationShader_.setUniform("point", VisibilityPolygons_[i][0].position);
#pragma endregion Vertex Shader Attenuation
		LightMap_.draw(VisibilityPolygons_[i], LightRenderState);

	}
	LightMap_.display();

	AmbientShader_.setUniform("ambientColour", Glsl::Vec4(0.27f, 0.15f, 0.3f, 0.6f));
	AmbientShader_.setUniform("lightMapTexture", LightMap_.getTexture());
	AmbientShader_.setUniform("resolution", Glsl::Vec2(GetRenderTexture()->getSize()));
#pragma endregion Light Render

	GetRenderTexture()->draw(S, RStates);//, RStates);

	/*
	for (const auto& GO : GameObjects_)
	{
		if (GO->IsActive() == true)
		{
			GetRenderTexture()->draw(*GO);
		}
	}
	*/

}

void PlayState::PostRender() const
{
	RenderStates Blend;
}

void PlayState::HandleEvents(sf::Event& Evnt, float Delta)
{
	if (Evnt.type == sf::Event::KeyPressed)
	{
		if (Evnt.key.code == sf::Keyboard::R)
		{
			LoadShaders();
		}

		if (Evnt.key.code == Keyboard::C)
		{
			VisibilityPolygons_.clear();
		}
		if (Evnt.key.code == Keyboard::X)
		{
			Image Img = LightMap_.getTexture().copyToImage();
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
			/*auto AsyncVisibilityCreate =
				std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos);*/
			sf::Clock C;
			C.restart();

			const float Offset = 5.f;

			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos);
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(Offset, 0));
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(Offset, Offset));
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(0.f, Offset));
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(Offset, -Offset));
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(-Offset, 0));
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(-Offset, Offset));
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(-Offset, -Offset));
			std::async(std::launch::async, &PlayState::DrawVisibilityPolygon, this, WorldMousePos + Vector2f(0.f, -Offset));


			/*
			DrawVisibilityPolygon(WorldMousePos);
			DrawVisibilityPolygon(WorldMousePos + Vector2f(Offset, 0));
			DrawVisibilityPolygon(WorldMousePos + Vector2f(Offset, Offset));
			DrawVisibilityPolygon(WorldMousePos + Vector2f(0.f, Offset));
			DrawVisibilityPolygon(WorldMousePos + Vector2f(Offset, -Offset));

			DrawVisibilityPolygon(WorldMousePos + Vector2f(-Offset, 0));
			DrawVisibilityPolygon(WorldMousePos + Vector2f(-Offset, Offset));
			DrawVisibilityPolygon(WorldMousePos + Vector2f(-Offset, -Offset));
			DrawVisibilityPolygon(WorldMousePos + Vector2f(0.f, -Offset));
			*/

			Time T = C.getElapsedTime();
			float Seconds = T.asSeconds();
			Int32 Milliseconds = T.asMilliseconds();
			Int64 Microseconds = T.asMicroseconds();
			DebugPrintF(DebugLog, L"Time taken to execute: %f Seconds \n%i Milliseconds \n%li Microseconds", Seconds, Milliseconds, Microseconds);

#pragma endregion VisibilityAsync
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
