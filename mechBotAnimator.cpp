/*******************************************************************
Dawson Vo
500879711
CPS511-01
********************************************************************/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include "mechBotAnimator.h"
#include "subdivcurve.h"
#include "Robot3D.h"
bool gameStart = false;
int numCirclePoints = 30;
double circleRadius = 0.2;
int hoveredCircle = -1;
int animate = 0; // 0 - stop, 1 - animate
int delay = 30; // milliseconds // animation handler
float canonPositionX = 0;
float canonPositionY = 10;
float canonMoveSpeed = 0.4;
bool canonLaserLive = false;
float canonLaserLength = 20.0;
float canonLaserSpeed = 7.0;
float canonLaserDuration = 0.0;
float canonLaserMaxDuration = 50.0;
float canonLaserCurrLength = 0;

bool FPV = false;
bool turretHit = false;
GLdouble randomSequence[5];

int numBots = 2;
int score = 0;
int botsleft = numBots;
Robot bots[9]; // max 9 bots


GLdouble worldLeft = -24;
GLdouble worldRight = 24;
GLdouble worldBottom = -18;
GLdouble worldTop = 18;
GLdouble worldCenterX = 0.0;
GLdouble worldCenterY = 0.0;
GLdouble wvLeft = -24;
GLdouble wvRight = 24;
GLdouble wvBottom = -18;
GLdouble wvTop = 18;

GLint glutWindowWidth = 800;
GLint glutWindowHeight = 600;
GLint viewportWidth = glutWindowWidth;
GLint viewportHeight = glutWindowHeight;

// Ground Mesh material
GLfloat groundMat_ambient[]    = {0.4, 0.4, 0.4, 1.0};
GLfloat groundMat_specular[]   = {0.01, 0.01, 0.01, 1.0};
GLfloat groundMat_diffuse[]   = {0.4, 0.4, 0.7, 1.0};
GLfloat groundMat_shininess[]  = {1.0};

GLfloat light_position0[] = {4.0, 8.0, 8.0, 1.0};
GLfloat light_diffuse0[] = {1.0, 1.0, 1.0, 1.0};

GLfloat light_position1[] = {-4.0, 8.0, 8.0, 1.0};
GLfloat light_diffuse1[] = {1.0, 1.0, 1.0, 1.0};

GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat model_ambient[]  = {0.5, 0.5, 0.5, 1.0};

// 
GLdouble spin  = 0.0;



// Use circles to **draw** (i.e. visualize) subdivision curve control points
Circle circles[MAXCONTROLPOINTS];

int lastMouseX;
int lastMouseY;
int windowSub, windowMain, windowScore;
int windowMainSizeX = 800, windowMainSizeY = 600;
GLdouble aspect = (GLdouble)windowMainSizeX / windowMainSizeY;
GLdouble eyeX = 0.0, eyeY = 6.0, eyeZ = 22.0;
GLdouble zNear = 0.1, zFar = 40.0;
GLdouble fov = 60.0;

int main(int argc, char* argv[])
{
	glutInit(&argc, (char **)argv); 
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(glutWindowWidth,glutWindowHeight);
	glutInitWindowPosition(50,100);  

	srand((unsigned int)time(NULL));
	initRobot();

	// The main Window
	windowMain = glutCreateWindow("Mech Bot"); 
		glutPositionWindow(900,100);  
		glutDisplayFunc(displayMain);
		glutReshapeFunc(reshape3D);
		glutMouseFunc(mouseButtonHandler3D);
		glutMouseWheelFunc(mouseScrollWheelHandler3D);
		glutMotionFunc(mouseMotionHandler3D);
		//accept keyboard input on both 3d window as well
		glutKeyboardFunc(keyboardHandler);
		glutSpecialFunc(specialKeyHandler);
		// Initialize the 3D system
		init3DSurfaceWindow();
		initTextures();
		initRobotTexture();

	// The other view Window
	windowSub = glutCreateSubWindow(windowMain, 30, 30, windowMainSizeX * 0.3, windowMainSizeY * 0.3);
		glClearColor(0.4F, 0.4F, 0.4F, 0.0F);
		glutDisplayFunc(displaySub);
		glutReshapeFunc(reshape3D);
		initTextures();
		initRobotTexture();

	// The score window
	windowScore = glutCreateSubWindow(windowMain, windowMainSizeX - (windowMainSizeX * 0.2), 30, windowMainSizeX * 0.2, windowMainSizeY * 0.2);
		glClearColor(0.4F, 0.4F, 0.4F, 0.0F);
		glClearDepth(1.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_LIGHT0);

		glutDisplayFunc(displayScore);



	// Annnd... ACTION!!
	glutMainLoop(); 

	return 0;
}

void initRobotTexture()
{
	for (int i = 0; i < 9; i++)
	{
		bots[i].initRobotTexture();
	}
}
void initRobot()
{
	for (int i = 0; i < numBots; i++)
	{
		generateRandom(randomSequence);
		bots[i].initSubdivisionCurve(randomSequence);
	}
	updateCanonPosition();
}
void generateRandom(GLdouble arr [10])
{
	arr[0] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[1] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[2] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[3] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[4] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[5] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[6] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[7] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[8] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	arr[9] = (-10) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-10))));
	
}


RGBpixmap pix[20];
GLuint texture[20];
GLUquadric* body = gluNewQuadric();
void initTextures()
{
	//glClearColor(0.3, 0.1, 0.2, 0.0);
	//glClearDepth(1.0);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(20, texture);

	pix[0].readBMPFile("ground.bmp"); // body
	glBindTexture(GL_TEXTURE_2D, texture[0]); // select current texture (0)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pix[0].nCols, // image width
		pix[0].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pix[0].pixel); // the pixels

	pix[1].readBMPFile("turretalive.bmp"); //tire
	glBindTexture(GL_TEXTURE_2D, texture[1]); // select current texture (0)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pix[1].nCols, // image width
		pix[1].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pix[1].pixel); // the pixels

	pix[2].readBMPFile("turretdead.bmp"); //tire
	glBindTexture(GL_TEXTURE_2D, texture[2]); // select current texture (0)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pix[2].nCols, // image width
		pix[2].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pix[2].pixel); // the pixels

	pix[3].readBMPFile("beam.bmp"); //tire
	glBindTexture(GL_TEXTURE_2D, texture[3]); // select current texture (0)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pix[3].nCols, // image width
		pix[3].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pix[3].pixel); // the pixels
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);
}

void keyboardHandler(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case 27:
		// Esc, q, or Q key = Quit 
		exit(0);
		break;
	case '1':
		numBots = 1;
		resetGame();
		break;
	case '2':
		numBots = 2;
		resetGame();
		break;
	case '3':
		numBots = 3;
		resetGame();
		break;
	case '4':
		numBots = 4;
		resetGame();
		break;
	case '5':
		numBots = 5;
		resetGame();
		break;
	case '6':
		numBots = 6;
		resetGame();
		break;
	case '7':
		numBots = 7;
		resetGame();
		break;
	case '8':
		numBots = 8;
		resetGame();
		break;
	case '9':
		numBots = 9;
		resetGame();
		break;
	case 'A':
	case 'a':
		// Add code to create timer and call animation handler
		if (gameStart == false)
		{
			gameStart = true;
			glutTimerFunc(delay, collisionDetection, 0);
			glutTimerFunc(delay, animationHandler, 0);
			glutTimerFunc(delay, animationHandlerShooting, 0);
			// Use this to set to 3D window and redraw it
			glutSetWindow(windowMain);
			glutPostRedisplay();
			glutSetWindow(windowSub);
			glutPostRedisplay();
		}
		break;
	case 'R':
	case 'r':
		// reset object position at beginning of curve
		resetGame();
		//.......
		break;
	case 'V':
	case 'v':
		if (FPV == true)
		{
			FPV = false;
		}
		else
		{
			FPV = true;
		}
		break;
	case ' ':
		if (canonLaserLive == false && gameStart == true && turretHit == false) {

			canonLaserLive = true;
			glutTimerFunc(delay, animationHandlerTurretShooting, 0);
			// Use this to set to 3D window and redraw it
			glutSetWindow(windowMain);
			glutPostRedisplay();
			glutSetWindow(windowSub);
			glutPostRedisplay();
		}
	}
	glutSetWindow(windowMain);
	glutPostRedisplay();
	glutSetWindow(windowSub);
	glutPostRedisplay();
}

void resetGame()
{
	// stop animation
	botsleft = numBots;
	gameStart = false;
	turretHit = false;
	canonLaserLive = false;
	canonPositionX = 0;
	for (int i = 0; i < numBots; i++)
	{
		bots[i].currentCurvePoint = 0;
		bots[i].robotLive = true;
		bots[i].bulletLive = false;
		bots[i].scale = 1.3;
	}
	initRobot();
}
void updateCanonPosition()
{
	for (int i = 0; i < numBots; i++)
	{
		bots[i].canonPositionX = canonPositionX;
		bots[i].canonPositionY = canonPositionY;
	}
}

void specialKeyHandler(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		// add code here
		if (canonPositionX > -10 && turretHit == false) {
			canonPositionX -= canonMoveSpeed;
			updateCanonPosition();
			glutSetWindow(windowMain);
			glutPostRedisplay();
			glutSetWindow(windowSub);
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_RIGHT:
		// add code here;
		if (canonPositionX < 10 && turretHit == false) {
			canonPositionX += canonMoveSpeed;
			updateCanonPosition();
			glutSetWindow(windowMain);
			glutPostRedisplay();
			glutSetWindow(windowSub);
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_F1:
		printf("---------Difficulty------------\n\n");
		printf("1-9 - Select number of bots\n\n");
		printf("---------Controls------------\n\n");
		printf("Left Arrow - Move Turret Left\n");
		printf("Right Arrow - Move Turret Right\n");
		printf("Spacebar - Shoot Turret Laser\n");
		printf("A - Start Game\n");
		printf("R - Reset Game\n\n");
		printf("---------View------------\n\n");
		printf("V - Switch between 1st person and 3rd person\n");
		printf("Scrollwheel - Zoom while in 3rd person\n");
		printf("Rightclick + drag - Zoom while in 3rd person\n");
		break;

	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glutWindowWidth = (GLsizei) w;
	glutWindowHeight = (GLsizei) h;
	glViewport(0, 0, glutWindowWidth, glutWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(wvLeft, wvRight, wvBottom, wvTop);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void init3DSurfaceWindow()
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear

	glViewport(0, 0, (GLsizei)windowMainSizeX, (GLsizei)windowMainSizeY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void reshape3D(int w, int h)
{
	glutWindowWidth = (GLsizei) w;
	glutWindowHeight = (GLsizei) h;
	glViewport(0, 0, glutWindowWidth, glutWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,aspect,zNear,zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}



void displayMain()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (FPV == true)
	{
		gluLookAt(canonPositionX, 1, canonPositionY, canonPositionX, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	else
	{
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	drawGround();
	drawBot();
	drawTurret();
	glutSwapBuffers();
}

void displaySub()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (FPV == false)
	{
		gluLookAt(canonPositionX, 1, canonPositionY, canonPositionX, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	else
	{
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	drawGround();
	drawBot();
	drawTurret();
	glutSwapBuffers();
}

void displayScore()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	char lvlText[100];
	char scoreText[100];
	char botLeftText[100];

	sprintf(lvlText, "Bot Level: %d", numBots);
	sprintf(scoreText, "Score: %d", score);
	sprintf(botLeftText, "Remaining: %d", botsleft);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3f(-0.8f, 0.4f, 0.0f);
	for (int i = 0; lvlText[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lvlText[i]);
	}

	glRasterPos3f(-0.8f, -0.4f, 0.0f);
	for (int i = 0; scoreText[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
	}

	glRasterPos3f(-0.8f, 0.0f, 0.0f);
	for (int i = 0; botLeftText[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, botLeftText[i]);
	}

	glEnable(GL_LIGHTING);
	glFlush();
	glutSwapBuffers();
}


GLfloat robotBody_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotBody_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat robotBody_mat_diffuse[] = { 0.1f,0.35f,0.1f,1.0f };
GLfloat robotBody_mat_shininess[] = { 20.0F };

void drawBot()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, robotBody_mat_shininess);
		

	glPushMatrix();
	for (int i = 0; i < numBots; i++)
	{
		bots[i].drawRobot();
	}
	glPopMatrix();
	glPopMatrix();
}

GLfloat turretBody_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat turretBody_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat turretBody_mat_diffuse[] = { 0.35f,0.35f,0.35f,1.0f };
GLfloat turretBody_mat_shininess[] = { 20.0F };
GLfloat turretBodyHit_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat turretBodyHit_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat turretBodyHit_mat_diffuse[] = { 0.35f,0.1f,0.1f,1.0f };
GLfloat turretBodyHit_mat_shininess[] = { 20.0F };
void drawTurret() 
{
	glPushMatrix();
		
		glTranslatef(canonPositionX, 0, 0);
		drawLaser();
		if (turretHit == true)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, turretBodyHit_mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, turretBodyHit_mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, turretBodyHit_mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, turretBodyHit_mat_shininess);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, turretBody_mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, turretBody_mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, turretBody_mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, turretBody_mat_shininess);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
		}

		glPushMatrix();
			glTranslatef(0,0,canonPositionY);
			glRotatef(90, 1, 0, 0);
			glScalef(1, 1, 1);
			gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
			gluDisk(gluNewQuadric(), 0, 1.0, 20, 20);
			glTranslatef(0, 0, 1.0);
			gluDisk(gluNewQuadric(), 0, 1.0, 20, 20);
			glTranslatef(0, 0, -1.0);
			gluSphere(gluNewQuadric(), 0.8, 30, 30);
			gluCylinder(gluNewQuadric(), 1, 1.5, 1, 20, 10);
			glTranslatef(0, 0, -0.5);
			glRotatef(90, 1, 0, 0);
			if (turretHit == true)
			{
				glRotatef(-45, 1, 0, 0);
				glRotatef(-45, 0, 1, 0);
			}
			gluCylinder(gluNewQuadric(), 0.25, 0.25, 1.5, 20, 10);
			gluCylinder(gluNewQuadric(), 0.15, 0.15, 2.5, 20, 10);
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}

void drawGround() {
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, groundMat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, groundMat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, groundMat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, groundMat_shininess);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-24.0f, -1.0f, -24.0f);
	glVertex3f(-24.0f, -1.0f, 24.0f);
	glVertex3f(24.0f, -1.0f, 24.0f);
	glVertex3f(24.0f, -1.0f, -24.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}
int currentButton;

void mouseButtonHandler3D(int button, int state, int x, int y)
{

	currentButton = button;
	lastMouseX = x;
	lastMouseY = y;
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		
		break;
	case GLUT_MIDDLE_BUTTON:
		
		break;
	default:
		break;
	}
}

void mouseScrollWheelHandler3D(int button, int dir, int xMouse, int yMouse)
{
	switch(dir)
	{
		case 1:
			eyeZ -= 1;
			break;

		case -1:
			eyeZ += 1;
			break;
		default:
			break;
	}
	glutSetWindow(windowMain);
	glutPostRedisplay();
	glutSetWindow(windowSub);
	glutPostRedisplay();
	return;

}

void mouseMotionHandler3D(int x, int y)
{
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;
	if (currentButton == GLUT_LEFT_BUTTON) {
		
	}
	if (currentButton == GLUT_RIGHT_BUTTON) 
	{
		if (dy > 0) {
			eyeZ -= 0.05;
		}
		if (dy < 0) {
			eyeZ += 0.05;
		}
	}
	else if (currentButton == GLUT_MIDDLE_BUTTON) 
	{
	}
	lastMouseX = x;
	lastMouseY = y;
	glutPostRedisplay();

}

// Animation Handlers
void animationHandler(int param)
{	
	for (int i = 0; i < numBots; i++)
	{
		if (bots[i].robotLive == true && bots[i].currentCurvePoint < bots[i].subcurve.numCurvePoints - 1 && gameStart == true)
		{
			bots[i].currentCurvePoint++;
			bots[i].robotWheelAngle += 10;
		}
		else if (bots[i].currentCurvePoint == bots[i].subcurve.numCurvePoints - 1 && bots[i].scale > 0)
		{
			bots[i].scale -= 0.1;
		}
	}
	if (gameStart == true)
	{
		glutSetWindow(windowMain);
		glutPostRedisplay();
		glutSetWindow(windowSub);
		glutPostRedisplay();
		glutSetWindow(windowScore);
		glutPostRedisplay();
		glutTimerFunc(delay, animationHandler, 0);
	}
}
void animationHandlerShooting(int param)
{
	for (int i = 0; i < numBots; i++)
	{
		if (rand() % 100 < 5 && bots[i].bulletLive == false && bots[i].robotLive == true)
		{
			bots[i].shoot();
			bots[i].bulletLive = true;
		}
		if (bots[i].bulletLive == true)
		{
			bots[i].shoot();
		}

	}
	if (gameStart == true)
	{
		glutSetWindow(windowMain);
		glutPostRedisplay();
		glutSetWindow(windowSub);
		glutPostRedisplay();
		glutSetWindow(windowScore);
		glutPostRedisplay();
		glutTimerFunc(delay, animationHandlerShooting, 0);
	}
}

GLfloat laser_mat_ambient[] = { 0.1f,1.0f,1.0f,1.0f };
GLfloat laser_mat_specular[] = { 0.1f,1.0f,1.0f,1.0f };
GLfloat laser_mat_diffuse[] = { 0.1f,1.0f,1.0f,1.0f };
GLfloat laser_mat_shininess[] = { 2.0F };
void drawLaser()
{
	if (canonLaserLive == true && turretHit == false) {
		
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, laser_mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, laser_mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, laser_mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, laser_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[3]);

		glTranslatef(0, 0.5, 10-canonLaserCurrLength);
		glScalef(0.5, 0.5, canonLaserCurrLength);
		glutSolidCylinder(0.3,1,20,20);
		glPopMatrix();

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
	}
}

void animationHandlerTurretShooting(int param)
{
	if (canonLaserLive == true && canonLaserDuration <= canonLaserMaxDuration/2 && turretHit == false)
	{
		drawLaser();
		canonLaserCurrLength += canonLaserSpeed;
		canonLaserDuration++;
		glutTimerFunc(delay, animationHandlerTurretShooting, 0);
	}
	else if (canonLaserLive == true && canonLaserDuration > canonLaserMaxDuration / 2 && canonLaserDuration <= canonLaserMaxDuration && turretHit == false)
	{
		drawLaser();
		canonLaserCurrLength -= canonLaserSpeed;
		canonLaserDuration++;
		glutTimerFunc(delay, animationHandlerTurretShooting, 0);
	}
	else
	{
		canonLaserLive = false;
		canonLaserDuration = 0;
		canonLaserCurrLength = 0;
	}
	glutSetWindow(windowMain);
	glutPostRedisplay();
	glutSetWindow(windowSub);
	glutPostRedisplay();
	glutSetWindow(windowScore);
	glutPostRedisplay();
}

void collisionDetection(int param)
{
	for (int i = 0; i < numBots; i++)
	{
		float distanceBulletCanon = sqrt(pow(canonPositionX - bots[i].bulletX,2) + pow(canonPositionY - bots[i].bulletY,2)*1.0);
		if (distanceBulletCanon < 1)
		{
			turretHit = true;
			break;
		}
		else
		{
			//turretHit = false; // if turret can tank multiple hits add hp variable
		}

		float distanceLaserRobot = sqrt(pow(canonPositionX - bots[i].robotPositionX, 2)*1.0); // Laser is line so Y includes bot's Y
		if (canonLaserLive == true && distanceLaserRobot < 0.5 && bots[i].robotLive==true)
		{
			score++;
			botsleft--;
			bots[i].robotLive = false;
		}

		if (bots[i].robotLive == false && bots[i].scale > 0)
		{
			//bots[i].scale /= 2;
			bots[i].scale -= 0.01;
		}
	}
	glutSetWindow(windowMain);
	glutPostRedisplay();
	glutSetWindow(windowSub);
	glutPostRedisplay();
	glutSetWindow(windowScore);
	glutPostRedisplay();
	glutTimerFunc(delay, collisionDetection, 0);
	
}

// Some Utility Functions

Vector3D crossProduct(Vector3D a, Vector3D b){
	Vector3D cross;
	
	cross.x = a.y * b.z - b.y * a.z;
	cross.y = a.x * b.z - b.x * a.z;
	cross.z = a.x * b.y - b.x * a.y;
	
	return cross;
}

Vector3D normalize(Vector3D a){
	GLdouble norm = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	Vector3D normalized;
	normalized.x = a.x/norm;
	normalized.y = a.y/norm;
	normalized.z = a.z/norm;
	return normalized;
}








