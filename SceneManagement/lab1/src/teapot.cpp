/////////////////////////////////////////////////////////////////////////
////
//// Example:
//// Classic OpenGL Programming: drawing a Teapot using GLUT.
//// Compatibility mode.
////
//// (c)2013 by Carlos Martinho
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
//#define CAPTION "Hello Old World"
//
//int WinX = 640, WinY = 480;
//int WindowHandle = 0;
//unsigned int FrameCount = 0;
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
//	if(isOpenGLError()) {
//		std::cerr << error << std::endl;
//		exit(EXIT_FAILURE);
//	}
//}
//
///////////////////////////////////////////////////////////////////////// CALLBACKS
//
//void cleanup()
//{
//}
//
//void setViewProjectionMatrix()
//{
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(60.0, (GLdouble)WinX/(GLdouble)WinY, 1.0, 5.0);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(2.0f,0.75f,2.0f, 0.0f,0.75f,0.0f, 0.0f,1.0f,0.0f);
//}
//
//void drawAxis()
//{
//	glLineWidth(5.0f);
//	glPushMatrix();
//		glTranslatef(0.0f,0.0f,0.0f);
//		glScalef(1.0f,1.0f,1.0f);
//		glColor3f(0.9f,0.0f,0.0f);
//		glBegin(GL_LINES);
//			glVertex3f(0.0f,0.0f,0.0f);
//			glVertex3f(1.0f,0.0f,0.0f);
//		glEnd();
//		glColor3f(0.0f,0.9f,0.0f);
//		glBegin(GL_LINES);
//			glVertex3f(0.0f,0.0f,0.0f);
//			glVertex3f(0.0f,1.0f,0.0f);
//		glEnd();
//		glColor3f(0.0f,0.0f,0.9f);
//		glBegin(GL_LINES);
//			glVertex3f(0.0f,0.0f,0.0f);
//			glVertex3f(0.0f,0.0f,1.0f);
//		glEnd();
//	glPopMatrix();
//	glLineWidth(1.0f);
//}
//
//void drawTeapot()
//{
//	glColor3f(0.9f,0.9f,0.9f);
//	glPushMatrix();
//		glTranslatef(0.0f, 0.75f, 0.0f);
//		glutWireTeapot(1.0);
//	glPopMatrix();
//}
//
//void display()
//{
//	++FrameCount;
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	setViewProjectionMatrix();
//	drawAxis();
//	drawTeapot();
//	glutSwapBuffers();
//
//	checkOpenGLError("ERROR: Could not draw scene.");
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
//	glutSetWindowTitle(s.c_str());
//    FrameCount = 0;
//    glutTimerFunc(1000, timer, 0);
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
//	glutTimerFunc(0,timer,0);
//}
//
//void setupOpenGL() {
//	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
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
//void setupGLEW() {
//	glewExperimental = GL_TRUE;
//	GLenum result = glewInit() ; 
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
//	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
//	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//	//glutInitContextProfile(GLUT_CORE_PROFILE);
//
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//	
//	glutInitWindowSize(WinX, WinY);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	WindowHandle = glutCreateWindow(CAPTION);
//	if(WindowHandle < 1) {
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