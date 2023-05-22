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

float rotationX, rotationY, rotationZ;

float rotationXSpeed = 0.05F;
float rotationYSpeed = 0.05F;
float rotationZSpeed = 0.01F;

int screenWidth = SCREEN_WIDTH, screenHeight = SCREEN_HEIGHT;
float cubeWidthHeight = 20;
int distanceFromCam = 100;
float horizontalOffset = 0;
float K1 = 40;
float incrementSpeed = 1.0F;

float zBuffer[SCREEN_SIZE];
char buffer[SCREEN_SIZE];


float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
    return j * sin(rotationX) * sin(rotationY) * cos(rotationZ) - k * cos(rotationX) * sin(rotationY) * cos(rotationZ) +
           j * cos(rotationX) * sin(rotationZ) + k * sin(rotationX) * sin(rotationZ) +
           i * cos(rotationY) * cos(rotationZ);
}

float calculateY(int i, int j, int k) {
    return j * cos(rotationX) * cos(rotationZ) + k * sin(rotationX) * cos(rotationZ) -
           j * sin(rotationX) * sin(rotationY) * sin(rotationZ) + k * cos(rotationX) * sin(rotationY) * sin(rotationZ) -
           i * cos(rotationY) * sin(rotationZ);
}

float calculateZ(int i, int j, int k) {
    return k * cos(rotationX) * cos(rotationY) - j * sin(rotationX) * cos(rotationY) + i * sin(rotationY);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = (int)(screenWidth / 2 + horizontalOffset + K1 * ooz * x * 2);
    yp = (int)(screenHeight / 2 + K1 * ooz * y);

    idx = xp + yp * screenWidth;
    if (idx >= 0 && idx < screenWidth * screenHeight)
    {
        if (ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

void sleepMilliseconds(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void printCubeColored() {
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
    initUnicodeLib();

    printf(ESC_CLEAR_SCREEN);
    while (1)
    {
        memset(buffer, BACKGROUND_CHARACTER, (size_t)screenWidth * screenHeight);
        memset(zBuffer, 0, (unsigned long)screenWidth * screenHeight * sizeof(float));
        cubeWidthHeight = 20;

        for (float cubeX = -cubeWidthHeight; cubeX < cubeWidthHeight; cubeX += incrementSpeed)
        {
            for (float cubeY = -cubeWidthHeight; cubeY < cubeWidthHeight; cubeY += incrementSpeed)
            {
                calculateForSurface(cubeX, cubeY, -cubeWidthHeight, FACE_1_CHARACTER);
                calculateForSurface(cubeWidthHeight, cubeY, cubeX, FACE_2_CHARACTER);
                calculateForSurface(-cubeWidthHeight, cubeY, -cubeX, FACE_3_CHARACTER);
                calculateForSurface(-cubeX, cubeY, cubeWidthHeight, FACE_4_CHARACTER);
                calculateForSurface(cubeX, -cubeWidthHeight, -cubeY, FACE_5_CHARACTER);
                calculateForSurface(cubeX, cubeWidthHeight, cubeY, FACE_6_CHARACTER);
            }
        }
        printCubeColored();
        //        printCube();

        rotationX += rotationXSpeed;
        rotationY += rotationYSpeed;
        rotationZ += rotationZSpeed;
        sleepMilliseconds(16);
    }
    return 0;
}
