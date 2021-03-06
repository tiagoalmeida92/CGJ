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
#include "qtrn.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "Animation.hpp"

#define CAPTION "Hello Modern 3D World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

Mesh meshTriangle, meshCube, meshParallelogram;

Shader shader;

SceneGraph scenegraph;

GLint Camera_UId;

Vec3 eye = { 0, 0, 8 };
Vec3 center = { 0, 0, 0 };
Vec3 up = { 0, 1, 0 };

Mat4 viewMatrix = lookAt(eye, center, up);
Mat4 perspectiveMatrix = perspective(30, (float)640 / 480, 1, 10);

qtrn q;
int frameRotationX, frameRotationY;
int last_time;

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


#define ANIMATION_TIME_MS 1500

bool animating, reverse_anim = true;
vector<Animation> animations;
int animation_ms;
Vec3 groundTranslation;

////////////////////////////////////////// ERRORS

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

int calculateDelta() {
	int current_time = glutGet(GLUT_ELAPSED_TIME);
	int delta = current_time - last_time;
	last_time = current_time;
	return delta;
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
		scaling4(Vec3(5.0f, 5.0f, 0.25f))
		);

	figure = n->createNode();
	cubeT = figure->createNode();
	cubeT->setMatrix(translate(cube_start));
	SceneNode* cube = cubeT->createNode();
	cube->setMesh(&meshCube);
	cube->setMatrix(rotate4(AXIS_Z, 45)
		* scaling4(Vec3(1.1f, 1.1f, 1.1f)));

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
		shader.BindAttributeLocation(Mesh::VERTICES, "inPosition");
		if (meshTriangle.TexcoordsLoaded)
			shader.BindAttributeLocation(Mesh::TEXCOORDS, "inTexcoord");
		if (meshTriangle.NormalsLoaded)
			shader.BindAttributeLocation(Mesh::NORMALS, "inNormal");
		shader.addUniform("Matrix");
		Camera_UId = shader.addUniform("Camera");
	}
}

void animateScene(int delta) {
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
	qtrn qtX = fromAngleAxis(frameRotationX, Vec4{ 1,0,0,1 });
	qtrn qtY = fromAngleAxis(frameRotationY, Vec4{ 0,1,0,1 });
	q = qtX * qtY * q;
	frameRotationX = frameRotationY = 0;

	scenegraph.getCamera()->setViewMatrix(viewMatrix * q.toMatrix());
}


void drawScene()
{
	int delta = calculateDelta();
	if (animating) {
		animateScene(delta);
	}

	setViewProjectionMatrix();
	scenegraph.draw();

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
	animations.push_back(Animation(figure, Vec3(), Vec3(0.5f, 0,0.0f), 0, 0));
	//animations.push_back(Animation(figure, Vec3(), Vec3(0, 0,0.5f), AXIS_X, 0, 90));
}


void destroyShaderProgram()
{
	shader.destroyShader();
	checkOpenGLError("ERROR: Could not destroy shaders.");
}


void destroyMeshes()
{
	meshTriangle.destroy();
	meshCube.destroy();
	meshParallelogram.destroy();
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void cleanup()
{
	destroyShaderProgram();
	destroyMeshes();
	scenegraph.destroy();
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

int last_mouse_x, last_mouse_y;


#define MOVE_OFFSET 0.1f
#define OFFSET_CAMERA 4.5f
#define EYE_OFFSET 0.3f

void onMouse(int button, int state, int x, int y)
{
	if (button == 3) {
		eye.z -= EYE_OFFSET;
	}
	else if (button == 4) {
		eye.z += EYE_OFFSET;
	}
	viewMatrix = lookAt(eye, center, up);
}

void onMotion(int x, int y) {
	if (x > last_mouse_x) {
		frameRotationY += OFFSET_CAMERA;
	}
	else if (x < last_mouse_x) {
		frameRotationY -= OFFSET_CAMERA;
	}
	if (y > last_mouse_y) {
		frameRotationX += OFFSET_CAMERA;
	}
	else if (y < last_mouse_y) {
		frameRotationX -= OFFSET_CAMERA;
	}
	last_mouse_x = x;
	last_mouse_y = y;
}


void onKey(unsigned char key, int x, int y) {
	if (key == 'w') {
		groundTranslation.y += MOVE_OFFSET;
	}
	else if (key == 's') {
		groundTranslation.y -= MOVE_OFFSET;
	}
	else if (key == 'a') {
		groundTranslation.x -= MOVE_OFFSET;
	}
	else if (key == 'd') {
		groundTranslation.x += MOVE_OFFSET;
	}
	else if (key == 'f') {
		animating = true;
		reverse_anim = !reverse_anim;
	}
	scenegraph.getRoot()->setMatrix(translate(groundTranslation));
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
	checkOpenGLError("ERROR: Could not create shaders.");
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