#ifndef  __LOADING_FUNC_H__
#define  __LOADING_FUNC_H__

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>

// load OBJ model file data
bool loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals);
GLuint loadBMP2Texture(const char * imagepath);

/*** END OF FILE ***/

#endif /* __LOADING_FUNC_H__ */