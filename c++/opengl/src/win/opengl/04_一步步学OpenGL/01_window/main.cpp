#include <iostream>
#include <GL/freeglut.h>

//���� freeGLUT��GLEW
//GLee����������glfw
// ��Ⱦ����
void RenderScenceCB()
{
	// �����ɫ����
	glClear(GL_COLOR_BUFFER_BIT);

	// ����ǰ��Ļ���
	glutSwapBuffers();
}
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);// ��ʼ��glut

	// GLUT��ѡ�����á�GLUT_DOUBLE�ڶ�����Ⱦ��������˫������ƣ�ά������ͼ�񻺳����ݣ���Ļ��ʾһ��ͼ��ʱ�ں�̨ͬʱ������һ��ͼ�񻺳����ݣ�������ʾ������ɫ���塣����ͨ����Ҫ���������ã�����������ѡ�����ú�����������
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);// ��ʾģʽ: ˫���壬RGBA

	// ��������
	glutInitWindowSize(500, 500);// ���ڳߴ��С
	glutInitWindowPosition(100, 100);// ����λ��
	glutCreateWindow("01_window");// ���ڱ���

	// ������������һ������ϵͳ�й����ģ������еĳ�������Ľ�����ͨ���¼��ص�������GLUT�����ײ㴰��ϵͳ�Ľ���Ϊ�����ṩ�˼����ص�����ѡ�����������ֻ��һ�����ص������һ֡ͼ���������Ⱦ����������ص������᲻�ϵر�GLUT�ڲ�ѭ�����á�
	// ��ʼ��Ⱦ
	glutDisplayFunc(RenderScenceCB);// �ص�
	
	// ������պ����ɫֵ
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearColor(0.0f,0.2f, 0.2f, 0.2f);

	// ֪ͨ��ʼGLUT�ڲ�ѭ��
	glutMainLoop();
	return 0;
}