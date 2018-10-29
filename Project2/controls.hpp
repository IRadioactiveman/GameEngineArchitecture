#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInput(SDL_Window* win, int width, int height, const Uint8* state);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif