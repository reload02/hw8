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

    load_mesh("bunny.modelobj");       // bunny.obj 로드
    init_VAO_VBO();
    glutDisplayFunc(display);     // 디스플레이 콜백 설정
    glutMainLoop();               // 렌더링 루프 시작

    return 0;
}
