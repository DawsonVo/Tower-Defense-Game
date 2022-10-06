/*******************************************************************
Dawson Vo
500879711
CPS511-01
********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define _USE_MATH_DEFINES
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "subdivcurve.h"
#include <ctime>
#include "RGBpixmap.h"


// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat robotBodyW_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotBodyW_mat_specular[] = { 0.45f,0.45f,0.45f,1.0f };
GLfloat robotBodyW_mat_diffuse[] = { 0.9f,0.9f,0.9f,1.0f };
GLfloat robotBodyW_mat_shininess[] = { 32.0F };

GLfloat robotTire_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotTire_mat_specular[] = { 0.45f,0.45f,0.45f,1.0f };
GLfloat robotTire_mat_diffuse[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotTire_mat_shininess[] = { 32.0F };

GLfloat robotHead_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotHead_mat_specular[] = { 0.45f,0.45f,0.45f,1.0f };
GLfloat robotHead_mat_diffuse[] = { 0.5f,0.5f,0.5f,1.0f };
GLfloat robotHead_mat_shininess[] = { 32.0F };

GLfloat robotGun_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotGun_mat_specular[] = { 0.45f,0.45f,0.45f,1.0f };
GLfloat robotGun_mat_diffuse[] = { 0.9f,0.9f,0.9f,1.0f };
GLfloat robotGun_mat_shininess[] = { 32.0F };

GLfloat robotBullet_mat_ambient[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat robotBullet_mat_specular[] = { 1.0f,0.1f,0.1f,1.0f };
GLfloat robotBullet_mat_diffuse[] = { 1.0f,0.1f,0.1f,1.0f };
GLfloat robotBullet_mat_shininess[] = { 32.0F };

class Robot {
public:
	bool robotLive = true;
	float scale = 1.3;
	float bulletX;
	float bulletY;
	float bulletSpeed = 0.6;
	float bulletAngle;
	int currentCurvePointBullet = 0;
	
	bool bulletLive = false;

	SubdivisionCurve subcurve;
	float canonPositionX = 0;
	float canonPositionY = 0;
	int currentCurvePoint = 0;

	// Note how everything depends on robot body dimensions so that can scale entire robot proportionately
// just by changing robot body scale
	float robotBodyWidth = 8.0;
	float robotBodyLength = 10.0;
	float robotBodyDepth = 8.0;
	float headWidth = 0.4 * robotBodyWidth;
	float headLength = headWidth;
	float headDepth = headWidth;
	float upperArmLength = robotBodyLength;
	float upperArmWidth = 0.125 * robotBodyWidth;
	float gunLength = upperArmLength / 4.0;
	float gunWidth = upperArmWidth;
	float gunDepth = upperArmWidth;
	float stanchionLength = robotBodyLength;
	float stanchionRadius = 0.1 * robotBodyDepth;
	float baseWidth = 2 * robotBodyWidth;
	float baseLength = 0.25 * stanchionLength;
	float robotPositionX = 0.0;
	float robotPositionY = 0.0;
	float robotWalkSpeed = 1.0;

	// Control Robot body forward
	float robotFwdDistance = 0.0;
	float robotWheelAngle = 0.0;

	// Control Robot body rotation on base
	float robotAngle = 0.0;

	// Control arm rotation
	float shoulderAngle = -40.0;
	float gunAngle = 0.0;

	// Default Mesh Size
	int meshSize = 16;


	RGBpixmap robotPix[20];
	GLuint robotTexture[20];
	GLUquadric* body = gluNewQuadric();
	void initRobotTexture()
	{
		//glClearColor(0.3, 0.1, 0.2, 0.0);
		//glClearDepth(1.0);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glGenTextures(20, robotTexture);

		robotPix[0].readBMPFile("robotbody.bmp"); // body
		glBindTexture(GL_TEXTURE_2D, robotTexture[0]); // select current texture (0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D( // initialize texture
			GL_TEXTURE_2D, // texture is 2-d
			0, // resolution level 0
			GL_RGB, // internal format
			robotPix[0].nCols, // image width
			robotPix[0].nRows, // image height
			0, // no border
			GL_RGB, // my format
			GL_UNSIGNED_BYTE, // my type
			robotPix[0].pixel); // the pixels

		robotPix[1].readBMPFile("robottire.bmp"); //tire
		glBindTexture(GL_TEXTURE_2D, robotTexture[1]); // select current texture (0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D( // initialize texture
			GL_TEXTURE_2D, // texture is 2-d
			0, // resolution level 0
			GL_RGB, // internal format
			robotPix[1].nCols, // image width
			robotPix[1].nRows, // image height
			0, // no border
			GL_RGB, // my format
			GL_UNSIGNED_BYTE, // my type
			robotPix[1].pixel); // the pixels

		robotPix[2].readBMPFile("robothead.bmp"); //tire
		glBindTexture(GL_TEXTURE_2D, robotTexture[2]); // select current texture (0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D( // initialize texture
			GL_TEXTURE_2D, // texture is 2-d
			0, // resolution level 0
			GL_RGB, // internal format
			robotPix[2].nCols, // image width
			robotPix[2].nRows, // image height
			0, // no border
			GL_RGB, // my format
			GL_UNSIGNED_BYTE, // my type
			robotPix[2].pixel); // the pixels

		robotPix[3].readBMPFile("beam.bmp"); //tire
		glBindTexture(GL_TEXTURE_2D, robotTexture[3]); // select current texture (0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D( // initialize texture
			GL_TEXTURE_2D, // texture is 2-d
			0, // resolution level 0
			GL_RGB, // internal format
			robotPix[3].nCols, // image width
			robotPix[3].nRows, // image height
			0, // no border
			GL_RGB, // my format
			GL_UNSIGNED_BYTE, // my type
			robotPix[3].pixel); // the pixels

		robotPix[4].readBMPFile("turretdead.bmp"); //tire
		glBindTexture(GL_TEXTURE_2D, robotTexture[4]); // select current texture (0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D( // initialize texture
			GL_TEXTURE_2D, // texture is 2-d
			0, // resolution level 0
			GL_RGB, // internal format
			robotPix[4].nCols, // image width
			robotPix[4].nRows, // image height
			0, // no border
			GL_RGB, // my format
			GL_UNSIGNED_BYTE, // my type
			robotPix[4].pixel); // the pixels

		glDisable(GL_TEXTURE_2D);

	}

	void initSubdivisionCurve(GLdouble x[10]) {
		// Initialize 3 control points of the subdivision curve

		
		subcurve.controlPoints[0].x = x[0];
		subcurve.controlPoints[0].y = 15;

		subcurve.controlPoints[1].x = x[1];
		subcurve.controlPoints[1].y = 10;

		subcurve.controlPoints[1].x = x[2];
		subcurve.controlPoints[1].y = 7;

		subcurve.controlPoints[2].x = x[3];
		subcurve.controlPoints[2].y = 5;

		subcurve.controlPoints[3].x = x[4];
		subcurve.controlPoints[3].y = 1;

		subcurve.controlPoints[4].x = x[5];
		subcurve.controlPoints[4].y = -1;

		subcurve.controlPoints[4].x = x[6];
		subcurve.controlPoints[4].y = -4;

		subcurve.controlPoints[4].x = x[7];
		subcurve.controlPoints[4].y = -6;

		subcurve.controlPoints[4].x = x[8];
		subcurve.controlPoints[4].y = -11;

		subcurve.controlPoints[4].x = x[9];
		subcurve.controlPoints[4].y = -20;



		subcurve.numControlPoints = 5;
		subcurve.subdivisionSteps = 6;
		computeSubdivisionCurve(&subcurve);
	}



	void calcBotAngle()
	{
		if (currentCurvePoint + 1 < subcurve.numCurvePoints) {
			float x = subcurve.curvePoints[currentCurvePoint + 1].x - subcurve.curvePoints[currentCurvePoint].x;
			float y = (-subcurve.curvePoints[currentCurvePoint + 1].y) - (-subcurve.curvePoints[currentCurvePoint].y);
			robotAngle = atan(x / y) * (180 / M_PI);
		}
	}

	void calcGunAngle()
	{
		float x = canonPositionX - subcurve.curvePoints[currentCurvePoint].x;
		float y = canonPositionY - (-subcurve.curvePoints[currentCurvePoint].y);
		gunAngle = (atan(x / y) * (180 / M_PI)) - robotAngle;
	}

	void shoot()
	{
		if (bulletLive == false)
		{
			bulletLive = true;
			bulletAngle = gunAngle + robotAngle;
			bulletX = subcurve.curvePoints[currentCurvePoint].x;
			bulletY = (-subcurve.curvePoints[currentCurvePoint].y);
		}
		else
		{
			bulletX += sin(M_PI * 2 *(bulletAngle/360.0)) * bulletSpeed;
			bulletY += cos(M_PI * 2 * (bulletAngle / 360.0)) * bulletSpeed;
			currentCurvePointBullet++;
		}
	}



	void drawBullet()
	{
		if (bulletLive == true)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, robotBullet_mat_ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, robotBullet_mat_specular);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBullet_mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SHININESS, robotBullet_mat_shininess);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, robotTexture[3]);

			glPushMatrix();
				glTranslatef(bulletX, 0.25, bulletY);
				//glTranslatef(bulletcurve.curvePoints[currentCurvePointBullet].x, 0.5, -bulletcurve.curvePoints[currentCurvePointBullet].y);
				glRotatef(bulletAngle, 0, 1, 0);
				glScalef(scale*0.3, scale * 0.3, scale * 1);
				glutSolidCube(1);
			glPopMatrix();

			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_2D);

		}
		if (bulletY > canonPositionY+10)
		{
			bulletLive = false;
			currentCurvePointBullet = 0;
			glutPostRedisplay();
		}
	}

	void updateRobotPosition()
	{
		robotPositionX = subcurve.curvePoints[currentCurvePoint].x;
		robotPositionY = -subcurve.curvePoints[currentCurvePoint].y;
	}

	void drawRobot()
	{	
		//recalc all angles before drawing
		calcBotAngle();
		calcGunAngle();
		updateRobotPosition();
		drawBullet();




		glPushMatrix();
		// spin robot on base. 
		glTranslatef(subcurve.curvePoints[currentCurvePoint].x, 1.0, -subcurve.curvePoints[currentCurvePoint].y);
		glRotatef(robotAngle, 0.0, 1.0, 0.0);
		glScalef(scale * 0.1, scale * 0.1, scale * 0.1);


		glPushMatrix(); //Level 1 Robot Hierachy - Body
		drawBody(); // Contains 2 levels of hierachy
		glPushMatrix(); // Level 2 Robot Hierachy - Shoulder
		glTranslatef(-0.5 * robotBodyWidth, 0.4 * robotBodyLength, -0 * robotBodyDepth);
		drawShoulder(); // Contains 2 levels of hierachy

		glPushMatrix(); // Level 3 Robot Hierachy - Legs
		glTranslatef(0.5 * robotBodyWidth, -0.4 * robotBodyLength, -0 * robotBodyDepth);
		drawLeftLeg();
		drawRightLeg(); // Contains 3 levels of hierachy
		drawWheel(); // Contains 4 levels of Hierachy
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		// Position head with respect to parent (body)
		glTranslatef(0, 0.4 * robotBodyLength + 0.1 * headLength, 0); // this will be done last
		drawHead(); // Contains 5 level of Hierachy
		glPopMatrix();
		glPushMatrix(); // Making both guns - Gun Object drawGun() contains 4 levels of of Hierachy
			// Position both Guns with respect to parent (body)
		glTranslatef(0 * robotBodyWidth, 0 * robotBodyLength, 0 * robotBodyDepth);

		glPushMatrix();
		// Position Left Gun with respect to parent (body) - Level 2 Hierachy
		glTranslatef(0.25 * robotBodyWidth, 0.2 * robotBodyLength, 0 * robotBodyDepth);
		drawGun();
		glPopMatrix();
		glPushMatrix();
		// Position Right Gun with respect to parent (body)- Level 2 Hierachy
		glTranslatef(-0.25 * robotBodyWidth, 0.2 * robotBodyLength, 0 * robotBodyDepth);
		drawGun();
		glPopMatrix();
		glPopMatrix();

		glPopMatrix();

		// don't want to spin fixed base
		//drawLowerBody();

		glPopMatrix();

		

	}
private:
	void drawBody()
	{
		
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotBodyW_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotBodyW_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBodyW_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotBodyW_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[0]);


		glPushMatrix(); // Level 1 Hierachy - Middle Torso
		glTranslatef(0, 0.15 * robotBodyLength, 0);

		glPushMatrix(); //Level 2 Hierachy - Bottom round
		glScalef(0.25 * robotBodyWidth, 0.25 * robotBodyLength, 0.25 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();

		glScalef(0.25 * robotBodyWidth, 0.3 * robotBodyLength, 0.25 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
	}

	void drawShoulder()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotBodyW_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotBodyW_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBodyW_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotBodyW_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[0]);

		glPushMatrix(); // Level 1 Hierachy - Shoulder
		glPushMatrix(); // Level 2 Hierachy - Right Cap
		glTranslatef(0.02 * robotBodyWidth, 0, 0);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();

		glPushMatrix(); // Level 2 Hierachy - Left Cap
		glTranslatef(0.98 * robotBodyWidth, 0, 0);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();

		glRotatef(90.0, 0.0, 1.0, 0.0);
		glScalef(0.1 * robotBodyWidth, 0.1 * robotBodyWidth, 1 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
	}

	void drawRightLeg()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotBodyW_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotBodyW_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBodyW_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotBodyW_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[0]);


		glPushMatrix(); // Level 1 Hierachy - Knee
		glPushMatrix(); // Level 2 Hierachy - Kneecap Right
		glTranslatef(-0.48 * robotBodyWidth, -0.19 * robotBodyLength, -0.75 * robotBodyWidth);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();
		glPushMatrix(); // Level 2 Hierachy - Kneecap Left
		glTranslatef(-0.22 * robotBodyWidth, -0.19 * robotBodyLength, -0.75 * robotBodyWidth);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();

		glPushMatrix(); // Level 2 Hierachy - Upper Limb Middle
		glTranslatef(-0.4 * robotBodyWidth, -0.15 * robotBodyLength, -0.7 * robotBodyWidth);
		glRotatef(-45.0, 1.0, 0.0, 0.0);
		glScalef(0.05 * robotBodyWidth, 0.05 * robotBodyWidth, 1 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPushMatrix(); // Level 3 Hierachy - Upper Limb Top
		glTranslatef(-0 * robotBodyWidth, 0.13 * robotBodyLength, -0 * robotBodyWidth);
		glScalef(0.6, 0.6, 1);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glPushMatrix(); // Level 3 Hierachy - Upper Limb Bottom
		glTranslatef(-0 * robotBodyWidth, -0.13 * robotBodyLength, -0 * robotBodyWidth);
		glScalef(0.6, 0.6, 1);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glPopMatrix();

		glPushMatrix(); // Level 2 Hierachy - Lower Limb Middle
		glTranslatef(-0.3 * robotBodyWidth, -0.2 * robotBodyLength, -0.7 * robotBodyWidth);
		glRotatef(45.0, 1.0, 0.0, 0.0);
		glScalef(0.05 * robotBodyWidth, 0.05 * robotBodyWidth, 1 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPushMatrix(); // Level 3 Hierachy - Lower Limb Top
		glTranslatef(0, 0.13 * robotBodyLength, 0);
		glScalef(0.6, 0.6, 1);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glPushMatrix(); // Level 3 Hierachy - Lower Limb Bottom
		glTranslatef(0, -0.13 * robotBodyLength, 0);
		glScalef(0.6, 0.6, 1);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glPopMatrix();

		glRotatef(90.0, 0.0, 1.0, 0.0);
		glTranslatef(0.75 * robotBodyWidth, -0.19 * robotBodyLength, -0.5 * robotBodyWidth);
		glScalef(0.1 * robotBodyWidth, 0.1 * robotBodyWidth, 0.3 * robotBodyWidth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
	}

	void drawLeftLeg()
	{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[0]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotBodyW_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotBodyW_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBodyW_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotBodyW_mat_shininess);
		//uppertop
		glPushMatrix();
		glTranslatef(0.4 * robotBodyWidth, -0.15 * robotBodyLength, -0.8 * robotBodyWidth);
		glRotatef(-45.0, 1.0, 0.0, 0.0);
		glScalef(0.03 * robotBodyWidth, 0.03 * robotBodyWidth, 1.08 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		//uppermid
		glPushMatrix();
		glTranslatef(0.4 * robotBodyWidth, -0.15 * robotBodyLength, -0.7 * robotBodyWidth);
		glRotatef(-45.0, 1.0, 0.0, 0.0);
		glScalef(0.05 * robotBodyWidth, 0.05 * robotBodyWidth, 1 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		//upperlower
		glPushMatrix();
		glTranslatef(0.4 * robotBodyWidth, -0.22 * robotBodyLength, -0.68 * robotBodyWidth);
		glRotatef(-45.0, 1.0, 0.0, 0.0);
		glScalef(0.03 * robotBodyWidth, 0.03 * robotBodyWidth, 1.06 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		//kneecap
		glPushMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glTranslatef(0.75 * robotBodyWidth, -0.19 * robotBodyLength, 0.2 * robotBodyWidth);
		glScalef(0.1 * robotBodyWidth, 0.1 * robotBodyWidth, 0.3 * robotBodyWidth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.48 * robotBodyWidth, -0.19 * robotBodyLength, -0.75 * robotBodyWidth);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.22 * robotBodyWidth, -0.19 * robotBodyLength, -0.75 * robotBodyWidth);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();
		//lowertop
		glPushMatrix();
		glTranslatef(0.3 * robotBodyWidth, -0.21 * robotBodyLength, -0.8 * robotBodyWidth);
		glRotatef(45.0, 1.0, 0.0, 0.0);
		glScalef(0.03 * robotBodyWidth, 0.03 * robotBodyWidth, 1.08 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		//lowermid
		glPushMatrix();
		glTranslatef(0.3 * robotBodyWidth, -0.22 * robotBodyLength, -0.7 * robotBodyWidth);
		glRotatef(45.0, 1.0, 0.0, 0.0);
		glScalef(0.05 * robotBodyWidth, 0.05 * robotBodyWidth, 1 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		//lowerlower
		glPushMatrix();
		glTranslatef(0.3 * robotBodyWidth, -0.16 * robotBodyLength, -0.68 * robotBodyWidth);
		glRotatef(45.0, 1.0, 0.0, 0.0);
		glScalef(0.03 * robotBodyWidth, 0.03 * robotBodyWidth, 1.06 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
	}

	void drawWheel()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotBodyW_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotBodyW_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBodyW_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotBodyW_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[0]);
		glPushMatrix(); // Level 1 Hierachy 
		//rotating and positioning wheel
		glTranslatef(-0.4 * robotBodyWidth, -0.75 * robotBodyLength, 0 * robotBodyDepth);
		glRotatef(robotWheelAngle, 1.0, 0.0, 0.0);
		glPushMatrix();// Level 2 Hierachy - Centre Axle

		glPushMatrix(); // Level 3 Hierachy - Left End Cap
		glTranslatef(0.78 * robotBodyWidth, 0 * robotBodyLength, 0 * robotBodyDepth);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(90.0, 1.0, 0.0, 1.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();

		glPushMatrix(); // Level 3 Hierachy - Right End Cap
		glTranslatef(0.02 * robotBodyWidth, 0 * robotBodyLength, 0 * robotBodyDepth);
		glScalef(0.001, 0.1 * robotBodyWidth, 0.1 * robotBodyDepth);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glScalef(0.1 * robotBodyWidth, 0.1 * robotBodyWidth, 0.8 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();

		glPushMatrix();// Level 2 Hierachy - Wheel
		glTranslatef(0.4 * robotBodyWidth, 0 * robotBodyLength, 0 * robotBodyDepth);
		glScalef(0.26 * robotBodyWidth, 0.5 * robotBodyLength, 0.625 * robotBodyDepth);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10); //inner rim

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();// Level 3 Hierachy - Tire
			//Texture Mapping for Tire - Black
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotTire_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotTire_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotTire_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotTire_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[1]);
		//glTranslatef(0, -0.77 * robotBodyLength, 0 * robotBodyWidth);
		glScalef(0.11 * robotBodyWidth, 0.12 * robotBodyLength, 0.15 * robotBodyDepth);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix(); // Level 4 Hierachy - Tire Spin Indicator
			// White
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotBodyW_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotBodyW_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBodyW_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotBodyW_mat_shininess);
		glTranslatef(0, -0.12 * robotBodyLength, 0);
		glScalef(0.03 * robotBodyWidth, 0.01 * robotBodyLength, 0.03 * robotBodyLength);
		//glRotatef(90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();
		glPopMatrix();

		glPopMatrix();

		//indicator for spin


	}


	void drawHead()
	{
		// Set robot material properties per body part. Can have seperate material properties for each part
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotHead_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotHead_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotHead_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotHead_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[2]);

		glPushMatrix(); // Level 1 Hierachy - Base Head
		glRotatef(gunAngle, 0, 1, 0);
		glPushMatrix(); // Level 2 Hierachy - Top Head
		glTranslatef(0, 0.12 * robotBodyLength, 0);
		glScalef(0.25 * robotBodyWidth, 0.25 * robotBodyLength, 0.25 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);


		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[3]);
		glPushMatrix(); // Level 3,4,5 Hierachy - Eyes
		glTranslatef(-0.04 * robotBodyWidth, 0.12 * robotBodyLength, 0);
		glScalef(0.03 * robotBodyWidth, 0.03 * robotBodyWidth, 0.35 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);

		glPushMatrix();
		glTranslatef(0.42 * robotBodyWidth, 0.15 * robotBodyLength, 0);
		glScalef(0.14 * robotBodyWidth, 0.14 * robotBodyWidth, 0.11 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);

		glPushMatrix();
		glTranslatef(-0.22 * robotBodyWidth, 0.15 * robotBodyLength, 0);
		glScalef(0.05 * robotBodyWidth, 0.05 * robotBodyWidth, 0.15 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		glScalef(0.25 * robotBodyWidth, 0.5 * headLength, 0.25 * robotBodyDepth);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);

		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);

	}

	void drawGun()
	{
		// Set robot material properties per body part. Can have seperate material properties for each part
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotGun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotGun_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotGun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotGun_mat_shininess);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, robotTexture[4]);


		glPushMatrix(); // Level 1 Hierachy - Base Object
			// Rotate arm at shoulder
		glRotatef(gunAngle, 0.0, 1.0, 0.0); // Rotate Gun
		glScalef(1.8, 1.8, 1.8);
		glPushMatrix(); // Level 2 Hierachy - Socket/Mount		
		glPushMatrix(); // Level 3 Hierachy - Larger Barrel
		glPushMatrix(); // Level 4 Hierachy - Outer Barrel
		glScalef(0.04 * robotBodyWidth, 0.04 * robotBodyWidth, 0.24 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();

		glScalef(0.03 * robotBodyWidth, 0.03 * robotBodyWidth, 0.4 * robotBodyDepth);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
		glPopMatrix();

		glScalef(0.1 * robotBodyWidth, 0.08 * robotBodyLength, 0.1 * robotBodyDepth);
		gluSphere(gluNewQuadric(), 1.0, 20, 10);
		glPopMatrix();

		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
	}
};