#ifndef  __STATUSCHECKER_H__
#define  __STATUSCHECKER_H__

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>

// shader installation common checker
bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
// check shader
bool checkShaderStatus(GLuint shaderID);
// check program
bool checkProgramStatus(GLuint programID);
// read shader code
std::string readShaderCode(const char* fileName);
// compile and install shader
GLint installShaders(GLchar* vs_path, GLchar* fs_path);

/*** END OF FILE ***/

#endif /* __STATUSCHECKER_H__ */