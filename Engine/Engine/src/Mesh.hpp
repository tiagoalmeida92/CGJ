#pragma once

#include "GL\glew.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

typedef struct {
	GLfloat x, y, z;
} Vertex;

typedef struct {
	GLfloat u, v;
} Texcoord;

typedef struct {
	GLfloat nx, ny, nz;
} Normal;

class Mesh {
public:
	const static GLuint VERTICES = 0;
	const static GLuint TEXCOORDS = 1;
	const static GLuint NORMALS = 2;

	std::vector<Vertex> Vertices;
	std::vector<Texcoord> Texcoords;
	std::vector<Normal> Normals;
	bool TexcoordsLoaded, NormalsLoaded;
	
	Mesh();
	Mesh(std::string filepath);

	void Mesh::draw();
	void destroy();

private:
	GLuint vao_id;
	GLuint vbo_vertices_id;
	GLuint vbo_normals_id;
	GLuint vbo_texcoords_id;

	std::vector<Vertex> vertexData;
	std::vector<Texcoord> texcoordData;
	std::vector<Normal> normalData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);
	void processMeshData();
	void freeMeshData();
	void Mesh::createBufferObjects();
};

