/*Name: Vincent Wu
  csid: vncentwu
 */

#ifndef __GEOM__
#define __GEOM__
#include <iostream>
#include <fstream>
//#include <cstring>
#include <vector>
#include <stdlib.h>
//#include <glut.h>
#include <GL/glut.h> //for linux
//#include <glut.h> //windows
#include <math.h>
#include <stdio.h>
//#include <string.h>
#include <float.h>

using namespace std;

enum mode{
	FACES,
	POINTS,
	LINES,
};


class Trimesh
{
	public:

		vector<vector<int> > faces; //contains a list of faces, which is a list on vertex index
		vector<vector<float> > vertices;
		vector<vector<float> > faces_normals; //contains a list of normal vectors corresponding to each face

		vector<vector<float> > transformations;
		vector<vector<float> > world_transformations;

		char name[100];


		float max_x;
		float max_y;
		float max_z;
		float min_x;
		float min_y;
		float min_z;
		unsigned long int count;

		Trimesh()
		{
			max_x = -FLT_MAX;
			max_y = -FLT_MAX;
			max_z = -FLT_MAX;
			min_x = FLT_MAX;
			min_y = FLT_MAX;
			min_z = FLT_MAX;
			count = 0;
		}

		void setName(const char* s)
		{
			strcpy(name, s);
		}

		void writeLoading()
		{
			unsigned long int t = (count % 5000);
			//cout << t << " " << count;
			if(t == 0)
				cout << "\r[|         ]";
			else if(t == 500)
				cout << "\r[||        ]";
			else if(t == 1000)
				cout << "\r[|||       ]";
			else if(t == 1500)
				cout << "\r[||||      ]";
			else if(t == 2000)
				cout << "\r[|||||     ]";
			else if(t == 2500)
				cout << "\r[||||||    ]";
			else if(t == 3000)
				cout << "\r[|||||||   ]";
			else if(t == 3500)
				cout << "\r[||||||||  ]";
			else if(t == 4000)
				cout << "\r[||||||||| ]";
			else if(t == 4500)
				cout << "\r[||||||||||]";
			fflush;
		}


		void writeDone()
		{
			cout << "\rLoad finished.\n\n";
		}

		void addTransformation(float t, float theta, float x, float y, float z)
		{
			if(t == 4.0f)
			{
				transformations.clear();
				world_transformations.clear();
				return;
			}
			vector<float> xd;		
			xd.push_back((float)t);
			xd.push_back(theta);
			xd.push_back(x);
			xd.push_back(y);
			xd.push_back(z);

			transformations.push_back(xd);

		}

		void applyTransformations()
		{
			for(int i = transformations.size() - 1; i >= 0; i--)
			{
				vector<float> v = transformations[i];
				if(v[0] == 1.0f)
					glTranslatef(v[2], v[3], v[4]);
			}
			for(int i = transformations.size() - 1; i >= 0; i--)
			{
				vector<float> v = transformations[i];
				if(v[0] == 2.0f)
					glScalef(v[2], v[3], v[4]);
			}
			for(int i = transformations.size() - 1; i >= 0; i--)
			{
				vector<float> v = transformations[i];
				if(v[0] == 3.0f)
					glRotatef(v[1], v[2], v[3], v[4]);
			}			
		}

		void applyWorldTransformations()
		{
			for(int i = world_transformations.size() - 1; i >= 0; i--)
			{
				vector<float> v = world_transformations[i];
				if(v[0] == 1.0f)
					glTranslatef(v[2], v[3], v[4]);
			}
			for(int i = transformations.size() - 1; i >= 0; i--)
			{
				vector<float> v = world_transformations[i];
				if(v[0] == 2.0f)
					glScalef(v[2], v[3], v[4]);
			}
			for(int i = world_transformations.size() - 1; i >= 0; i--)
			{
				vector<float> v = world_transformations[i];
				if(v[0] == 3.0f)
					glRotatef(v[1], v[2], v[3], v[4]);
			}			
		}		

		void reverseTransformations(int i)
		{
			for(int i = transformations.size() - 1; i >= 0; i--)
			{
				vector<float> v = transformations[i];
				if(v[0] == 1.0f)
					glTranslatef(v[2], v[3], v[4]);
				if(v[0] == 2.0f)
					glScalef(v[2], v[3], v[4]);
				if(v[0] == 3.0f)
					glRotatef(v[1], v[2], v[3], v[4]);
			}
		}




		void addVertex(float values[3])
		{
			max_x = values[0] > max_x ? values[0] : max_x;
			max_y = values[1] > max_y ? values[1] : max_y;
			max_z = values[2] > max_z ? values[2] : max_z;
			min_x = values[0] < min_x ? values[0] : min_x;
			min_y = values[1] < min_y ? values[1] : min_y;
			min_z = values[2] < min_z ? values[2] : min_z;
			//cout << "max:" << max_x << " " << max_y << " " << max_z << "\n" << endl;
			//cout << "min:" << min_x << " " << min_y << " " << min_z << "\n" << endl;
			writeLoading();
			count++;
			
				 
			vector<float> v(values, values + 3);
			vertices.push_back(v);
		}

		void addFace(int tri[3])
		{
			vector<int> vec(tri, tri + 3);
			faces.push_back(vec);
			vector<float> v1 = vertices[tri[0]];
			vector<float> v2 = vertices[tri[1]];
			vector<float> v3 = vertices[tri[2]];
			
			float u[3];
			u[0] = (v2[0]-v1[0]);
			u[1] = (v2[1]-v1[1]);
			u[2] = (v2[2]-v1[2]);
			float v[3];
			v[0] = (v3[0]-v1[0]);
			v[1] = (v3[1]-v1[1]);
			v[2] = (v3[2]-v1[2]);

			//vector<float> normal;
			float n[3];
			n[0] = ( u[1] * v[2] - u[2] * v[1] ); //y1z2 - y2z1
			n[1] = ( u[2] * v[0] - u[0] * v[2] ); //z1x2 - z2x1
			n[2] = ( u[0] * v[1] - u[1] * v[0] ); //x1y2 - x2y1
			float length = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
			n[0] = n[0]/length;
			n[1] = n[1]/length;
			n[2] = n[2]/length;
			writeLoading();
			count++;
			faces_normals.push_back(vector<float>(n, n+3));
		}

		float get_min_z()
		{
			return min_z;
		}

		float get_xy_diff()
		{
			float x_diff = max_x - min_x;
			float y_diff = max_y - min_y;

			return (x_diff > y_diff) ? x_diff : y_diff;
		}

		float get_diam()
		{
			float x_diff = max_x - min_x;
			float y_diff = max_y - min_y;
			float z_diff = max_z - min_z;
			return max(x_diff, max(y_diff, z_diff));
		}

		vector<float> get_target()
		{
			//cout << vertices.size() <<endl;
			float t[3];
			t[0] = (max_x + min_x)/2.0f;
			t[1] = (max_y + min_y)/2.0f;
			t[2] = (max_z + min_z)/2.0f;

			//cout << min_x << "   " << max_x << "\n";

			//cout << t[0] << endl;
			//cout << t[1] << endl;
			//cout << t[2] << endl;
			//for (int i = 0; i < 3; i++) 
				//cout << t[i] << "\n" << endl;
			return vector<float>(t, t+3);

			
			//return vertices[0];
		}

		void drawFaces(int mode)
		{
			glColor3f(200.f/255.f, 200.f/255.f, 200.f/255.f);
			for (unsigned i=0; i < faces.size(); i++)
			{
				switch(mode)
				{
					case FACES:
						glBegin(GL_TRIANGLES);
					case POINTS:
						glBegin(GL_POINTS);
					case LINES:
						glBegin(GL_LINES);
				}
				glBegin(GL_TRIANGLES);
					vector<int> face = faces[i];
					vector<float> normal = faces_normals[i];
					for(unsigned j = 0; j < 3; j++)
					{
						glNormal3f(normal[0], normal[1], normal[2]);
						int index = face[j];
						vector<float> vertex = vertices[index];
						glVertex3f(vertex[0], vertex[1], vertex[2]);
	
					}
				glEnd();
			}

		}

		void drawVNormals()
		{
			glColor3f(1.f, 0.f, 0.f);
			for (unsigned i=0; i < faces.size(); i++)
			{
				vector<int> face = faces[i];
				vector<float> normal = faces_normals[i];
				for(unsigned j = 0; j < 3; j++)
				{
					glBegin(GL_LINES);
						//glNormal3f(normal[0], normal[1], normal[2]);
						int index = face[j];
						vector<float> vertex = vertices[index];
						glVertex3f(vertex[0], vertex[1], vertex[2]);
						glVertex3f(vertex[0] + normal[0] * get_diam() / 3.0f, vertex[1] + normal[1] * get_diam() / 3.0f, 
							vertex[2] + normal [2] * get_diam() / 3.0f);
						glVertex3f(vertex[0] - normal[0] * get_diam() / 3.0f, vertex[1] - normal[1] * get_diam() / 3.0f, 
							vertex[2] - normal [2] * get_diam() / 3.0f);					
					glEnd();
				}
			}
			glColor3f(1.f, 1.f, 1.f);
		}

		void drawFNormals()
		{
			glColor3f(0.f, 0.f, 1.f);
			for (unsigned i=0; i < faces.size(); i++)
			{
				float x = 0.f;
				float y = 0.f;
				float z = 0.f;
				vector<int> face = faces[i];
				//cout << "x: " << x << endl;
				//cout << "y: " << y << endl;
				//cout << "z: " << z << endl;
				for(unsigned j = 0; j <3; j++)
				{
					int index = face[j];
					vector<float> vertex = vertices[index];
					x += vertex[0];
					y += vertex[1];
					z += vertex[2];
									//cout << "x: " << x << endl;
				//cout << "y: " << y << endl;
				//cout << "z: " << z << endl;
				}

				x /= 3.0f;
				y /= 3.0f;
				z /= 3.0f;
				//cout << "xcc: " << x << endl;
				//cout << "ycc: " << y << endl;
				//cout << "zcc: " << z << endl;


				vector<float> normal = faces_normals[i];
				glBegin(GL_LINES);
					glVertex3f(x, y, z);
					glVertex3f(x + normal[0] * get_diam() / 3.0f, y + normal[1] * get_diam()/3.0f, 
						z + normal [2] * get_diam()/3.0f);
					glVertex3f(x - normal[0] * get_diam()/3.0f, y - normal[1] * get_diam()/3.0f, 
						z - normal [2] * get_diam()/3.0f);					
				glEnd();

			}
			glColor3f(1.f, 1.f, 1.f);
		}


};
#endif