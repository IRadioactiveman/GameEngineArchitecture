#ifndef RAYCAST_HPP
#define RAYCAST_HPP

void screenToWorld(
	int mouseX,
	int mouseY,
	int screenWidth,
	int screenHeight,
	glm::mat4 viewMatrix,
	glm::mat4 projectionMatrix,
	glm::vec3& outOrigin,
	glm::vec3& outDirection
);

bool testIntersection(
	glm::vec3 rayOrigin,
	glm::vec3 rayDirection,
	glm::vec3 aabbMin,
	glm::vec3 aabbMax,
	glm::mat4 modelMatrix,
	float& intersectionDistance
);

#endif