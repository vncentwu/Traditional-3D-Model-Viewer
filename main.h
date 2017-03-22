#include <stdlib.h>
//#include <glut.h>
#include <GL/glut.h> //for linux
//#include <glut.h> //windows
#include <math.h>
#include <stdio.h>
//#include <string>
#include <iostream>
//#include <thread>
//#include <future>
#include "loader.h"

using namespace std;


bool mousePressed;
bool rightPressed;
bool middlePressed;
GLfloat x_click;
GLfloat y_click;
GLfloat y_rotate;
GLfloat x_rotate;
GLfloat x_translate;
GLfloat y_translate;
GLfloat Perm_y_rotate;
GLfloat Perm_x_rotate;
GLfloat perm_x_translate;
GLfloat perm_y_translate;
bool lighting_enabled;
bool perspective_mode_enabled =  true;
bool auto_rotate_enabled = false;
GLfloat zoom = 0;
GLfloat perm_zoom = 0;
int rotate_speed;
int auto_rotate_speed = 1;
vector<Trimesh*> meshes;
//future<void> fut;
bool future_initialized;
char command[1000];
//string command;
bool newCommand;
TrimeshLoader* loader;

int BuildPopupMenu();
void myReshape(GLsizei width, GLsizei height);
void myDisplay();
void myReshape();
void idle();
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void wait_in();
void initGL();
void Keyboard(unsigned char key, int x, int y);