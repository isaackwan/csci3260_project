#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glui\glui.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "shader_install.h"
#include "Loading_func.h"
#include "Bind_Data.h"

using namespace std;
using glm::vec3;
using glm::mat4;
// ============================= //
// window size
int Win_w, Win_h;
float camera_fov = 45.0;

int  mainWindowID;
GLint programID;
GLUI* glui;

GLUI_RadioGroup* viewpoint_group;

GLuint TextureEarth;
GLuint TextureMars;
GLuint TextureAeroplane;
GLuint TextureSkybox;
GLuint TextureMoon;

extern GLuint earthVao;
extern GLuint marsVao;
extern GLuint aeroplaneVao;
extern GLuint moonVao;
extern GLuint skyboxVao;

extern int drawEarthSize;
extern int drawMarsSize;
extern int drawAeroplaneSize;
extern int drawMoonSize;

// view matrix
glm::mat4 common_viewM;
glm::mat4 common_projection;

int viewpoint_switch;

float earth_innRot_Degree = 0.0f;

// ============================= //
float radius = 42.0f;
float initViewHorizontal = -90.0f;
float initViewVertical = -90.0f;

float cameraX = 0;
float cameraY = 0;
float cameraZ = radius;

float viewRotateDegree[3] = { 0.0f, 0.0f, 0.0f };

float a_brightness = 1.0f;
float d_brightness = 0.5f;
float s_brightness = 0.7f;

GLuint aeroplaneRotatePosition;

// Struct for the control panel
struct control_s {
	int fog_enabled;
	int fog_color;
};

control_s control = {};

void Mouse_Wheel_Func(int button, int state, int x, int y)
{
	if ((button == 3) || (button == 4))
	{
		if (state == GLUT_UP) return;
		if (button == 3)
		{
			radius -= 1.0f;
			cameraX = radius* cos(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraY = radius* cos(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraZ = radius* sin(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
		}
		else
		{
			radius += 1.0f;
			cameraX = radius* cos(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraY = radius* cos(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraZ = radius* sin(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
		}
	}
}


GLint mouseState_x = 0;
GLint mouseState_y = 0;
void PassiveMouse(int x, int y)
{
	mouseState_x = x - 450;
	mouseState_y = y - 450;
	glutPostRedisplay();

}

void LoadAllTextures()
{
	TextureEarth = loadBMP2Texture("texture/earth.bmp");
	TextureMars = loadBMP2Texture("texture/glass_a.bmp");
	TextureAeroplane = loadBMP2Texture("texture/helicopter.bmp");
	TextureMoon = loadBMP2Texture("texture/grass.bmp");
	TextureSkybox = loadBMP2Texture("texture/white.bmp");
}

void sendDataToOpenGL()
{
	//Load objects and bind to VAO & VBO
	bindEarth("model_obj/planet.obj");
	bindMars("model_obj/planet.obj");
	bindAeroplane("model_obj/Arc170.obj");
	bindMoon("model_obj/planet.obj");
	bindSkybox();
	// load all textures
	LoadAllTextures();
}

float zLightPos = 0.0f;
float yLightPos = 20.0f;
void set_lighting()
{
	glUseProgram(programID);

	// ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	glm::vec3 ambientLight(a_brightness, a_brightness, a_brightness);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	// diffusion
	GLint kd = glGetUniformLocation(programID, "coefficient_d");
	glm::vec3 vec_kd(d_brightness, d_brightness, d_brightness);
	glUniform3fv(kd, 1, &vec_kd[0]);
	// specular
	GLint ks = glGetUniformLocation(programID, "coefficient_s");
	glm::vec3 vec_ks(s_brightness, s_brightness, s_brightness);
	glUniform3fv(ks, 1, &vec_ks[0]);
	// eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	vec3 eyePosition(cameraX, cameraY, cameraZ);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glm::vec3 lightPosition(0.0, yLightPos, zLightPos);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// light color
	GLint lightColorUniformLocation = glGetUniformLocation(programID, "lightColor");
	glm::vec4 lightColor(0.0, 0.7, 1.0, 0.0);
	glUniform4fv(lightColorUniformLocation, 1, &lightColor[0]);
}

void drawEarth(void)
{
	//return;
	//earth
	GLfloat scale_fact = 3.0f;

	glUseProgram(programID);

	glBindVertexArray(earthVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(earth_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureEarth);
	glUniform1i(TextureID, 0);
	glDrawArrays(GL_TRIANGLES, 0, drawEarthSize);
	glActiveTexture(GL_TEXTURE1);
}

void drawMars(void)
{
	//return;

	glUseProgram(programID);

	glBindVertexArray(marsVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(1.9f));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(earth_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(18.0f, -18.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureMars);
	glUniform1i(TextureID, 0);
	glDrawArrays(GL_TRIANGLES, 0, drawMarsSize);
	glActiveTexture(GL_TEXTURE1);
}

void drawAeroplane(void)
{
	//return;

	glUseProgram(programID);

	glBindVertexArray(aeroplaneVao);
	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(0.0083f));
	glm::mat4 rotate1 = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0, 1, 0));
	glm::mat4 rotate3 = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0, 0, 1));
	glm::mat4 translate1 = glm::translate(glm::mat4(), glm::vec3(13.6f, 0, 0));
	glm::mat4 rotate2 = glm::rotate(glm::mat4(), aeroplaneRotatePosition / 250.0f, glm::vec3(0, 0, 1));
	glm::mat4 translate2 = glm::translate(glm::mat4(), glm::vec3(0, 5.0f, 0));
	glm::mat4 Model = translate2 * rotate2 * translate1 * rotate3 * rotate1 * scale;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureAeroplane);
	glUniform1i(TextureID, 1);
	glDrawArrays(GL_TRIANGLES, 0, drawAeroplaneSize);
	glActiveTexture(GL_TEXTURE2);
}

void drawMoon(void)
{
	//return;

	glUseProgram(programID);

	glBindVertexArray(moonVao);
	//glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(0.0083f));
	/*glm::mat4 rotate1 = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0, 1, 0));
	glm::mat4 rotate3 = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0, 0, 1));
	glm::mat4 translate1 = glm::translate(glm::mat4(), glm::vec3(13.6f, 0, 0));
	glm::mat4 rotate2 = glm::rotate(glm::mat4(), aeroplaneRotatePosition / 250.0f, glm::vec3(0, 0, 1));
	glm::mat4 translate2 = glm::translate(glm::mat4(), glm::vec3(0, 5.0f, 0));
	glm::mat4 Model = translate2 * rotate2 * translate1 * rotate3 * rotate1;*/

	glm::mat4 scale1 = glm::mat4(0.7f);
	glm::mat4 rotate1 = glm::rotate(glm::mat4(1.0f), glm::radians(earth_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 translate1 = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, 0.0f, 0.0f));
	glm::mat4 rotate2 = glm::rotate(glm::mat4(), (aeroplaneRotatePosition + 8) / -250.0f, glm::vec3(0, 0, 1));
	glm::mat4 translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f));
	glm::mat4 Model = translate2 * rotate2 * translate1 * rotate1 * scale1;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureMoon);
	glUniform1i(TextureID, 3);
	glDrawArrays(GL_TRIANGLES, 0, drawMoonSize);
	glActiveTexture(GL_TEXTURE4);
}

void drawSkybox(void)
{
	//return;

	glUseProgram(programID);

	glBindVertexArray(skyboxVao);

	glm::mat4 Model = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(4.0f));

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureSkybox);
	glUniform1i(TextureID, 2);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE3);
}

void paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// ================================ //
	// view matrix
	switch (viewpoint_switch) {
	case 1: // Right
		common_viewM = glm::lookAt(glm::vec3(cameraZ, cameraY, cameraX), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case 2: // Bottom
		common_viewM = glm::lookAt(glm::vec3(cameraX, cameraZ, 1+cameraY), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case 0:
		common_viewM = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(7, -5, 0), glm::vec3(0, 1, 0));
		break;
	case 3:
		common_viewM = glm::lookAt(glm::vec3(cameraX + float(mouseState_x) / 20, cameraY - float(mouseState_y) / 20, cameraZ), glm::vec3(7, -5, 0), glm::vec3(0, 1, 0));
		break;
	default:
		std::cerr << "Unknown viewpoint_switch value: " << viewpoint_switch << ". Not changing common_viewM." << std::endl;
	}
	
	// projection matrix
	common_projection = glm::perspective(camera_fov, 1.0f, 0.1f, 200.0f);
	
	//=== draw ===//
	// set lighting parameters
	set_lighting();
	// draw earth
	drawEarth();
	drawMars();
	drawAeroplane();
	drawMoon();
	drawSkybox();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Shaders_Installer()
{
	programID = installShaders("shader/Common.vs", "shader/Common.frag");

	if (!checkProgramStatus(programID))
		return;
}

void initializedGL(void)
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// install all shaders
	Shaders_Installer();
	// load required obj and textures
	sendDataToOpenGL();
}

void timerFunction(int id)
{
	earth_innRot_Degree += 0.3;
	
	glutPostRedisplay();
	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
}

void keyboardFunc(unsigned char key, int x, int y) {
	/* This hotkeys are used for debugging only */
	if (key == 'q') {
		cameraX += 1.0f;
	}
	else if (key == 'z') {
		cameraX -= 1.0f;
	}
	else if (key == 'w') {
		cameraY += 1.0f;
	}
	else if (key == 'x') {
		cameraY -= 1.0f;
	}
	else if (key == 'e') {
		cameraZ += 1.0f;
	}
	else if (key == 'c') {
		cameraZ -= 1.0f;
	} /* Spec-required keyboard operations start here */
	else if (key == 'a') {
		viewpoint_group->set_int_val(0);
	}
	else if (key == 's') {
		viewpoint_group->set_int_val(1);
	}
	else if (key == 'd') {
		viewpoint_group->set_int_val(2);
	}
	std::cout << "cameraX: " << cameraX << ", cameraY: " << cameraY << ", cameraZ: " << cameraZ << std::endl;

	//enable move camera by mouse by space bar
	if (key == 32)
	{
		viewpoint_switch = 3;
	}
}

void myGlutReshape(int width, int height) {
	GLUI_Master.auto_set_viewport();
}

void idleFunction() {
	aeroplaneRotatePosition += 1;
	glutPostRedisplay();
}

void initializeGlui(GLUI* glui) {
	glui->add_separator();
	glui->add_statictext("i-Navigation@CUHK");
	glui->add_separator();

	GLUI_Panel *render_control = new GLUI_Rollout(glui, "Render Control", true);

	GLUI_Panel *fog_panel = glui->add_panel_to_panel(render_control, "Fog");
	glui->add_checkbox_to_panel(render_control, "Fog On/Off", &control.fog_enabled, 1);
	GLUI_RadioGroup *fog_colour = glui->add_radiogroup_to_panel(fog_panel, &control.fog_color, 0);
	glui->add_radiobutton_to_group(fog_colour, "Ivory");
	glui->add_radiobutton_to_group(fog_colour, "Ocean Blue");

	GLUI_Panel *move_normal_map = glui->add_panel_to_panel(render_control, "Move/Normal/Map");
	GLUI_Panel *viewpoint = glui->add_panel_to_panel(render_control, "Viewpoint");
	viewpoint_group = glui->add_radiogroup_to_panel(viewpoint, &viewpoint_switch);
	glui->add_radiobutton_to_group(viewpoint_group, "Default");
	glui->add_radiobutton_to_group(viewpoint_group, "Right");
	glui->add_radiobutton_to_group(viewpoint_group, "Bottom");
	glui->add_radiobutton_to_group(viewpoint_group, "Mouse Mode");
}

int main(int argc, char *argv[])
{
	/*Initialization of GLUT library*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(940, 800);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	mainWindowID = glutCreateWindow("i-Navigation");

	// initialize openGL
	initializedGL();
	// draw
	glutDisplayFunc(paintGL);

	GLUI_Master.set_glutMouseFunc(Mouse_Wheel_Func);
	GLUI_Master.set_glutKeyboardFunc(&keyboardFunc);
	GLUI_Master.set_glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
	GLUI_Master.set_glutPassiveMotionFunc(PassiveMouse);

	// GLUI
	GLUI_Master.set_glutIdleFunc(&idleFunction);
	GLUI_Master.set_glutReshapeFunc(&myGlutReshape);
	glui = GLUI_Master.create_glui_subwindow(mainWindowID, GLUI_SUBWINDOW_RIGHT);
	glui->set_main_gfx_window(mainWindowID);
	initializeGlui(glui);

	/*Enter the GLUT event processing loop which never returns.*/
	glutMainLoop();

	return -1;
}