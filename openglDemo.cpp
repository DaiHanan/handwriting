//#include<windows.h>
//
//#include<GL/glut.h>
//
//#include<GL/gl.h>
//
///*����ͷ�ļ�һ���Ǳ���ӵģ�windows.hͷ�ļ���Ϊ����windowsϵͳ�ӿ���ϵ����*/
//
//void init(void)
//
//{
//
//	glClearColor(1.0, 1.0, 1.0, 0.0);//������ɫ��RGBΪ111����ɫ
//
//	glMatrixMode(GL_PROJECTION);//�趨ͶӰ��ʽ
//
//	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
//
//	/*�趨ӳ�����򣬲���Ϊ(minX,maxX,minY,maxY),(minX,minYΪԭ��)��
//
//	��maxX,maxY��Ϊ���Ͻǵĵ�*/
//
//
//
//}
//
//void linesegment(void)
//
//{
//
//	//�趨����
//
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glColor3f(0.0, 0.4, 0.2);//�趨�����RGB��ɫ
//
//	glBegin(GL_LINES);//��ʾһ��֮�Ҷϣ���˵�������glVertex������ָ��
//
//	glVertex2i(180, 15);//ָ����һ����
//
//	glVertex2i(10, 145);//ָ���ڶ�����
//
//	glEnd();//����
//
//	glFlush();
//}
//
//int main(int argc, char** argv)
//{
//
//	glutInit(&argc, argv);//��ʼ��glut
//
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//
//	glutInitWindowPosition(50, 100);//������ʾ���ڵ����Ͻǳ�ʼλ��
//
//	glutInitWindowSize(400, 300);//���ô��ڴ�С�������ֱ�Ϊ��͸�
//
//	glutCreateWindow("AnExample");//����
//
//	init();
//
//	glutDisplayFunc(linesegment);
//
//	glutMainLoop();
//
//	return 0;
//}