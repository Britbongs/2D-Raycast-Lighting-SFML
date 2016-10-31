#include "stdafx.hpp"
#include "MeshCollider/MeshCollider.h"

MeshCollider::MeshCollider()
{

}

MeshCollider::MeshCollider(const std::vector<Vector2f>& VertArray)
{
	InitialisePoints(VertArray);
}

MeshCollider::~MeshCollider()
{
	TransformedPoints_.clear();
	NormalList_.clear();
	LocalPoints_.clear();
}

void MeshCollider::UpdatMeshCollider(const Transform& ObjectTransform)
{
	//Update the transformed points list
	UpdateTransformedPoints(ObjectTransform);
	//Update normal list
	UpdateNormals();
}

Vector2f MeshCollider::GetNormal(Int32 Index) const
{
	if (Index >= 0 && Index < (Int32)NormalList_.size())
	{
		return NormalList_[Index];
	}
	return Normal(0.f, 0.f);
}

Vector2f MeshCollider::GetTransformedPoint(Int32 Index) const
{
	if (Index >= 0 && Index < (Int32)TransformedPoints_.size())
	{
		return TransformedPoints_[Index];
	}
	return Normal(0.f, 0.f);
}

Int32 MeshCollider::GetPointCount() const
{
	return (Int32)TransformedPoints_.size();
}

Int32 MeshCollider::GetNormalListSize() const
{
	return (Int32)NormalList_.size();
}

void MeshCollider::SetPointsList(const std::vector<Vector2f>& VertArray)
{
	InitialisePoints(VertArray);
}

// Private Functions 

void MeshCollider::InitialisePoints(const std::vector<Vector2f>&  VertArray)
{
	assert((signed)VertArray.size() <= 4);
	LocalPoints_.clear();
	TransformedPoints_.clear();
	NormalList_.clear();
	for (auto& Vert : VertArray)
	{
		//Fill the local points vector
		LocalPoints_.push_back(Vert);
	}

	//Initialise transformed points to have the same values as the local points until a transform is applied
	TransformedPoints_.resize(LocalPoints_.size());

	for (Int32 i = 0; i < (Int32)TransformedPoints_.size(); ++i)
	{
		TransformedPoints_[i] = LocalPoints_[i];
	}

	NormalList_.resize(TransformedPoints_.size());
	UpdateNormals();
}

void MeshCollider::UpdateTransformedPoints(const Transform & ObjectTransform)
{
	for (Int32 PointIndex = 0; PointIndex < (Int32)LocalPoints_.size(); ++PointIndex)
	{//Update the transformed points array using the objects transform
		TransformedPoints_[PointIndex] = ObjectTransform.transformPoint(LocalPoints_[PointIndex]);
	}
}

void MeshCollider::UpdateNormals()
{
	//TODO move to maths utility
	auto MakePerpendicular = [](const Normal& Edge)
	{
		return Normal(-Edge.y, Edge.x);
	};

	for (Int32 PointIndex = 0; PointIndex < (Int32)TransformedPoints_.size(); ++PointIndex)
	{
		Point PointA = TransformedPoints_[PointIndex];

		//if the another point exists after our current one, use it as pointb, otherwise use the first point
		Int32 NextIndex = PointIndex + 1 < (Int32)TransformedPoints_.size() ? PointIndex + 1 : 0;

		Point PointB = TransformedPoints_[NextIndex];

		Normal Edge = PointA - PointB;

		NormalList_[PointIndex] = MakePerpendicular(Edge);
	}
}
