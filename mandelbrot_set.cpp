#define GLUT_DISABLE_ATEXIT_HACK
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include <stdio.h>
#include<string.h>

/*
*defining a RGB struct to color the pixel
*/
struct Type_rgb{
       float r;
       float g;
       float b;
};
int flag = 0;

/*
* pixel variable contain the value of the color pixel in
* the picture.
* pattern is a predefine set of color for a particular
* value.
*/
struct Type_rgb pixels[840*1440], pattern[999];


void init();
void display();
char welcome[10] = "WELCOME";
char *  description[11][200];
void mouse(int,int,int,int);
void keys(unsigned char,int,int);
void welcome_display();
void description_display();
void mandelbrot_display();
void thank_you_display();
void mandelbrotset();
void menu(int id);
void text_display(GLfloat,GLfloat,char*);

int main(int argc, char** argv){
/*
* Here basic Opengl initialization.
*/
    glutInit(&argc, argv);
    glutInitWindowSize (1440, 840);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition (0,0);
    glutCreateWindow ("Mandelbrotset");

    init();
/*
* connecting the Display function
*/
    glutFullScreen();
   glutMouseFunc(mouse);
   glutKeyboardFunc(keys);
    glutDisplayFunc(display);
    glutCreateMenu(menu);
    glutAddMenuEntry("exit",0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

/*
* starting the activities
*/

    glutMainLoop();
    return 0;
}

void init() {

    glViewport(0, 0, 1440, 840);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity( );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity( );
    gluOrtho2D(0, 1440, 0, 840);

     int i;
   float r, g, b;
/*
* Initializing all the pixels to white.
*/
   for(i = 0; i < 840*1440; i++){
      pixels[i].r = 1;
      pixels[i].g = 1;
      pixels[i].b = 1;
   }

    i = 0;
/*
* Initializing all the pattern color till 9*9*9
*/
   for(r = 0.1; r <= 0.9; r= r+0.1)
      for(g = 0.1; g <= 0.9; g = g+0.1)
         for(b = 0.1; b <= 0.9; b = b+0.1){
            pattern[i].r = b;
            pattern[i].g = r;
            pattern[i].b = g;
            i++;
         }
/*
* Initializing the rest of the pattern as 9*9*9 is 729.
* and we need up to 999 pattern as the loop bailout
* condition is 1000.
*/

   for( ; i <= 999; i++){
      pattern[i].r = 1;
      pattern[i].g = 1;
      pattern[i].b = 1;
   }


}

void display() {
    glClearColor(1,0.5,0.5,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5,0,0);
    if(flag==0) welcome_display();
    else if(flag==1) description_display();
    else if(flag==2) mandelbrot_display();
    else if(flag==3) thank_you_display();

	glutSwapBuffers();
	glFlush();
	if(flag==1) mandelbrotset();
}

void welcome_display() {
    text_display(300,700,"filepath:\\welcome.txt");
}

void description_display() {
    text_display(100,700,"filepath:\\mandelbrot_set_description.txt");
}

void mandelbrot_display() {
    glClearColor(0.2,0.1,0.1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(0,0);
    glDrawPixels(1440, 840, GL_RGB, GL_FLOAT, pixels);
}

void mouse(int btn,int state,int x,int y) {
    printf("%d %d\n",x,y);
    if(btn==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&flag==0) {
            flag=1;
            glutRemoveMenuItem(1);
            glutAddMenuEntry("Generate Mandelbrot Set",1);
            glutAddMenuEntry("exit",0);
            display();
    }
   /* if(btn==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&flag==1) {
        if(x>=500&&x<=600&&(840-y)>=50&&(840-y)<=100) {
                flag=2;
                display();
        }
    */
}

void mandelbrotset(){
/*
* x0 :- is the real part of c value
*       will range from -2.5 to 1.1.
* y0 :- is the imaginary part of c value
*       will range from -1 to 1.1.
* x and y :- is the real and imaginary part of Zn.
* iteration :- is to keep control variable of the number
*       of iteration
* max_iteration :- maximum number of iteration
*        (which is one of bailout condition)
* loc :- represent the location pixel of the
*       current x,y coordinate.
*/

float x0, y0, x, y, xtemp;
int iteration, max_iteration, loc=0;
printf("\nstart");
for(y0 = -1; y0 < 1.1; y0 = y0 + 0.0025)
   for(x0 = -2.5; x0 < 1.1; x0 = x0 + 0.0025){
     x = 0;
     y = 0;
     iteration = 0;
     max_iteration = 1000;
/*
* (x*x) + (y*y) < (2*2) is the 2nd bailout condition ie
* the mandelbrot set is always within a radius of 2.
*/
while(((x*x) + (y*y) < (2*2)) && iteration < max_iteration){
   xtemp = (x*x) - (y*y) + x0;
   y = (2*x*y) + y0;

   x = xtemp;
   iteration = iteration + 1;
   }
   if(loc>1211000) break;
if(iteration >= 999){
/*
* setting color pixel to Mandelbrot set coordinate
*to black.
*/
     pixels[loc].r = 0;
     pixels[loc].g = 0;
     pixels[loc].b = 0;
}else{
/*
* setting color pixel to the reset of the coordinate by the
* pattern of no of iteration before bailout.
*/
     pixels[loc].r = pattern[iteration].r;
     pixels[loc].g = pattern[iteration].g;
     pixels[loc].b = pattern[iteration].b;
}
loc = loc + 1;
}
}

void menu(int id) {
    if(id==1) {
        glutRemoveMenuItem(1);
        flag=2;
        display();
    }
    if(id==0) {
            flag=3;
            glutRemoveMenuItem(1);
            display();
    }
}

void text_display(GLfloat x,GLfloat y,char* file) {
    char buff[200];
    char *c;
    FILE *fp;
    if ((fp = fopen(file,"r")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }
   if (fp) {
        while (fgets(buff, 200, fp)!=NULL){
            glRasterPos2f(x,y);
            for (c=buff; *c!='\0'; c++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*c);
            }
            y -= 38;
        }
        fclose(fp);
    }
}

void thank_you_display() {
    char *c;
    glTranslatef(350,500,0);
    glScalef(0.5,0.5,1);
    glLineWidth(7);
    for(c="T H A N K  Y O U !";*c!='\0';c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN,*c);
    }

}

void keys(unsigned char key,int x,int y) {
    if(flag==3) exit(0);
}
