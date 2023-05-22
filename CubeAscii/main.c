#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>

#include "cUnicodeLib.h"

#define OPTION_CUBE_COUNT "-c"
#define OPTION_CUBE_GRAY_MODE "-g"
#define OPTION_HELP "-h"

#define FRAME_DELAY_MILLISECONDS 16

#define MAX_CUBE_COUNT 3

#define BACKGROUND_CHARACTER '.'
#define FACE_1_CHARACTER '@'
#define FACE_2_CHARACTER '$'
#define FACE_3_CHARACTER '~'
#define FACE_4_CHARACTER '#'
#define FACE_5_CHARACTER ';'
#define FACE_6_CHARACTER '+'

int screenWidth, screenHeight, screenSize;

float* zBuffer;
char* buffer;

float K1 = 40;

typedef struct Cube Cube;
struct Cube {
    float rotationX, rotationY, rotationZ;
    float rotationXSpeed, rotationYSpeed, rotationZSpeed;
    float cubeWidthHeight;
    float horizontalOffset, verticalOffset;
    int distanceFromCam;
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
    cube.distanceFromCam = 100;
    return cube;
}

Cube createCustomCube(float rotationX, float rotationY, float rotationZ, float rotationXSpeed, float rotationYSpeed, float rotationZSpeed, float cubeWidthHeight, float horizontalOffset, float verticalOffset, int distanceFromCam) {
    Cube cube;
    cube.rotationX = rotationX;
    cube.rotationY = rotationY;
    cube.rotationZ = rotationZ;
    cube.rotationXSpeed = rotationXSpeed;
    cube.rotationYSpeed = rotationYSpeed;
    cube.rotationZSpeed = rotationZSpeed;
    cube.cubeWidthHeight = cubeWidthHeight;
    cube.horizontalOffset = horizontalOffset;
    cube.verticalOffset = verticalOffset;
    cube.distanceFromCam = distanceFromCam;
    return cube;
}

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

void calculateForSurface(int cubeX, int cubeY, int cubeZ, Cube* cube, char ch) {
    float x = calculateX(cubeX, cubeY, cubeZ, cube);
    float y = calculateY(cubeX, cubeY, cubeZ, cube);
    float z = calculateZ(cubeX, cubeY, cubeZ, cube) + cube->distanceFromCam;

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

    for (int cubeX = -halfCubeLength; cubeX < halfCubeLength; cubeX++)
    {
        for (int cubeY = -halfCubeLength; cubeY < halfCubeLength; cubeY++)
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

void printToConsoleColored() {
    printf(ESC_CURSOR_HOME);
    for (int k = 0; k < screenSize; k++)
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

void printToConsole() {
    printf(ESC_CURSOR_HOME);
    for (int k = 0; k < screenSize; k++)
    {
        putchar(k % screenWidth ? buffer[k] : '\n');
    }
}

void printUsage(const char* programName) {
    printf("Usage: %s [OPTIONS]\n"
           "Options:\n"
           "  %s <count>    Number of cubes to render (default: 1)\n"
           "  %s            Render in gray mode\n"
           "  %s            Print this help message\n"
           "\n",
        programName, OPTION_CUBE_COUNT, OPTION_CUBE_GRAY_MODE, OPTION_HELP);
}

void printUnknownArgumentError(const char* argument) {
    fprintf(stderr, "Unknown argument: %s\n", argument);
}

int main(int argc, char** argv) {
    void (*printCubePtr)() = printToConsoleColored;
    int cubeCount = 1;

    /* Handle arguments */
    for (int i = 1; i < argc; i++)
    {
        switch (argv[i][0])
        {
        case '-':
            if (strcmp(argv[i], OPTION_CUBE_COUNT) == 0)
            {
                if (i + 1 < argc)
                {
                    cubeCount = atoi(argv[i + 1]);
                    if (cubeCount < 1)
                    {
                        fprintf(stderr, "Cube count must be greater than 0\n");
                        return 1;
                    }
                    else if (cubeCount > MAX_CUBE_COUNT)
                    {
                        fprintf(stderr, "Cube count must be less than %d\n", MAX_CUBE_COUNT);
                        return 1;
                    }
                    i++;
                }
                else
                {
                    printUsage(argv[0]);
                    return 1;
                }
            }
            else if (strcmp(argv[i], OPTION_CUBE_GRAY_MODE) == 0)
            {
                if (i + 1 < argc)
                {
                    printCubePtr = printToConsole;
                    i++;
                }
                else
                {
                    printUsage(argv[0]);
                    return 1;
                }
            }
            else if (strcmp(argv[i], OPTION_HELP) == 0)
            {
                printUsage(argv[0]);
                return 0;
            }
            else
            {
                printUnknownArgumentError(argv[i]);
                return 1;
            }
            break;
        default:
            printUsage(argv[0]);
            return 1;
        }
    }

    /* Initialize unicode library */
    initUnicodeLib();

    /* Clear screen */
    printf(ESC_CLEAR_SCREEN);

    /* Initialize cube */
    Cube cubeArray[MAX_CUBE_COUNT];
    cubeArray[0] = createCube();

    if (cubeCount == 1)
    {
        screenWidth = 60;
        screenHeight = 30;
    }
    else if (cubeCount == 2)
    {
        /*
         * float rotationX, float rotationY, float rotationZ, float rotationXSpeed, float rotationYSpeed,
         * float rotationZSpeed, float cubeWidthHeight, float horizontalOffset, float verticalOffset,
         * int distanceFromCam*/
        screenWidth = 100;
        screenHeight = 30;
        cubeArray[1] = createCustomCube(0, 0, 0, -0.03F, 0.05F, -0.02F, 20, -30, 0, 100);
        cubeArray[0].horizontalOffset = 15;
    }
    else if (cubeCount == 3)
    {
        screenWidth = 120;
        screenHeight = 30;
        cubeArray[2] = createCustomCube(0, 0, 0, -0.03F, 0.05F, -0.02F, 10, +30, 0, 100);
    }
    else
    {
        fprintf(stderr, "Cube count must be less than %d\n", MAX_CUBE_COUNT);
        return 1;
    }

    screenSize = screenWidth * screenHeight;
    buffer = malloc(screenSize * sizeof(char));
    zBuffer = malloc(screenSize * sizeof(float));

    /* Main loop */
    while (1)
    {
        /* Refresh buffers */
        memset(buffer, BACKGROUND_CHARACTER, (size_t)screenWidth * screenHeight);
        memset(zBuffer, 0, (unsigned long)screenWidth * screenHeight * sizeof(float));

        /* Update buffers */
        for (int i = 0; i < cubeCount; i++)
        {
            updateBuffers(&cubeArray[i]);
        }

        /* Display buffers to console */
        printCubePtr();

        /* Rotate cube */
        for (int i = 0; i < cubeCount; i++)
        {
            rotateCube(&cubeArray[i]);
        }

        /* Delay */
        sleepMilliseconds(FRAME_DELAY_MILLISECONDS);
    }

    free(buffer);
    free(zBuffer);

    return 0;
}
