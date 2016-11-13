///////////////////////////////////////////////////////////////////////
//
//  Loading OBJ mesh from external file
//
//	Final individual assignment:
//	1.	Create classes: Scene, Camera and Mesh (that loads a
//		mesh from a Wavefront OBJ file) and build a small
//		scenegraph of your tangram scene (you may create more 
//		classes if needed).
//	2.	Create a ground object and couple the tangram figure
//		to the ground. Press keys to move the ground about: 
//		the tangram figure must follow the ground.
//	3.	Animate between closed puzzle (initial square) and 
//		tangram figure by pressing a key.
//	4.	Spherical camera interaction through mouse. It should
//		be possible to interact while animation is playing.
//
//	Team assignment:
//	Pick your team (3 elements from one same lab) for the team
//	assignment you will be working until the end of the semester,
//	and register it online.
//
// (c) 2013-16 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "gl_shader.hpp"
#include "vec.hpp"
#include "matrix_factory.hpp"
#include "GLGameObject.hpp"
#include "qtrn.hpp"
#include "Mesh.hpp"

#define CAPTION "Hello Modern 3D World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

Mesh mesh;

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

GLuint VaoId;
GLuint VboVertices, VboTexcoords, VboNormals;

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint Matrix_UId;

Shader shader;


///////////////////////////////////MATRICES
#define PI 3.14159265358979f

Mat4 I = identity4();
//Mat4 translateAbitLeft = translate(Vec3(-0.1f, 0.0f, 0));
//Mat4 translateLeft = translate(Vec3(-0.25f, 0.0f, 0));
//Mat4 translateTopLeft = translate(Vec3(-0.3f, 0.5f, 0));
//Mat4 big_triangle_1_translate = translate(Vec3(0.0f, -0.5f, 0));
//Mat4 big_triangle_2_translate = translate(Vec3(-0.005f, -0.83f, 0));
//Mat4 translate25Right = translate(Vec3(0.25f, 0.0f, 0));
//Mat4 translateRight = translate(Vec3(0.5f, 0.0f, 0));
//Mat4 translateBottom = translate(Vec3(0.0f, -0.5f, 0));
//Mat4 translateAlotBottom = translate(Vec3(0.0f, -0.65f, 0));
//Mat4 smallTriangle1Translate = translate(Vec3(-0.2f, 0.83f, 0));
//Mat4 smallTriangle2Translate = translate(Vec3(0.05f, 0.83f, 0));
//Mat4 translateTop = translate(Vec3(0.0f, 0.5f, 0));
//
//Mat4 translateMediumTriangle = translate(Vec3(0.5f, 0.0f, 0));
//Mat4 parallelogramTranslate = translate(Vec3(0.0f, 0.32f, 0));
//Mat4 translate25Bottom = translate(Vec3(0.0f, -0.25f, 0));
//
//Mat4 scaleMedium = scaling4(Vec3(1.0f, 1.0f, 0.3f));
//Mat4 scaleNegative = scaling4(Vec3(-1.0f, -1.0f, -1.0f));
//Mat4 scaleBig = scaling4(Vec3(1.3f, 1.3f, 1.5f));
//Mat4 scaleSmall = scaling4(Vec3(0.75f, 0.75f, 0.6f));
//
//Mat4 rotate90Left = rotate4(Vec3(0.0f, 0.0f, 1.0f), 90);
//Mat4 rotateRight = rotate4(Vec3(0.0f, 0.0f, 1.0f), 270);
//Mat4 rotate180 = rotate4(Vec3(0.0f, 0.0f, 1.0f), 180);

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}

void createShaderProgram()
{
	shader = CreateProgram("glscripts/triangle.vert", "glscripts/triangle.frag");
	if (shader.compiled) {
		ProgramId = shader.ProgramId;

		BindAttributeLocation(ProgramId, VERTICES, "inPosition");
		if (mesh.TexcoordsLoaded)
			BindAttributeLocation(ProgramId, TEXCOORDS, "inTexcoord");
		if (mesh.NormalsLoaded)
			BindAttributeLocation(ProgramId, NORMALS, "inNormal");
		glLinkProgram(ProgramId);
		Matrix_UId = GetUniformLocation(ProgramId, "Matrix");
	}

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{

	glUseProgram(0);
	DestroyShader(shader);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), &mesh.Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (mesh.TexcoordsLoaded)
	{
		glGenBuffers(1, &VboTexcoords);
		glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
		glBufferData(GL_ARRAY_BUFFER, mesh.Texcoords.size() * sizeof(Texcoord), &mesh.Texcoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEXCOORDS);
		glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);
	}
	if (mesh.NormalsLoaded)
	{
		glGenBuffers(1, &VboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
		glBufferData(GL_ARRAY_BUFFER, mesh.Normals.size() * sizeof(Normal), &mesh.Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMALS);
		glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboNormals);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

#define TRIANGLE_INDEX 0
#define SQUARE_INDEX 24
#define PARALLELOGRAM_INDEX 60


#define INDEX(idx) (void*)idx

Mat4 xViewMatrixRotation = identity4();
Mat4 yViewMatrixRotation = identity4();
Mat4 zViewMatrixRotation = identity4();

Vec3 eye = { 0, 0, 8 };
Vec3 center = { 0, 0, 0 };
Vec3 up = { 0, 1, 0 };

Mat4 viewMatrix = lookAt(eye, center, up);

Mat4 orthoMatrix = ortho(-2, 2, -2, 2, 1, 10);
Mat4 perspectiveMatrix = perspective(30, (float)640 / 480, 1, 10);
Mat4* currentProjection = &perspectiveMatrix;

//
//void drawTriangles() {
//	vector <int> faceElements = { 3,3,6,6,6 };
//
//	vector<int> colors = { 12, 13, 14, 15, 16 };
//	GLGameObject triangle3d = GLGameObject(colors, 0, faceElements, UniformId, UniformColorId, (big_triangle_1_translate * rotateRight * scaleBig));
//	triangle3d.draw();
//	
//	vector<int> colors2 = { 17, 18, 19, 20, 21 };
//	GLGameObject triangle2 = GLGameObject(colors2, 0, faceElements, UniformId, UniformColorId, (big_triangle_2_translate * rotate90Left * scaleBig));
//	triangle2.draw();
//
//	vector<int> colors3 = { 22, 23, 24, 25, 26 };
//	GLGameObject triangle3 = GLGameObject(colors3, 0, faceElements, UniformId, UniformColorId, (translateMediumTriangle * rotateRight * scaleMedium));
//	triangle3.draw();
//
//	vector<int> colors4 = { 27, 28, 29, 30, 31 };
//	GLGameObject triangle4 = GLGameObject(colors4, 0, faceElements, UniformId, UniformColorId, (smallTriangle1Translate * scaleSmall));
//	triangle4.draw();
//
//	vector<int> colors5 = { 32, 33, 34, 35, 36 };
//	GLGameObject triangle5 = GLGameObject(colors5, 0, faceElements, UniformId, UniformColorId, (smallTriangle2Translate * scaleSmall));
//	triangle5.draw();
//
//}
//
//
//void drawSquares() {
//	vector<int> vec = { 0,1,2,3,4,5 };
//	vector <int> faceElements = { 6,6,6,6,6,6 };
//
//	GLGameObject cube = GLGameObject(vec, SQUARE_INDEX, faceElements, UniformId, UniformColorId, translate25Bottom);
//	cube.draw();
//}
//
//void drawParalelogram() {
//	vector<int> colors = { 6,7,8,9,10,11 };
//	vector <int> faceElements = { 6,6,6,6,6,6 };
//
//	GLGameObject paralellogram = GLGameObject(colors, PARALLELOGRAM_INDEX, faceElements, UniformId, UniformColorId, parallelogramTranslate * scaleBig);
//	paralellogram.draw();
//}

bool gimbalLock = false;
float xDegrees = 0;
float yDegrees = 0;
float zDegrees = 0;

qtrn q;

#define OFFSET_CAMERA 4.5f
#define TO_RAD(f) (f * (PI / 180))

int frameRotationX;
int frameRotationY;

void drawScene()
{
	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	qtrn qtX = fromAngleAxis(frameRotationX, Vec4{ 1,0,0,1 });
	qtrn qtY = fromAngleAxis(frameRotationY, Vec4{ 0,1,0,1 });
	q = qtX * qtY * q;
	frameRotationX = frameRotationY = 0;

	glUniformMatrix4fv(Matrix_UId, 1, GL_FALSE, (*currentProjection * viewMatrix * q.toMatrix()).convert_opengl());
	
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.Vertices.size());

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

int clicked = -1;
int mouseX;
int mouseY;

#define EYE_OFFSET 0.1f

//button 0 LEFT
//button 1 MIDDLE
//button 2 RIGHT
//button 3 mwhell up
//button 4 mwhell down
void onMouse(int button, int state, int x, int y)
{
	if (state == 0) {
		return;
	}
	if (button == 3) {
		eye.z -= EYE_OFFSET;
	}
	else if (button == 4) {
		eye.z += EYE_OFFSET;
	}
}

void onMotion(int x, int y) {
	if (x > mouseX) {
		frameRotationY += OFFSET_CAMERA;
	}
	else if (x < mouseX) {
		frameRotationY -= OFFSET_CAMERA;
	}
	if (y > mouseY) {
		frameRotationX += OFFSET_CAMERA;
	}
	else if (y < mouseY) {
		frameRotationX -= OFFSET_CAMERA;
	}
	mouseX = x;
	mouseY = y;

}

void onKey(unsigned char key, int x, int y) {
	if (key == 'p') {
		if (currentProjection == &orthoMatrix) {
			currentProjection = &perspectiveMatrix;
		}
		else {
			currentProjection = &orthoMatrix;
		}
	}
	if (key == 'g') {
		gimbalLock = !gimbalLock;
		cout << "Gimbal " << gimbalLock << endl;
	}
	else if (key == 'r') {
		eye.x = 0;
		eye.y = 0;
		eye.z = 5;
		q = qtrn();
		xViewMatrixRotation = yViewMatrixRotation = zViewMatrixRotation = identity4();
	}
}

void onArrows(int key, int x, int y) {

}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(onKey);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	glutSpecialFunc(onArrows);
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	mesh = Mesh("meshes/monkey.obj");
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////