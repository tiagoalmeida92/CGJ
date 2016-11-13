///////////////////////////////////////////////////////////////////////
//
// Model Hierarchy using matrix stack
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
GLuint UboId;

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
	MeshManager::instance()->add("cube",mesh);
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
	ShaderProgramManager::instance()->add("stack",program);
}

/////////////////////////////////////////////////////////////////////// Matrix Stack

void createScene() 
{
	glGenBuffers(1, &UboId);
	glBindBuffer(GL_UNIFORM_BUFFER, UboId);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GLMatrix) * 2, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, UboId);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void setViewProjectionMatrix()
{
	mat4 ViewMatrix = MatrixFactory::createTranslationMat4(vec3(0.0f, 0.0f, -Distance)) * MatrixFactory::createMat4FromQtrn(q);
	mat4 ProjectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix(30.0f, 640.0f / 480.0f, 1.0f, 100.0f);
//	mat4 ProjectionMatrix = MatrixFactory::createOrthographicProjectionMatrix(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);

	glBindBuffer(GL_UNIFORM_BUFFER, UboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLMatrix), ViewMatrix.data);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLMatrix), sizeof(GLMatrix), ProjectionMatrix.data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void drawMesh()
{
	ShaderProgram* program = ShaderProgramManager::instance()->get("stack");
	glUniformMatrix4fv(program->uniforms["ModelMatrix"].index, 1, GL_FALSE, ModelMatrixStack.current.data);
	MeshManager::instance()->get("cube")->draw();
}

void drawEnvironment()
{
/**/
	ModelMatrixStack.pushMatrix();
		ModelMatrixStack.translate(vec3(0.0f, -0.25f, 0.0f));
		ModelMatrixStack.scale(vec3(10.0f, 0.25f, 10.0f));
		drawMesh();
	ModelMatrixStack.popMatrix();
/**/
	ModelMatrixStack.pushMatrix();
		ModelMatrixStack.translate(vec3(4.5f, 0.0f, 4.5f));
		ModelMatrixStack.scale(vec3(1.0f, 3.0f, 1.0f));
		drawMesh();
	ModelMatrixStack.popMatrix();
/**/ 
	ModelMatrixStack.pushMatrix();
		ModelMatrixStack.translate(vec3(-4.5f, 0.0f, 4.5f));
		ModelMatrixStack.scale(vec3(1.0f, 3.0f, 1.0f));
		drawMesh();
	ModelMatrixStack.popMatrix();
/**/
	ModelMatrixStack.pushMatrix();
		ModelMatrixStack.translate(vec3(-4.5f, 0.0f, -4.5f));
		ModelMatrixStack.scale(vec3(1.0f, 3.0f, 1.0f));
		drawMesh();
	ModelMatrixStack.popMatrix();
/**/
	ModelMatrixStack.pushMatrix();
		ModelMatrixStack.translate(vec3(4.5f, 0.0f, -4.5f));
		ModelMatrixStack.scale(vec3(1.0f, 3.0f, 1.0f));
		drawMesh();
	ModelMatrixStack.popMatrix();
/**/
}

void drawMachine()
{
	ModelMatrixStack.pushMatrix();
/**/	
		// Base
		ModelMatrixStack.translate(Position);
		ModelMatrixStack.rotate(Angle0, AXIS3D_Y);
/**/
		// Base Bottom
		ModelMatrixStack.pushMatrix();
			ModelMatrixStack.scale(vec3(2.0f, 0.25f, 2.0f));
			drawMesh();
		ModelMatrixStack.popMatrix();
/**/
		// Base left
		ModelMatrixStack.pushMatrix();
			ModelMatrixStack.translate(vec3(0.75f, 0.25f, 0.0f));
			ModelMatrixStack.scale(vec3(0.5f,1.5f,2.0f));
			drawMesh();
		ModelMatrixStack.popMatrix();
/**/
		// Base right
		ModelMatrixStack.pushMatrix();
			ModelMatrixStack.translate(vec3(-0.75f, 0.25f, 0.0f));
			ModelMatrixStack.scale(vec3(0.5f, 1.5f, 2.0f));
			drawMesh();
		ModelMatrixStack.popMatrix();
/**/
		// Lower Arm Articulation
		ModelMatrixStack.translate(vec3(0.0f, 1.0f, 0.0f));
		ModelMatrixStack.rotate(Angle1, AXIS3D_X);
/**/
		// Lower Arm
		ModelMatrixStack.pushMatrix();
			ModelMatrixStack.translate(vec3(0.0f, -0.5f, 0.0f));
			ModelMatrixStack.scale(vec3(1.0f, 2.5f, 0.5f));
			drawMesh();
		ModelMatrixStack.popMatrix();
/**/
		// Upper Arm Articulation
		ModelMatrixStack.translate(vec3(0.0f, 2.0f, 0.0f));
		ModelMatrixStack.rotate(Angle2, AXIS3D_X);
/**/
		// Upper Arm
		ModelMatrixStack.pushMatrix();
			ModelMatrixStack.scale(vec3(1.0f, 2.5f, 0.5f));
			drawMesh();
		ModelMatrixStack.popMatrix();
/**/
		// Pincer Articulation
		ModelMatrixStack.translate(vec3(0.0f, 2.25f, -0.25f));
/**/
		// Left Pincer
		ModelMatrixStack.pushMatrix();
			ModelMatrixStack.translate(vec3(-0.5f, 0.0f, 0.0f));
			ModelMatrixStack.rotate(Angle3, AXIS3D_Y);
			ModelMatrixStack.rotate(90, AXIS3D_Z);
			ModelMatrixStack.scale(vec3(0.5f, 2.0f, 0.25f));
			drawMesh();
		ModelMatrixStack.popMatrix();
/**/
		// Right Pincer
		ModelMatrixStack.pushMatrix();
			ModelMatrixStack.translate(vec3(0.5f, 0.0f, 0.0f));
			ModelMatrixStack.rotate(-Angle3, AXIS3D_Y);
			ModelMatrixStack.rotate(-90, AXIS3D_Z);
			ModelMatrixStack.rotate(180, AXIS3D_Y);
			ModelMatrixStack.scale(vec3(0.5f, 2.0f, 0.25f));
			drawMesh();
		ModelMatrixStack.popMatrix();
/**/
	ModelMatrixStack.popMatrix();
}

void drawMatrixStack()
{
	setViewProjectionMatrix();
	ShaderProgram* program = ShaderProgramManager::instance()->get("stack");

	ModelMatrixStack.loadIdentity();
	program->bind();
	drawEnvironment();
	drawMachine();
	program->unbind();
}

/////////////////////////////////////////////////////////////////////// Scene

void drawScene()
{
	drawMatrixStack();

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
	if(state == GLUT_UP) 
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
	} else {
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
	GLenum result = glewInit() ; 
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

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	int window_handle = glutCreateWindow(CAPTION);
	if(window_handle < 1) {
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