#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <math.h>

// gcc -o TP4_1 TP4_1.c -lglut -lGL -lm -pthread -lpthread  && ./TP4_1

void affiche();
void display();
void *Thread_affichage(void *arg);

// Ajout de la structure question 2
typedef struct s_threadData
{
    float x, y, r;
    int vitesse, taille, directionX, directionY, nbRebonds, cptRebonds;
} ThreadData;

ThreadData data;

void *Thread_affichage(void *arguments)
{
    printf("Thread affichage\n");
    int nbarg = 1;
    glutInit(&nbarg, (char **)arguments);        // fonction d’initialisation
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // utilise le codage RGB en double buffer
    glutInitWindowSize(500, 500);                // taille de la fenêtre que l’on va créer
    glutInitWindowPosition(50, 10);              // position de la fenêtre sur l’écran
    glutCreateWindow("Bonjour");                 // nom la fenêtre
    glClearColor(1.0, 1.0, 1.0, 0.0);            // défini la couleur d’effacement par défaut, ici blanc
    glOrtho(0, 500, 0, 500, -1.0, 1.0);          // défini la position du repère dans la scène visible
    glutDisplayFunc(display);                    // défini la fonction appelée pour afficher la scène
    glutIdleFunc(affiche);                       // défini la fonction lancée lorsque le système n’a plus rien a faire
    glutMainLoop();        
}

void affiche()
{
    glutPostRedisplay();
    usleep(10000);
}

void drawCircle(float cx, float cy, float r, int num_segments)
{
    float theta = 3.1415926 * 2 / (float)num_segments;
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float x = r;//we start at angle = 0 

    float y = 0;
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);//output vertex 

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -y;
        float ty = x;

        //add the tangential vector 

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        //correct using the radial factor 

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // efface le buffer
    glBegin(GL_LINE_LOOP);            // trace des lignes entre des couples de points
    //glVertex3f(10, 10, 0.0);      // point 1
    //glVertex3f(100, 100, 0.0);    // point 2
    glColor3f(0.0, 0.5, 0.5);

    // La balle n'est plus affichée si elle a fait le nombre de rebonds prévus (question 3)
    if(data.cptRebonds < data.nbRebonds)
    {
        drawCircle(data.x, data.y, data.r, 100);
    }
    glEnd();
    glutSwapBuffers(); // affiche à l’écran le buffer dans lequel nous avons dessiné
}

void majCoordBalle(){

    while(1){

        // Gestion des rebonds pour la question 3

        // if the ball is at the right of the screen, we change the direction
        if(data.x + data.r >= 500){
            data.directionX = -1;
            data.cptRebonds++;
        }

        // if the ball is at the left of the screen, we change the direction
        if(data.x - data.r <= 0){
            data.directionX = 1;
            data.cptRebonds++;
        }

        //if the ball is at the top of the screen, we change the direction
        if(data.y + data.r >= 500){
            data.directionY = -1;
            data.cptRebonds++;
        }

        //if the ball is at the bottom of the screen, we change the direction
        if(data.y - data.r <= 0){
            data.directionY = 1;
            data.cptRebonds++;
        }

        //we update the position of the ball
        data.x += data.directionX * data.vitesse;
        data.y += data.directionY * data.vitesse;
        
        usleep(10000);
    }
}

int main(int argc, char *argv[])
{
    pthread_t thread_balle, thread_affichage;

    srand(time(NULL));
    data.r = rand() % 70 + 30;
    data.vitesse = (rand() % 4) + 1;

    if(rand() % 2 == 0){
        data.directionX = -1;
    }
    else{
        data.directionX = 1;
    }

    if(rand() % 2 == 0){
        data.directionY = -1;
    }
    else{
        data.directionY = 1;
    }
    
    data.cptRebonds = 0;
    data.nbRebonds = (rand() % 11) + 1;
    // Generate a random integer between 50 and 450
    data.x = rand() % (450 - 50 + 1) + 50;
    data.y = rand() % (450 - 50 + 1) + 50;

    if (pthread_create(&thread_affichage, NULL, Thread_affichage, argv) == 0)
    {
        printf("Thread affichage ok !\n");
    }

    if (pthread_create(&thread_balle, NULL, majCoordBalle, NULL) == 0)
    {
        printf("Thread balle ok !\n");
    }

    /* Première question 1
    while (1)
    {
        // if the ball touch the side of the window, we stop the program
        if(data.x-data.r == 0 || data.x+data.r == 500 || data.y-data.r == 0 || data.y+data.r == 500){
            printf("Fin du programme\n");
            exit(0);
        }       
    }
    */

    pthread_join(thread_balle, NULL);
    pthread_join(thread_affichage, NULL);

    return 0;
}