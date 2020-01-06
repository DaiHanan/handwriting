//#include<windows.h>
//
//#include<GL/glut.h>
//
//#include<GL/gl.h>
//
///*三个头文件一般是必须加的，windows.h头文件是为了与windows系统接口联系起来*/
//
//void init(void)
//
//{
//
//	glClearColor(1.0, 1.0, 1.0, 0.0);//背景颜色，RGB为111，白色
//
//	glMatrixMode(GL_PROJECTION);//设定投影方式
//
//	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
//
//	/*设定映射区域，参数为(minX,maxX,minY,maxY),(minX,minY为原点)；
//
//	（maxX,maxY）为右上角的点*/
//
//
//
//}
//
//void linesegment(void)
//
//{
//
//	//设定曲线
//
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glColor3f(0.0, 0.4, 0.2);//设定物体的RGB颜色
//
//	glBegin(GL_LINES);//显示一组之弦断，其端点坐标在glVertex函数中指定
//
//	glVertex2i(180, 15);//指定第一个点
//
//	glVertex2i(10, 145);//指定第二个点
//
//	glEnd();//结束
//
//	glFlush();
//}
//
//int main(int argc, char** argv)
//{
//
//	glutInit(&argc, argv);//初始化glut
//
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//
//	glutInitWindowPosition(50, 100);//设置显示窗口的左上角初始位置
//
//	glutInitWindowSize(400, 300);//设置窗口大小，参数分别为宽和高
//
//	glutCreateWindow("AnExample");//标题
//
//	init();
//
//	glutDisplayFunc(linesegment);
//
//	glutMainLoop();
//
//	return 0;
//}