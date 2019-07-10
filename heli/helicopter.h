

class Helicopter{

      public:
         float height,velocity;  //y position and vertical speed of copter
         float upwardThrust;     //the force that it is pushing up with
         Helicopter(float _height, float _vel);
         void Draw();   //draws to screen
		

};

Helicopter::Helicopter(float _height, float _vel)
{
   height=_height;
   velocity=_vel;
   upwardThrust=0;
}


//
//void Helicopter::Draw()
//{
//	  
//	//glClear(GL_COLOR_BUFFER_BIT);
//	//glClearColor(0.1f,0.1f,0.1f,0.0);
//	//glColor3f(1,0,0);
//	
//
//	glEnable(GL_ALPHA_TEST);
//			
//   glColor3f(1.0,0.0,0.0);
//
//	
//
//    
//    //body
//    glBegin(GL_POLYGON);
//
////	glTexCoord2f(150.0,height);
//     glVertex2f(150,height);
//    
//	// glTexCoord2f(300.0,height);
//	 glVertex2f(300,height);
//
//	 //glTexCoord2f(300.0,height-50);
//     glVertex2f(300,height-50);
//
////	 glTexCoord2f(150.0,height-50);
//     glVertex2f(150,height-50);
//    /* glVertex2f(175,height-25);
//     glVertex2f(170,height-35);
//     glVertex2f(150,height-45);
//     glVertex2f(120,height-45);
//     glVertex2f(100,height-30);
//     glVertex2f(85,height-30);
//     glVertex2f(80,height-45);
//     glVertex2f(75,height-45);
//     glVertex2f(75,height-5);
//     glVertex2f(80,height-5);
//     glVertex2f(85,height-20);*/
//    glEnd();
//  //   glutSwapBuffers();
//	glFlush();
//
//    //blade
//  /*  glBegin(GL_LINE_STRIP);
//     glVertex2f(133.5,height-5);
//     glVertex2f(133.5,height);
//     glVertex2f(90,height);
//     glVertex2f(90,height+5);
//     glVertex2f(180,height+5);
//     glVertex2f(180,height);
//     glVertex2f(137.5,height);
//     glVertex2f(137.5,height-5);
//    glEnd();
//    */
//    //window
//    //glBegin(GL_LINE_STRIP);
//    // glVertex2f(150,height-5);
//    // glVertex2f(167,height-25);
//    // glVertex2f(175,height-25);
//    //glEnd();
//    //
//    ////bottom rail
//    //glBegin(GL_LINE_STRIP);
//    // glVertex2f(120,height-45);
//    // glVertex2f(120,height-50);
//    // glVertex2f(115,height-50);
//    // glVertex2f(110,height-45);
//    // glVertex2f(105,height-50);
//    // glVertex2f(110,height-55);
//    // glVertex2f(160,height-55);
//    // glVertex2f(165,height-50);
//    // glVertex2f(160,height-45);
//    // glVertex2f(155,height-50);
//    // glVertex2f(150,height-50);
//    // glVertex2f(150,height-45);
//    // glVertex2f(147,height-45);
//    // glVertex2f(147,height-50);
//    // glVertex2f(123,height-50);
//    // glVertex2f(123,height-45);
//    //glEnd();
//
////	glBegin(GL_POLYGON);
////		
////	//	glTexCoord2f(1.0,2.0);
////		glVertex2f(100.0,25.0);
////		
//////		glTexCoord2f(3.0,2.0);
////		glVertex2f(200.5,25.0);
////
////	//	glTexCoord2f(3.0,1.0);
////		glVertex2f(200.0,40.0);
////
////		//glTexCoord2f(2.0,3.0);
////		glVertex2f(100.5,40.0);
////
////	glEnd();
//}
//






void Helicopter::Draw()
{
 //   glColor3f(1,1,1);
    
    //body
    glBegin(GL_POLYGON);
     glVertex2f(100,height-20);
     glVertex2f(120,height-5);
     glVertex2f(150,height-5);
     glVertex2f(170,height-15);
     glVertex2f(175,height-25);
     glVertex2f(170,height-35);
     glVertex2f(150,height-45);
     glVertex2f(120,height-45);
     glVertex2f(100,height-30);
     glVertex2f(85,height-30);
     glVertex2f(80,height-45);
     glVertex2f(75,height-45);
     glVertex2f(75,height-5);
     glVertex2f(80,height-5);
     glVertex2f(85,height-20);
    glEnd();
    
    //blade
    glBegin(GL_POLYGON);
     glVertex2f(133.5,height-5);
     glVertex2f(133.5,height);
     glVertex2f(90,height);
     glVertex2f(90,height+5);
     glVertex2f(180,height+5);
     glVertex2f(180,height);
     glVertex2f(137.5,height);
     glVertex2f(137.5,height-5);
    glEnd();
    
    //window
    glBegin(GL_LINE_STRIP);
     glVertex2f(150,height-5);
     glVertex2f(167,height-25);
     glVertex2f(175,height-25);
    glEnd();
    
    //bottom rail
    glBegin(GL_POLYGON);
     glVertex2f(120,height-45);
     glVertex2f(120,height-50);
     glVertex2f(115,height-50);
     glVertex2f(110,height-45);
     glVertex2f(105,height-50);
     glVertex2f(110,height-55);
     glVertex2f(160,height-55);
     glVertex2f(165,height-50);
     glVertex2f(160,height-45);
     glVertex2f(155,height-50);
     glVertex2f(150,height-50);
     glVertex2f(150,height-45);
     glVertex2f(147,height-45);
     glVertex2f(147,height-50);
     glVertex2f(123,height-50);
     glVertex2f(123,height-45);
    glEnd();
}