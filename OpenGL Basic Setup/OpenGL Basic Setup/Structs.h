#pragma once
#include <vector>

struct VertexColor {
	glm::vec3 Position, Color;
	glm::vec2 TexCoords;
};

struct VertexTexture
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
};

struct Collision {
	glm::vec3 position, size;

	Collision(glm::vec3 pos, glm::vec3 sz) : position(pos), size(sz) {}
};

struct AABB {
	glm::vec3 min;
	glm::vec3 max;
};

inline AABB CalculateAABB(const Collision& object) {
	AABB box;
	box.min = object.position - object.size * 0.5f;
	box.max = object.position + object.size * 0.5f;
	return box;
}

inline bool CheckAABBCollision(const AABB& a, const AABB& b) {
	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z);
}