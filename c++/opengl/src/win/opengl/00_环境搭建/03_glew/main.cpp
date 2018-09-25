#include<GL/glew.h>
#include<GL/freeglut.h>
#include<iostream>

using namespace std;

//�ı䴰�ڴ�С
void changeViewport(int w, int h)
{
	glViewport(0, 0, w, h);
}

//��Ⱦ
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	//��ʼ��
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow("LJLHelloWord");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("GLEW ERROR");
		return 1;
	}

	glutMainLoop();

	return 0;
}
