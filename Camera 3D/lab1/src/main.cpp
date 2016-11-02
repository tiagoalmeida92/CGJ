/////////////////////////////////////////////////////////////////////////
////
//// Assignment consists in the following:
////
//// - Create the following changes to your scene, making it fully 3D:
////   - Extrude your TANs into the 3rd dimension. The TANs should have
////     slightly different "heights".
////   - The new faces of each TAN should share the same hue as the 
////     original top face color but have different levels of saturation 
////     and brightness (use an external app if needed).
////
//// - Add the following functionality:
////   - Create a View Matrix from (eye, center, up) parameters.
////   - Create an Orthographic Projection Matrix from (left-right, 
////     bottom-top, near-far) parameters.
////   - Create a Perspective Projection Matrix from (fovy, aspect,
////     nearZ, farZ) parameters.
////
//// - Add the following dynamics to the application:
////   - Create a free 3D camera controlled by the mouse allowing to 
////     visualize the scene through all its angles.
////   - Change perspective from orthographic to perspective and back as
////     a response to pressing the key 'p'.
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
//#include "gl_shader.hpp"
//#include "vec.hpp"
//#include "matrix_factory.hpp"
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
//GLuint VaoId, VboId[3];
//GLint ProgramId;
//Shader shader;
//
//GLint UboId, UniformId;
//const GLuint UBO_BP = 0;
//GLint UniformColorId;
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
//
//void createShaderProgram()
//{
//	shader = CreateProgram("glscripts/triangle_new.vert", "glscripts/triangle.frag");
//	if (shader.compiled) {
//		ProgramId = shader.ProgramId;
//
//		BindAttributeLocation(ProgramId, VERTICES, "in_Position");
//		BindAttributeLocation(ProgramId, COLORS, "in_Color");
//
//		UniformColorId = GetUniformLocation(ProgramId, "Color");
//
//		UniformId = GetUniformLocation(ProgramId, "ModelMatrix");
//		UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
//		glUniformBlockBinding(ProgramId, UboId, UBO_BP);
//
//	}
//	checkOpenGLError("ERROR: Could not create shaders." );
//}
//
//void destroyShaderProgram()
//{
//	glUseProgram(0);
//	DestroyShader(shader);
//
//	checkOpenGLError("ERROR: Could not destroy shaders.");
//}
//
///////////////////////////////////////////////////////////////////////// VAOs & VBOs
//
////const Vec2 Vertices[] =
////{
////	Vec2{ 0.0f, -0.25f },	//0
////	Vec2{ 0.25f, -0.25f},
////	Vec2{ 0.0f, 0.25f},
////	Vec2{ -0.25f, -0.25f}, //2
////	Vec2{ 0.25f, -0.25f},
////	Vec2{ -0.25f, 0.25f},
////	Vec2{ 0.25f, 0.25f},  //6
////	Vec2{ -0.5f, -0.25f},
////	Vec2{ 0.5f, 0.25f},   //8
////};
//
//const Vec3 Vertices[] =
//{
//	Vec3{ 0.0f, -0.25f, 0.0f },	//0
//	Vec3{ 0.25f, -0.25f, 0.0f },
//	Vec3{ 0.0f, 0.25f, 0.0f },
//	Vec3{ -0.25f, -0.25f, 0.0f }, //3
//	Vec3{ 0.25f, -0.25f, 0.0f },
//	Vec3{ -0.25f, 0.25f, 0.0f },
//	Vec3{ 0.25f, 0.25f, 0.0f },  //6
//	Vec3{ -0.5f, -0.25f, 0.0f },
//	Vec3{ 0.5f, 0.25f, 0.0f },   //8
//
//
//	Vec3{ 0.0f, -0.25f, 1.0f },	//9
//	Vec3{ 0.25f, -0.25f, 1.0f },
//	Vec3{ 0.0f, 0.25f, 1.0f },
//	Vec3{ -0.25f, -0.25f, 1.0f }, //12
//	Vec3{ 0.25f, -0.25f, 1.0f },
//	Vec3{ -0.25f, 0.25f, 1.0f },
//	Vec3{ 0.25f, 0.25f, 1.0f },  //15
//	Vec3{ -0.5f, -0.25f, 1.0f },
//	Vec3{ 0.5f, 0.25f, 1.0f }   //17
//
//};
//
//const GLubyte Indices[] =
//{
//	0,1,2, //Right triangle
//		   //Square
//	3,4,5,  //Triangle 1
//	4,6,5,  //Triangle 2
//			//Paralelogram
//	7,1,8,  //Triangle 1
//	7,8,5	//Triangle 2
//	,9,10,11,12,13,14,15,16,17
//
//};
//
//
//const GLfloat Colors[] {
//	0.5f, 0.5f, 1.0f, 1.0f,
//	0.0f, 0.5f, 0.5f, 1.0f,
//	0.75f, 0.5f, 0.0f, 1.0f,
//	0.75f, 0.0f, 0.5f, 1.0f,
//	1.0f, 1.0f, 0.0f, 1.0f,
//	0.0f, 1.0f, 1.0f, 1.0f,
//	0.5f, 0.0f, 0.5f, 1.0f,
//};
//
//
//
//void createBufferObjects()
//{
//	glGenVertexArrays(1, &VaoId);
//	glBindVertexArray(VaoId);
//	{
//		glGenBuffers(3, VboId);
//
//		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//		glEnableVertexAttribArray(VERTICES);
//		glVertexAttribPointer(VERTICES, 2, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
//		
//		//glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
//		//glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
//		//glEnableVertexAttribArray(COLORS);
//		//glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, 0, 0);
//
//
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
//
//		glBindBuffer(GL_UNIFORM_BUFFER, VboId[2]);
//		glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4) * 2, 0, GL_STREAM_DRAW);
//		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId[2]);
//	}
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
//}
//
//void destroyBufferObjects()
//{
//	glBindVertexArray(VaoId);
//	glDisableVertexAttribArray(VERTICES);
//	glDisableVertexAttribArray(COLORS);
//	glDeleteBuffers(3, VboId);
//	glDeleteVertexArrays(1, &VaoId);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//	glBindVertexArray(0);
//
//	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
//}
//
///////////////////////////////////////////////////////////////////////// SCENE
//
//
//#define PI 3.14159265358979f
//
//Mat4 I = identity4();
//Mat4 translateAbitLeft = translate(Vec3(-0.1f, 0.0f, 0));
//Mat4 translateLeft = translate(Vec3(-0.25f, 0.0f, 0));
//Mat4 translateTopLeft = translate(Vec3(-0.3f, 0.5f, 0));
//Mat4 big_triangle_1_translate = translate(Vec3(0.0f, -0.5f, 0));
//Mat4 big_triangle_2_translate = translate(Vec3(-0.005f, -0.83f, 0));
//Mat4 translate25Right = translate(Vec3(0.25f, 0.0f, 0));
//Mat4 translateRight = translate(Vec3(0.5f, 0.0f, 0));
//Mat4 translateBottom = translate(Vec3(0.0f, -0.5f, 0));
//Mat4 translateAlotBottom = translate(Vec3(0.0f, -0.65f, 0));
//Mat4 smallTriangle1Translate = translate(Vec3(-0.2f, 0.69f, 0));
//Mat4 smallTriangle2Translate = translate(Vec3(0.05f, 0.69f, 0));
//Mat4 translateTop = translate(Vec3(0.0f, 0.5f, 0));
//
//Mat4 translateMediumTriangle = translate(Vec3(0.5f, 0.0f, 0));
//Mat4 translate25Top = translate(Vec3(0.0f, 0.25f, 0));
//Mat4 translate25Bottom = translate(Vec3(0.0f, -0.25f, 0));
//Mat4 scaleMedium = scaling4(Vec3(1.0f, 1.0f, 0));
//Mat4 scaleNegative = scaling4(Vec3(-1.0f, -1.0f, 0));
//Mat4 scaleBig = scaling4(Vec3(1.3f, 1.3f, 0));
//Mat4 scaleSmall = scaling4(Vec3(0.75f, 0.75f, 0));
//Mat4 rotate90Left = rotate4(Vec3(0.0f, 0.0f, 1.0f), PI/2);
//Mat4 rotateRight = rotate4(Vec3(0.0f, 0.0f, 1.0f), ((3*PI) / 2));
//Mat4 rotate180 = rotate4(Vec3(0.0f, 0.0f, 1.0f), PI);
//
//
//Mat4 ModelMatrix = {
//	1.0f,  0.0f,  0.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,  0.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f,  1.0f
//}; 
//
//   // Eye(5,5,5) Center(0,0,0) Up(0,1,0)
//Mat4 ViewMatrix1 = {
//	0.70f, -0.41f,  0.58f,  0.00f,
//	0.00f,  0.82f,  0.58f,  0.00f,
//	-0.70f, -0.41f,  0.58f,  0.00f,
//	0.00f,  0.00f, -8.70f,  1.00f
//}; 
//
//   // Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
//Mat4 ViewMatrix2 = {
//	-0.70f, -0.41f, -0.58f,  0.00f,
//	0.00f,  0.82f, -0.58f,  0.00f,
//	0.70f, -0.41f, -0.58f,  0.00f,
//	0.00f,  0.00f, -8.70f,  1.00f
//}; 
//
//// Orthographic LeftRight(-2,2) TopBottom(-2,2) NearFar(1,10)
//Mat4 ProjectionMatrix1 = {
//	0.50f,  0.00f,  0.00f,  0.00f,
//	0.00f,  0.50f,  0.00f,  0.00f,
//	0.00f,  0.00f, -0.22f,  0.00f,
//	0.00f,  0.00f, -1.22f,  1.00f
//}; // Column Major
//
//// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
//Mat4 ProjectionMatrix2 = {
//	2.79f,  0.00f,  0.00f,  0.00f,
//	0.00f,  3.73f,  0.00f,  0.00f,
//	0.00f,  0.00f, -1.22f, -1.00f,
//	0.00f,  0.00f, -2.22f,  0.00f
//};
//
//int color = 0;
//void bindNewColor() {
//	glUniform4fv(UniformColorId, 1, &Colors[color*4]);
//	color++;
//}
//
//#define TRIANGLE_INDEX (void*)0
//#define SQUARE_INDEX (void*)3
//#define PARALLELOGRAM_INDEX (void*)9
//
//void drawTriangles() {
//
//	bindNewColor();
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, (big_triangle_1_translate * rotateRight * scaleBig).convert_opengl());
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, TRIANGLE_INDEX);
//	
//	bindNewColor();
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, (big_triangle_2_translate * rotateRight * scaleNegative * scaleBig).convert_opengl());
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, TRIANGLE_INDEX);
//
//	bindNewColor();
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, (translateMediumTriangle * rotateRight * scaleMedium).convert_opengl());
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, TRIANGLE_INDEX);
//
//	bindNewColor();
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, (smallTriangle1Translate * scaleSmall).convert_opengl());
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, TRIANGLE_INDEX);
//
//	bindNewColor();
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, (smallTriangle2Translate * scaleSmall).convert_opengl());
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, TRIANGLE_INDEX);
//}
//
//void drawSquares() {
//	bindNewColor();
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, (translate25Bottom).convert_opengl());
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, SQUARE_INDEX);
//}
//
//void drawParalelogram() {
//	bindNewColor();
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, (translate25Top).convert_opengl());
//
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, PARALLELOGRAM_INDEX);
//}
//
//void drawScene()
//{
//	if (shader.compiled == false) {
//		return;
//	}
//
//	color = 0;
//	
//
//	glBindBuffer(GL_UNIFORM_BUFFER, VboId[2]);
//	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMatrix1.data);
//	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), ProjectionMatrix2.data);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//	glBindVertexArray(VaoId);
//	glUseProgram(ProgramId);
//
//	glUniformMatrix4fv(UniformId, 1, GL_FALSE, ModelMatrix.data);
//	
//	drawSquares();
//	drawParalelogram();
//	drawTriangles();
//	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, (void*)15);
//
//	glUseProgram(0);
//	glBindVertexArray(0);
//
//	checkOpenGLError("ERROR: Could not draw scene.");
//}
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
//	system("pause");
//	exit(EXIT_SUCCESS);
//}
//
/////////////////////////////////////////////////////////////////////////