#pragma once

class MeshCollider
{
public:

	explicit MeshCollider(const std::vector<Vector2f>& VertArray);

	~MeshCollider();

	void UpdatMeshCollider(const Transform& ObjectTransform);

	Vector2f GetNormal(Int32 Index) const;

	Int32 GetNormalListSize() const;

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

