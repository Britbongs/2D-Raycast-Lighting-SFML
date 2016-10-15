#pragma once

class MeshCollider
{
public:

	MeshCollider();

	MeshCollider(const std::vector<Vector2f>& VertArray);

	~MeshCollider();

	void UpdatMeshCollider(const Transform& ObjectTransform);

	Vector2f GetNormal(Int32 Index) const;

	Vector2f GetTransformedPoint(Int32 Index) const;

	Int32 GetPointCount() const;

	Int32 GetNormalListSize() const;

	void SetPointsList(const std::vector<Vector2f>& VertArray);

private:

	void InitialisePoints(const std::vector<Vector2f>& VertArray);

	void UpdateTransformedPoints(const Transform& ObjectTransform);

	void UpdateNormals();

	//using declarations to differentiate axes from points
	using Point = Vector2f;
	using Normal = Vector2f;

	//Local space coordinates 
	std::vector<Point> LocalPoints_;

	//Global space coordinates
	std::vector<Point> TransformedPoints_;

	std::vector<Normal> NormalList_;

};

