#include "vaoClass/vaoClass.h"

// Constructor that generates a VAO ID
vaoClass::vaoClass() {
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void vaoClass::LinkVBO(vboClass& VBO, GLuint layout) {
	VBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void vaoClass::Bind() {
	glBindVertexArray(ID);
}

// Unbinds the VAO
void vaoClass::Unbind() {
	glBindVertexArray(0);
}

// Deletes the VAO
void vaoClass::Delete() {
	glDeleteVertexArrays(1, &ID);
}