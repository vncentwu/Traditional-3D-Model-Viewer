#include <stdlib.h>
//#include <glut.h>
#include <GL/glut.h> //for linux
//#include <glut.h> //windows
//#include <math.h>
#include <stdio.h>
#include <string>
#include <pthread.h>
#include <iostream>
#include <thread>
//#include <future>
#include "loader.h"
#include "main.h"

using namespace std;



vector<float> camera_target;


int main(int argc, char* argv[])
{
	//cout << "seg fault" << endl;
	
	
	loader = new TrimeshLoader();  
	float origin[3] = {0,0,0};
	camera_target.assign(origin, origin + 3);
	//cout << "seg fault1" << endl;


	if(argc >= 2)
	{
		Trimesh* mesh = new Trimesh();
		loader->loadOBJ(argv[1], mesh);
		meshes.push_back(mesh);
	}
	//cout << "seg fault2";
	glutInit(&argc, argv); //init glut library

	glutInitWindowSize(1000.f, 1000.f);
	glutInitWindowPosition(0,0);
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glutCreateWindow("Object loader");
	glutInitDisplayMode(GLUT_DOUBLE);

	
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(MouseButton);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(MouseMotion);
	glutIdleFunc(idle);
	initGL();
	

	//fut = std::async(std::launch::async, wait_in);
	//cout << "seg fault4";
	glutMainLoop();	
	
	
}



//standard set-up
void myDisplay()
{
	
	//cout << " ??? " << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	myReshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	float diam = 0;
	float z_min = 0;

	vector<float> camera_target;
	Trimesh* mesh = NULL;
	if(meshes.size() > 0)
	{
		mesh = meshes.back();
		if(mesh)
		{
			camera_target = mesh->get_target();
			diam = mesh->get_diam();
			z_min = mesh->get_min_z();
		}
	}
	gluLookAt(0, 0, 4.0f + diam * 4.0f + perm_zoom + zoom, camera_target[0], camera_target[1], camera_target[2], 0.0, 1.0, 0.0);
	//gluLookAt(0., 0., 2.*(diam),  camera_target[0], camera_target[1], camera_target[2], 0.0, 1.0, 0.0);


	float auto_rotate_deg = auto_rotate_enabled ? glutGet(GLUT_ELAPSED_TIME)/100.0f : 0;

	glTranslatef(x_translate/50.0f, -y_translate/50.0f, 0.f);

	//handle rotation about a point
	glTranslatef(camera_target[0], camera_target[1], camera_target[2]);
	float deg_x = Perm_x_rotate + x_rotate + auto_rotate_deg * auto_rotate_speed;
	float deg_y = Perm_y_rotate + y_rotate + auto_rotate_deg * auto_rotate_speed;
	glRotatef(-deg_x, 1, 0, 0); //rotating object, camera static'
	glRotatef(deg_y, 0, 1, 0);	
	glTranslatef(-camera_target[0], -camera_target[1], -camera_target[2]);

	mesh->applyTransformations();
	if(mesh != NULL)
		mesh->drawFaces();


	glDisable(GL_LIGHTING);
	glFlush();
	glutSwapBuffers();
	
}



void wait_in()
{
	while(1)
	{
		if(!newCommand)
		{
			cout << "Guest> ";
			char s[1000];
			//getline(cin, s);
			cin.getline(s, 1000);
			strcpy(command, s);
			newCommand = true;
		}
	}
}


void flushTransformations()
{
	Perm_x_rotate = 0;
	Perm_y_rotate = 0;
	x_rotate = 0;
	y_rotate = 0;
	perm_zoom = 0;
	zoom = 0;
	perm_x_translate = 0;
	perm_y_translate = 0;
	x_translate = 0;
	y_translate = 0;

}



void parseInput()
{
	
	//vector<string> v;
	//split(command, ' ', v);
    std::vector<char*> v;
    char* chars_array = strtok(command, " ");
    while(chars_array)
    {
        v.push_back(chars_array);
        chars_array = strtok(NULL, " ");
    }

	Trimesh* mesh = meshes.back();

	if(!strcmp(v[0], "L") & v.size() >= 2)
	{
		cout << "Loading file: " << v[1] << "\n";
		Trimesh* mesh = new Trimesh();
		if(loader->loadOBJ(v[1], mesh))
			meshes.push_back(mesh);
	}
	else if(!strcmp(v[0], "D"))
	{
		meshes.pop_back();
	}
	else if(!strcmp(v[0], "I"))
	{
		mesh->addTransformation(4.0f, 0.f, 0.f, 0.f, 0.f);
		flushTransformations();
	}
	else if(!strcmp(v[0], "T") && v.size() >= 4)
	{
		mesh->addTransformation(1.0f, 0.f, atof(v[1]), atof(v[2]), atof(v[3]));
	}
	else if(!strcmp(v[0], "S") && v.size() >= 4)
	{
		mesh->addTransformation(2.0f, 0.f, atof(v[1]), atof(v[2]), atof(v[3]));
	}
	else if(!strcmp(v[0], "R") && v.size() >= 5)
	{
		mesh->addTransformation(1.0f, atof(v[1]), atof(v[2]), atof(v[3]), atof(v[4]));
	}
	else if(!strcmp(v[0], "V"))
	{
		//transofmrs in local
	}
	else if(!strcmp(v[0], "W"))
	{
		//transforms in world
	}



	newCommand = false;
}



void idle()
{
	if(newCommand)
		parseInput();
	glutPostRedisplay();
}




void myReshape(GLsizei width, GLsizei height)
{
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar

	if(meshes.size() > 0)
	{
		Trimesh* mesh = meshes.back();
		if(mesh)
		{
			vector<float> v = mesh->get_target();
			float diff = mesh->get_xy_diff();
			float diam = mesh->get_diam();
			
			float left = v[0] - diam; 
			float right = v[0] + diam;
			float bottom = v[1] - diam; 
			float top = v[1] + diam; 
			//cout << left  << "   " <<right;
			if ( aspect < 1.0 ) { 
			// window taller than wide 
				bottom /= aspect; 
				top /= aspect; 
			} else { 
				left *= aspect; 
				right *= aspect;
			}
					
			if(perspective_mode_enabled)
			{
				//glFrustum(left, right, bottom, top, 0.1f, 0.1f + diam); 
				gluPerspective(45, aspect, 5.0f, 5.0f + diam * 50.0f);
				//gluPerspective(45, aspect, 0.1f, 100.0f);
				//cout << 2 * diam;
			}
			else
				glOrtho(left, right, bottom, top, 1.0f, 1.0f + diam); 
		}

	}
	
   
		

}


void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glDisable(GL_CULL_FACE);


	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);


}

void MouseButton(int button, int state, int x, int y) {
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
	

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		perm_zoom += zoom;
		zoom = 0;
		mousePressed = true;
		x_click = x;
		y_click = y;
	}
	else if(button == GLUT_LEFT_BUTTON)
	{
		 mousePressed = false;

		 Perm_x_rotate += x_rotate;
		 Perm_y_rotate +=  y_rotate;
		 x_rotate = 0;
		 y_rotate = 0;
		 x_click = 0;
		 y_click = 0;
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		rightPressed = true;
		x_click = x;
		y_click = y;
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		 rightPressed = false;

		 perm_zoom += zoom;
		 zoom = 0;
		 x_click = 0;
		 y_click = 0;
	}
	else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{

		middlePressed = true;
		x_click = x;
		y_click = y;
	}
	else if(button == GLUT_MIDDLE_BUTTON)
	{
		 middlePressed = false;
		 perm_x_translate += x_translate;
		 perm_y_translate += y_translate;
		 x_translate = 0;
		 y_translate = 0;
		 x_click = 0;
		 y_click = 0;
	}
	//cout << "BUTTON: " << button << endl;
}

void MouseMotion(int x, int y) {
  // If button1 pressed, zoom in/out if mouse is moved up/down.
	if(x_click == 0 && y_click == 0)
		return;
  if (mousePressed)
    {
      x_rotate = y - y_click;
	  y_rotate = x - x_click;
	  //cout << "xrotate: " << x_rotate << endl;
    }
  if(rightPressed)
  {
	  zoom = ((float)(y - y_click))/100.0f;
  }
  if(middlePressed)
  {
	  x_translate = x - x_click;
	  y_translate = y - y_click;
  }

	//glutPostRedisplay();
}

enum {
  MENU_LIGHTING = 1,
  MENU_POLYMODE,
  MENU_PERSPECTIVE,
  MENU_AUTO_ROTATE,
  MENU_TEXTURING,
  MENU_INCREASE_ROTATE_SPEED,
  MENU_INCREASE_AUTO_ROTATE_SPEED,
  MENU_DECREASE_ROTATE_SPEED,
  MENU_DECREASE_AUTO_ROTATE_SPEED,
  MENU_LEVEL_0,
  MENU_LEVEL_1,
  MENU_LEVEL_2,
  MENU_LEVEL_3,
  MENU_LEVEL_4,
  MENU_LEVEL_5,
  MENU_EXIT
};

void SelectFromMenu(int idCommand) {
  switch (idCommand)
    {
		 case MENU_PERSPECTIVE:
			perspective_mode_enabled = !perspective_mode_enabled;
			break;
		 case MENU_LIGHTING:
			lighting_enabled = !lighting_enabled;
			break;
		 case MENU_AUTO_ROTATE:
			 auto_rotate_enabled = !auto_rotate_enabled;
			 break;
		 case MENU_INCREASE_ROTATE_SPEED:
			 rotate_speed++;
			 break;
		 case MENU_INCREASE_AUTO_ROTATE_SPEED:
			 auto_rotate_speed++;
			 break;
		 case MENU_DECREASE_ROTATE_SPEED:
			 rotate_speed--;
			 break;
		 case MENU_DECREASE_AUTO_ROTATE_SPEED:
			 auto_rotate_speed--;
			 break;
    }
  // Almost any menu selection requires a redraw
  glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key)
  {
  case 27:             // ESCAPE key
	  exit (0);
	  break;
  case 'l':
	  SelectFromMenu(MENU_LIGHTING);
	  break;
  case 'r':
	  SelectFromMenu(MENU_AUTO_ROTATE);
	  break;
  case 'p':
	  SelectFromMenu(MENU_PERSPECTIVE);
	  break;
  case 't':
	  SelectFromMenu(MENU_TEXTURING);
	  break;
  case '+':
	  SelectFromMenu(MENU_INCREASE_AUTO_ROTATE_SPEED);
	  break;
  case '-':
	  SelectFromMenu(MENU_DECREASE_AUTO_ROTATE_SPEED);
	  break;
  case '0':
	  SelectFromMenu(MENU_LEVEL_0);
	  break;
	case '1':
	  SelectFromMenu(MENU_LEVEL_1);
	  break;
	case '2':
	  SelectFromMenu(MENU_LEVEL_2);
	  break;
	case '3':
	  SelectFromMenu(MENU_LEVEL_3);
	  break;
	case '4':
	  SelectFromMenu(MENU_LEVEL_4);
	  break;
	case '5':
	  SelectFromMenu(MENU_LEVEL_5);
	  break;
	}
}





