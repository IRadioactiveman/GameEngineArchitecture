// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

// Include GLEW
#include <glew.h>

// Include SDL
#include <SDL.h>
#include <SDL_opengl.h>

// Include glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

// Include Bullet
#include <btBulletDynamicsCommon.h>

// Include loaders
#include "shader.hpp"

// Include controls
#include "controls.hpp"

// Include raycasting
#include "raycast.hpp"

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

int windowWidth = 800;
int windowHeight = 600;
std::string message;

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
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

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

	// Enable culling (backfaces of models are not shown)
	glEnable(GL_CULL_FACE);

	// Initialize variable for keyboard input
	const Uint8 *state;

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

	glm::vec3 pos1(3.0f, 4.0f, 5.0f);
	glm::vec3 pos2(-3.0f, -4.0f, -5.0f);
	glm::vec3 pos[] = { glm::vec3(0.0f, 0.0f, 0.0f), pos1, pos2 };
	// Model matrices, an identity matrix (model will be at the origin) and some variations without rotation
	glm::mat4 model1 = glm::mat4(1.0f);
	//glm::mat4 model2 = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec3(13.0f, 15.0f, 22.0f));
	//glm::mat4 model3 = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)), glm::vec3(17.0f, 23.0f, 35.0f));
	glm::mat4 model2 = glm::translate(glm::mat4(1.0f), pos1);
	glm::mat4 model3 = glm::translate(glm::mat4(1.0f), pos2);

	/*
	// Initialize Bullet. This strictly follows http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World, 
	// even though we won't use most of this stuff.

   // Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	std::vector<btRigidBody*> rigidbodies;

	// In this example, all monkeys will use the same collision shape : 
	// A box of 2m*2m*2m (1.0 is the half-extent !)
	btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < 3; i++) {

		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
			btQuaternion(pos[0].x, pos[0].y, pos[0].z, 0.0f),
			btVector3(pos[i].x, pos[i].y, pos[i].z)
		));

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,                  // mass, in kg. 0 -> Static object, will never move.
			motionstate,
			boxCollisionShape,  // collision shape of body
			btVector3(0, 0, 0)    // local inertia
		);
		btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);

		rigidbodies.push_back(rigidBody);
		dynamicsWorld->addRigidBody(rigidBody);

		// Small hack : store the mesh's index "i" in Bullet's User Pointer.
		// Will be used to know which object is picked. 
		// A real program would probably pass a "MyGameObjectPointer" instead.
		rigidBody->setUserPointer((void*)i);
	}
	*/

	// main loop
	do {
		/*
		// Instantiate cvariables for bullet
		btVector3 p0 = rigidbodies[0]->getCenterOfMassPosition();
		glm::vec3 v0 = pos[0];
		*/

		// Check keyboard state
		state = SDL_GetKeyboardState(NULL);

		//SDL_ShowCursor(SDL_DISABLE);

		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use controls to manipulate matrices and get them
		computeMatricesFromInput(window, windowWidth, windowHeight, state);
		glm::mat4 projectionMatrix = getProjectionMatrix();
		glm::mat4 viewMatrix = getViewMatrix();

		// MVP matrices for all models
		glm::mat4 mvp1 = projectionMatrix * viewMatrix * model1;
		glm::mat4 mvp2 = projectionMatrix * viewMatrix* model2;
		glm::mat4 mvp3 = projectionMatrix * viewMatrix * model3;

		// Array to draw all of them
		glm::mat4 mvps[] = { mvp1, mvp2, mvp3 };
		glm::mat4 models[] = { model1, model2, model3 };

		/*
		// Checking collision via bullet
		int x, y;
		if (SDL_GetMouseState(&x, &y) && SDL_BUTTON(1)) {

			glm::vec3 rayOrigin;
			glm::vec3 rayDirection;

			screenToWorld(
				windowWidth/2,
				windowHeight/2,
				windowWidth,
				windowHeight,
				viewMatrix,
				projectionMatrix,
				rayOrigin,
				rayDirection
			);

			printf("%f, %f, %f \n", rayOrigin.x, rayOrigin.y, rayOrigin.z);
			printf("%f, %f, %f \n", rayDirection.x, rayDirection.y, rayDirection.z);

			glm::vec3 outEnd = rayOrigin + rayDirection * 1000.0f;

			btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(outEnd.x, outEnd.y, outEnd.z), btVector3(outEnd.x, outEnd.y, outEnd.z));
			dynamicsWorld->rayTest(btVector3(rayOrigin.x, rayOrigin.y, rayOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z), RayCallback);
			if (RayCallback.hasHit()) {
				std::ostringstream oss;
				oss << "mesh " << (size_t)RayCallback.m_collisionObject->getUserPointer();
				message = oss.str();
			}
			else {
				message = "background";
			}

			std::printf("%s\n", message.c_str());

		}*/


		// Raycasting
		int x, y;
		if (SDL_GetMouseState(&x, &y) && SDL_BUTTON(1)) {

			// Set up variables to store to
			glm::vec3 rayOrigin;
			glm::vec3 rayDirection;
			
			// Get world coordinates from screen
			screenToWorld(
				x,
				y,
				windowWidth,
				windowHeight,
				viewMatrix,
				projectionMatrix,
				rayOrigin,
				rayDirection
			);

			printf("Origin:		%f, %f, %f \n", rayOrigin.x, rayOrigin.y, rayOrigin.z);
			printf("Direction:	%f, %f, %f \n", rayDirection.x, rayDirection.y, rayDirection.z);
			
			// Again setting up variables
			float intersectionDistance;
			glm::vec3 aabbMin(-1.0f, -1.0f, -1.0f);
			glm::vec3 aabbMax(1.0f, 1.0f, 1.0f);

			// Actual testing
			if (testIntersection(
				rayOrigin,
				rayDirection,
				aabbMin,
				aabbMax,
				models[0],
				intersectionDistance
			)) {
				printf("Distance:	%f\n", intersectionDistance);
			}
			else {
				printf("background\n");
			}
		}

		// Drawing
		for (int i = 0; i < 3; i++) {

			// Send mvp to vertex shader
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvps[i][0][0]);

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
				color,				// must match shader
				3,                  // size, number of vertices
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride, data between vertices, e.g. attributes
				(void*)0            // array buffer offset in the beginning
			);

			// Draw triangle
			glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

			glDisableVertexAttribArray(position);
			glDisableVertexAttribArray(color);

		}

		// Check wether top right x is presed
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}

		SDL_GL_SwapWindow(window);

	} while (!state[SDL_SCANCODE_ESCAPE]);

	// OpenGL cleanup
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderID);

	SDL_ShowCursor(SDL_ENABLE);
	SDL_GL_DeleteContext(context);
	SDL_Quit();

	/*
	// Bullet cleanup
	for (int i = 0; i < rigidbodies.size(); i++) {
		dynamicsWorld->removeRigidBody(rigidbodies[i]);
		delete rigidbodies[i]->getMotionState();
		delete rigidbodies[i];
	}
	delete boxCollisionShape;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
	*/
	
	return 0;
}