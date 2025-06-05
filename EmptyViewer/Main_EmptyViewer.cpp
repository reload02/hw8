// main_Phong_Shader.cpp
#include "mesh_loader.h"
//#include "frame.h"

#include <Windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>



#define M_PI 3.14159265358979323846

using namespace glm;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //start_timing(); // Ÿ�̸� ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // ���� ���ǿ� ���� ���� Perspective ���� (OpenGL�� ��ǥ �������� ��ȣ ������)
    float l = -0.1f, r = 0.1f, b = -0.1f, t = 0.1f, n = 0.1f, f = 1000.0f;
    GLfloat proj[16] = {
        2 * n / (r - l), 0,           0,                          0,
        0,         2 * n / (t - b),   0,                          0,
        (r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n),            -1,
        0,         0,          -2 * f * n / (f - n),                0
    };
    glLoadMatrixf(proj);
    // ��ȯ ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0); // ī�޶� ����
    glTranslatef(0.1f, -1.0f, -1.5f);         // ��ġ �̵�
    glScalef(10.0f, 10.0f, 10.0f);            // ������
     // ���� ���� (fixed pipeline)
    GLfloat norm =glm::sqrt(3.0f); // ��((-1)^2 + (-1)^2 + (-1)^2)
    GLfloat lightDir[] = { -1.0f, -1.0f ,- 1.0f, 0.0f };

    GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat globalAmbient2[] = { 0.6f, 0.6f, 0.6f, 1.0f };


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
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

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);


    // ������ (��� ���)
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

    //// FPS ����
    //float t = stop_timing();
    //gTotalFrames++;
    //gTotalTimeElapsed += t;
    //float fps = gTotalFrames / gTotalTimeElapsed;

    //char title[128];
    //sprintf("Bunny Viewer:  FPS");
    glutSetWindowTitle("title");

    glutSwapBuffers();
    glutPostRedisplay(); // ���� ������ ��û
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("Bunny Viewer");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    load_mesh("bunny.obj");       // bunny.obj �ε�

    glutDisplayFunc(display);     // ���÷��� �ݹ� ����
    glutMainLoop();               // ������ ���� ����

    return 0;
}
