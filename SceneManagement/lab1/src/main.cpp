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
#include "Scene.hpp"
#include "Animation.hpp"

#define CAPTION "Hello Modern 3D World"
#define ANIMATION_TIME_MS 1000

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

Mesh meshTriangle;
Mesh meshCube;
Mesh meshParallelogram;

SceneGraph scenegraph;

GLuint ProgramId;
GLint Camera_UId;

Shader shader;

SceneNode* figure;
SceneNode * cubeT;
SceneNode * parallelogramT;
SceneNode * big_triangles_t;
SceneNode * triangle_big_1T;
SceneNode * triangle_big_2T;
SceneNode * triangle_medT;
SceneNode * triangle_small_1T;
SceneNode * triangle_small_2T;

Vec3 cube_start(0.37f, 0.0f, 0.0f);
Vec3 cube_end(0.58f, 0.02f, 0);
Vec3 paral_start(-0.2f, -0.55f, 0.0f);
Vec3 paral_end(-1.0f, -0.250f, 0.0f);
Vec3 sm_triangle1_start(0.38, 0.38f, 0);
Vec3 sm_triangle1_end(-1.25f, 0.28f, 0);
Vec3 sm_triangle2_start(0.0f, 0.0f, 0);
Vec3 sm_triangle2_end(0.30f, -0.22f, 0);
Vec3 m_triangle_start(0.750f, -0.75f, 0);
Vec3 m_triangle_end(-0.2f, 0.85f, 0);
Vec3 l_triangle1_start(0.0f, 0.0f, 0);
Vec3 l_triangle1_end(0.30f, 0.3f, 0);
Vec3 l_triangle2_start(0.0f, 0.0f, 0);
Vec3 l_triangle2_end(-0.75f, -0.75f, 0);



bool animating, reverse_anim = true;
vector<Animation> animations;
int animation_ms;
Vec3 groundTranslation;



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

void createMeshes()
{
	meshTriangle = Mesh("meshes/triangle.obj");
	meshCube = Mesh("meshes/cube.obj");
	meshParallelogram = Mesh("meshes/parallelogram.obj");
}



void createScene() {
	scenegraph.setCamera(new Camera(Camera_UId));
	scenegraph.getCamera()->setProjectionMatrix(
		perspective(30, (float)640 / 480, 1, 10));
	SceneNode * n = scenegraph.getRoot();
	n->setShaderProgram(&shader);

	SceneNode * ground = n->createNode();
	ground->setMesh(&meshCube);
	ground->setMatrix(
		translate(Vec3(0.0f, 0.0f, -0.25f)) *
		scaling4(Vec3(7.0f, 7.0f, 0.25f))
		);

	figure = n->createNode();
	cubeT = figure->createNode();
	cubeT->setMatrix(translate(cube_start));
	SceneNode* cube = cubeT->createNode();
	cube->setMesh(&meshCube);
	cube->setMatrix(rotate4(AXIS_Z, 45) 
		* scaling4(Vec3(1.1f,1.1f,1.1f)));
	
	parallelogramT = figure->createNode();
	parallelogramT->setMatrix(translate(paral_start));
	
	SceneNode * parallelogram = parallelogramT->createNode();
	parallelogram->setMesh(&meshParallelogram);
	parallelogram->setMatrix(
		scaling4(Vec3(1.0f, 0.75f, 1.0f)) 
		);

	triangle_big_1T = figure->createNode();
	SceneNode * triangleBig1 = triangle_big_1T->createNode();
	triangle_big_1T->setMatrix(translate(l_triangle1_start));
	triangleBig1->setMesh(&meshTriangle);
	triangleBig1->setMatrix(
		rotate4(AXIS_Z, 45) *
		scaling4(Vec3(4.2f, 4.2f, 1.0f))
		);


	triangle_big_2T = figure->createNode();
	triangle_big_2T->setMatrix(translate(l_triangle2_start));
	SceneNode * triangleBig2 = triangle_big_2T->createNode();
	triangleBig2->setMesh(&meshTriangle);
	triangleBig2->setMatrix(
		rotate4(AXIS_Z, 135) *
		scaling4(Vec3(4.2f, 4.2f, 1.0f))
		);

	triangle_medT = figure->createNode();
	triangle_medT->setMatrix(translate(m_triangle_start));
	SceneNode * triangleMedium = triangle_medT->createNode();
	triangleMedium->setMesh(&meshTriangle);
	triangleMedium->setMatrix(
		rotate4(AXIS_Z, 90) *
		scaling4(Vec3(3.1f, 3.1f, 1.0f))
		);

	triangle_small_1T = figure->createNode();
	triangle_small_1T->setMatrix(translate(sm_triangle1_start));
	SceneNode * triangleSmall1 = triangle_small_1T->createNode();
	triangleSmall1->setMesh(&meshTriangle);
	triangleSmall1->setMatrix(
		rotate4(AXIS_Z, -45) *
		scaling4(Vec3(2.1f, 2.1f, 1.0f))
		);

	triangle_small_2T = figure->createNode();
	triangle_small_2T->setMatrix(translate(sm_triangle2_start));
	SceneNode * triangleSmall2 = triangle_small_2T->createNode();
	triangleSmall2->setMesh(&meshTriangle);
	triangleSmall2->setMatrix(
		rotate4(AXIS_Z, 225) *
		scaling4(Vec3(2.1f, 2.1f, 1.0f))
		);




}

void createShaderProgram()
{
	shader = CreateProgram("glscripts/triangle.vert", "glscripts/triangle.frag");
	if (shader.compiled) {
		ProgramId = shader.ProgramId;

		BindAttributeLocation(ProgramId, Mesh::VERTICES, "inPosition");
		if (meshTriangle.TexcoordsLoaded)
			BindAttributeLocation(ProgramId, Mesh::TEXCOORDS, "inTexcoord");
		if (meshTriangle.NormalsLoaded)
			BindAttributeLocation(ProgramId, Mesh::NORMALS, "inNormal");
		glLinkProgram(ProgramId);
		addUniform(shader, "Matrix");
		//TODO
		Camera_UId = GetUniformLocation(ProgramId, "Camera");
	}

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{

	glUseProgram(0);
	DestroyShader(shader);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}


void destroyMeshes()
{
	meshTriangle.destroy();
	meshCube.destroy();
	meshParallelogram.destroy();
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

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
int last_time;

void updateAnimatedValues(int delta) {
	animation_ms += delta;
	for (size_t i = 0; i < animations.size(); i++)
	{
		animations[i].animate((float)animation_ms / ANIMATION_TIME_MS, reverse_anim);
	}

	if (animation_ms >= ANIMATION_TIME_MS) {
		animating = false;
		animation_ms = 0;
	}
}


void setViewProjectionMatrix()
{
	scenegraph.getCamera()->setViewMatrix(viewMatrix * q.toMatrix());

}


void drawScene()
{
	int current_time = glutGet(GLUT_ELAPSED_TIME);
	int delta = current_time - last_time;
	last_time = current_time;
	if (animating) {
		updateAnimatedValues(delta);
	}
	/*glBindVertexArray(VaoId);*/
	glUseProgram(ProgramId);

	qtrn qtX = fromAngleAxis(frameRotationX, Vec4{ 1,0,0,1 });
	qtrn qtY = fromAngleAxis(frameRotationY, Vec4{ 0,1,0,1 });
	q = qtX * qtY * q;
	frameRotationX = frameRotationY = 0;

	setViewProjectionMatrix();
	
	//Translate ground
	scenegraph.getRoot()->setMatrix(
		translate(groundTranslation)
		);
	
	scenegraph.draw();

	glUseProgram(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

//Animations

void createAnimations() {
	animations.push_back(Animation(cubeT, cube_start, cube_end, 0, 45));
	animations.push_back(Animation(parallelogramT, paral_start, paral_end, 0, 45, 0, 180));
	animations.push_back(Animation(triangle_big_1T, l_triangle1_start, l_triangle1_end, 0, 135));
	animations.push_back(Animation(triangle_big_2T, l_triangle2_start, l_triangle2_end, 0, -135));
	animations.push_back(Animation(triangle_small_1T, sm_triangle1_start, sm_triangle1_end, 0, 225));
	animations.push_back(Animation(triangle_small_2T, sm_triangle2_start, sm_triangle2_end, 0, 45));
	animations.push_back(Animation(triangle_medT, m_triangle_start, m_triangle_end, 0, 135));
	//animations.push_back(Animation(figure, Vec3(), Vec3(0, 0,0.5f), AXIS_X, 0, 90));
}

void cleanup()
{
	destroyShaderProgram();
	destroyMeshes();
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

#define EYE_OFFSET 0.3f

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
	viewMatrix = lookAt(eye, center, up);
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

#define MOVE_OFFSET 0.1f

void onKey(unsigned char key, int x, int y) {
	if (key == 'w') {
		groundTranslation.y += MOVE_OFFSET;
	}
	else if (key == 's') {
		groundTranslation.y -= MOVE_OFFSET;
	}else if (key == 'a') {
		groundTranslation.x -= MOVE_OFFSET;
	}
	else if (key == 'd') {
		groundTranslation.x += MOVE_OFFSET;
	}
	else if (key == 'f') {
		animating = true;
		reverse_anim = !reverse_anim;
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
	//mesh = Mesh("meshes/cube.obj");
	createMeshes();
	checkOpenGLError("ERROR: Could not create meshes (VAOs and VBOs).");
	createShaderProgram();
	//createBufferObjects();
	createScene();
	createAnimations();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////