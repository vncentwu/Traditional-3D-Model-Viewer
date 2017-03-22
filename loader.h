#ifndef __LOADER_H__
#define __LOADER_H__
#include <iostream>
#include <fstream>
#include <cstring>
#include "geom.h"
using namespace std;


#define LINE_SIZE    1024
#define TOKEN_LENGTH 8

enum TokenID { T_NONE=-1, T_VERT, T_FACE}; 

struct TokenPair 
{
	char strval[TOKEN_LENGTH];
	TokenID  tokID;

	//defines equal operator
	bool operator==(const TokenPair & rhs)  const
	{
		return strcmp(strval,rhs.strval) == 0 && tokID == rhs.tokID;
	}
	//defines not equal operator
	bool operator!=(const TokenPair & rhs) const
	{
		return !(*this==rhs);
	}
};

class TrimeshLoader
{
	public:

		static TokenPair EMPTY_PAIR;

		//list of tokenpairs
		static TokenPair tokenMap[];

		static char TOK_SEPS[];

		TokenPair * tokenMatch(char * srchtok)
		{
			if(!srchtok) return 0;
			TokenPair * ptokp = &tokenMap[0];
			for(; *ptokp != EMPTY_PAIR && strcmp(ptokp->strval,srchtok) != 0; ptokp++);
			if(*ptokp==EMPTY_PAIR) ptokp=0;
			return ptokp;
		}

		bool loadOBJ(const char * objfile, Trimesh * pmesh)
		{
			//cout << "loading: " << objfile << endl;
			ifstream ifs;
			char line[LINE_SIZE];
			char * tok;
			ifs.open(objfile);

			bool not_screwy = ifs.good();
			while(!ifs.eof() && not_screwy)
			{
				ifs.getline(line,LINE_SIZE);
				//cout << "line: " << line << endl;
				tok=strtok(line,TOK_SEPS);
				TokenPair * ptokp=tokenMatch(tok);
				if(ptokp)
				{
					switch(ptokp->tokID)
					{
						case T_VERT    : processVertex(tok,pmesh); break;
						case T_FACE    : processFace(tok,pmesh); break;
						default: processSkip(tok); break;
					}
				}
			}
			if(not_screwy)
				pmesh->writeDone();
			else
				cout << "File not found.\n\n";
			ifs.close();
			return not_screwy;
		}

		int readFloats(char * tok, float * buf, int bufsz)
		{
			int i=0;
			while((tok=strtok(0, TOK_SEPS)) != 0 && i<bufsz)
				buf[i++]=atof(tok);
			return i;
		}

		int readInts(char * tok, int * buf, int bufsz)
		{
			int i=0;
			while((tok=strtok(0, TOK_SEPS)) != 0 && i<bufsz)
				buf[i++]=atoi(tok);
			return i;
		}

		void processSkip(char * tok)
		{}


		void processVertex(char * tok, Trimesh * pmsh)
		{
			float values[3];
			int cnt=readFloats(tok,values,3);	
			if(cnt>=3) pmsh->addVertex(values);
		}

		bool processFace(char * tok, Trimesh * pmsh)
		{
			int ids[256]; 
			int cnt=readInts(tok,ids,256);
			if(cnt>=3)
			{
				int tri[3]={ids[0]-1,ids[1]-1,ids[2]-1};
				pmsh->addFace(tri);
				for(int i=3;i<cnt;i++)
				{
					tri[1]=tri[2];
					tri[2]=ids[i]-1;
					pmsh->addFace(tri);
				}
			}

			return true;
		}
};

TokenPair TrimeshLoader::EMPTY_PAIR={"",T_NONE};

TokenPair TrimeshLoader::tokenMap[] = { 
	{"v", T_VERT}, {"f",T_FACE}, 
	EMPTY_PAIR	
};

char TrimeshLoader::TOK_SEPS[] = " \t";
#endif
