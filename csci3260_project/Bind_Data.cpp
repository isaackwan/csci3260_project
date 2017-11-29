#include "Bind_Data.h"
#include "Loading_func.h"

int drawEarthSize = 0;
int drawAeroplaneSize = 0;

GLuint earthVao, aeroplaneVao, skyboxVao;

void bindEarth(const char * path)
{
	//Earth data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n);

	//sending earth data
	glGenVertexArrays(1, &earthVao);
	glBindVertexArray(earthVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawEarthSize = vao5_v.size();
}

void bindAeroplane(const char * path)
{
	//Earth data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n);

	//sending earth data
	glGenVertexArrays(1, &aeroplaneVao);
	glBindVertexArray(aeroplaneVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawAeroplaneSize = vao5_v.size();
}


void bindSkybox()
{
	glGenVertexArrays(1, &skyboxVao);
	glBindVertexArray(skyboxVao);

	GLuint skyboxVbo[2];
	glGenBuffers(2, skyboxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo[0]);
	const float box_vertices[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

	GLuint box_elements[] = {
		1, 2, 3,
		1, 2, 0,
		4, 7, 6,
		4, 7, 5,
		1, 6, 7,
		1, 6, 0,
		3, 4, 2,
		3, 4, 5,
		3, 7, 1,
		3, 7, 5,
		0, 4, 6,
		0, 6, 2,
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxVbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_elements), box_elements, GL_STATIC_DRAW);

	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	// uv coordinates for texture mapping
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	std::cout << skyboxVbo[0] << ", " << skyboxVbo[1] << std::endl;
}