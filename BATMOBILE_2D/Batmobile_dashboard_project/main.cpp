// Author: Milena Markovic

#define CRES 30

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "TestBed.h"

unsigned int compileShader(GLenum type, const char* source); 
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);

int main(void)
{
    Car car = getCar();
    
    if (!glfwInit()) 
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window; 
    unsigned int wWidth = 1000;
    unsigned int wHeight = 1000;
    const char wTitle[] = "Batmobile";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL); 
    if (window == NULL) 
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate(); 
        return 2; 
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) 
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    //////////////////////////////// BEGINING ///////////////////////////////////////
    unsigned int VAO[13];
    glGenVertexArrays(13, VAO);
    unsigned int VBO[13];
    glGenBuffers(13, VBO);

    ////////////////////////////// STATIC ELEMENTS //////////////////////////////////////////
    unsigned int staticShader = createShader("shaders/basic.vert", "shaders/basic.frag");
    unsigned int staticStride = (2 + 3) * sizeof(float);

    float staticFuelVertices[] = {
         -0.91, -0.01,   0.0, 0.0, 0.0,
         -0.79, -0.01,   0.0, 0.0, 0.0,
         -0.91,  1.0,    0.0, 0.0, 0.0,
         -0.79,  1.0,    0.0, 0.0, 0.0,

          0.91, -0.01,   0.0, 0.0, 0.0,
          0.79, -0.01,   0.0, 0.0, 0.0,
          0.91,  1.0,    0.0, 0.0, 0.0,
          0.79,  1.0,    0.0, 0.0, 0.0,

          0.71, 0.09,  0.0, 0.0, 0.0,
         -0.71, 0.09,  0.0, 0.0, 0.0,
          0.71, 0.92,  0.0, 0.0, 0.0,
         -0.71, 0.92,  0.0, 0.0, 0.0,

          0.7, 0.1,    1.0, 0.9, 0.2,
         -0.7, 0.1,    1.0, 0.9, 0.2,
          0.7, 0.91,    1.0, 0.9, 0.2,
         -0.7, 0.91,    1.0, 0.9, 0.2,

         -0.39, -0.2,  0.0, 0.0, 0.0,
         -0.19, -0.2,  0.0, 0.0, 0.0,
         -0.39, 0.0,  0.0, 0.0, 0.0,
         -0.19, 0.0,  0.0, 0.0, 0.0,

          0.19,  0.0,  0.0, 0.0, 0.0,
          0.39,  0.0,  0.0, 0.0, 0.0,
          0.19, -0.2,  0.0, 0.0, 0.0,
          0.39, -0.2,  0.0, 0.0, 0.0,

         -0.8, 0.1,    0.0, 0.0, 0.0,
         -0.78, 0.1,   0.0, 0.0, 0.0,
         -0.8, 0.2,    0.0, 0.0, 0.0,
         -0.78, 0.2,   0.0, 0.0, 0.0,
         -0.8, 0.3,    0.0, 0.0, 0.0,
         -0.78, 0.3,   0.0, 0.0, 0.0,
         -0.8, 0.4,    0.0, 0.0, 0.0,
         -0.78, 0.4,   0.0, 0.0, 0.0,
         -0.8, 0.5,    0.0, 0.0, 0.0,
         -0.78, 0.5,   0.0, 0.0, 0.0,
         -0.8, 0.6,    0.0, 0.0, 0.0,
         -0.78, 0.6,   0.0, 0.0, 0.0,
         -0.8, 0.7,    0.0, 0.0, 0.0,
         -0.78, 0.7,   0.0, 0.0, 0.0,
         -0.8, 0.8,    0.0, 0.0, 0.0,
         -0.78, 0.8,   0.0, 0.0, 0.0,
         -0.8, 0.9,    0.0, 0.0, 0.0,
         -0.78, 0.9,   0.0, 0.0, 0.0,

    };

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(staticFuelVertices), staticFuelVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, staticStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, staticStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ////////////////////////////////////////// TACHOMETER /////////////////////////////////////
    unsigned int textureShader = createShader("shaders/texture.vert", "shaders/texture.frag");
    unsigned int textureStride = (2 + 2) * sizeof(float);

    float tachometerVertices[] = {
        -0.9, -0.8,  0.0, 0.0,
        -0.05, -0.8,  1.0, 0.0,
        -0.9, -0.3,  0.0, 1.0,
        -0.05, -0.3,  1.0, 1.0,
    };

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tachometerVertices), tachometerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned tachometerTexture = loadImageToTexture("res/tachometer.jpg");
    glBindTexture(GL_TEXTURE_2D, tachometerTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTexLoc = glGetUniformLocation(textureShader, "uTex");
    glUniform1i(uTexLoc, 0);

    ////////////////////////////////////////// TACHOMETER ARROW //////////////////////////////////////////
    unsigned int tachometerArrowShader = createShader("shaders/tachometerArrow.vert", "shaders/tachometerArrow.frag");
    unsigned int uArrowTachometerLoc = glGetUniformLocation(tachometerArrowShader, "uTachometer");

    /////////////////////////////////////// ODOMETER ////////////////////////////////////////////
    unsigned int odometerShader = createShader("shaders/odometer.vert", "shaders/basic.frag");
    unsigned int odometerStride = (2 + 3) * sizeof(float);

    float odometerVertices[] = {
         -0.9, 0.0,   0.0, 1.0, 0.0,
         -0.8, 0.0,   0.0, 1.0, 0.0,
         -0.9, 1.0,   0.0, 1.0, 0.0,
         -0.8, 1.0,   0.0, 1.0, 0.0
    };

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(odometerVertices), odometerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, odometerStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, odometerStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    unsigned int uOdometerLoc = glGetUniformLocation(odometerShader, "uOdometer");

    //////////////////////////////////// CHECK ENGINE ///////////////////////////////////////////////
    unsigned int checkEngineShader = createShader("shaders/basic.vert", "shaders/checkEngine.frag");
    unsigned int checkEngineStride = 2 * sizeof(float);

    float checkEngineVertices[] = {
        -0.38,  -0.01,
        -0.2,   -0.01, 
        -0.38,  -0.19, 
        -0.2,   -0.19, 
    };

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(checkEngineVertices), checkEngineVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, checkEngineStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, checkEngineStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    unsigned int uCheckEngineLoc = glGetUniformLocation(checkEngineShader, "uCheckEngine");
    unsigned int uCheckEngineTimeLoc = glGetUniformLocation(checkEngineShader, "uTime");

    //////////////////////////////////// BATTERY ///////////////////////////////////////////////
    unsigned int batteryShader = createShader("shaders/basic.vert", "shaders/battery.frag");
    unsigned int batteryStride = 2 * sizeof(float);

    float batteryVertices[] = {
         0.2,  -0.01,
         0.38, -0.01,
         0.2,  -0.19, 
         0.38, -0.19
    };

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(batteryVertices), batteryVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, batteryStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, batteryStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    unsigned int uBatteryLoc = glGetUniformLocation(batteryShader, "uBattery");
    unsigned int uBatteryTimeLoc = glGetUniformLocation(batteryShader, "uTime");

    //////////////////////////////////// LETTERS ///////////////////////////////////////////////
    unsigned int lettersShader = createShader("shaders/basic.vert", "shaders/basic.frag");
    unsigned int lettersStride = (2 + 3) * sizeof(float);
    
    float lettersVertices[] = {
         -0.255,  -0.05, 0.0, 0.0, 0.0,
         -0.325, -0.05, 0.0, 0.0, 0.0,           // C
         -0.325, -0.15, 0.0, 0.0, 0.0,
         -0.255,  -0.15, 0.0, 0.0, 0.0,
         
         0.26, -0.05, 0.0, 0.0, 0.0,
         0.26, -0.15, 0.0, 0.0, 0.0,
         0.33, -0.15, 0.0, 0.0, 0.0,
         0.33, -0.10, 0.0, 0.0, 0.0,             // B
         0.26, -0.10, 0.0, 0.0, 0.0,              
         0.32, -0.08, 0.0, 0.0, 0.0,
         0.32, -0.05, 0.0, 0.0, 0.0,
         0.26, -0.05, 0.0, 0.0, 0.0,

         0.89, -0.05, 0.0, 0.0, 0.0,
         0.81, -0.05, 0.0, 0.0, 0.0,              
         0.81, -0.15, 0.0, 0.0, 0.0,
         0.89, -0.15, 0.0, 0.0, 0.0,             // G
         0.89, -0.1, 0.0, 0.0, 0.0,
         0.89, -0.1, 0.0, 0.0, 0.0,
         0.85, -0.1, 0.0, 0.0, 0.0,

         -0.91, -0.05, 0.0, 0.0, 0.0,
         -0.91, -0.15, 0.0, 0.0, 0.0,             // k
         -0.91, -0.1, 0.0, 0.0, 0.0,    
         -0.87, -0.08, 0.0, 0.0, 0.0,
         -0.91, -0.1, 0.0, 0.0, 0.0,
         -0.87, -0.15, 0.0, 0.0, 0.0,

         -0.85, -0.15, 0.0, 0.0, 0.0,
         -0.85, -0.08, 0.0, 0.0, 0.0,
         -0.79, -0.08, 0.0, 0.0, 0.0,             // m
         -0.79, -0.15, 0.0, 0.0, 0.0,             
         -0.82, -0.08, 0.0, 0.0, 0.0,
         -0.82, -0.15, 0.0, 0.0, 0.0,


    };

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lettersVertices), lettersVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, lettersStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, lettersStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /////////////////////////////////////////////GEAR CIRCLE///////////////////////////////////////////////////
    unsigned int gearCircleShader = createShader("shaders/basic.vert", "shaders/gearCircle.frag");
    unsigned int gearCircleStride = 2 * sizeof(float);

    float circle[CRES * 2 + 4]; 
    float r = 0.08;

    circle[0] = 0; 
    circle[1] = 0 - 0.1; 
    int i;
    for (i = 0; i <= CRES; i++)
    {

        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
        circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / CRES)) - 0.1; //Yi
    }

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, gearCircleStride, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    unsigned int uGearCircleTimeLoc = glGetUniformLocation(gearCircleShader, "uTime");
    unsigned int uGearLoc = glGetUniformLocation(gearCircleShader, "uGear");

    //////////////////////////////////// BAT ///////////////////////////////////////////////
    unsigned int batShader = createShader("shaders/bat.vert", "shaders/bat.frag");
    unsigned int batStride = 2 * sizeof(float);

    float batVertices[] = {
        -0.05, 0.6, 
        -0.03, 0.65,   // left ear
        -0.01, 0.6,   

         0.01, 0.6, 
         0.03, 0.65,   // right ear
         0.05, 0.6, 

         0.05, 0.53,  
        -0.05, 0.53,   // head - left triangle
        -0.05, 0.6,  

        -0.05, 0.6,   
         0.05, 0.6,    // head - right triangle
         0.05, 0.53, 

         -0.05, 0.53,  
         -0.25, 0.53,  // left wing - top
         -0.1, 0.68,  
            
         -0.05, 0.53, 
         -0.25,  0.53, // left wing - bottom
         -0.1, 0.3,  

         -0.05, 0.53, 
         -0.085, 0.4,   // left wing - small part
         -0.07, 0.35,  

          0.05, 0.53,  
          0.25,  0.53, // right wing - top
          0.1, 0.68,   

          0.05, 0.53,  
          0.25,  0.53, // right wing - bottom
          0.1, 0.3,   

          0.05, 0.53, 
          0.085, 0.4,   // right wing - small part
          0.07, 0.35,  

          -0.05, 0.53,  
           0.05, 0.53,  // body - right part
           0.068, 0.4, 

          -0.05, 0.53,  
          -0.068, 0.4,  // body - left part
           0.068, 0.4,  

           0.0,   0.3,  
          -0.068, 0.4,  // tail
           0.068, 0.4,  
    };

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(batVertices), batVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, batStride, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    unsigned int uPosLoc = glGetUniformLocation(batShader, "uPos");
    float x = 0, y = 0;

    ////////////////////////////////////////// SPEEDOMETER /////////////////////////////////////
    float speedometerVertices[] = {
         0.05, -0.8,  0.0, 0.0, 
         0.9, -0.8,  1.0, 0.0,
         0.05, -0.3,  0.0, 1.0,  
         0.9, -0.3,  1.0, 1.0,  
    };

    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tachometerVertices), speedometerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned speedometerTexture = loadImageToTexture("res/speedometer.jpg");
    glBindTexture(GL_TEXTURE_2D, speedometerTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    ////////////////////////////////////////// SPEEDOMETER ARROW //////////////////////////////////////////
    unsigned int speedometerArrowShader = createShader("shaders/speedometerArrow.vert", "shaders/speedometerArrow.frag");
    unsigned int uArrowSpeedometerLoc = glGetUniformLocation(speedometerArrowShader, "uSpeed");

    ///////////////////////////////////// GAS ////////////////////////////////////////////////////
    unsigned int gasShader = createShader("shaders/gas.vert", "shaders/basic.frag");
    unsigned int gasStride = (2 + 3) * sizeof(float);

    float gasVertices[] = {
         0.9, 0.0,   1.0, 0.0, 0.0,
         0.8, 0.0,   1.0, 0.0, 0.0,
         0.9, 0.0,   1.0, 0.0, 0.0,
         0.8, 0.0,   1.0, 0.0, 0.0
    };

    glBindVertexArray(VAO[11]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gasVertices), gasVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, gasStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, gasStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    unsigned int uGasLoc = glGetUniformLocation(gasShader, "uGas");

    //////////////////////////////////////////// NAME ////////////////////////////////////////////////////

    float nameVertices[] = {
        -0.6, -0.98,  0.0, 0.0,
         0.6, -0.98,  1.0, 0.0,
        -0.6, -0.83,  0.0, 1.0,
         0.6, -0.83,  1.0, 1.0,
    };

    glBindVertexArray(VAO[12]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nameVertices), nameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned nameTexture = loadImageToTexture("res/name.jpg");
    glBindTexture(GL_TEXTURE_2D, nameTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    ////////////////////////////////////////////////////////////////////////////////////////////////
  
    startSimulation(&car);
    float gas = 0.0;
    float maxGas = 0.99;
    float minGas = 0.0;

    while (!glfwWindowShouldClose(window)) 
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glClearColor(1.0, 1.0, 1.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT);

        //////////////////// STATIC //////////////
        glUseProgram(staticShader);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glDrawArrays(GL_LINE_STRIP, 24, 2);
        glDrawArrays(GL_LINE_STRIP, 26, 2);
        glDrawArrays(GL_LINE_STRIP, 28, 2);
        glDrawArrays(GL_LINE_STRIP, 30, 2);
        glDrawArrays(GL_LINE_STRIP, 32, 2);
        glDrawArrays(GL_LINE_STRIP, 34, 2);
        glDrawArrays(GL_LINE_STRIP, 36, 2);
        glDrawArrays(GL_LINE_STRIP, 38, 2);
        glDrawArrays(GL_LINE_STRIP, 40, 2);

        /////////////// TACHOMETEROMETER ///////////////
        glUseProgram(textureShader);
        glBindVertexArray(VAO[1]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tachometerTexture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        //////////////// TACHOMETEROMETER ARROW ////////////////////

        glUseProgram(tachometerArrowShader);
        glUniform1f(uArrowTachometerLoc, car.getTachometer());
        glBindVertexArray(VAO[2]);
        glLineWidth(5.0);
        glDrawArrays(GL_LINE_STRIP, 0, 2);

        //////////////////// ODOMETER //////////////

        float odometer = car.getOdometer() - (10 * floor(car.getOdometer() / 10));
        glUseProgram(odometerShader);
        glUniform1f(uOdometerLoc, odometer);
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


        //////////////// CHECK ENGINE /////////
        glUseProgram(checkEngineShader);
        glUniform1f(uCheckEngineLoc, car.getCheckEngineLight());
        glUniform1f(uCheckEngineTimeLoc, glfwGetTime());
        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        if (car.getCheckEngineLight() == true) {
            car.setCheckEngine(true);
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            if (car.getCheckEngineLight() == false) {
                car.setCheckEngine(true);
            }
            else {
                car.setCheckEngine(false);
            }
        }

        //////////////// BATTERY /////////
        glUseProgram(batteryShader);
        glUniform1f(uBatteryLoc, car.getBatteryProblemLight());
        glUniform1f(uBatteryTimeLoc, glfwGetTime());
        glBindVertexArray(VAO[5]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        if (car.getBatteryProblemLight() == true) {
            car.setBatteryLight(true);
        }

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            if (car.getBatteryProblemLight() == false) {
                car.setBatteryLight(true);
            }
            else {
                car.setBatteryLight(false);
            }
        }
        /////////////////// LETTERS //////////////
        glUseProgram(lettersShader);
        glBindVertexArray(VAO[6]);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        glDrawArrays(GL_LINE_STRIP, 4, 8);
        glDrawArrays(GL_LINE_STRIP, 12, 7);
        glDrawArrays(GL_LINE_STRIP, 19, 2);
        glDrawArrays(GL_LINE_STRIP, 21, 2);
        glDrawArrays(GL_LINE_STRIP, 23, 2);
        glDrawArrays(GL_LINE_STRIP, 25, 4);
        glDrawArrays(GL_LINE_STRIP, 29, 2);
        ////////////////// GEAR CIRCLE /////////////////////////
        glUseProgram(gearCircleShader);
        glUniform1f(uGearCircleTimeLoc, glfwGetTime());
        glUniform1i(uGearLoc, car.getGear());
        glBindVertexArray(VAO[7]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);

        //////////////// BAT /////////
        glUseProgram(batShader);
        glUniform2f(uPosLoc, x, y);    
        glBindVertexArray(VAO[8]);    

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            if (y <= 0.23) {
                y += 0.001;
            }
        }
       
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            if (x >= -0.45) {
                x -= 0.001;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            if (y >= -0.2) {
                y -= 0.001;
            }
        }     

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if (x <= 0.45) {
                x += 0.001;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            glDisable(GL_BLEND);
        }

        
        if (car.getGear() == 1) {
            glUseProgram(batShader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glEnable(GL_PROGRAM_POINT_SIZE);
            glPointSize(4);

            glBindVertexArray(VAO[8]);
            glDrawArrays(GL_TRIANGLES, 0, 39);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_PROGRAM_POINT_SIZE);
        }

        if (car.getGear() == 2) {
            glUseProgram(batShader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(3);
            glBindVertexArray(VAO[8]);
            glDrawArrays(GL_TRIANGLES, 0, 39);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (car.getGear() > 2) {
            glUseProgram(batShader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glBindVertexArray(VAO[8]);
            glDrawArrays(GL_TRIANGLES, 0, 39);
        }

        //////////////////// SPEEDOMETER //////////////
        glUseProgram(textureShader);
        glBindVertexArray(VAO[9]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, speedometerTexture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        //////////////// SPEEDOMETER ARROW ////////////////////
        glUseProgram(speedometerArrowShader);
        glUniform1f(uArrowSpeedometerLoc, car.getSpeed());
        glBindVertexArray(VAO[10]);
        glLineWidth(5.0);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
          
        //////////////////// GAS //////////////
        glUseProgram(gasShader);
        glUniform1f(uGasLoc, gas);
        glBindVertexArray(VAO[11]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            gas += 0.0003f;
            gas = (gas > maxGas) ? maxGas : gas;
        }

        if (glfwGetKey(window, GLFW_KEY_G) != GLFW_PRESS)
        {
            gas -= 0.0003f;
            gas = (gas <= minGas) ? minGas : gas;
        }

        /////////////// NAME ///////////////
        glUseProgram(textureShader);
        glBindVertexArray(VAO[12]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, nameTexture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        ////////////////////////////////////////////////////////


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    endSimulation(&car);
    glDeleteBuffers(13, VBO);
    glDeleteVertexArrays(13, VAO);
    glDeleteProgram(staticShader);
    glDeleteProgram(textureShader);
    glDeleteProgram(tachometerArrowShader);
    glDeleteProgram(odometerShader);
    glDeleteProgram(checkEngineShader);
    glDeleteProgram(batteryShader);
    glDeleteProgram(lettersShader);
    glDeleteProgram(gearCircleShader);
    glDeleteProgram(batShader);
    glDeleteProgram(speedometerArrowShader);
    glDeleteProgram(gasShader);
    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{

    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); 

    int shader = glCreateShader(type); 
    
    int success; 
    char infoLog[512]; 
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); 
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{

    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader; 

    program = glCreateProgram(); 

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); 
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); 
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); 
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}