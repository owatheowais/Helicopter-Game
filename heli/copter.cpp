
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <glut.h>

#include "helicopter.h"
#include "textures.h"
#include "wav.h"


using namespace std;

#define NUM_BOXES 25

float forceGrav=0.2;              //the acceleration speed of the helicopter
float width,height;               //width/height of the window
bool isStart=true;                //used to tell if game is starting
float boxWidth;                   //width of each green box in pixels
float boxHeights[NUM_BOXES];      //array to hold the y location of each of the 20 green boxes
float maxVel=20;                  //maximum vertical speed of the helicopter
bool gameover=false;               //used to tell if game is gameover or not
int maxChange=70;                 //the maximum number of pixels the green boxes can move up or down each time
int lastChange;                   //the last time that the game got harder (to prevent it from changing more than once every 3 seconds)
//int score=0;                      //current score
int highScore=0;                  //high score
time_t start,current;             //used to keep track of time to tell the game when to get harder (every 3 secs)
char type=2;                      //used to keep track of current tunnel type (0=boxes,1=line)

#define PI 3.141592

//RGBpixmap pix[1];

int x=0;
int y=0;
float waterMovement =0.0;
float grassMovement = 0.0;
float hillMovement =0.0;

float waterTextureOffset;
float grassTextureOffset;
float hillTextureOffset;


extern const float WATER_TEXTURE_SIZE=10.0;
extern const float GRASS_TEXTURE_SIZE=10.0;
extern const float HILL_TEXTURE_SIZE=10.0;

extern const float WATER_ALPHA=0.0;
extern const float GRASS_ALPHA=0.0;
extern const float HILL_ALPHA=0.0;

//extern const float WATER_TERTURE_TIME;
const float WATER_TERTURE_TIME=8.0f;
const float HILL_TERTURE_TIME=8.0f;
const float GRASS_TERTURE_TIME=8.0f;

const float STEP_TIME=0.02f;



unsigned int textuer[6];

GLuint t_grass;
GLuint t_hill;
GLuint t_water;


Helicopter heli(3500,0);
   


void InitializeBoxes()    //does everything that needs to be done each time game restarts
{ 
    
     heli.height=height/2+25; 
     
     for(int i=0; i<NUM_BOXES; i++)    //sets all of the green box positions equal
       boxHeights[i]=height/2+175;
}






void DrawBoxes(char type)
{
      glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex2f(0,550);
	glVertex2f(1300,550);
	glVertex2f(1300,560);
	glVertex2f(0,560);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(0,190);
	glVertex2f(1300,190);
	glVertex2f(1300,200);
	glVertex2f(0,200);
	glEnd();
  
}
void draw_wall()
   {     
          glPushMatrix();
		  glTranslatef(x,y,0.0);		

		

		 // glEnable(GL_TEXTURE_2D);
		//  glBindTexture(GL_TEXTURE_2D, 2012);
		  glBegin(GL_POLYGON);  
		//glColor3f(1.0, 1.0, 1.0);			
			//glTexCoord2f(1200.0 ,700.0 );
			glVertex2f(700.0 , 400.0 );
			//glTexCoord2f(1200.0 ,600.0 );
			glVertex2f(700.0 , 550.0 );
			//glTexCoord2f(1210.0 ,600.0 );
			glVertex2f(710.0 , 550.0 );
			//glTexCoord2f(1210.0 ,700.0 );
			glVertex2f(710.0 , 400.0 );
		   glEnd();
		   //glBegin(GL_POLYGON);  
		
		   glBegin(GL_POLYGON);
		//glColor3f(1.0, 1.0, 1.0);
		//glTexCoord2f(200.0 ,0.0 );
			//glTexCoord2f(800.0 ,350.0 );
			glVertex2f(1000.0 , 200.0 );
			//glTexCoord2f(800.0 ,500.0 );
			glVertex2f(1000.0 , 300.0 );
			//glTexCoord2f(810.0 ,500.0 );
			glVertex2f(1010.0 , 300.0 );
			//glTexCoord2f(810.0 ,350.0 );
			glVertex2f(1010.0 , 200.0 );
			glEnd();
			glBegin(GL_POLYGON);
		//glColor3f(1.0, 1.0, 1.0);
		//glTexCoord2f(200.0 ,0.0 );
			//glTexCoord2f(1400.0 ,350.0 );
			glVertex2f(1400.0 , 350.0 );
			//glTexCoord2f(1400.0 ,500.0 );
			glVertex2f(1400.0 , 500.0 );
			//glTexCoord2f(1410.0 ,500.0 );
			glVertex2f(1410.0 , 500.0 );
			//glTexCoord2f(1410.0 ,350.0 );
			glVertex2f(1410.0 , 350.0 );
   
      glEnd();
	  if(x>-1200)
	  {
		  x-=10;
	  }
	  else if(x==-1200)
	  {
	  x=0;
	  }
	
	  glPopMatrix();   
     glutSwapBuffers();
     glFlush();
   
   }

void myInit()
{
	t_hill = LoadTexture("hill.bmp");
	t_water = LoadTexture("water.bmp");
	t_grass = LoadTexture("grass.bmp");
	
	//t_water=texture[1]=LoadTexture("reflection.bmp");
	glEnable(GL_TEXTURE_2D);
}



void drawHill(int id)
{
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	glDisable(GL_NORMALIZE);

	glEnable(GL_BLEND);

	//glColor4f(1,1,1,HILL_ALPHA);




	glBegin(GL_QUADS);

	glTexCoord2f(0.0f+sin(hillMovement/10.0),0.0f);		glVertex2i(1400,730);			// right top corner
	glTexCoord2f(1.0f+sin(hillMovement/10.0),0.0f);		glVertex2i(0,730);			// left top corner
	glTexCoord2f(1.0f+sin(hillMovement/10.0),1.0f);		glVertex2i(0,550);			// left bottom corner
	glTexCoord2f(0.0f+sin(hillMovement/10.0),1.0f);		glVertex2i(1400,550);			// right bottom corner

	glEnd();

	
}


void drawWater(int id)
{
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	glDisable(GL_NORMALIZE);

	glEnable(GL_BLEND);

//	glColor4f(1,1,1,WATER_ALPHA);




	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f+sin(waterMovement/10.0),1.0f);		glVertex2i(1400,550);		// right top corner
	glTexCoord2f(1.0f+sin(waterMovement/10.0),0.0f);		glVertex2i(0,550);		// left top corner
	glTexCoord2f(1.0f+sin(waterMovement/10.0),1.0f);		glVertex2i(0,200);		// left bottom corner
	glTexCoord2f(0.0f+sin(waterMovement/10.0),0.0f);		glVertex2i(1400,200);		// right bottom corner

	glEnd();

	
}

void drawGrass(int id)
{
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	glDisable(GL_NORMALIZE);

	glEnable(GL_BLEND);

//	glColor4f(1,1,1,GRASS_ALPHA);




	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f+sin(grassMovement/10.0),0.0f);		glVertex2i(1400,200);		// right top corner
	glTexCoord2f(1.0f+sin(grassMovement/10.0),0.0f);		glVertex2i(0,200);			// left top corner
	glTexCoord2f(1.0f+sin(grassMovement/10.0),1.0f);		glVertex2i(0,0);			// left bottom corner
	glTexCoord2f(0.0f+sin(grassMovement/10.0),1.0f);		glVertex2i(1400,0);			// right bottom corner

	glEnd();

	
}

void animate(void)
{

 
	waterMovement-=0.001;
	
	hillMovement-=0.001;
	
	grassMovement-=0.001;
	
 //  glutPostRedisplay();

}


 

void Start()
{
if(isStart){
      InitializeBoxes();
      isStart=false;
   }
}

void movement()
{
//calculate the new velocity and height of the copter
   heli.velocity+=forceGrav/100-heli.upwardThrust/100;
   heli.height-=heli.velocity;
   
   //limits the copters vertical speed
   if(heli.velocity>maxVel)heli.velocity=maxVel;
   if(heli.velocity<-maxVel)heli.velocity=-maxVel;

}


void collision()
{
	
   //stop game if copter hits a green box
   if(heli.height>boxHeights[(int)floor(100/boxWidth)]-5 || heli.height<boxHeights[(int)floor(100/boxWidth)]-295)
  
     gameover=true;
}


void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   drawHill(t_hill);
	drawWater(t_water);
	drawGrass(t_grass);	
   
   time(&current);   //updates the current time
   
   Start();   
   
   
   DrawBoxes(type);
   
   movement();

   heli.Draw();

  
   collision(); 

   
   
  glutSwapBuffers();
   
   if(!gameover)
   glutPostRedisplay();

}

void mouse(int button, int state, int x, int y)
{
     if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
        heli.upwardThrust=2*forceGrav;
     else if(button==GLUT_LEFT_BUTTON)
        heli.upwardThrust=0;
}



 void wall()
   {
//     glClear (GL_COLOR_BUFFER_BIT);       
	  draw_wall();  
  // glutSwapBuffers();
//     glutPostRedisplay();
   }

 /*void Timer( int value)
   {

	   wall();
	   
	   glutTimerFunc(50, Timer, 1);
	   glutPostRedisplay();
   }*/

 void mytimer(int t)
{
	 wall();
	glutTimerFunc(70,mytimer,1);
	// glutSwapBuffers();
	//glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   width=w;
   height=h;
   gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

void keyboard(unsigned char key, int x, int y)
{char b;
     switch(key){
        case 27:
             exit(0);
             break;
        case 'R':
        case 'r':
             isStart=true;
             glutPostRedisplay();      
	
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (1300, 750); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Tunnel");
 //  glutFullScreen();
     glutTimerFunc(50,mytimer,1);
   glutDisplayFunc(display); 

   glutReshapeFunc(reshape);
   glutIdleFunc(animate);    
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   PlayBackgroundSound("heli-running2.wav");
 //  glutTimerFunc(50, Timer, 1);
 
   	myInit();
   glutMainLoop();
   return 0;
}