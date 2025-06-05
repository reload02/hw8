/*
	How to use this code:

	Call init_timer before starting rendering, i.e., before calling
	glutMainLoop. Then, make sure your display function is organized
	roughly as the example below.
*/
#include "frame.h"
#include "mesh_loader.h"

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


float  					gTotalTimeElapsed 	= 0;
int 					gTotalFrames		= 0;
GLuint 					gTimer;

void init_timer()
{
	glGenQueries(1, &gTimer);
}

void start_timing()
{
	glBeginQuery(GL_TIME_ELAPSED, gTimer);
}

float stop_timing()
{
	glEndQuery(GL_TIME_ELAPSED);
   
	GLint available = GL_FALSE;
    
    int safety_counter = 0;
    
    while (available == GL_FALSE ) {
        glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);
    }
	GLint result;
	glGetQueryObjectiv(gTimer, GL_QUERY_RESULT, &result);

	float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f);
    
	return timeElapsed;
}

/*
	Your display function should look roughly like the following.
*/
//void display()
//{
//	// TODO: Clear the screen and depth buffer.
//
// 	start_timing();
//
// 	// TODO: Draw the bunny.
//  	
//	float timeElapsed = stop_timing();
//  	gTotalFrames++;
//  	gTotalTimeElapsed += timeElapsed;
//  	float fps = gTotalFrames / gTotalTimeElapsed;
//  	char string[1024] = {0};
//  	sprintf(string, "OpenGL Bunny: %0.2f FPS", fps);
//  	glutSetWindowTitle(string);
//
//	glutPostRedisplay();
//  	glutSwapBuffers();
//}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    start_timing(); // 타이머 시작
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 문제 조건에 맞춘 수동 Perspective 설정 (OpenGL의 좌표 기준으로 부호 반전됨)
    float l = -0.1f, r = 0.1f, b = -0.1f, t = 0.1f, n = 0.1f, f = 1000.0f;
    GLfloat proj[16] = {
        2 * n / (r - l), 0,           0,                          0,
        0,         2 * n / (t - b),   0,                          0,
        (r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n),            -1,
        0,         0,          -2 * f * n / (f - n),                0
    };
    glLoadMatrixf(proj);
    // 변환 설정
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0); // 카메라 설정
    glTranslatef(0.1f, -1.0f, -1.5f);         // 위치 이동
    glScalef(10.0f, 10.0f, 10.0f);            // 스케일
    // 조명 설정 (fixed pipeline)
    GLfloat norm = glm::sqrt(3.0f); // √((-1)^2 + (-1)^2 + (-1)^2)
    GLfloat lightDir[] = { -1.0f, -1.0f ,-1.0f, 0.0f };
    GLfloat lightDir2[] = { 1.0f, 1.0f ,1.0f, 0.0f };

    GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat globalAmbient2[] = { 0.6f, 0.6f, 0.6f, 1.0f };


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir2);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat ka_kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, ka_kd);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ka_kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

    glEnable(GL_NORMALIZE);

    // 렌더링 (즉시 모드)
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < gTriangles.size(); i++) {
        int k0 = gTriangles[i].indices[0];
        int k1 = gTriangles[i].indices[1];
        int k2 = gTriangles[i].indices[2];

        glNormal3f(gNormals[k0].x, gNormals[k0].y, gNormals[k0].z);
        glVertex3f(gPositions[k0].x, gPositions[k0].y, gPositions[k0].z);

        glNormal3f(gNormals[k1].x, gNormals[k1].y, gNormals[k1].z);
        glVertex3f(gPositions[k1].x, gPositions[k1].y, gPositions[k1].z);

        glNormal3f(gNormals[k2].x, gNormals[k2].y, gNormals[k2].z);
        glVertex3f(gPositions[k2].x, gPositions[k2].y, gPositions[k2].z);
    }
    glEnd();

    float timeElapsed = stop_timing();
      	gTotalFrames++;
      	gTotalTimeElapsed += timeElapsed;
      	float fps = gTotalFrames / gTotalTimeElapsed;
      	char string[1024] = {0};
      	sprintf(string, "OpenGL Bunny: %0.2f FPS", fps);
      	glutSetWindowTitle(string);

    glutSwapBuffers();
    glutPostRedisplay(); // 다음 프레임 요청
}