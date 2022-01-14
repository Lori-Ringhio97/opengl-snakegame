/*
 * Snake.cpp
 *   Author: Lorenzo Rinieri
 *   Last Update: 14/01/2022
 *   Version: 1.0
 */

#include <iostream>
#include "ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <deque>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

/* Assigning codes to directions */
#define UP     1
#define DOWN   2
#define LEFT   3
#define RIGHT  4

/* Function for delaying the closure of the game window when the game is over */
/* Do not use sleep because it is not cross-platform */
#define DELAY(n) for(int i = 0; i < 1e5;i++)for(int j = 0; j < n*1e3;j++);   

/* Colors for the program in rgb format */
glm::vec4 mapBackgroundColor = {0.25, 0.90, 0.25, 1.0};
glm::vec4 snakeColor         = {0.00, 0.00, 0.00, 1.0};
glm::vec4 foodColor          = {1.00, 0.00, 0.00, 1.0};
glm::vec4 wallColor          = {0.00, 0.00, 0.00, 1.0};

/* Dimension of the game map */
int mapSize = 30;

/* Data structure for storing snake cordinates */
deque<pair<int,int>> snakeBody;

/* (x, y) coordinates for the position of food. */
int foodPosition[2];                 

/* Game variables */
int foodAvailable = 0;
int score = 0;
int paused = 1;
int finished = 0;
int direction = RIGHT;

/* Game variables for future extensions of the game: change game difficulty and lives */
int Difficulty = 5;
int currentLives = 1;
int gameSpeed = Difficulty*20;

/* Variables for graphic */
static unsigned int programId;
glm::mat4 Projection;  /* Projection Matrix di proiezione */
glm::mat4 Model; /* Matrix for changing from Object Reference System to World Reference System */

typedef struct { float x, y, z, r, g, b, a; } Point; /* Point definition */
unsigned int VAO_WALLS, VAO_SNAKE_HEAD, VAO_SNAKE_BODY, VAO_FOOD;
unsigned int VBO_WALLS, VBO_SNAKE_HEAD, VBO_SNAKE_BODY, VBO_FOOD;
unsigned int MatProj, MatModel;

/* Variable for wall's vertices */
int nVerticesWall = 6;
Point* wall = new Point[nVerticesWall];

/* Variable for Snake's head */
int nVerticesSnakeHead = 12;
Point* snakeHead = new Point[nVerticesSnakeHead];

/* Variable for Snake's body */
/* 6 vertices for the body and 4 vertices for the lines */
int nVerticesSnakeBody = 6;
int nVerticesSnakeBodyLines = 4;
Point* snakeBodyPoint = new Point[nVerticesSnakeBody + nVerticesSnakeBodyLines];

/* Variables for food */
int nVerticesFood = 6;
Point* food = new Point[nVerticesFood];


void showFinalScore()
{
    cout << "*************************************************************\n";
    cout << "Your Final Score is: " << score << "\n";    
}

void printGameGuide()
{
    printf("*************************GAME GUIDE***************************");
    printf("\nUse WASD as movement keys.");
    printf("\n\tPress W for Up, A for left, S for Down and D for Right");
    printf("\nR to Restart Game.");
    printf("\nPress the Spacebar to pause/resume the game.");
    printf("\nPress Esc or Q to Quit.");
    printf("\n\nPress the Spacebar to Start.");
    printf("\n*************************************************************\n");
}


void drawSnakeHead(int x, int y, glm::vec4 color, glm::vec4 eyeColor)
{   
    int counter = 0;

    /* Snake's head first 3 vertices */
    snakeHead[counter].x = x;	
    snakeHead[counter].y = y;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = color.r;
    snakeHead[counter].g = color.g;
    snakeHead[counter].b = color.b;
    snakeHead[counter].a = color.a;
    counter++;

    snakeHead[counter].x = x + 1;	
    snakeHead[counter].y = y;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = color.r;
    snakeHead[counter].g = color.g;
    snakeHead[counter].b = color.b;
    snakeHead[counter].a = color.a;
    counter++;

    snakeHead[counter].x = x + 1;	
    snakeHead[counter].y = y + 1;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = color.r;
    snakeHead[counter].g = color.g;
    snakeHead[counter].b = color.b;
    snakeHead[counter].a = color.a;
    counter++;

    /* Snake's head second 3 vertices */
    snakeHead[counter].x = x + 1;	
    snakeHead[counter].y = y + 1;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = color.r;
    snakeHead[counter].g = color.g;
    snakeHead[counter].b = color.b;
    snakeHead[counter].a = color.a;
    counter++;

    snakeHead[counter].x = x;	
    snakeHead[counter].y = y + 1;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = color.r;
    snakeHead[counter].g = color.g;
    snakeHead[counter].b = color.b;
    snakeHead[counter].a = color.a;
    counter++;

    snakeHead[counter].x = x;	
    snakeHead[counter].y = y;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = color.r;
    snakeHead[counter].g = color.g;
    snakeHead[counter].b = color.b;
    snakeHead[counter].a = color.a;
    counter++;

    /* Snake's eye first 3 vertices */
    snakeHead[counter].x = x + 0.65;	
    snakeHead[counter].y = y + 0.65;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = eyeColor.r;
    snakeHead[counter].g = eyeColor.g;
    snakeHead[counter].b = eyeColor.b;
    snakeHead[counter].a = eyeColor.a;
    counter++;

    snakeHead[counter].x = x + 0.85;	
    snakeHead[counter].y = y + 0.65;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = eyeColor.r;
    snakeHead[counter].g = eyeColor.g;
    snakeHead[counter].b = eyeColor.b;
    snakeHead[counter].a = eyeColor.a;
    counter++;

    snakeHead[counter].x = x + 0.85;	
    snakeHead[counter].y = y + 0.9;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = eyeColor.r;
    snakeHead[counter].g = eyeColor.g;
    snakeHead[counter].b = eyeColor.b;
    snakeHead[counter].a = eyeColor.a;
    counter++;

    /* Snake's eye second 3 vertices */
    snakeHead[counter].x = x + 0.85;	
    snakeHead[counter].y = y + 0.9;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = eyeColor.r;
    snakeHead[counter].g = eyeColor.g;
    snakeHead[counter].b = eyeColor.b;
    snakeHead[counter].a = eyeColor.a;
    counter++;

    snakeHead[counter].x = x + 0.65;	
    snakeHead[counter].y = y + 0.9;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = eyeColor.r;
    snakeHead[counter].g = eyeColor.g;
    snakeHead[counter].b = eyeColor.b;
    snakeHead[counter].a = eyeColor.a;
    counter++;

    snakeHead[counter].x = x + 0.65;	
    snakeHead[counter].y = y + 0.65;
    snakeHead[counter].z = 0;
	snakeHead[counter].r = eyeColor.r;
    snakeHead[counter].g = eyeColor.g;
    snakeHead[counter].b = eyeColor.b;
    snakeHead[counter].a = eyeColor.a;
}


void drawSnakeBody(int x, int y, glm::vec4 color, glm::vec4 lineColor)
{   
    int counter = 0;

    /* Snake's body first 3 vertices */
    snakeBodyPoint[counter].x = x;	
    snakeBodyPoint[counter].y = y;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = color.r;
    snakeBodyPoint[counter].g = color.g;
    snakeBodyPoint[counter].b = color.b;
    snakeBodyPoint[counter].a = color.a;
    counter++;

    snakeBodyPoint[counter].x = x + 1;	
    snakeBodyPoint[counter].y = y;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = color.r;
    snakeBodyPoint[counter].g = color.g;
    snakeBodyPoint[counter].b = color.b;
    snakeBodyPoint[counter].a = color.a;
    counter++;

    snakeBodyPoint[counter].x = x + 1;	
    snakeBodyPoint[counter].y = y + 1;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = color.r;
    snakeBodyPoint[counter].g = color.g;
    snakeBodyPoint[counter].b = color.b;
    snakeBodyPoint[counter].a = color.a;
    counter++;

    /* Snake's body second 3 vertices */
    snakeBodyPoint[counter].x = x + 1;	
    snakeBodyPoint[counter].y = y + 1;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = color.r;
    snakeBodyPoint[counter].g = color.g;
    snakeBodyPoint[counter].b = color.b;
    snakeBodyPoint[counter].a = color.a;
    counter++;

    snakeBodyPoint[counter].x = x;	
    snakeBodyPoint[counter].y = y + 1;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = color.r;
    snakeBodyPoint[counter].g = color.g;
    snakeBodyPoint[counter].b = color.b;
    snakeBodyPoint[counter].a = color.a;
    counter++;

    snakeBodyPoint[counter].x = x;	
    snakeBodyPoint[counter].y = y;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = color.r;
    snakeBodyPoint[counter].g = color.g;
    snakeBodyPoint[counter].b = color.b;
    snakeBodyPoint[counter].a = color.a;
    counter++;

    /* Vertices for the lines in the body of Snake */
    snakeBodyPoint[counter].x = x + 0.5;	
    snakeBodyPoint[counter].y = y + 1;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = lineColor.r;
    snakeBodyPoint[counter].g = lineColor.g;
    snakeBodyPoint[counter].b = lineColor.b;
    snakeBodyPoint[counter].a = lineColor.a;
    counter++;

    snakeBodyPoint[counter].x = x + 0.5;	
    snakeBodyPoint[counter].y = y;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = lineColor.r;
    snakeBodyPoint[counter].g = lineColor.g;
    snakeBodyPoint[counter].b = lineColor.b;
    snakeBodyPoint[counter].a = lineColor.a;
    counter++;

    snakeBodyPoint[counter].x = x;	
    snakeBodyPoint[counter].y = y + 0.5;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = lineColor.r;
    snakeBodyPoint[counter].g = lineColor.g;
    snakeBodyPoint[counter].b = lineColor.b;
    snakeBodyPoint[counter].a = lineColor.a;
    counter++;

    snakeBodyPoint[counter].x = x + 1;	
    snakeBodyPoint[counter].y = y + 0.5;
    snakeBodyPoint[counter].z = 0;
	snakeBodyPoint[counter].r = lineColor.r;
    snakeBodyPoint[counter].g = lineColor.g;
    snakeBodyPoint[counter].b = lineColor.b;
    snakeBodyPoint[counter].a = lineColor.a;
}


void drawBrick(int x, int y, glm::vec4 color)
{   
    int counter = 0;

    /* First 3 vertices */
    wall[counter].x = x + 0.05;	
    wall[counter].y = y + 0.05;
    wall[counter].z = 0;
	wall[counter].r = color.r;
    wall[counter].g = color.g;
    wall[counter].b = color.b;
    wall[counter].a = color.a;
    counter++;

    wall[counter].x = x + 0.95;	
    wall[counter].y = y + 0.05;
    wall[counter].z = 0;
	wall[counter].r = color.r;
    wall[counter].g = color.g;
    wall[counter].b = color.b;
    wall[counter].a = color.a;
    counter++;

    wall[counter].x = x + 0.95;	
    wall[counter].y = y + 0.95;
    wall[counter].z = 0;
	wall[counter].r = color.r;
    wall[counter].g = color.g;
    wall[counter].b = color.b;
    wall[counter].a = color.a;
    counter++;

    /* Second 3 vertices */
    wall[counter].x = x + 0.95;	
    wall[counter].y = y + 0.95;
    wall[counter].z = 0;
	wall[counter].r = color.r;
    wall[counter].g = color.g;
    wall[counter].b = color.b;
    wall[counter].a = color.a;
    counter++;

    wall[counter].x = x + 0.05;	
    wall[counter].y = y + 0.95;
    wall[counter].z = 0;
	wall[counter].r = color.r;
    wall[counter].g = color.g;
    wall[counter].b = color.b;
    wall[counter].a = color.a;
    counter++;

    wall[counter].x = x + 0.05;	
    wall[counter].y = y + 0.05;
    wall[counter].z = 0;
	wall[counter].r = color.r;
    wall[counter].g = color.g;
    wall[counter].b = color.b;
    wall[counter].a = color.a;
}


void displayFood()
{
    if(!foodAvailable)
    {
        /* create food */
        int fx = rand() % (mapSize - 2) + 1; 
        int fy = rand() % (mapSize - 2) + 1;

        /* Make sure that the food doesn't overlap with Snake's body */
        int overlap = 1;
        while(overlap)
        {
            for(auto part:snakeBody)
            {
                if(part.first == fx && part.second == fy)
                {
                    fx = rand() % mapSize + 1;
                    fy = rand() % mapSize + 1;
                    break;
                }
            }
            overlap = 0;
            foodPosition[0] = fx;
            foodPosition[1] = fy;
        }
    }
    
    foodAvailable = 1;
}


void drawFood(int x, int y, glm::vec4 color)
{   
    int counter = 0;
    
    food[counter].x = x;	
    food[counter].y = y + 0.5;
    food[counter].z = 0;
	food[counter].r = color.r;
    food[counter].g = color.g;
    food[counter].b = color.b;
    food[counter].a = color.a;
    counter++;

    food[counter].x = x + 0.5;	
    food[counter].y = y + 1;
    food[counter].z = 0;
	food[counter].r = color.r;
    food[counter].g = color.g;
    food[counter].b = color.b;
    food[counter].a = color.a;
    counter++;

    food[counter].x = x + 1;	
    food[counter].y = y + 0.5;
    food[counter].z = 0;
	food[counter].r = color.r;
    food[counter].g = color.g;
    food[counter].b = color.b;
    food[counter].a = color.a;
    counter++;

    food[counter].x = x;	
    food[counter].y = y + 0.5;
    food[counter].z = 0;
	food[counter].r = color.r;
    food[counter].g = color.g;
    food[counter].b = color.b;
    food[counter].a = color.a;
    counter++;

    food[counter].x = x + 0.5;	
    food[counter].y = y + 0;
    food[counter].z = 0;
	food[counter].r = color.r;
    food[counter].g = color.g;
    food[counter].b = color.b;
    food[counter].a = color.a;
    counter++;

    food[counter].x = x + 1;	
    food[counter].y = y + 0.5;
    food[counter].z = 0;
	food[counter].r = color.r;
    food[counter].g = color.g;
    food[counter].b = color.b;
    food[counter].a = color.a;
}


void moveSnake(int newDirection)
{
    direction = newDirection;
    int delX = 0;
    int delY = 0;
    int mapEdge = 0;
    int snake_part_axis = 0;

    switch(direction)
    {
        case UP:
            delY = 1;
            break;
        case DOWN:
            delY = -1;
            break;
        case LEFT:
            delX = -1;
            break;
        case RIGHT:
            delX = 1;
            break;
    }

    for(auto part: snakeBody)
    {
        if((part.first == (snakeBody[0].first + delX)) && (part.second == (snakeBody[0].second + delY)))
        {
            currentLives--;
            cout << "Be Careful! You got bit.\n";
            if(currentLives <= 0)
            {
                cout << "Game Over.";
                showFinalScore();
                DELAY(2);   
                exit(0);
            }
        }
    }

    if(snakeBody[0].first <= 0 || snakeBody[0].first >= mapSize -1 || snakeBody[0].second <= 0 || snakeBody[0].second >= mapSize - 1)
    {
        cout << "Oh NO! You ran into wall. Game Over.\n";
        showFinalScore();
        DELAY(2);   
        exit(0);
    }

    int grow = 0;
    if(snakeBody[0].first + delX == foodPosition[0] && snakeBody[0].second + delY == foodPosition[1])
    {
        grow = 1;
        score += grow;
        foodAvailable = 0;
    }

    snakeBody.push_front({snakeBody[0].first + delX,snakeBody[0].second + delY});
    if(!grow)
    {
        snakeBody.pop_back();
    }
    
    glutPostRedisplay();
}


void TimerFunc(int val)
{
    if(!paused)
        moveSnake(direction);
    glutTimerFunc(gameSpeed,TimerFunc,0);    
}

void init(void)
{
    Projection = glm::ortho(0.0f, float(mapSize), 0.0f, float(mapSize));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

    /* Draw Walls */
	drawBrick(0, 0, wallColor);
	glGenVertexArrays(1, &VAO_WALLS);
	glBindVertexArray(VAO_WALLS);
	glGenBuffers(1, &VBO_WALLS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_WALLS);
	glBufferData(GL_ARRAY_BUFFER, nVerticesWall * sizeof(Point), &wall[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

    /* Draw Snake's head */
	drawSnakeHead(snakeBody[0].first, snakeBody[0].second, snakeColor, mapBackgroundColor);
	glGenVertexArrays(1, &VAO_SNAKE_HEAD);
	glBindVertexArray(VAO_SNAKE_HEAD);
	glGenBuffers(1, &VBO_SNAKE_HEAD);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_SNAKE_HEAD);
	glBufferData(GL_ARRAY_BUFFER, nVerticesSnakeHead * sizeof(Point), &snakeHead[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

    /* Draw Snake's body */
	drawSnakeBody(snakeBody[1].first, snakeBody[1].second, snakeColor, mapBackgroundColor);
	glGenVertexArrays(1, &VAO_SNAKE_BODY);
	glBindVertexArray(VAO_SNAKE_BODY);
	glGenBuffers(1, &VBO_SNAKE_BODY);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_SNAKE_BODY);
	glBufferData(GL_ARRAY_BUFFER, (nVerticesSnakeBody + nVerticesSnakeBodyLines) * sizeof(Point), &snakeBodyPoint[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

    /* Draw Food */
    drawFood(0, 0, foodColor);
    glGenVertexArrays(1, &VAO_FOOD);
	glBindVertexArray(VAO_FOOD);
	glGenBuffers(1, &VBO_FOOD);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_FOOD);
	glBufferData(GL_ARRAY_BUFFER, nVerticesFood * sizeof(Point), &food[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

    /* Background Color */
    glClearColor(mapBackgroundColor.r, mapBackgroundColor.g, mapBackgroundColor.b, mapBackgroundColor.a);

    glutSwapBuffers();
}


void drawScene()
{   
    glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(programId);

	/* Draw walls */
    glBindVertexArray(VAO_WALLS);
    for(int i = 0; i < mapSize; i++)
    {
        Model = glm::mat4(1.0);
	    Model = glm::translate(Model, glm::vec3(double(i), 0.0, 0.0));
	    Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));
	    glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	    glDrawArrays(GL_TRIANGLES, 0, nVerticesWall);
	    
        Model = glm::mat4(1.0);
	    Model = glm::translate(Model, glm::vec3(0.0, double(i), 0.0));
	    Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));
	    glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	    glDrawArrays(GL_TRIANGLES, 0, nVerticesWall);

        Model = glm::mat4(1.0);
	    Model = glm::translate(Model, glm::vec3(double(i), double(mapSize - 1), 0.0));
	    Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));
	    glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	    glDrawArrays(GL_TRIANGLES, 0, nVerticesWall);

        Model = glm::mat4(1.0);
	    Model = glm::translate(Model, glm::vec3(double(mapSize - 1), double(i), 0.0));
	    Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));
	    glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	    glDrawArrays(GL_TRIANGLES, 0, nVerticesWall);
    }
    glBindVertexArray(0);

    /* Draw Snake head */
    Model = glm::mat4(1.0);
	Model = glm::translate(Model, glm::vec3(double(snakeBody[0].first), double(snakeBody[0].second), 0.0));
	Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
    glBindVertexArray(VAO_SNAKE_HEAD);
    glDrawArrays(GL_TRIANGLES, 0, nVerticesSnakeHead);
    glBindVertexArray(0);

    /* Draw Snake Body */
    glBindVertexArray(VAO_SNAKE_BODY);
    for(int i = 1; i < snakeBody.size(); i++)
    {   
        Model = glm::mat4(1.0);
	    Model = glm::translate(Model, glm::vec3(double(snakeBody[i].first), double(snakeBody[i].second), 0.0));
	    Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));
	    glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
        glDrawArrays(GL_TRIANGLES, 0, nVerticesSnakeBody);
        glLineWidth(2.0);
		glDrawArrays(GL_LINE_STRIP, nVerticesSnakeBody, nVerticesSnakeBodyLines / 2);
        glLineWidth(2.0);
		glDrawArrays(GL_LINE_STRIP, nVerticesSnakeBody + (nVerticesSnakeBodyLines / 2), nVerticesSnakeBodyLines / 2);
    }
    glBindVertexArray(0);

    /* Draw Food */
    displayFood();
    Model = glm::mat4(1.0);
    Model = glm::translate(Model, glm::vec3(double(foodPosition[0]), double(foodPosition[1]), 0.0));
	Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
    glBindVertexArray(VAO_FOOD);
    glDrawArrays(GL_TRIANGLES, 0, nVerticesFood);
    glBindVertexArray(0);

   glutSwapBuffers();
}


void initializeGame()
{
    srand(time(0));

    score = 0;
    currentLives = 1;

    // Start with Snake's body of size 3
    snakeBody.clear();
    snakeBody.push_back({5, mapSize / 2});
    snakeBody.push_back({4, mapSize / 2});
    snakeBody.push_back({3, mapSize / 2});

    moveSnake(RIGHT);
    
    paused = 1;
    foodAvailable = 0;
    printGameGuide();
}


void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'W':
        case 'w':
        {
            if(!paused)
            if(direction == LEFT || direction == RIGHT){
                moveSnake(UP);
            }
            break;
        }
        case 'S':
        case 's':
        {
            if(!paused)
            if(direction == LEFT || direction == RIGHT){
                moveSnake(DOWN);
            }
            break;
        }

        case 'A':
        case 'a':
        {
            if(!paused)
            if(direction == UP || direction == DOWN){
                moveSnake(LEFT);
            }
            break;
        }
        case 'D':
        case 'd':
        {
            if(!paused)
            if(direction == UP || direction == DOWN){\
                moveSnake(RIGHT);
            }
            break;
        }

        case ' ':
        {   
            if(paused == 0)
            	cout << "You Stopped\nPaused the game\n";
            else
            	cout << "You Resumed/Started Game\n";
            paused = !paused;
            break;
        }


        case 'R':
        case 'r':
        {
            showFinalScore();
            cout << "You restarted the Game.\n";
            initializeGame();
            break;
        }

        case 27:
        case 'q':
        {
            cout << "You pressed exit.\n";
            showFinalScore();
            exit(0);
        }
    }
}


void initShader(void)
{
	GLenum ErrorCheckValue = glGetError();

	char *vertexShader = (char *)"vertexShader_C_M.glsl";
	char *fragmentShader = (char *)"fragmentShader_C_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);
}


int main(int argc,char ** argv)
{
    glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(mapSize * 20, mapSize * 20);
    glutInitWindowPosition(250, 100);
    glutCreateWindow("Snake Game");
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(gameSpeed, TimerFunc, 0);
    
    glewExperimental = GL_TRUE;
    glewInit();
	initShader();
	init();

    initializeGame();
    glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

    return 0;
}
