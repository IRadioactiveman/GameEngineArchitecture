// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include SDL
#include <SDL.h>
#include <SDL_opengl.h>

// Include glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

// Create view and projection matrix to return
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

// Getter methods for matrices
glm::mat4 getViewMatrix() {
	return viewMatrix;
}

glm::mat4 getProjectionMatrix() {
	return projectionMatrix;
}

// Initial +z
glm::vec3 pos = glm::vec3(0,0,5);
// Angle towards -z
float horizontalAngle = 3.14f;
// Initial horizontal angle
float verticalAngle = 0.0f;
// Initial field of view
float fov = 45.0f;

// Move speed units/sec
float speed = 0.05f;
float mouseSpeed = 0.005f;


void computeMatricesFromInput(SDL_Window* win, int width, int height, const Uint8* state) {

	static double lastTime = SDL_GetTicks();

	double currentTime = SDL_GetTicks();
	float deltaTime = float(currentTime - lastTime);

	int xPos, yPos;
	SDL_GetMouseState(&xPos, &yPos);

	SDL_WarpMouseInWindow(win, width/2, height/2);

	horizontalAngle += mouseSpeed * float(width/2 - xPos);
	verticalAngle += mouseSpeed * float(height/2 - yPos);

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
		pos += direction * deltaTime * speed;
	}

	if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
		pos -= direction * deltaTime * speed;
	}

	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
		pos += right * deltaTime * speed;
	}

	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
		pos -= right * deltaTime * speed;
	}

	projectionMatrix = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

	viewMatrix = glm::lookAt(
		pos,				// Camera is here
		pos + direction,	// and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	lastTime = currentTime;
}