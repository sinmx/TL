#include <iostream>
// glew ��glut֮ǰ����
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <ogldev/ogldev_math_3d.h>
#include <ogldev/ogldev_util.h>

using namespace std;

GLuint VBO;

void RenderScenceCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// ������������
	glEnableVertexAttribArray(0);
	// ��GL_ARRAY_BUFFER������
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	// ���߹��߽���bufer�е�����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// ��ʼ���Ƽ���ͼ��
	glDrawArrays(GL_POINTS, 0, 1);
	// ���ö�������
	glDisableVertexAttribArray(0);
	// ����ǰ�󻺴�
	glutSwapBuffers();
}

void CreateVertexBuffer()
{
	// ��������һ������Ķ�������
	Vector3f Vertices[1];
	// ����������Ļ����
	Vertices[0] = Vector3f(0.0f,0.0f,0.0f);
	
	// ����������
	glGenBuffers(1, &VBO);
	// �󶨻�����

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �󶨶�������
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);// ��ʼ��glut

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);// ��ʾģʽ ˫���壬rgba

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("02_windows����");

	// ��ʼ��Ⱦ
	glutDisplayFunc(RenderScenceCB);

	// ���glew�Ƿ������������glut��ʼ��֮��
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		cout << "glew ��ʼ�� err : " << glewGetErrorString(res);
		return -1;
	}


	// ���������ɫֵ
	glClearColor(0.0f, 0.2f, 0.2f, 0.2f);

	// �������㻺����
	CreateVertexBuffer();

	// ֪ͨglut�ڲ�ѭ��
	glutMainLoop();
	return 0;
}
