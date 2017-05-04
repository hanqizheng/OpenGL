#include <cmath>
#include <gl/glut.h>

#define PI 3.14159265

float camera_x = 50.0f, camera_y = 200.0f, camera_z = 50.0f;
float cameralook_x = 0.0f, cameralook_y =-1.0f, cameralook_z = 1.0f;


//
//这里要说明一下camera_x,y,z是表示摄像机的摆设位置，x,y,z描述了摄像机在三位坐标系的具体位置
//而cameralook_x,y,z本身并没有意义，要 将camera_x,y,z 与相对应的cameralook_x,y,z相加 一并使用才会有意义，
//因为gluLookAt这个函数的形参设置如下：（摄像机x，摄像机y，摄像机z，摄像机想要看到的那个点的x，y，z，0.0f,1.0f,0.0f）
//实际上是  摄像机想要看到的那个点的x，y，z  减去  摄像机的x，摄像机的y，摄像机的z  =  表示摄像机朝向的一个向量
//而这里    将camera_x,y,z 与相对应的cameralook_x,y,z   相加   在经过上述的相减运算  可以发现，代表摄像机朝向的向量 就恰好变成了（cameralook_x,cameralook_y,cameralook_z）
//所以采用这种方法，对  cameralook_x,y,z  进行设置，在程序里可以更加便捷的操作摄像机的朝向
//


float upDownAngle = -PI/4.0;  //全局变量表示摄像机抬头和低头时的角度
float leftRightAngle = PI;    //全局变量表示摄像机左转和右转时的角度

float leftRightDeltaAngle = 0.0;   //全局变量表示摄像机左转和右转时  每次变化的  角度
float upDownDeltaAngle = 0.0;      //全局变量表示摄像机抬头和低头时  每次变化的  角度

int fb_isMove = 0;//三个变量分别判断摄像机是否有移动操作
int lr_isMove = 0;//如果值 = 0，则没有，值 = 1或-1则有
int ud_isMove = 0;//1代表正向移动，-1代表逆向移动


//myShape 函数进行一些必要的  程序前准备
void myShape(int w,int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(90, 1.0*(w / h), 1, 1000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	//初始化一个摄像机的位置与视角
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x+camera_x,cameralook_y+camera_y,cameralook_z+camera_z,
		0.0f, 0.1f, 0.0f);
}

//摄像机 视角 左转或者右转
void leftOrRight(float angle)
{	
	cameralook_x = sin(angle);//这里要进行简单的角度计算来获取摄像机视角左转右转之后的朝向，并将cameralook_x,y,z相对应改变
	cameralook_z = -cos(angle);
	glLoadIdentity();
	//计算完以后再次设置转动以后的摄像机位置以及视角朝向
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}

//摄像机 视角 抬头或者低头
void upOrDown(float angle)
{
	cameralook_y = sin(angle);
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}

//摄像机 位置 前进或者后退
void forwardOrBackwardMove(int fb_isMove)
{
	camera_x = camera_x + fb_isMove*(cameralook_x)*0.01;
	camera_z = camera_z + fb_isMove*(cameralook_z)*0.01;
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}

//摄像机 位置 左移或者右移
void leftOrRightMove(int lr_isMove) 
{
	camera_x = camera_x + lr_isMove*(cameralook_z)*0.01;
	camera_z = camera_z + lr_isMove*(-cameralook_x)*0.01;
	//camera_z = camera_z + lr_isMove*(-sin(PI-leftRightAngle))*0.01;
	//camera_x = camera_x + lr_isMove*(cos(PI-leftRightAngle))*0.01;
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}

//摄像机 位置 上升或者下降
void upOrDownMove(int ud_isMove) 
{
	camera_y = camera_y + ud_isMove*0.01;
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}


//其实不难发现 摄像机的视角或者位置的改变 伴随着一些简单的角度与坐标计算，可以根据程序列出式子，在纸上写写


//现实函数，完成摄像机的 视角，位置改变以及绘制网格地面
void display(void)
{

	if (fb_isMove)//如果fb_isMove不为0，就要移动，
		forwardOrBackwardMove(fb_isMove);
	
	if (lr_isMove)//同上
		leftOrRightMove(lr_isMove);
	
	if (ud_isMove)//同上
		upOrDownMove(ud_isMove);
	
	//如果左右旋转每次变化的角度 ！= 0，就说明角度变化，执行if语句里的
	if (leftRightDeltaAngle) {
		leftRightAngle += leftRightDeltaAngle;
		leftOrRight(leftRightAngle);
	}
	
	//同上
	if (upDownDeltaAngle) {
		
		upDownAngle += upDownDeltaAngle;
		
		//这里的两个if是防止角度抬起超界形成360旋转，所以设定了抬起和低头都不能大于90°
		if (upDownAngle > PI / 2) {
			upDownAngle = PI / 2;
		}
		if (upDownAngle < -PI / 2) {
			upDownAngle = -PI / 2;
		}
		upOrDown(upDownAngle);
	}

	//绘制网格地面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);
	//纵向竖线横向绘制
	for (float i = 0.0; i < 600.0f; i += 10.0f) {
		glBegin(GL_LINES);
		glVertex3f(i, 0.0f, 0.0f);
		glVertex3f(i, 0.0f, 600.0f);
		glEnd();
	}
	//横向竖线纵向绘制
	for (float j = 0.0; j < 600.0f; j += 10.0f) {
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, j);
		glVertex3f(600.0f, 0.0f, j);
		glEnd();
	}
	glutSwapBuffers();
}

//获取一般按键事件
void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		if (fb_isMove == 1)//如果已经在移动，那再次按下就停止，所以变为0
			fb_isMove = 0;
		else
			fb_isMove = 1;//如果是0，表示没有移动，所以变成1，进行移动
		break;
	case 's':
		if (fb_isMove == -1)
			fb_isMove = 0;
		else
			fb_isMove = -1;
		break;
	case 'a':
		if (lr_isMove == 1)
			lr_isMove = 0;
		else
			lr_isMove = 1;
		break;
	case 'd':
		if (lr_isMove == -1)
			lr_isMove = 0;
		else
			lr_isMove = -1;
		break;
	case 'r':
		if (ud_isMove == 1)
			ud_isMove = 0;
		else
			ud_isMove = 1;
		break;
	case 'f':
		if (ud_isMove == -1)
			ud_isMove = 0;
		else
			ud_isMove = -1;
		break;
	
	}
}

//获取特殊键位的按下事件，按下即进行转动
void pressKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		leftRightDeltaAngle = -0.001f; break;
	case GLUT_KEY_RIGHT:
		leftRightDeltaAngle = 0.001f; break;
	case GLUT_KEY_UP:
		upDownDeltaAngle = 0.001f; break;
	case GLUT_KEY_DOWN:
		upDownDeltaAngle = -0.001f; break;
	}
}

//获取特殊键位的抬起事件，抬起即停止移动
void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		leftRightDeltaAngle = 0.0f; break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		upDownDeltaAngle = 0.0f; break;
	}
}

//main
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 360);
	glutCreateWindow("Scene roaming program test 场景漫游");
	
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutKeyboardFunc(mykeyboard);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(myShape);

	glutMainLoop();

}