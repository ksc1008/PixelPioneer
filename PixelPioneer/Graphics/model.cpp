#include "graphics.h"
#include "model.h"
#include "../debug.h"
#define DEBUG

GLModel::GLModel(GLMaterial* material, mesh* mesh)
{
	setMesh(mesh);
	setMaterial(material);

    Debugger::getInstance()->writeLine(m_mesh->verticesCount);
}

void GLModel::bindBuffer() {
    glActiveTexture(GL_TEXTURE0);
    m_material->bind();
    glBindVertexArray(VAO);
}

void GLModel::setMaterial(GLMaterial* mat) {
	m_material = mat;
}

void GLModel::setMesh(mesh* mesh) {
	m_mesh = mesh;


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh->vertices[0]) * mesh->verticesCount * 5, mesh->vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void GLModel::draw()
{
    glDrawArrays(GL_TRIANGLES, 0, m_mesh->verticesCount);
}
