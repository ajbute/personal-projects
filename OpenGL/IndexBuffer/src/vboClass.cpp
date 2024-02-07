#include "vboClass/vboClass.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
vboClass::vboClass(GLfloat* vertices, GLsizeiptr size) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBO
void vboClass::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void vboClass::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void vboClass::Delete() {
	glDeleteBuffers(1, &ID);
}