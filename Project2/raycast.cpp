// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <glew.h>

// Include SDL
#include <SDL.h>
#include <SDL_opengl.h>

// Include glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

#include "raycast.hpp"

// Translate screen coordinates to 3D coordinates
void screenToWorld(
	int mouseX,
	int mouseY,
	int screenWidth,
	int screenHeight,
	glm::mat4 viewMatrix,
	glm::mat4 projectionMatrix,
	glm::vec3& outOrigin,
	glm::vec3& outDirection
) {
	// Start point of ray
	glm::vec4 rayStart(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
		-1.0f,
		1.0f
	);

	// Endpoint of ray
	glm::vec4 rayEnd(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
		0.0f,
		1.0f
	);

	// Invert view and projection matrix (reverse matrix multiplications)
	glm::mat4 invProjMatrix = glm::inverse(projectionMatrix);
	glm::mat4 invViewMatrix = glm::inverse(viewMatrix);

	glm::vec4 rayStartCamera = invProjMatrix * rayStart;
	rayStartCamera /= rayStartCamera.w;

	glm::vec4 rayStartWorld = invViewMatrix * rayStartCamera;
	rayStartWorld /= rayStartWorld.w;

	glm::vec4 rayEndCamera = invProjMatrix * rayEnd;
	rayEndCamera /= rayEndCamera.w;

	glm::vec4 rayEndWorld = invProjMatrix * rayEndCamera;
	rayEndWorld /= rayEndWorld.w;

	glm::vec3 rayDirWorld = (rayEndWorld - rayStartWorld);
	rayDirWorld = glm::normalize(rayDirWorld);

	outOrigin = glm::vec3(rayStartWorld);
	outDirection = glm::vec3(rayDirWorld);
}

// OBB intersection test
bool testIntersection(
	glm::vec3 rayOrigin,
	glm::vec3 rayDirection,
	glm::vec3 aabbMin,			// smallest x,y,z coordionates of bounding box
	glm::vec3 aabbMax,			// biggest x,y,z coordionates of bounding box
	glm::mat4 modelMatrix,		// model matrix to transform bounding box
	float& intersectionDistance	// distance between camera and object
) {
	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
	glm::vec3 delta(OBBposition - rayOrigin);

	{
		glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(rayDirection, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + aabbMin.x) / f; // Intersection with the "left" plane
			float t2 = (e + aabbMax.x) / f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2
			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + aabbMin.x > 0.0f || -e + aabbMax.x < 0.0f)
				return false;
		}
	}

	{
		glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(rayDirection, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabbMin.y) / f;
			float t2 = (e + aabbMax.y) / f;
			
			if (t1 > t2) {float w = t1; t1 = t2; t2 = w;}
			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMax < tMin)
				return false;

		}
		else {
			if (-e + aabbMin.y > 0.0f || -e + aabbMax.y < 0.0f)
				return false;
		}
	}

	{
		glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(rayDirection, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabbMin.z) / f;
			float t2 = (e + aabbMax.z) / f;

			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }
			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMax < tMin)
				return false;

		}
		else {
			if (-e + aabbMin.z > 0.0f || -e + aabbMax.z < 0.0f)
				return false;
		}
	}

	intersectionDistance = tMin;
	return true;
}