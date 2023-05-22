#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "cUnicodeLib.h"

#define SCREEN_WIDTH 60
#define SCREEN_HEIGHT 30
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define BACKGROUND_CHARACTER '.'
#define FACE_1_CHARACTER '@'
#define FACE_2_CHARACTER '$'
#define FACE_3_CHARACTER '~'
#define FACE_4_CHARACTER '#'
#define FACE_5_CHARACTER ';'
#define FACE_6_CHARACTER '+'

int screenWidth = SCREEN_WIDTH, screenHeight = SCREEN_HEIGHT;

float zBuffer[SCREEN_SIZE];
char buffer[SCREEN_SIZE];

int distanceFromCam = 100;
float K1 = 40;
float charIncrementSpeed = 1.0F;


typedef struct Cube Cube;
struct Cube {
    float rotationX, rotationY, rotationZ;
    float rotationXSpeed, rotationYSpeed, rotationZSpeed;
    float cubeWidthHeight;
    float horizontalOffset, verticalOffset;
};

Cube createCube() {
    Cube cube;
    cube.rotationX = 0;
    cube.rotationY = 0;
    cube.rotationZ = 0;
    cube.rotationXSpeed = 0.05F;
    cube.rotationYSpeed = 0.05F;
    cube.rotationZSpeed = 0.01F;
    cube.cubeWidthHeight = 40;
    cube.horizontalOffset = 0;
    cube.verticalOffset = 0;
    return cube;
}

// Cube createCube(float rotationX, float rotationY, float rotationZ, float rotationXSpeed, float rotationYSpeed, float rotationZSpeed, float cubeWidthHeight, float horizontalOffset, float verticalOffset) {
//     Cube cube;
//     cube.rotationX = rotationX;
//     cube.rotationY = rotationY;
//     cube.rotationZ = rotationZ;
//     cube.rotationXSpeed = rotationXSpeed;
//     cube.rotationYSpeed = rotationYSpeed;
//     cube.rotationZSpeed = rotationZSpeed;
//     cube.cubeWidthHeight = cubeWidthHeight;
//     cube.horizontalOffset = horizontalOffset;
//     cube.verticalOffset = verticalOffset;
//     return cube;
// }

float calculateX(int i, int j, int k, Cube* cube) {
    return j * sin(cube->rotationX) * sin(cube->rotationY) * cos(cube->rotationZ) - k * cos(cube->rotationX) * sin(cube->rotationY) * cos(cube->rotationZ) +
           j * cos(cube->rotationX) * sin(cube->rotationZ) + k * sin(cube->rotationX) * sin(cube->rotationZ) +
           i * cos(cube->rotationY) * cos(cube->rotationZ);
}

float calculateY(int i, int j, int k, Cube* cube) {
    return j * cos(cube->rotationX) * cos(cube->rotationZ) + k * sin(cube->rotationX) * cos(cube->rotationZ) -
           j * sin(cube->rotationX) * sin(cube->rotationY) * sin(cube->rotationZ) + k * cos(cube->rotationX) * sin(cube->rotationY) * sin(cube->rotationZ) -
           i * cos(cube->rotationY) * sin(cube->rotationZ);
}

float calculateZ(int i, int j, int k, Cube* cube) {
    return k * cos(cube->rotationX) * cos(cube->rotationY) - j * sin(cube->rotationX) * cos(cube->rotationY) + i * sin(cube->rotationY);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, Cube* cube, char ch) {
    float x = calculateX(cubeX, cubeY, cubeZ, cube);
    float y = calculateY(cubeX, cubeY, cubeZ, cube);
    float z = calculateZ(cubeX, cubeY, cubeZ, cube) + distanceFromCam;

    float ooz = 1 / z;

    int xp = (int)(screenWidth / 2 + cube->horizontalOffset + K1 * ooz * x * 2);
    int yp = (int)(screenHeight / 2 + cube->verticalOffset + K1 * ooz * y);

    int idx = xp + yp * screenWidth;
    if (idx >= 0 && idx < screenWidth * screenHeight)
    {
        if (ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

void updateBuffers(Cube* cube) {
    const int halfCubeLength = cube->cubeWidthHeight / 2;

    for (float cubeX = -halfCubeLength; cubeX < halfCubeLength; cubeX += charIncrementSpeed)
    {
        for (float cubeY = -halfCubeLength; cubeY < halfCubeLength; cubeY += charIncrementSpeed)
        {
            calculateForSurface(cubeX, cubeY, -halfCubeLength, cube, FACE_1_CHARACTER);
            calculateForSurface(halfCubeLength, cubeY, cubeX, cube, FACE_2_CHARACTER);
            calculateForSurface(-halfCubeLength, cubeY, -cubeX, cube, FACE_3_CHARACTER);
            calculateForSurface(-cubeX, cubeY, halfCubeLength, cube, FACE_4_CHARACTER);
            calculateForSurface(cubeX, -halfCubeLength, -cubeY, cube, FACE_5_CHARACTER);
            calculateForSurface(cubeX, halfCubeLength, cubeY, cube, FACE_6_CHARACTER);
        }
    }
}

void rotateCube(Cube* cube) {
    cube->rotationX += cube->rotationXSpeed;
    cube->rotationY += cube->rotationYSpeed;
    cube->rotationZ += cube->rotationZSpeed;
}

void sleepMilliseconds(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void printToConsole() {
    printf(ESC_CURSOR_HOME);
    for (int k = 0; k < SCREEN_SIZE; k++)
    {
        switch (buffer[k])
        {
        case FACE_1_CHARACTER:
            printf(ESC_FG_RED);
            break;
        case FACE_2_CHARACTER:
            printf(ESC_FG_GREEN);
            break;
        case FACE_3_CHARACTER:
            printf(ESC_FG_BLUE);
            break;
        case FACE_4_CHARACTER:
            printf(ESC_FG_YELLOW);
            break;
        case FACE_5_CHARACTER:
            printf(ESC_FG_MAGENTA);
            break;
        case FACE_6_CHARACTER:
            printf(ESC_FG_CYAN);
            break;
        case BACKGROUND_CHARACTER:
            printf(ESC_RESET_FG);
            break;
        }

        putchar(k % screenWidth ? buffer[k] : '\n');
    }
}

void printCube() {
    printf(ESC_CURSOR_HOME);
    for (int k = 0; k < SCREEN_SIZE; k++)
    {
        putchar(k % screenWidth ? buffer[k] : '\n');
    }
}

int main() {
    /* Initialize unicode library */
    initUnicodeLib();

    /* Clear screen */
    printf(ESC_CLEAR_SCREEN);

    /* Initialize cube */
    Cube cube = createCube();

    while (1)
    {
        /* Refresh buffers */
        memset(buffer, BACKGROUND_CHARACTER, (size_t)screenWidth * screenHeight);
        memset(zBuffer, 0, (unsigned long)screenWidth * screenHeight * sizeof(float));

        /* Update buffers */
        updateBuffers(&cube);

        /* Display buffers to console */
        printToConsole(); // printCube();

        /* Rotate cube */
        rotateCube(&cube);

        /* Delay */
        sleepMilliseconds(16);
    }

    return 0;
}
