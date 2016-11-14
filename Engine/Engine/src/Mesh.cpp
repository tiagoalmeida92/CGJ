#include "Mesh.hpp"

Mesh::Mesh(){}

Mesh::Mesh(std::string filename) {
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
	TexcoordsLoaded = (texcoordIdx.size() > 0);
	NormalsLoaded = (normalIdx.size() > 0);
	processMeshData();
	freeMeshData();
	createBufferObjects();
}

void Mesh::draw() {
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
	glBindVertexArray(0);
}

void Mesh::createBufferObjects()
{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	{
		glGenBuffers(1, &vbo_vertices_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_id);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (TexcoordsLoaded)
	{
		glGenBuffers(1, &vbo_texcoords_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords_id);
		glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Texcoord), &Texcoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEXCOORDS);
		glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);
	}
	if (NormalsLoaded)
	{
		glGenBuffers(1, &vbo_normals_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_id);
		glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMALS);
		glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::destroy() {
	glBindVertexArray(vao_id);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteBuffers(1, &vbo_vertices_id);
	glDeleteBuffers(1, &vbo_texcoords_id);
	glDeleteBuffers(1, &vbo_normals_id);
	glDeleteVertexArrays(1, &vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Mesh::parseVertex(std::stringstream& sin)
{
	Vertex v;
	sin >> v.x >> v.y >> v.z;
	vertexData.push_back(v);
}

void Mesh::parseTexcoord(std::stringstream& sin)
{
	Texcoord t;
	sin >> t.u >> t.v;
	texcoordData.push_back(t);
}

void Mesh::parseNormal(std::stringstream& sin)
{
	Normal n;
	sin >> n.nx >> n.ny >> n.nz;
	normalData.push_back(n);
}

void Mesh::parseFace(std::stringstream& sin)
{
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(sin, token, '/');
		if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) normalIdx.push_back(std::stoi(token));
	}
}

void Mesh::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}



void Mesh::processMeshData()
{
	for (unsigned int i = 0; i < vertexIdx.size(); i++) {
		unsigned int vi = vertexIdx[i];
		Vertex v = vertexData[vi - 1];
		Vertices.push_back(v);
		if (TexcoordsLoaded) {
			unsigned int ti = texcoordIdx[i];
			Texcoord t = texcoordData[ti - 1];
			Texcoords.push_back(t);
		}
		if (NormalsLoaded) {
			unsigned int ni = normalIdx[i];
			Normal n = normalData[ni - 1];
			Normals.push_back(n);
		}
	}
}

void Mesh::freeMeshData()
{
	vertexData.clear();
	texcoordData.clear();
	normalData.clear();
	vertexIdx.clear();
	texcoordIdx.clear();
	normalIdx.clear();
}