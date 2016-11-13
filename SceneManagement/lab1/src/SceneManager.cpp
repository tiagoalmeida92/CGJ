///////////////////////////////////////////////////////////////////////
//
// Model Hierarchy using scene manager
//
//	Copyright (c) 2013-2016 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <string>

#include "engine/engine.h"
using namespace engine;

#define CAPTION "Modern OpenGL Engine"
int WinX = 640, WinY = 480;

GLuint UBO_BP = 0;

MatrixStack ModelMatrixStack;

float Angle0 = 0.0f, Angle1 = -45.0f, Angle2 = -135.0f, Angle3 = 90.0f;
const float DistanceStep = 1.5f;
float Distance = 20.0f;
vec3 Position(0.0f, 0.0f, 0.0f);
vec3 Front;

int LastMousePositionX, LastMousePositionY;
float RotationAngleX = 0.0f;
float RotationAngleY = 0.0f;

qtrn q;

/////////////////////////////////////////////////////////////////////// Mesh

void createMeshes()
{
	Mesh* mesh;
	mesh = new Mesh("../../assets/models/cube.mesh");
	mesh->create();
	MeshManager::instance()->add("cube", mesh);
}

/////////////////////////////////////////////////////////////////////// Shaders

void createShaderPrograms()
{
	ShaderProgram* program = new ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "src/ShaderTutorial/0-viewer/0a-viewer-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "src/ShaderTutorial/0-viewer/0a-viewer-fs.glsl");
	program->addAttribute("inVertex", Mesh::VERTICES);
	program->addAttribute("inNormal", Mesh::NORMALS);
	program->addUniform("ModelMatrix");
	program->addUniformBlock("Camera", UBO_BP);
	program->create();
	ShaderProgramManager::instance()->add("stack", program);
}

/////////////////////////////////////////////////////////////////////// SceneGraph

SceneNode *machine, *lowerarmA, *upperarmA, *pincerAL, *pincerAR;

void createEnvironmentSceneGraph(SceneGraph* scenegraph)
{
	Mesh* mesh = MeshManager::instance()->get("cube");

	SceneNode *ground = scenegraph->createNode();
	ground->setMesh(mesh);
	ground->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(0.0f, -0.25f, 0.0f)) *
		MatrixFactory::createScaleMat4(vec3(10.0f, 0.25f, 10.0f))
		);
	SceneNode *column1 = scenegraph->createNode();
	column1->setMesh(mesh);
	column1->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(4.5f, 0.0f, 4.5f)) *
		MatrixFactory::createScaleMat4(vec3(1.0f, 3.0f, 1.0f))
		);
	SceneNode *column2 = scenegraph->createNode();
	column2->setMesh(mesh);
	column2->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(-4.5f, 0.0f, 4.5f)) *
		MatrixFactory::createScaleMat4(vec3(1.0f, 3.0f, 1.0f))
		);
	SceneNode *column3 = scenegraph->createNode();
	column3->setMesh(mesh);
	column3->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(4.5f, 0.0f, -4.5f)) *
		MatrixFactory::createScaleMat4(vec3(1.0f, 3.0f, 1.0f))
		);
	SceneNode *column4 = scenegraph->createNode();
	column4->setMesh(mesh);
	column4->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(-4.5f, 0.0f, -4.5f)) *
		MatrixFactory::createScaleMat4(vec3(1.0f, 3.0f, 1.0f))
		);
}

void createMachineSceneGraph(SceneGraph* scenegraph)
{
	Mesh* mesh = MeshManager::instance()->get("cube");

	machine = scenegraph->createNode();

	SceneNode *base = machine->createNode();
	base->setMesh(mesh);
	base->setMatrix(
		MatrixFactory::createScaleMat4(vec3(2.0f, 0.25f, 2.0f))
		);
	SceneNode *baseL = machine->createNode();
	baseL->setMesh(mesh);
	baseL->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(0.75f, 0.25f, 0.0f)) *
		MatrixFactory::createScaleMat4(vec3(0.5f, 1.5f, 2.0f))
		);
	SceneNode *baseR = machine->createNode();
	baseR->setMesh(mesh);
	baseR->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(-0.75f, 0.25f, 0.0f)) *
		MatrixFactory::createScaleMat4(vec3(0.5f, 1.5f, 2.0f))
		);
	lowerarmA = machine->createNode();

	SceneNode *lowerarm = lowerarmA->createNode();
	lowerarm->setMesh(mesh);
	lowerarm->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(0.0f, -0.5f, 0.0f)) *
		MatrixFactory::createScaleMat4(vec3(1.0f, 2.5f, 0.5f))
		);
	upperarmA = lowerarmA->createNode();

	SceneNode *upperarm = upperarmA->createNode();
	upperarm->setMesh(mesh);
	upperarm->setMatrix(
		MatrixFactory::createScaleMat4(vec3(1.0f, 2.5f, 0.5f))
		);
	SceneNode* pincer = upperarmA->createNode();

	pincerAL = pincer->createNode();
	pincerAL->setMesh(mesh);

	pincerAR = pincer->createNode();
	pincerAR->setMesh(mesh);
}

void createScene()
{
	SceneGraph* scenegraph = new SceneGraph();
	scenegraph->setCamera(new Camera(UBO_BP));

	scenegraph->getCamera()->setProjectionMatrix(
		MatrixFactory::createPerspectiveProjectionMatrix(30.0f, 640.0f / 480.0f, 1.0f, 100.0f));
	//	scenegraph->getCamera()->setProjectionMatrix(
	//		MatrixFactory::createOrthographicProjectionMatrix(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f));

	SceneNode *n = scenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::instance()->get("stack"));

	createEnvironmentSceneGraph(scenegraph);
	createMachineSceneGraph(scenegraph);

	SceneGraphManager::instance()->add("main", scenegraph);
}

void setViewProjectionMatrix()
{
	mat4 t = MatrixFactory::createTranslationMat4(vec3(0.0f, 0.0f, -Distance));
	mat4 r = MatrixFactory::createMat4FromQtrn(q);
	SceneGraphManager::instance()->get("main")->getCamera()->setViewMatrix(t * r);
}

void drawSceneGraph()
{
	setViewProjectionMatrix();

	machine->setMatrix(
		MatrixFactory::createTranslationMat4(Position) *
		MatrixFactory::createRotationMat4(Angle0, AXIS3D_Y)
		);
	lowerarmA->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(0.0f, 1.0f, 0.0f)) *
		MatrixFactory::createRotationMat4(Angle1, AXIS3D_X)
		);
	upperarmA->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(0.0f, 2.0f, 0.0f)) *
		MatrixFactory::createRotationMat4(Angle2, AXIS3D_X)
		);
	pincerAL->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(-0.5f, 2.25f, -0.25f)) *
		MatrixFactory::createRotationMat4(Angle3, AXIS3D_Y) *
		MatrixFactory::createRotationMat4(90.0f, AXIS3D_Z) *
		MatrixFactory::createScaleMat4(vec3(0.5f, 2.0f, 0.25f))
		);
	pincerAR->setMatrix(
		MatrixFactory::createTranslationMat4(vec3(0.5f, 2.25f, -0.25f)) *
		MatrixFactory::createRotationMat4(-Angle3, AXIS3D_Y) *
		MatrixFactory::createRotationMat4(-90.0f, AXIS3D_Z) *
		MatrixFactory::createRotationMat4(180.0f, AXIS3D_Y) *
		MatrixFactory::createScaleMat4(vec3(0.5f, 2.0f, 0.25f))
		);
	SceneGraphManager::instance()->get("main")->draw();
}

/////////////////////////////////////////////////////////////////////// Scene

void drawScene()
{
	drawSceneGraph();

	ASSERT_GL_ERROR("ERROR: Could not draw scene.")
}

/////////////////////////////////////////////////////////////////////// Simulation

class updater : public IUpdatable
{
public:
	void update(unsigned int elapsed) {
		float astep = 0.05f * elapsed;
		float vstep = 0.0025f * elapsed;
		if (KeyBuffer::instance()->isKeyDown('a')) Angle0 += astep;
		if (KeyBuffer::instance()->isKeyDown('d')) Angle0 -= astep;
		if (KeyBuffer::instance()->isKeyDown('q')) Angle1 += astep;
		if (KeyBuffer::instance()->isKeyDown('z')) Angle1 -= astep;
		if (KeyBuffer::instance()->isKeyDown('e')) Angle2 += astep;
		if (KeyBuffer::instance()->isKeyDown('c')) Angle2 -= astep;
		if (KeyBuffer::instance()->isKeyDown(' ')) Angle3 += astep;
		if (KeyBuffer::instance()->isKeyDown('x')) Angle3 -= astep;
		if (KeyBuffer::instance()->isKeyDown('w')) Position = Position + Front * vstep;
		if (KeyBuffer::instance()->isKeyDown('s')) Position = Position - Front * vstep;

		q = qtrn(RotationAngleX, AXIS3D_X) * qtrn(RotationAngleY, AXIS3D_Y) * q;
		RotationAngleX = RotationAngleY = 0.0f;

		qtrn qr(Angle0, AXIS3D_Y);
		qtrn qi(-AXIS3D_Z);
		qtrn qf = qr * qi * inverse(qr);
		Front = vec3(qf.x, qf.y, qf.z);
	}
};

void createSimulation()
{
	Simulation::instance()->add(new updater());
}

/////////////////////////////////////////////////////////////////////// Callbacks

void cleanup()
{
	MeshManager::instance()->destroy();
	ShaderProgramManager::instance()->destroy();
	SceneGraphManager::instance()->destroy();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	Simulation::instance()->update();
	glutPostRedisplay();
}

void keyboard_down(unsigned char key, int x, int y)
{
	KeyBuffer::instance()->pressKey(key);
}

void keyboard_up(unsigned char key, int x, int y)
{
	KeyBuffer::instance()->releaseKey(key);
}

void mouse(int button, int state, int x, int y)
{
	LastMousePositionX = x;
	LastMousePositionY = y;
	if (state == GLUT_UP)
		RotationAngleX = RotationAngleY = 0.0f;
}

void mouseMotion(int x, int y)
{
	RotationAngleX = (float)(y - LastMousePositionY);
	LastMousePositionY = y;
	RotationAngleY = (float)(x - LastMousePositionX);
	LastMousePositionX = x;
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0) {
		Distance -= DistanceStep;
	}
	else {
		Distance += DistanceStep;
	}
}
void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

/////////////////////////////////////////////////////////////////////// Setup

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard_down);
	glutKeyboardUpFunc(keyboard_up);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutReshapeFunc(reshape);
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
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

void setupGLEW() {
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
	int window_handle = glutCreateWindow(CAPTION);
	if (window_handle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();

	ASSERT_GL_ERROR("ERROR: General setup.")
		createMeshes();
	ASSERT_GL_ERROR("ERROR: Mesh creation.")
		createShaderPrograms();
	ASSERT_GL_ERROR("ERROR: Shader creation.")
		createScene();
	ASSERT_GL_ERROR("ERROR: Scene creation.")
		createSimulation();
	ASSERT_GL_ERROR("ERROR: Simulation creation.")
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////