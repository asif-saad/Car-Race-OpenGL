#pragma #ifndef simulation_h
#define simulation_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <string>
#include <iostream>


using namespace std;

class simulation {
public:
	glm::vec3 position;
	float rotation;
	float velocity;

};
