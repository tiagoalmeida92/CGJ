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
	std::vector<Vertex> Vertices;
	std::vector<Texcoord> Texcoords;
	std::vector<Normal> Normals;
	bool TexcoordsLoaded, NormalsLoaded;
	
	Mesh();
	Mesh(std::string filepath);

private:
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
};

