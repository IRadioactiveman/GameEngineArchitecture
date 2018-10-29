// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include loaders
#include "shader.hpp"

// Include GLEW
#include <glew.h>
//using namespace shader;

// Include SDL
#include <SDL.h>
#include <SDL_opengl.h>

// Include glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

// Basic triangle
static const GLfloat shapeToDraw[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

// Vertex colors
static const GLfloat colorsToDraw[] = {
   1.0f, 0.0f, 0.0f,
   0.0f, 1.0f, 0.0f,
   0.0f, 0.0f, 1.0f,
};

static const GLfloat cube[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
};

static const GLfloat cubeColors[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f
};

int main(int argc, char *argv[])
{
	// Initialize SDL video mode
	SDL_Init(SDL_INIT_VIDEO);

	// Set SDL attributes
	// Set OpenGL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Set OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// Enable multisampling in SDL
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// Create window
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

	// Create SDL context
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Create window event for closing window
	SDL_Event windowEvent;

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		return -1;
	}

	// Enable multisampling in OpenGL
	glEnable(GL_MULTISAMPLE);

	// Enable depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load shaders
	GLuint shaderID = LoadShaders("simpleVertexShader.txt", "simpleFragmentShader.txt");
	// Use shaders
	glUseProgram(shaderID);
	
	// Get to use the MVP matrix in the vertex shader
	GLuint mvpID = glGetUniformLocation(shaderID, "MVP");

	// Projection matrix; 45 degree field of view, 4:3 ratio, display range 0.1 <-> 100
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// View matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 model = glm::mat4(1.0f);

	// MVP matrix
	glm::mat4 mvp = projection * view * model;

	// Create VBO
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// Create VBO for colors
	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);

	// "main" loop
	do {

		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Send mvp to vertex shader
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

		// Find and enable vertex positions
		GLint position = glGetAttribLocation(shaderID, "vertexPosition");
		glEnableVertexAttribArray(position);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		// Define attributes
		glVertexAttribPointer(
			position,          // must match shader
			3,                  // size, number of vertices
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride, data between vertices, e.g. attributes
			(void*)0            // array buffer offset in the beginning
		);

		GLint color = glGetAttribLocation(shaderID, "vertexColor");
		glEnableVertexAttribArray(color);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		// Define attributes
		glVertexAttribPointer(
			color,          // must match shader
			3,                  // size, number of vertices
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride, data between vertices, e.g. attributes
			(void*)0            // array buffer offset in the beginning
		);

		// Draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3*12);

		glDisableVertexAttribArray(position);
		glDisableVertexAttribArray(color);

		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}

		SDL_GL_SwapWindow(window);

	} while (true);

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderID);

	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}