#include <cmath>
#include <gl/glut.h>

#define PI 3.14159265

float camera_x = 50.0f, camera_y = 200.0f, camera_z = 50.0f;
float cameralook_x = 0.0f, cameralook_y =-1.0f, cameralook_z = 1.0f;


//
//����Ҫ˵��һ��camera_x,y,z�Ǳ�ʾ������İ���λ�ã�x,y,z���������������λ����ϵ�ľ���λ��
//��cameralook_x,y,z����û�����壬Ҫ ��camera_x,y,z �����Ӧ��cameralook_x,y,z��� һ��ʹ�òŻ������壬
//��ΪgluLookAt����������β��������£��������x�������y�������z���������Ҫ�������Ǹ����x��y��z��0.0f,1.0f,0.0f��
//ʵ������  �������Ҫ�������Ǹ����x��y��z  ��ȥ  �������x���������y���������z  =  ��ʾ����������һ������
//������    ��camera_x,y,z �����Ӧ��cameralook_x,y,z   ���   �ھ����������������  ���Է��֣������������������� ��ǡ�ñ���ˣ�cameralook_x,cameralook_y,cameralook_z��
//���Բ������ַ�������  cameralook_x,y,z  �������ã��ڳ�������Ը��ӱ�ݵĲ���������ĳ���
//


float upDownAngle = -PI/4.0;  //ȫ�ֱ�����ʾ�����̧ͷ�͵�ͷʱ�ĽǶ�
float leftRightAngle = PI;    //ȫ�ֱ�����ʾ�������ת����תʱ�ĽǶ�

float leftRightDeltaAngle = 0.0;   //ȫ�ֱ�����ʾ�������ת����תʱ  ÿ�α仯��  �Ƕ�
float upDownDeltaAngle = 0.0;      //ȫ�ֱ�����ʾ�����̧ͷ�͵�ͷʱ  ÿ�α仯��  �Ƕ�

int fb_isMove = 0;//���������ֱ��ж�������Ƿ����ƶ�����
int lr_isMove = 0;//���ֵ = 0����û�У�ֵ = 1��-1����
int ud_isMove = 0;//1���������ƶ���-1���������ƶ�


//myShape ��������һЩ��Ҫ��  ����ǰ׼��
void myShape(int w,int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(90, 1.0*(w / h), 1, 1000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	//��ʼ��һ���������λ�����ӽ�
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x+camera_x,cameralook_y+camera_y,cameralook_z+camera_z,
		0.0f, 0.1f, 0.0f);
}

//����� �ӽ� ��ת������ת
void leftOrRight(float angle)
{	
	cameralook_x = sin(angle);//����Ҫ���м򵥵ĽǶȼ�������ȡ������ӽ���ת��ת֮��ĳ��򣬲���cameralook_x,y,z���Ӧ�ı�
	cameralook_z = -cos(angle);
	glLoadIdentity();
	//�������Ժ��ٴ�����ת���Ժ�������λ���Լ��ӽǳ���
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}

//����� �ӽ� ̧ͷ���ߵ�ͷ
void upOrDown(float angle)
{
	cameralook_y = sin(angle);
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}

//����� λ�� ǰ�����ߺ���
void forwardOrBackwardMove(int fb_isMove)
{
	camera_x = camera_x + fb_isMove*(cameralook_x)*0.01;
	camera_z = camera_z + fb_isMove*(cameralook_z)*0.01;
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}

//����� λ�� ���ƻ�������
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

//����� λ�� ���������½�
void upOrDownMove(int ud_isMove) 
{
	camera_y = camera_y + ud_isMove*0.01;
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z,
		cameralook_x + camera_x, cameralook_y + camera_y, cameralook_z + camera_z,
		0.0f, 1.0f, 0.0f);
}


//��ʵ���ѷ��� ��������ӽǻ���λ�õĸı� ������һЩ�򵥵ĽǶ���������㣬���Ը��ݳ����г�ʽ�ӣ���ֽ��дд


//��ʵ���������������� �ӽǣ�λ�øı��Լ������������
void display(void)
{

	if (fb_isMove)//���fb_isMove��Ϊ0����Ҫ�ƶ���
		forwardOrBackwardMove(fb_isMove);
	
	if (lr_isMove)//ͬ��
		leftOrRightMove(lr_isMove);
	
	if (ud_isMove)//ͬ��
		upOrDownMove(ud_isMove);
	
	//���������תÿ�α仯�ĽǶ� ��= 0����˵���Ƕȱ仯��ִ��if������
	if (leftRightDeltaAngle) {
		leftRightAngle += leftRightDeltaAngle;
		leftOrRight(leftRightAngle);
	}
	
	//ͬ��
	if (upDownDeltaAngle) {
		
		upDownAngle += upDownDeltaAngle;
		
		//���������if�Ƿ�ֹ�Ƕ�̧�𳬽��γ�360��ת�������趨��̧��͵�ͷ�����ܴ���90��
		if (upDownAngle > PI / 2) {
			upDownAngle = PI / 2;
		}
		if (upDownAngle < -PI / 2) {
			upDownAngle = -PI / 2;
		}
		upOrDown(upDownAngle);
	}

	//�����������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);
	//�������ߺ������
	for (float i = 0.0; i < 600.0f; i += 10.0f) {
		glBegin(GL_LINES);
		glVertex3f(i, 0.0f, 0.0f);
		glVertex3f(i, 0.0f, 600.0f);
		glEnd();
	}
	//���������������
	for (float j = 0.0; j < 600.0f; j += 10.0f) {
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, j);
		glVertex3f(600.0f, 0.0f, j);
		glEnd();
	}
	glutSwapBuffers();
}

//��ȡһ�㰴���¼�
void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		if (fb_isMove == 1)//����Ѿ����ƶ������ٴΰ��¾�ֹͣ�����Ա�Ϊ0
			fb_isMove = 0;
		else
			fb_isMove = 1;//�����0����ʾû���ƶ������Ա��1�������ƶ�
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

//��ȡ�����λ�İ����¼������¼�����ת��
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

//��ȡ�����λ��̧���¼���̧��ֹͣ�ƶ�
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
	glutCreateWindow("Scene roaming program test ��������");
	
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutKeyboardFunc(mykeyboard);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(myShape);

	glutMainLoop();

}