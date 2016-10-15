#include "stdafx.h"
#include <iostream> 
#include <vld.h>
#define ASSERT assert
#define REPETITIONS 100000
using namespace std;

enum Geometry
{
	eTriangle,
	eSquare
};


class MeshCollider
{
public:

	MeshCollider(VertexArray& VArry)
	{
		AxesNormals_.resize(VArry.getVertexCount());
		Vertices_.resize(VArry.getVertexCount());
		TransformedVerts_.resize(VArry.getVertexCount());

		for (Int32 VertIndex = 0; VertIndex < (Int32)VArry.getVertexCount(); ++VertIndex)
		{
			Vertices_[VertIndex] = VArry[VertIndex].position;
		}
	}

	~MeshCollider()
	{

	}

	const std::vector<Vector2f>& GetAxesNormals()
	{
		return AxesNormals_;
	}

	Int32 GetVertexCount()
	{
		return (Int32)Vertices_.size();
	}

	Vector2f GetVertexPosition(Int32 VertIndex)
	{
		return TransformedVerts_[VertIndex];
	}

	void UpdateMesh(const Transform& VertTransform)
	{
		UpdateVertices(VertTransform);
		UpdateAxesNormals(VertTransform);
	}

private:

	void UpdateVertices(const Transform& VertTransform);
	void UpdateAxesNormals(const Transform& VertTransform);

	std::vector<Vector2f> Vertices_;
	std::vector<Vector2f> TransformedVerts_;
	std::vector<Vector2f> AxesNormals_;

};

class Entity : public Drawable, public Transformable
{

public:

	explicit Entity(Geometry G)
		: Shape_(G)
	{

	}
	void InitPoints();

	MeshCollider& GetMeshCollider() const
	{
		return *MeshCollider_;
	}

	void Update()
	{
		MeshCollider_->UpdateMesh(getTransform());
	}

	void SetColour(Color Col)
	{
		for (Int32 i = 0; i < VertArray_.getVertexCount(); ++i)
		{
			VertArray_[i].color = Col;
		}
	}
private:

	void Entity::draw(RenderTarget& RTarget, RenderStates RStates) const override
	{
		RStates.transform *= getTransform();
		RTarget.draw(VertArray_, RStates);
	}

	VertexArray VertArray_;

	MeshCollider* MeshCollider_;

	Geometry Shape_ = eTriangle;
};


void Entity::InitPoints()
{
	switch (Shape_)
	{
	case eTriangle:
	{
		VertArray_.resize(3);
		VertArray_.setPrimitiveType(PrimitiveType::Triangles);
		VertArray_[0].color = Color::Blue;
		VertArray_[0].position = Vector2f(0.f, 0.f);

		VertArray_[1].color = Color::Blue;
		VertArray_[1].position = Vector2f(0.5f, -1.f);

		VertArray_[2].color = Color::Blue;
		VertArray_[2].position = Vector2f(1.f, 0.f);

		this->setOrigin(VertArray_[0].position);
		MeshCollider_ = new MeshCollider(VertArray_);

		break;
	}
	case eSquare:
		break;
	}

}

void MeshCollider::UpdateAxesNormals(const Transform& VertTransform)
{

	Vector2f VertA, VertB;
	Vector2f Edge;

	for (Int32 VertIndex = 0; VertIndex < (Int32)Vertices_.size(); ++VertIndex)
	{
		VertA = TransformedVerts_[VertIndex];
		//If we're not on the last value in the array, then use the next vertex
		//else use the first vertex
		VertB = TransformedVerts_[VertIndex + 1 < (Int32)Vertices_.size() ? VertIndex + 1 : 0];
		Edge = VertA - VertB;

		//Get perpendicular outward pointing normal to the axes (not normalised)
		float Temp = Edge.x;

		Edge.x = -Edge.y;
		Edge.y = Temp;

		AxesNormals_[VertIndex] = Edge;
	}
}

void MeshCollider::UpdateVertices(const Transform& VertTransform)
{
	for (Int32 VertIndex = 0; VertIndex < (Int32)Vertices_.size(); ++VertIndex)
	{
		TransformedVerts_[VertIndex] = VertTransform.transformPoint(Vertices_[VertIndex]);
	}
}

bool DoMeshsCollide(MeshCollider& MeshA, MeshCollider& MeshB)
{

	auto AxesA = MeshA.GetAxesNormals();
	auto AxesB = MeshB.GetAxesNormals();

	struct Projection
	{
		float Min = 0.f;
		float Max = 0.f;
	};

	Projection ProjA, ProjB;

	auto GetProjection = [=](Vector2f Axis, MeshCollider& Collider)
	{
		Projection Proj;
		Proj.Max = Proj.Min = DotProduct(Axis, Collider.GetVertexPosition(0));

		for (Int32 i = 1; i < Collider.GetVertexCount(); ++i)
		{
			float DP = DotProduct(Axis, Collider.GetVertexPosition(i));
			Proj.Max = Max(DP, Proj.Max);
			Proj.Min = Min(DP, Proj.Min);

		}
		return Proj;
	};

	auto DoProjectionsOverlap = [=](Projection& ProjA, Projection& ProjB)
	{
		return ProjB.Max >= ProjA.Min && ProjB.Min <= ProjA.Max;
	};
	Projection ProjectionA;
	Projection ProjectionB;
	for (Int32 i = 0; i < (Int32)AxesA.size(); ++i)
	{
		ProjectionA = GetProjection(AxesA[i], MeshA);
		ProjectionB = GetProjection(AxesA[i], MeshB);

		if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
			return (false);
	}

	for (Int32 i = 0; i < (Int32)AxesB.size(); ++i)
	{
		ProjectionA = GetProjection(AxesB[i], MeshA);
		ProjectionB = GetProjection(AxesB[i], MeshB);

		if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
			return(false);
	}
	return (true);
}

/*
int main(int argc, char* argv[], char* envp[])
{
	RenderWindow RWindow(VideoMode(1024, 720), "SATTest", Style::Close);
	RWindow.setFramerateLimit(80u);
	Entity E(eTriangle);

	E.InitPoints();
	E.scale(100, 100);
	E.setPosition(600, 600);

	Entity B(eTriangle);

	B.InitPoints();
	B.scale(100, 100);
	B.setPosition(500, 600);

	//E.Update();
	//B.Update();

	//Clock C;
	//Time T;
	///*for (int i = 0; i < REPETITIONS; ++i)
	//{
	//	C.restart();
	//	T += C.getElapsedTime();
	//}

	//std::cout << "Overall Execution Time : " << T.asMilliseconds() << " ms\n";
	//std::cout << "Average Execution Time : " << T.asMicroseconds() / REPETITIONS << " microseconds\n";
	//system("PAUSE");
	//return 0;

	sf::Clock DeltaClock;
	while (RWindow.isOpen())
	{
		float Delta = DeltaClock.restart().asSeconds();
		Event Evnt;

		while (RWindow.pollEvent(Evnt))
		{
			if (Evnt.type == Event::EventType::Closed)
			{
				RWindow.close();
			}

			if (Evnt.type == Event::EventType::KeyPressed)
			{
				if (Evnt.key.code == Keyboard::Escape)
				{
					RWindow.close();
				}
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			B.move(60.f* Delta, 0.f);
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			B.move(-60.f* Delta, 0.f);
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			B.move(0.f, -60.f* Delta);
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			B.move(0.f, 60.f* Delta);
		}

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			B.rotate(-10.f* Delta);
		}

		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			B.rotate(10.f* Delta);
		}

		E.Update();
		B.Update();
		
		if (DoMeshsCollide(B.GetMeshCollider(), E.GetMeshCollider()))
		{
			E.SetColour(Color::Red);
			B.SetColour(Color::Red);
		}
		else
		{
			E.SetColour(Color::Green);
			B.SetColour(Color::Green);
		}

		RWindow.clear();
		RWindow.draw(E);
		RWindow.draw(B);
		RWindow.display();

	}

	
	return 0;
}
*/

int main()
{
	RenderWindow RWindow; 
	RWindow.create(VideoMode(640, 480), "Hello", Style::Close);

	while (RWindow.isOpen())
	{
		Event evnt; 
		while (RWindow.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed)
			{
				RWindow.close();
			}
		}
	} 

	return 0;
}