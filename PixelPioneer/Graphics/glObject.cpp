#include "graphics.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLOjbect {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 orientation;
	GLModel* model;
};