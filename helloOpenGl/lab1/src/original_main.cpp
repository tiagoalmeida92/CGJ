/////////////////////////////////////////////////////////////////////////
////
//// Drawing two instances of a triangle in Modern OpenGL.
//// A "hello world" of Modern OpenGL.
////
//// Assignment : Create Shader Abstraction 
////					(e.g. check compilation/linkage for errors, read from file) 
////			  : Manage Multiple Drawable Entities (using your vertex and matrix classes)
////              Draw a set of 7 TANs (i.e. TANGRAM shapes) of different colors: 
////              (1) 3 different TAN geometric shapes at the origin:
////					- right triangle
////					- square
////					- parallelogram
////              (2) 7 TANs of different colors put together to form a shape of your choice:
////					- 2 big right triangles
////					- 1 medium right triangle
////					- 2 small right triangles
////					- 1 square
////					- 1 parallelogram;
////
//// (c) 2013-16 by Carlos Martinho
////
/////////////////////////////////////////////////////////////////////////
//
//#include <iostream>
//#include <sstream>
//#include <string>
//
//#include "GL/glew.h"
//#include "GL/freeglut.h"
//
//#define CAPTION "Hello Modern 2D World"
//
//int WinX = 640, WinY = 480;
//int WindowHandle = 0;
//unsigned int FrameCount = 0;
//
//#define VERTICES 0
//#define COLORS 1
//
//GLuint VaoId, VboId[2];
//GLuint VertexShaderId, FragmentShaderId, ProgramId;
//GLint UniformId;
//
///////////////////////////////////////////////////////////////////////// ERRORS
//
//bool isOpenGLError() {
//	bool isError = false;
//	GLenum errCode;
//	const GLubyte *errString;
//	while ((errCode = glGetError()) != GL_NO_ERROR) {
//		isError = true;
//		errString = gluErrorString(errCode);
//		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
//	}
//	return isError;
//}
//
//void checkOpenGLError(std::string error)
//{
//	if (isOpenGLError()) {
//		std::cerr << error << std::endl;
//		exit(EXIT_FAILURE);
//	}
//}
//
///////////////////////////////////////////////////////////////////////// SHADERs
//
//const GLchar* VertexShader =
//{
//	"#version 330 core\n"
//
//	"in vec4 in_Position;\n"
//	"in vec4 in_Color;\n"
//	"out vec4 ex_Color;\n"
//
//	"uniform mat4 Matrix;\n"
//
//	"void main(void)\n"
//	"{\n"
//	"	gl_Position = Matrix * in_Position;\n"
//	"	ex_Color = in_Color;\n"
//	"}\n"
//};
//
//const GLchar* FragmentShader =
//{
//	"#version 330 core\n"
//
//	"in vec4 ex_Color;\n"
//	"out vec4 out_Color;\n"
//
//	"void main(void)\n"
//	"{\n"
//	"	out_Color = ex_Color;\n"
//	"}\n"
//};
//
//void createShaderProgram()
//{
//	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
//	glCompileShader(VertexShaderId);
//
//	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
//	glCompileShader(FragmentShaderId);
//
//	ProgramId = glCreateProgram();
//	glAttachShader(ProgramId, VertexShaderId);
//	glAttachShader(ProgramId, FragmentShaderId);
//
//	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
//	glBindAttribLocation(ProgramId, COLORS, "in_Color");
//	glLinkProgram(ProgramId);
//	UniformId = glGetUniformLocation(ProgramId, "Matrix");
//
//	checkOpenGLError("ERROR: Could not create shaders.");
//}
//
//void destroyShaderProgram()
//{
//	glUseProgram(0);
//	glDetachShader(ProgramId, VertexShaderId);
//	glDetachShader(ProgramId, FragmentShaderId);
//
//	glDeleteShader(FragmentShaderId);
//	glDeleteShader(VertexShaderId);
//	glDeleteProgram(ProgramId);
//
//	checkOpenGLError("ERROR: Could not destroy shaders.");
//}
//
///////////////////////////////////////////////////////////////////////// VAOs & VBOs
//
//typedef struct
//{
//	GLfloat XYZW[4];
//	GLfloat RGBA[4];
//} Vertex;
//
//const Vertex Vertices[] =
//{
//	{ { 0.25f, 0.25f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
//	{ { 0.75f, 0.25f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
//	{ { 0.50f, 0.75f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
//};
//
//const GLubyte Indices[] =
//{
//	0,1,2
//};
//
//void createBufferObjects()
//{
//	glGenVertexArrays(1, &VaoId);
//	glBindVertexArray(VaoId);
//	{
//		glGenBuffers(2, VboId);
//
//		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//		glEnableVertexAttribArray(VERTICES);
//		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//		glEnableVertexAttribArray(COLORS);
//		glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].XYZW));
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
//	}
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
//}
//
//void destroyBufferObjects()
//{
//	glBindVertexArray(VaoId);
//	glDisableVertexAttribArray(VERTICES);
//	glDisableVertexAttribArray(COLORS);
//	glDeleteBuffers(2, VboId);
//	glDeleteVertexArrays(1, &VaoId);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
//}
//
///////////////////////////////////////////////////////////////////////// SCENE
//
//typedef GLfloat Matrix[16];
//
//const Matrix I = {
//	1.0f,  0.0f,  0.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,  0.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//	0.0f,  0.0f,  0.0f,  1.0f
//}; // Row Major (GLSL is Column Major)
//
//const Matrix M = {
//	1.0f,  0.0f,  0.0f, -1.0f,
//	0.0f,  1.0f,  0.0f, -1.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//	0.0f,  0.0f,  0.0f,  1.0f
//}; // Row Major (GLSL is Column Major)
//
//void drawScene()
//{
//	glBindVertexArray(VaoId);
//	glUseProgram(ProgramId);
//
//	glUniformMatrix4fv(UniformId, 1, GL_TRUE, I);
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
//
//	glUniformMatrix4fv(UniformId, 1, GL_TRUE, M);
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
//
//	glUseProgram(0);
//	glBindVertexArray(0);
//
//	checkOpenGLError("ERROR: Could not draw scene.");
//}
//
///////////////////////////////////////////////////////////////////////// CALLBACKS
//
//void cleanup()
//{
//	destroyShaderProgram();
//	destroyBufferObjects();
//}
//
//void display()
//{
//	++FrameCount;
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	drawScene();
//	glutSwapBuffers();
//}
//
//void idle()
//{
//	glutPostRedisplay();
//}
//
//void reshape(int w, int h)
//{
//	WinX = w;
//	WinY = h;
//	glViewport(0, 0, WinX, WinY);
//}
//
//void timer(int value)
//{
//	std::ostringstream oss;
//	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
//	std::string s = oss.str();
//	glutSetWindow(WindowHandle);
//	glutSetWindowTitle(s.c_str());
//	FrameCount = 0;
//	glutTimerFunc(1000, timer, 0);
//}
//
///////////////////////////////////////////////////////////////////////// SETUP
//
//void setupCallbacks()
//{
//	glutCloseFunc(cleanup);
//	glutDisplayFunc(display);
//	glutIdleFunc(idle);
//	glutReshapeFunc(reshape);
//	glutTimerFunc(0, timer, 0);
//}
//
//void checkOpenGLInfo()
//{
//	const GLubyte *renderer = glGetString(GL_RENDERER);
//	const GLubyte *vendor = glGetString(GL_VENDOR);
//	const GLubyte *version = glGetString(GL_VERSION);
//	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
//	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
//	std::cerr << "OpenGL version " << version << std::endl;
//	std::cerr << "GLSL version " << glslVersion << std::endl;
//}
//
//void setupOpenGL()
//{
//	checkOpenGLInfo();
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glDepthMask(GL_TRUE);
//	glDepthRange(0.0, 1.0);
//	glClearDepth(1.0);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//}
//
//void setupGLEW()
//{
//	glewExperimental = GL_TRUE;
//	GLenum result = glewInit();
//	if (result != GLEW_OK) {
//		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	GLenum err_code = glGetError();
//}
//
//void setupGLUT(int argc, char* argv[])
//{
//	glutInit(&argc, argv);
//
//	glutInitContextVersion(3, 3);
//	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//	glutInitContextProfile(GLUT_CORE_PROFILE);
//
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//
//	glutInitWindowSize(WinX, WinY);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	WindowHandle = glutCreateWindow(CAPTION);
//	if (WindowHandle < 1) {
//		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
//		exit(EXIT_FAILURE);
//	}
//}
//
//void init(int argc, char* argv[])
//{
//	setupGLUT(argc, argv);
//	setupGLEW();
//	setupOpenGL();
//	createShaderProgram();
//	createBufferObjects();
//	setupCallbacks();
//}
//
//int main(int argc, char* argv[])
//{
//	init(argc, argv);
//	glutMainLoop();
//	exit(EXIT_SUCCESS);
//}
//
/////////////////////////////////////////////////////////////////////////