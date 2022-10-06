

typedef struct Vector2D
{
	GLdouble x, y;
	GLdouble nx, ny;
	GLdouble u; // running distance
} Vector2D;

typedef struct Vector3D
{
	GLdouble x, y, z;
} Vector3D;

// Struct prototypes

typedef struct SubdivisionCurve SubdivisionCurve;
typedef struct Rect Rect;
typedef struct Button Button;

Vector3D crossProduct(Vector3D a, Vector3D b);
Vector3D fourVectorAverage(Vector3D a, Vector3D b, Vector3D c, Vector3D d);
Vector3D normalize(Vector3D a);


//void calcGunAngle(Robot&);
//void calcBotAngle(Robot&);

// Function prototypes
void generateRandom(GLdouble arr[5]);
void reshape(int w, int h);

void keyboardHandler(unsigned char key, int x, int y);
void animationHandler(int param);
void animationHandlerShooting(int param);
void animationHandlerTurretShooting(int param);
void collisionDetection(int param);
void initTextures();
void drawLaser();
void initRobot();
void initRobotTexture();
void resetGame();
void updateCanonPosition();
void drawBot();
void drawTurret();
void drawGround();

Vector2D getPoint(Vector2D *currentPoints, int numCurrentPoints, int j) ;
Vector2D subdivide(Vector2D v1, Vector2D v2, Vector2D v3, Vector2D v4) ;
void computeSubdivisionCurve(SubdivisionCurve *subcurvePointer) ;

void specialKeyHandler(int key, int x, int y);
void constructCircle(double radius, int numPoints, Vector2D* circlePts);
void computeRunningDistances(SubdivisionCurve *subcurvePointer);
void computeNormalVectors(SubdivisionCurve *subcurvePointer);

void init3DSurfaceWindow();
void displayMain();
void displaySub();
void displayScore();
void reshape3D(int w, int h);
void mouseButtonHandler3D(int button, int state, int x, int y);
void mouseScrollWheelHandler3D(int button, int dir, int xMouse, int yMouse);
void mouseMotionHandler3D(int xMouse, int yMouse);
void animationHandler(int param);


