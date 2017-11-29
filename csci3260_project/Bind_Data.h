#ifndef  __BIND_DATA_H__
#define  __BIND_DATA_H__

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>

void bindEarth(const char * path);
void bindMars(const char * path);
void bindAeroplane(const char * path);
void bindMoon(const char * path);
void bindSkybox();

/*** END OF FILE ***/

#endif /* __BIND_DATA_H__ */