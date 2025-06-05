// main_Phong_Shader.cpp
#include "frame.h"
#include "mesh_loader.h"

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



GLuint vao, vbo, ebo;
GLsizei indexCount;

struct Vertex {
    GLfloat position[3];
    GLfloat normal[3];
};

void init_VAO_VBO() {
    std::vector<Vertex> vertexData;
    std::vector<GLuint> indexData;

    for (const Triangle& tri : gTriangles) {
        for (int i = 0; i < 3; ++i) {
            int idx = tri.indices[i];
            Vertex v;
            v.position[0] = gPositions[idx].x;
            v.position[1] = gPositions[idx].y;
            v.position[2] = gPositions[idx].z;
            v.normal[0] = gNormals[idx].x;
            v.normal[1] = gNormals[idx].y;
            v.normal[2] = gNormals[idx].z;
            vertexData.push_back(v);
            indexData.push_back(indexData.size());
        }
    }

    indexCount = static_cast<GLsizei>(indexData.size());

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);  // Unbind VAO
}


void display() {
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


    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // FPS 측정 및 출력
    float timeElapsed = stop_timing();
    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;

    char title[128];
    sprintf(title, "OpenGL Bunny (VAO): %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutSwapBuffers();
    glutPostRedisplay(); // 다음 프레임 요청

   
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("Bunny Viewer");

    glewInit();
    init_timer();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    load_mesh("bunny.obj");       // bunny.obj 로드
    init_VAO_VBO();
    glutDisplayFunc(display);     // 디스플레이 콜백 설정
    glutMainLoop();               // 렌더링 루프 시작

    return 0;
}
