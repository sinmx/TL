#include <iostream>
#include <stdio.h>
#include <string.h>
// glew ��glut֮ǰ����
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <ogldev/ogldev_math_3d.h>
#include <ogldev/ogldev_util.h>

using namespace std;

GLuint VBO;
const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static void RenderScenceCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// ������������
	glEnableVertexAttribArray(0);
	// ��GL_ARRAY_BUFFER������
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	// ���߹��߽���bufer�е�����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// ��ʼ���Ƽ���ͼ�Σ���������������
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// ���ö�������
	glDisableVertexAttribArray(0);
	// ����ǰ�󻺴�
	glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
	// ��ʼ��Ⱦ
	glutDisplayFunc(RenderScenceCB);
}


void CreateVertexBuffer()
{
	// ��������3������Ķ�������
	Vector3f Vertices[3];
	// ����������Ļ����
	Vertices[0] = Vector3f(-1.0f,-1.0f,0.0f);
	Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

	// ����������
	glGenBuffers(1, &VBO);
	// �󶨻�����

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// �󶨶�������
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

// ʹ��shader�ı�����shader���󣬲���shader����ɫ��������
static int AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// ����shader���Ͳ�����������shader����
	GLuint ShaderObj = glCreateShader(ShaderType);
	// ����Ƿ���ɹ�
	if (ShaderObj == 0)
	{
		cout << "creating shader type ��" << ShaderType << endl;
		return -1;
	}

	// ����shader�Ĵ���Դ
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint  Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);// ����shader����

	// ���shader��ش���
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL,InfoLog);
		cout << "compiling shader type ��" << ShaderType << "infolog : "<< InfoLog << endl;
		return -1;
	}

	// ������õ�shader����󶨵�program object���������
	glAttachShader(ShaderProgram, ShaderObj);
}

// ������ɫ������
static int CompileShaders()
{
	// ������ɫ������
	GLuint ShaderProgram = glCreateProgram();

	// ����Ƿ񴴽��ɹ�
	if (ShaderProgram == 0)
	{
		cout << "creating shader program error" << endl;
		return -1;
	}

	// ��ɫ���ı��ַ�����
	string vs, fs;

	// �ֱ��ȡ��ɫ���ļ��е��ı����ַ���������
	if (!ReadFile(pVSFileName, vs))
	{
		return -1;
	}

	if (!ReadFile(pFSFileName, fs))
	{
		return -1;
	}

	// ��Ӷ�����ɫ����Ƭ����ɫ��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	// ����shader��ɫ�����򣬲������ش���
	GLint Success = 0;
	GLchar Errorlog[1024] = { 0 };
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram,GL_LINK_STATUS,&Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(Errorlog), NULL, Errorlog);
		cout << "linking shader program " << Errorlog << endl;
		return -1;
	}

	// �����֤��ǰ����״̬�Ƿ���Ա�ִ��
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(Errorlog), NULL, Errorlog);
		cout << "Invalid shader program : " << Errorlog << endl;
		return -1;
	}
	
	// ���ù�����������ʹ�����洴����shader����
	glUseProgram(ShaderProgram);
	return 0;
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);// ��ʼ��glut

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);// ��ʾģʽ ˫���壬rgba

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("window");

	InitializeGlutCallbacks();

	// ���glew�Ƿ������������glut��ʼ��֮��
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		cout << "glew ��ʼ�� err : " << glewGetErrorString(res);
		return -1;
	}
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// ���������ɫֵ
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);

	// �������㻺����
	CreateVertexBuffer();

	// ������ɫ��
	CompileShaders();
	// ֪ͨglut�ڲ�ѭ��
	glutMainLoop();
	return 0;
}
