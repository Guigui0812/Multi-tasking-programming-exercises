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

// gcc -o TP4_2 TP4_2.c -lglut -lGL -lm -pthread -lpthread  && ./TP4_2

void affiche();
void display();
void *Thread_affichage(void *arg);

// Ajout de la structure question 2
typedef struct s_threadData
{
    float x, y, r;
    GLfloat colors[3];
    int vitesse, taille, directionX, directionY, nbRebonds, cptRebonds, vivant, numBall;
} ThreadData;

ThreadData data[10];
int nbBalls;

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
    float tangetial_factor = tanf(theta); // calculate the tangential factor

    float radial_factor = cosf(theta); // calculate the radial factor

    float x = r; // we start at angle = 0

    float y = 0;
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy); // output vertex

        // calculate the tangential vector
        // remember, the radial vector is (x, y)
        // to get the tangential vector we flip those coordinates and negate one of them

        float tx = -y;
        float ty = x;

        // add the tangential vector

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        // correct using the radial factor

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // efface le buffer
    glBegin(GL_LINE_LOOP);        // trace des lignes entre des couples de points
    // glVertex3f(10, 10, 0.0);      // point 1
    // glVertex3f(100, 100, 0.0);    // point 2

    for (int i = 0; i < nbBalls; i++)
    {
        if (data[i].vivant == 1)
        {
            glColor3f(data[i].colors[0], data[i].colors[1], data[i].colors[2]);
            drawCircle(data[i].x, data[i].y, data[i].r, 100);
        }
    }

    glEnd();
    glutSwapBuffers(); // affiche à l’écran le buffer dans lequel nous avons dessiné

    for (int j = 0; j < nbBalls; j++)
    {
        int ballsAlive = nbBalls;

        for (int j = 0; j < nbBalls; j++)
        {
            if (data[j].vivant == 1)
            {
                ballsAlive++;
            }
            else
            {
                ballsAlive--;
            }
        }

        if (ballsAlive == 0)
        {
            
            printf("All balls are dead, exiting program\n");
            pthread_exit(0);
        }
    }
}

void majCoordBalle(void *numBall)
{
    int i = *((int *) numBall);

    while (1)
    {
        // Gestion des rebonds pour la question 3

        if (data[i].vivant == 1)
        {
            // if the ball is at the right of the screen, we change the direction
            if (data[i].x + data[i].r >= 500)
            {
                data[i].directionX = -1;
                data[i].cptRebonds++;
            }

            // if the ball is at the left of the screen, we change the direction
            if (data[i].x - data[i].r <= 0)
            {
                data[i].directionX = 1;
                data[i].cptRebonds++;
            }

            // if the ball is at the top of the screen, we change the direction
            if (data[i].y + data[i].r >= 500)
            {
                data[i].directionY = -1;
                data[i].cptRebonds++;
            }

            // if the ball is at the bottom of the screen, we change the direction
            if (data[i].y - data[i].r <= 0)
            {
                data[i].directionY = 1;
                data[i].cptRebonds++;
            }

            if (data[i].cptRebonds >= data[i].nbRebonds)
            {
                data[i].vivant = 0;
                free(numBall);
                pthread_exit(0);
            }

            // we update the position of the ball
            data[i].x += data[i].directionX * data[i].vitesse;
            data[i].y += data[i].directionY * data[i].vitesse;
        }

        usleep(10000);
    }
}

void createBall()
{
    for (int i = 0; i < nbBalls; i++)
    {
        data[i].r = rand() % 70 + 30;
        data[i].vitesse = (rand() % 4) + 1;

        if (rand() % 2 == 0)
        {
            data[i].directionX = -1;
        }
        else
        {
            data[i].directionX = 1;
        }

        if (rand() % 2 == 0)
        {
            data[i].directionY = -1;
        }
        else
        {
            data[i].directionY = 1;
        }

        data[i].cptRebonds = 0;
        data[i].vivant = 1;

        data[i].nbRebonds = (rand() % 20) + 1;
        data[i].x = rand() % (450 - 50 + 1) + 50;
        data[i].y = rand() % (450 - 50 + 1) + 50;

        data[i].colors[0] = (GLfloat)((float)rand() / (float)(RAND_MAX)) * 1.0;
        data[i].colors[1] = (GLfloat)((float)rand() / (float)(RAND_MAX)) * 1.0;
        data[i].colors[2] = (GLfloat)((float)rand() / (float)(RAND_MAX)) * 1.0;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    // nbBalls is an integer in 1 and 10
    nbBalls = (rand() % 10) + 1;
    pthread_t thread_affichage;
    pthread_t balles[10];
    int i = 0;
    
    createBall();

    for (i = 0; i < nbBalls; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;

        if (pthread_create(&balles[i], NULL, majCoordBalle, arg) == 0)
        {
            printf("Ball %d created\n", i);
        }
    }

    if (pthread_create(&thread_affichage, NULL, Thread_affichage, argv) == 0)
    {
        printf("Thread affichage ok !\n");
    }

    for (int i = 0; i < nbBalls; i++)
    {
        pthread_join(balles[i], NULL);
    }
    pthread_join(thread_affichage, NULL);

    return 0;
}