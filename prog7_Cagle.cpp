/*******************************
Name: Bryan Cagle
Class: CSCE 4230.001
Due Date: 05-05-2016
Description: The purpose of this program
is to display a piecewise cubic bezier curve.

Based of bezcurve.c redbook example
*******************************/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
# else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <algorithm>
#include <stdio.h>

static float zoom = -15, //zoom variable
			 mouse_x=0, //used to store mouse x location
			 mouse_y=0; //used to store mouse y location

static int  width=0, 
			height=0, 
			currPoint=0, //Used to give the selected control point a different color
			xrot=180,
			yrot=180, 
			dragx, 
			dragy, 
			dragging=0;
			
static float // initial values for the control points
x1 = -4, y1 = -4, z1 = 0, //bottom left control point - p0
x2 = -4, y2 = 4, z2 = 0, //top left control point - p1
x3 = 0, y3 = 4, z3 = 0, //top middle control point - p2
x4 = 0, y4 = 0, z4 = 0, //middle control point - p3 
x5 = 0, y5 = -4, z5 = 0, //bottom middle control point - p4
x6 = 4, y6 = -4, z6 = 0, //bottom right control point - p5
x7 = 4, y7 = 4, z7 = 0; //top right control point - p6

static float ctrlpoints[7][3] = {
    { x1, y1, z1}, 
	{ x2, y2, z2}, 
	{ x3, y3, z3}, 
	{ x4, y4, z4}, 
	{ x5, y5, z5},
    { x6, y6, z6}, 
	{ x7, y7, z7}};
	
static bool collinear,start;
float *X, *Y, *Z;
void MenuSelect(int);
void init(void)
{
	//Stores modified control points for when a transformation is applied
	GLfloat ctrlpointsChanged[7][3] = {
        { x1, y1, z1}, { x2, y2, z2}, { x3, y3, z3}, { x4, y4, z4}, { x5, y5, z5},
        {x6, y6, z6}, {x7, y7, z7}
    };
    std::copy(ctrlpointsChanged, ctrlpointsChanged + 7, ctrlpoints);
	glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 7, &ctrlpointsChanged[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
}

void display(void)
{
	 
	//applies camera rotations when used with the idle function
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0,0,-10); //moves back far enough to not get cut off from rendering
   glRotatef(-xrot,1,0,0);
   glRotatef(yrot,0,1,0);
    GLint alias = 30;
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= alias; i++)
        glEvalCoord1f((GLfloat) i/alias);
    glEnd();
    glPointSize(8.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 7; i++) {
		if(i==currPoint)
		glColor3f(1,0,0); //changes control point color to red if it is selected
		else
		glColor3f(1,1,1);
        glVertex3fv(&ctrlpoints[i][0]);
    }
    glEnd();
    glFlush();
	if(!start){
	MenuSelect('1');
	start=true;
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
   width=w;
   height=h;
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
               5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
   else
      glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 
               5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
   gluPerspective(zoom,(GLfloat)w/(GLfloat)h,1,20);
   
}


void MenuSelect(int idCommand)
{
	switch(idCommand)
	{
	
	//camera controls
	case 'z': //zoom in        
        zoom += .10;
        break;
    case 'Z': //zoom out
        /* zooms in */
        zoom -= .10;
        break;
		
	// wasd moves selected control point up, if it is one of the collinear points, then all three
	// of those points are moved at the same time
	case 'w': //move control point up
		if(collinear==false)
			*Y +=0.5;
		else{
			y3 += 0.5;
            y4 += 0.5;
            y5 += 0.5;
		}
		break;
	case 's'://move control point down
		if(collinear==false)
			*Y -=0.5;
		else{
			y3 -= 0.5;
            y4 -= 0.5;
            y5 -= 0.5;
		}
		break;
	case 'a'://move control point left
		if(collinear==false)
			*X -=0.5;
		else{
			x3 -= 0.5;
            x4 -= 0.5;
            x5 -= 0.5;
		}
		break;
	case 'd'://move control point right
		if(collinear==false)
			*X +=0.5;
		else{
			x3 += 0.5;
            x4 += 0.5;
            x5 += 0.5;
		}
		break;
		
	//The control point's coordinates are mapped to the pointer, which then selects that control point
	//and allows it to be used when wasd is used.
	case '1': //control point 1
		collinear = false;
        X = &x1;
        Y = &y1;
        Z = &z1;
		currPoint=0;
		break;
	case '2': //control point 2
	collinear = false;
        X = &x2;
        Y = &y2;
        Z = &z2;
		currPoint=1;
        break;
	case '3': //control point 3
		collinear = true;
        X = &x3;
        Y = &y3;
        Z = &z3;
		currPoint=2;
		
		break;
	case '4': //control point 4
		collinear = true;
        X = &x4;
        Y = &y4;
        Z = &z4;
		currPoint=3;
		break;
	case '5': //control point 5
		collinear = true;
        X = &x5;
        Y = &y5;
        Z = &z5;
		currPoint=4;
		break;
	case '6': //control point 6
		collinear = false;
        X = &x6;
        Y = &y6;
        Z = &z6;
		currPoint=5;
		break;
	case '7': //control point 7
		collinear = false;
        X = &x7;
        Y = &y7;
        Z = &z7;
		currPoint=6;
		break;
	case 8:
		xrot=(xrot+5)%360;
		glutPostRedisplay();
		reshape(width,height);
		break;
	case 9:
		xrot=(xrot-5)%360;
		glutPostRedisplay();
		reshape(width,height);
		break;
	case 10:
		yrot=(yrot-5)%360;
		glutPostRedisplay();
		reshape(width,height);
		break;
	case 11:
		yrot=(yrot+5)%360;
		glutPostRedisplay();
		reshape(width,height);
		break;
	case 27: exit(0); break; //ends program
	default: break;
	}
	init();
	glutPostRedisplay();
	reshape(width,height);
}

//builds the menu 
int BuildMenu(void)
{
	int menu;
	menu=glutCreateMenu(MenuSelect);
	glutAddMenuEntry("Zoom In (z)", 'z');
	glutAddMenuEntry("Zoom Out (Z)", 'Z');
	glutAddMenuEntry("Rotate Graph Up (Up Arrow)", 8);
	glutAddMenuEntry("Rotate Graph Down (Down Arrow)", 9);
	glutAddMenuEntry("Rotate Graph Left (Left Arrow)", 10);
	glutAddMenuEntry("Rotate Graph Right (Right Arrow)", 11);
	glutAddMenuEntry("Move Control Point Up (w)", 'w');
	glutAddMenuEntry("Move Control Point Down (s)", 's');
	glutAddMenuEntry("Move Control Point Left (a)", 'a');
	glutAddMenuEntry("Move Control Point Right (d)", 'd');
	glutAddMenuEntry("Select Control Point p0 (1)", '1');
	glutAddMenuEntry("Select Control Point p1 (2)", '2');
	glutAddMenuEntry("Select Control Point p2 (3)", '3');
	glutAddMenuEntry("Select Control Point p3 (4)", '4');
	glutAddMenuEntry("Select Control Point p4 (5)", '5');
	glutAddMenuEntry("Select Control Point p5 (6)", '6');
	glutAddMenuEntry("Select Control Point p6 (7)", '7');
}

//redirects normal keyboard inputs to the menu inputs
void keyboard(unsigned char key, int  x, int y)
{
	switch(key){
	case 27:
		MenuSelect(27);
      		break;
    case 'z':
		MenuSelect('z');
        break;
	case 'Z':
        MenuSelect('Z');
        break;
	case 'w':
		MenuSelect('w');
        break;
	case 's':
		MenuSelect('s');
        break;
	case 'a':
		MenuSelect('a');
        break;
	case 'd':
		MenuSelect('d');
        break;
	case '1':
		MenuSelect('1');
        break;
	case '2':
		MenuSelect('2');
        break;
	case '3':
		MenuSelect('3');
        break;
	case '4':
		MenuSelect('4');
        break;
	case '5':
		MenuSelect('5');
        break;
	case '6':
		MenuSelect('6');
        break;
	case '7':
		MenuSelect('7');
        break;
	default:
        break;
	}
}

//For use when arrows keys are used to transform the model
void SpecialInput(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		MenuSelect(8);
		break;	
	case GLUT_KEY_DOWN:
		MenuSelect(9);
		break;
	case GLUT_KEY_LEFT:
		MenuSelect(10);
		break;
	case GLUT_KEY_RIGHT:
		MenuSelect(11);
		break;
	default: 
		break;
	}
}

//rotates the camera when the mouse is dragged
void mouseMotion(int x, int y)
{

    if(dragging==1){
		yrot+=(x-dragx)%360;
		xrot+=(y-dragy)%360;
		dragx=x;
		dragy=y;
	}
}

//Toggles the mouse being held down
void mouse(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		dragging=1;
		dragx=x;
		dragy=y;
	}
	else
		dragging=0;
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Program 7: Bezier Curve");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc(display);
    glutKeyboardFunc (keyboard);
	glutSpecialFunc(SpecialInput);
    BuildMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMouseFunc(mouse);
    glutMotionFunc( mouseMotion );
    glutMainLoop();
    return 0;
}