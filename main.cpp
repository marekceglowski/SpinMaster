#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "mathLib.h"

using namespace std;

//Blueprints for functions
int main(int argc, char *argv[]);
void display(void);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void passive(int x, int y);
void timer(int value);
void createParticle(int x, int y);
void deleteParticle(int x, int y);
void moveParticles(bool inward, int x, int y);
void rotateParticles(int x, int y);
void explodeParticles(int x, int y);
void wobbleParticles();
void fileWrite();
void fileLoad(string filePath);
void printCommands();
void particleColorChange(int colorChange);
void flashParticles();
float randomFloat(float min, float max);
struct Particle;

static vector<Particle> particles;
static int screenWidth = 600;           //Screen Width
static int screenHeight = 600;          //Screen Height
static int globalRange = 400;           //Global Range Increment
static int globalSpeed = 0;             //Global Speed Increment
static int globalSize = 0;              //Global Size Increment
static int startingParticles = 2000;    //Starting Particle Amount
static int particleCounter = 0;         //Particle Counter for Measuring Time
static int explosionTimer = 50;         //Time to Switch Explosion Direction
static bool lMouseDown = false;         //Left Mouse Held Down
static bool rMouseDown = false;         //Right Mouse Held Down
static bool spaceBar = false;           //Space Bar Pressed
static bool pFlash = false;             //Flash Particles
static bool tracerMode = false;         //Tracer Mode Trigger
static bool savedSpinMode = false;      //Saved Spin Trigger
static bool explodeMode = false;        //Explosion Trigger
static bool explodeBackwards = false;   //Explode Backwards Trigger
static bool wobbleMode = false;         //Wobble Mode Trigger
static point2D mousePoint;              //Mouse Cursor Position
static point2D savedPoint;              //Saved Spin Position

struct Particle
{
    point2D position;
    colour color;
    vec2D direction;
    int size;
    float range;
    float speed;
    bool wobbleUp;
};

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);  //Above createParticle to make sure it is run before display() is run
    glutInitWindowSize(screenWidth,screenHeight);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow("2D Particle Flocking System");

    //Create starting particles randomly on screen
    for (int i=0;i<startingParticles;i++)
        createParticle(rand() % screenWidth, rand() % screenHeight);
    printCommands();
    mousePoint = point2D(0,0);
    savedPoint = point2D(0,0);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive);
    glutTimerFunc(5,timer,5);
    gluOrtho2D(0,screenWidth,screenHeight,0);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void display(void)
{
    if (tracerMode == false)
        glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1,0.1,0.1,1);

    if (spaceBar == false)
    {
        if (savedSpinMode == true)
            rotateParticles(savedPoint.x,savedPoint.y);

        for (int i=0;i<particles.size();i++)    //For every particle
        {
            glPointSize(particles.at(i).size + globalSize);  //Set size
            glColor3f(particles.at(i).color.r,particles.at(i).color.g,particles.at(i).color.b); //Set color

            glBegin(GL_POINTS); //Draw particle
                glVertex2f(particles.at(i).position.x,particles.at(i).position.y);
            glEnd();
        }

        glFlush();
        glutSwapBuffers();
    }
}

void keyboard(unsigned char key, int mX, int mY)
{
    mousePoint = point2D(mX,mY);
    if (spaceBar == false)
    {
        if (glutGetModifiers() == 0)
        {
            if (key == 'a' || key == 'A')
            {
                createParticle(mX,mY);
            }
            else if (key == 'n' || key == 'N')
            {
                cout << "NumOfParticles: " << particles.size() << endl;
            }
            else if (key == 'r' || key == 'R')
            {
                particles.clear();
                //Create starting particles randomly on screen
                for (int i=0;i<startingParticles;i++)
                    createParticle(rand() % screenWidth, rand() % screenHeight);
                pFlash = false;             //Flash Mode Trigger
                tracerMode = false;         //Tracer Mode Trigger
                savedSpinMode = false;      //Saved Spin Trigger
                explodeMode = false;        //Explosion Trigger
                explodeBackwards = false;   //Explode Backwards Trigger
                wobbleMode = false;         //Wobble Mode Trigger
                explosionTimer = 50;
                globalSpeed = 0;
                globalSize = 0;

            }
            else if (key == 'd' || key == 'D')
            {
                if (particles.size() > 0)
                    deleteParticle(mX,mY);
            }
            else if (key == 's' || key == 'S')
            {
                if (particles.size() > 0)
                    rotateParticles(mX,mY);
            }
            else if (key == 'c' || key == 'C')
            {
                particles.clear();
                pFlash = false;
            }
            else if (key == 'f' || key == 'F')
            {
                if (pFlash == false)
                {
                    for (int i=0;i<particles.size();i++)
                        particles.at(i).color = colour(0,0,0);
                    pFlash = true;
                }
                else
                    pFlash = false;
            }
            else if (key == 't' || key == 'T')
            {
                if (tracerMode == false)
                    tracerMode = true;
                else
                    tracerMode = false;
            }
            else if (key == 'q' || key == 'Q')
            {
                savedPoint = point2D(mX,mY);
                if (savedSpinMode == false)
                    savedSpinMode = true;
                else
                    savedSpinMode = false;
            }
            else if (key == 'e' || key == 'E')
            {
                if (explodeMode == false)
                    explodeMode = true;
                else
                    explodeMode = false;
            }
            else if (key == 'j' || key == 'J')
            {
                if (explodeMode == false)
                    explodeMode = true;
                else
                    explodeMode = false;
                explosionTimer = 1;
            }
            else if (key == 'w' || key == 'W')
            {
                if (wobbleMode == false)
                    wobbleMode = true;
                else
                    wobbleMode = false;
            }
            else if (key == 'i' || key == 'I')
            {
                fileWrite();
            }
            else if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' ||
                     key == '5' || key == '6' || key == '7')
            {
                if (particles.size() > 0)
                    particleColorChange(key);
            }
            else if (key == '=' || key == '+')
            {
                globalRange = globalRange + 10;
                for (int i=0;i<particles.size();i++)
                    particles.at(i).range = particles.at(i).range + 1;
            }
            else if (key == '-' || key == '_')
            {
                globalRange = globalRange - 10;
                for (int i=0;i<particles.size();i++)
                    particles.at(i).range = particles.at(i).range - 1;
            }
            else if (key == '[' || key == '{')
            {
                globalSize--;
            }
            else if (key == ']' || key == '}')
            {
                globalSize++;
            }
        }   //end of: if (glutGetModifiers() == 0)
        else if (glutGetModifiers() == 1)
        {
            if (key == '1' || key == '!')
                fileLoad("C:\\1.txt");
            else if (key == '2' || key == '@')
                fileLoad("C:\\2.txt");
            else if (key == '3' || key == '#')
                fileLoad("C:\\3.txt");
            else if (key == '4' || key == '$')
                fileLoad("C:\\4.txt");
            else if (key == '5' || key == '%')
                fileLoad("C:\\5.txt");
            else if (key == '6' || key == '^')
                fileLoad("C:\\6.txt");
            else if (key == '7' || key == '&')
                fileLoad("C:\\7.txt");
            else if (key == '8' || key == '*')
                fileLoad("C:\\8.txt");
            else if (key == '9' || key == '(')
                fileLoad("C:\\9.txt");
            else if (key == '0' || key == ')')
                fileLoad("C:\\0.txt");

        }
    }

    //Keys below are unnaffected by spaceBar
    if (key == ' ')
    {
        if (spaceBar == false)
            spaceBar = true;
        else
            spaceBar = false;
    }
    if (key == 27)    //ESC
    {
        exit(EXIT_SUCCESS);
    }
}

void special(int key, int x, int y)
{
    if (spaceBar == false)
    {
        if (key == GLUT_KEY_UP)
        {
            globalSpeed++;
            for (int i=0;i<particles.size();i++)
                particles.at(i).speed = particles.at(i).speed + 1;
        }
        else if (key == GLUT_KEY_DOWN)
        {
            globalSpeed--;
            for (int i=0;i<particles.size();i++)
                particles.at(i).speed = particles.at(i).speed - 1;
        }
        else if (key == GLUT_KEY_LEFT)
        {
            if (explosionTimer > 1)
                explosionTimer--;
        }
        else if (key == GLUT_KEY_RIGHT)
        {
            explosionTimer++;
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    mousePoint = point2D(x,y);
    if (spaceBar == false)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                lMouseDown = true;
                moveParticles(true,x,y);
            }
            else //state == GLUT_UP
                lMouseDown = false;
        }
        if (button == GLUT_RIGHT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                rMouseDown = true;
                moveParticles(false,x,y);
            }
            else //state == GLUT_UP
                rMouseDown = false;
        }
    }
}

void motion(int x, int y)
{
    mousePoint = point2D(x,y);
    if (spaceBar == false)
    {
        if (lMouseDown == true)
            moveParticles(true,x,y);
        if (rMouseDown == true)
            moveParticles(false,x,y);
    }
}

void passive(int x, int y)
{
    mousePoint = point2D(x,y);
}

void timer(int value)
{
    if (spaceBar == false)
    {
        if (lMouseDown == true)
            moveParticles(true,mousePoint.x,mousePoint.y);
        if (rMouseDown == true)
            moveParticles(false,mousePoint.x,mousePoint.y);
        if (explodeMode == true)
            explodeParticles(mousePoint.x,mousePoint.y);
        if (wobbleMode == true)
            wobbleParticles();

        if (explodeMode == true)
        {
            if (particleCounter % explosionTimer == 0)
            {
                if (explodeBackwards == false)  //Reverse direction of movement
                    explodeBackwards = true;
                else
                    explodeBackwards = false;
                particleCounter = 0;
            }
            particleCounter++;
        }

        glutPostRedisplay();
    }
    if (pFlash == true)
        flashParticles();
    glutTimerFunc(5,timer,5);
}

//Creates a single particle at position x,y
void createParticle(int x, int y)
{
    Particle part;
    point2D position = point2D(x, y);
    part.position = position;

    float r = randomFloat(0,1);
    float g = randomFloat(0,1);
    float b = randomFloat(0,1);
    part.color = colour(r,g,b);
    part.range = (rand() % 40 + 20) + globalRange;
    part.size = (rand() % 5 + 1);
    part.speed = (rand() % 5 + 2) + globalSpeed;
    part.wobbleUp = true;

    particles.begin();
    particles.push_back(part);
    //glutPostRedisplay();
}

//Deletes a single particle closest to position x,y
void deleteParticle(int x, int y)
{
    int closestParticleElement = 0;     //Place in the vector that is
    point2D mousePos = point2D(x,y);                //Save mouse's position into mousePos
        for (int i=0;i<particles.size();i++)    //Repeat for all particles
        {
            point2D particlePos = particles.at(i).position; //Save particle's location into particlePos
            point2D closestParticlePos = particles.at(closestParticleElement).position; //Save closest particle's position

            //if distance between cursor and current particle is greater than distance between cursor and "closest" particle
            if (_2DMathLib::fastDistance(particlePos,mousePos) <= _2DMathLib::fastDistance(closestParticlePos,mousePos))
                closestParticleElement = i;
        }
        particles.erase(particles.begin()+closestParticleElement); //Erases particle at position [closestParticleElement]
        particles.end();    //Moves vector marker to end
}

//Moves particles within range of point x,y
//Inward decides if particles attract (true) or deflect (false)
void moveParticles(bool inward, int x, int y)
{
    point2D mousePos = point2D(x,y);        //Save mouse's position into mousePos
    for (int i=0;i<particles.size();i++)    //Repeat stuff below for all particles
    {
        point2D particlePos = particles.at(i).position; //Save particle's location into particlePos

        if (_2DMathLib::distance(particlePos,mousePos) < particles.at(i).range) //if distance between the positions < range
        {
            vec2D vec = _2DMathLib::createVector(particlePos,mousePos); //create vector
            vec = _2DMathLib::normalize(vec);                           //normalize for the direction without length
            if (particles.at(i).speed > 0)
                vec = _2DMathLib::vectorMultiply(vec,particles.at(i).speed);//increase length by speed
            else
                vec = _2DMathLib::vectorMultiply(vec,0);//Set speed to 0

            if (inward==false)                              //if moving outwards
                vec = _2DMathLib::vectorMultiply(vec,-1);   //go backward
            point2D newPoint = _2DMathLib::movePoint(particlePos,vec);  //move position of particle
            particles.at(i).position = newPoint;                        //apply new position
        }
    }
}

void rotateParticles(int x, int y)
{
    int angle = 1;
    for (int i=0;i<particles.size();i++)
    {
        point2D particlePos = particles.at(i).position;
        point2D mousePos = point2D(x,y);
        if (_2DMathLib::distance(particlePos,mousePos) < particles.at(i).range) //if distance between the positions < range
        {
            particlePos.x -= mousePos.x;
            particlePos.y -= mousePos.y;

            float xNew = particlePos.x * cos(angle) - particlePos.y * sin(angle);
            float yNew = particlePos.x * sin(angle) + particlePos.y * cos(angle);

            particles.at(i).position = point2D(mousePos.x + xNew,mousePos.y + yNew);
        }
    }
}

void explodeParticles(int x, int y)
{
    for (int i=0;i<particles.size();i++)
    {
        point2D particlePos = particles.at(i).position;
        point2D mousePos = point2D(x,y);
        if (explodeBackwards == false)
        {
            if (particlePos.x >= mousePos.x)
                particles.at(i).position.x += randomFloat(0,5);
            else
                particles.at(i).position.x -= randomFloat(0,5);
            if (particlePos.y >= mousePos.y)
                particles.at(i).position.y += randomFloat(0,5);
            else
                particles.at(i).position.y -= randomFloat(0,5);
        }
        else
        {
            if (particlePos.x >= mousePos.x)
                particles.at(i).position.x -= randomFloat(0,5);
            else
                particles.at(i).position.x += randomFloat(0,5);
            if (particlePos.y >= mousePos.y)
                particles.at(i).position.y -= randomFloat(0,5);
            else
                particles.at(i).position.y += randomFloat(0,5);
        }

    }
}

void wobbleParticles()
{
    for (int i=0;i<particles.size();i++)
    {
        if (particles.at(i).wobbleUp == true)
        {
            if (particles.at(i).size < 10)
                particles.at(i).size++;
            else
                particles.at(i).wobbleUp = false;
        }
        else
        {
            if (particles.at(i).size > 1)
                particles.at(i).size--;
            else
                particles.at(i).wobbleUp = true;
        }
    }
}

void fileWrite()
{
    ofstream fout;
    string filePath;
    cout << "Please enter full file path:" << endl;
    cin >> filePath;

    fout.open(filePath.c_str());

    fout << globalRange << "\n";
    fout << globalSpeed << "\n";
    fout << globalSize << "\n";
    fout << startingParticles << "\n";
    fout << particleCounter << "\n";
    fout << explosionTimer << "\n";
    fout << spaceBar << "\n";
    fout << pFlash << "\n";
    fout << tracerMode << "\n";
    fout << savedSpinMode << "\n";
    fout << explodeMode << "\n";
    fout << explodeBackwards << "\n";
    fout << wobbleMode << "\n";
    fout << mousePoint.x << "\n";
    fout << mousePoint.y << "\n";
    fout << savedPoint.x << "\n";
    fout << savedPoint.y << "\n";
    fout << particles.size() << "\n";

    for (int i=0;i<particles.size();i++)
    {
        fout << particles.at(i).position.x << "\n";
        fout << particles.at(i).position.y << "\n";
        fout << particles.at(i).color.r << "\n";
        fout << particles.at(i).color.g << "\n";
        fout << particles.at(i).color.b << "\n";
        fout << particles.at(i).direction.x << "\n";
        fout << particles.at(i).direction.y << "\n";
        fout << particles.at(i).size << "\n";
        fout << particles.at(i).range << "\n";
        fout << particles.at(i).speed << "\n";
        fout << particles.at(i).wobbleUp << "\n";
    }
    fout << flush;
    fout.close();
    cout << "File saved." << endl;
}

void fileLoad(string filePath)
{
    particles.clear();
    ifstream fin(filePath.c_str());

    fin >> globalRange;
    fin >> globalSpeed;
    fin >> globalSize;
    fin >> startingParticles;
    fin >> particleCounter;
    fin >> explosionTimer;
    fin >> spaceBar;
    fin >> pFlash;
    fin >> tracerMode;
    fin >> savedSpinMode;
    fin >> explodeMode;
    fin >> explodeBackwards;
    fin >> wobbleMode;
    fin >> mousePoint.x;
    fin >> mousePoint.y;
    fin >> savedPoint.x;
    fin >> savedPoint.y;
    int particleAmount;
    fin >> particleAmount;

    for (int i=0;i<particleAmount;i++)
    {
        Particle p;
        point2D pos;
        colour col;
        vec2D dir;
        int psize;
        float rang;
        float spe;
        bool wob;

        fin >> pos.x;
        fin >> pos.y;
        fin >> col.r;
        fin >> col.g;
        fin >> col.b;
        fin >> dir.x;
        fin >> dir.y;
        fin >> psize;
        fin >> rang;
        fin >> spe;
        fin >> wob;

        p.position = pos;
        p.color = col;
        p.direction = dir;
        p.size = psize;
        p.range = rang;
        p.speed = spe;
        p.wobbleUp = wob;

        particles.begin();
        particles.push_back(p);
    }
}

//Prints a list of commands to the terminal
void printCommands()
{
    cout << "Command List:" << endl;
    cout << " LMouse     Attract Particles" << endl;
    cout << " RMouse     Deflect Particles" << endl;
    cout << " a          Add Particle at Cursor" << endl;
    cout << " d          Delete Closest Particle to Cursor" << endl;
    cout << " s          Spin Particles Around Cursor" << endl;
    cout << " q          Spin Particles Around Saved Point" << endl;
    cout << " w          Wobble Mode" << endl;
    cout << " e          Explode Mode" << endl;
    cout << " j          Jiggle Mode" << endl;
    cout << " f          Flash Mode" << endl;
    cout << " t          Tracer Mode" << endl;
    cout << " n          Output Number of Particles" << endl;
    cout << " c          Clear Screen" << endl;
    cout << " r          Reset Particles" << endl;
    cout << " 1-7        Change Color" << endl;
    cout << " +/-        Increase/Decrease Global Range" << endl;
    cout << " Up/Down    Increase/Decrease Global Speed" << endl;
    cout << " Spacebar   Pause Application" << endl;
    cout << " ESC        Quit Application" << endl;
    cout << endl;
}

void particleColorChange(int newColor)
{
    colour cNewColor;
    switch (newColor)
    {
    case 49:
        cNewColor = colour(1,0,0);
        break;
    case 50:
        cNewColor = colour(0,1,0);
        break;
    case 51:
        cNewColor = colour(0,0,1);
        break;
    case 52:
        cNewColor = colour(1,1,0);
        break;
    case 53:
        cNewColor = colour(0,1,1);
        break;
    case 54:
        cNewColor = colour(1,0,1);
        break;
    case 55:
        cNewColor = colour(1,1,1);
        break;
    }

    for (int i=0;i<particles.size();i++)
        particles.at(i).color = cNewColor;

}

void flashParticles()
{
    for (int i=0;i<particles.size();i++)
    {
        float r = particles.at(i).color.r;
        float g = particles.at(i).color.g;
        float b = particles.at(i).color.b;

        if (r == 0 && g == 0 && b == 0)
            particles.at(i).color.r += 0.05;
        else if (r >= 1 && g <= 0 && b <= 0)
            particles.at(i).color.g += 0.05;
        else if (r >= 1 && g >= 1 && b <= 0)
            particles.at(i).color.r -= 0.05;
        else if (r <= 0 && g >= 1 && b <= 0)
            particles.at(i).color.b += 0.05;
        else if (r <= 0 && g >= 1 && b >= 1)
            particles.at(i).color.g -= 0.05;
        else if (r <= 0 && g <= 0 && b >= 1)
            particles.at(i).color.r += 0.05;
        else if (r >= 1 && g <= 0 && b >= 1)
            particles.at(i).color.b -= 0.05;
        else if (r < 1 && g <= 0 && b >= 1)
            particles.at(i).color.r += 0.05;
        else if (r < 1 && g >= 1 && b <= 0)
            particles.at(i).color.r -= 0.05;
        else if (r < 1 && g <= 0 && b <= 0)
            particles.at(i).color.r += 0.05;
        else if (r <= 0 && g < 1 && b >= 1)
            particles.at(i).color.g -= 0.05;
        else if (r >= 1 && g <= 0 && b < 1)
            particles.at(i).color.b -= 0.05;
        else if (r >= 1 && g < 1 && b <= 0)
            particles.at(i).color.g += 0.05;
        else if (r <= 0 && g >= 1 && b < 1)
            particles.at(i).color.b += 0.05;
    }
}

//Returns a random number from 0 to 1
float randomFloat(float min, float max)
{
    return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

