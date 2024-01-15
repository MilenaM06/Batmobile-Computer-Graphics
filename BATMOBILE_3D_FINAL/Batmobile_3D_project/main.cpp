//Author: Milena Markovic
#define CRES 30
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"

const unsigned int wWidth = 1000;
const unsigned int wHeight = 1000;

static unsigned loadImageToTexture(const char* filePath);

void HandleUpDown(GLFWwindow* window, bool& upActive, bool& downActive, glm::vec3& streetCameraPosition, float& forwardCameraSpeed, glm::vec3& streetCameraFront, float& backwardCameraSpeed, float&  kilometersInTotal);
void HandleLeftRight(GLFWwindow* window, bool upActive, float& sideCameraSpeed, float forwardCameraSpeed, glm::vec3& streetCameraPosition, glm::vec3& streetCameraFront, glm::vec3& streetCameraUp, float backwardCameraSpeed);
void HandleRotation(GLFWwindow* window, glm::vec3& streetCameraFront);
void HandleNightVision(GLFWwindow* window, glm::vec3& lightA, glm::vec3& lightD, glm::vec3& lightS);
void HandleBatMoving(GLFWwindow* window, float& batY, float& batX);
void HandleDashboardLights(GLFWwindow* window, bool& checkEngineActive, glm::vec3& pointLightCheckEngine, bool& batteryActive, glm::vec3& pointLightBattery);
void HandleGear(bool upActive, bool downActive, float& currentSpeed, float forwardCameraSpeed, float backwardCameraSpeed, int& gear);
void HandleSpotlight(GLFWwindow* window, glm::vec3& reflectorLight);

int main()
{
    if(!glfwInit())
    {
        std::cout << "GLFW fail!\n" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window fail!\n" << std::endl;
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() !=GLEW_OK)
    {
        std::cout << "GLEW fail! :(\n" << std::endl;
        return -3;
    }

    /////////////////////////////// DASHBOARD ////////////////////////////////////

    float dashboardVertices[] =
    {
        //X     Y      Z     
        -1.0, -1.0, 0.0,  0.0f, 0.0f, 1.0f, 0.0, 0.0,  // 1 - L D
         1.0, -1.0, 0.0,  0.0f, 0.0f, 1.0f, 1.0, 0.0,  // 2 - R D
         1.0,  1.0, 0.0,  0.0f, 0.0f, 1.0f, 1.0, 1.0, // 3 - R U
         1.0,  1.0, 0.0,  0.0f, 0.0f, 1.0f, 1.0, 1.0, // 3 - R U 
        -1.0,  1.0, 0.0,  0.0f, 0.0f, 1.0f, 0.0, 1.0,  // 4 - L U
        -1.0, -1.0, 0.0,  0.0f, 0.0f, 1.0f, 0.0, 0.0,   // 1 - L D
    };

    unsigned int VAODashboard;
    glGenVertexArrays(1, &VAODashboard);
    glBindVertexArray(VAODashboard);
    unsigned int VBODashboard;
    glGenBuffers(1, &VBODashboard);
    glBindBuffer(GL_ARRAY_BUFFER, VBODashboard);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dashboardVertices), dashboardVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //////////////////////////////////////////// BAT ///////////////////////////////////////////////
    float batVertices[] =
    {
        //X     Y      Z     
        
        -0.3, -0.7, 0.1, 0.0, 0.0, 0.0,// 1 - L D
         0.3, -0.7, 0.1, 0.0, 0.0, 0.0, // 2 - R D
         0.3,  0.8, 0.1, 0.0, 0.0, 0.0, // 3 - R U
         0.3,  0.8, 0.1, 0.0, 0.0, 0.0, // 3 - R U      // head
        -0.3,  0.8, 0.1, 0.0, 0.0, 0.0, // 4 - L U
        -0.3, -0.7, 0.1, 0.0, 0.0, 0.0, // 1 - L D

        -0.9,  0.7, 0.1, 0.0, 0.0, 0.0,
        -0.9, -0.7, 0.1, 0.0, 0.0, 0.0,  // left wing
         0.3,  0.0, 0.1, 0.0, 0.0, 0.0, 

         0.9, -0.7, 0.1, 0.0, 0.0, 0.0,
         0.9,  0.7, 0.1, 0.0, 0.0, 0.0,  // right wing
        -0.3,  0.0, 0.1, 0.0, 0.0, 0.0,      

    };

    unsigned int VAOBat;
    glGenVertexArrays(1, &VAOBat);
    glBindVertexArray(VAOBat);
    unsigned int VBOBat;
    glGenBuffers(1, &VBOBat);
    glBindBuffer(GL_ARRAY_BUFFER, VBOBat);
    glBufferData(GL_ARRAY_BUFFER, sizeof(batVertices), batVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //////////////////////////// DASHBOARD LIGHTS ///////////////////////////////
    float dashboardLightVertices[] =
    {
        //X     Y      Z     
        -0.5, -0.5, 0.1, 0.0, 0.0, 0.0,// 1 - L D
         0.5, -0.5, 0.1, 0.0, 0.0, 0.0, // 2 - R D
         0.5,  0.5, 0.1, 0.0, 0.0, 0.0, // 3 - R U 
         0.5,  0.5, 0.1, 0.0, 0.0, 0.0, // 3 - D U 
        -0.5,  0.5, 0.1, 0.0, 0.0, 0.0, // 4 - L U
        -0.5, -0.5, 0.1, 0.0, 0.0, 0.0, // 1 - L D
    };

    unsigned int VAODashboardLights;
    glGenVertexArrays(1, &VAODashboardLights);
    glBindVertexArray(VAODashboardLights);
    unsigned int VBODashboardLights;
    glGenBuffers(1, &VBODashboardLights);
    glBindBuffer(GL_ARRAY_BUFFER, VBODashboardLights);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dashboardLightVertices), dashboardLightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /////////////////////////////// ROAD ////////////////////////////////////////

    float roadVertices[] = {        
        // X   Y     Z    U     V
        -0.9, -0.5, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, // 1 - L F
        -0.9, -0.5,  1.0, 0.0, 1.0, 0.0,0.0, 5.0, // 4 - L B
         0.9, -0.5,  1.0, 0.0, 1.0, 0.0,1.0, 5.0, // 3 - R F
         0.9, -0.5,  1.0, 0.0, 1.0, 0.0,1.0, 5.0, // 3 - R F
         0.9, -0.5, -1.0, 0.0, 1.0, 0.0,1.0, 0.0, // 2 - R B 
        -0.9, -0.5, -1.0, 0.0, 1.0, 0.0,0.0, 0.0  // 1 - L B     
    };

    unsigned int VAORoad;
    glGenVertexArrays(1, &VAORoad);
    glBindVertexArray(VAORoad);
    unsigned int VBORoad;
    glGenBuffers(1, &VBORoad);
    glBindBuffer(GL_ARRAY_BUFFER, VBORoad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVertices), roadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    /////////////////////////////// BUILDINGS ///////////////////////////////
    float buildingVertices[] =
    {
         // X     Y     Z     NX    NY    NZ    U     V    
         // FRONT SIDE
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // L U
        // LEFT SIDE
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // L D
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
        // RIGHT SIDE
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // L D
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // R U
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
        // BOTTOM SIDE
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // R D
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // R U
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // L U
        // TOP SIDE
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // L D
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // L U
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // L U
        // BACK SIDE
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // L D
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // L U
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // R U
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // L U


    };

    unsigned int VAOBuilding;
    glGenVertexArrays(1, &VAOBuilding);
    glBindVertexArray(VAOBuilding);
    unsigned int VBOBuilding;
    glGenBuffers(1, &VBOBuilding);
    glBindBuffer(GL_ARRAY_BUFFER, VBOBuilding);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buildingVertices), buildingVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /////////////////////////////// MOON ////////////////////////////////////

    float moonVertices[] =
    {
        //X     Y    Z         
        -1.0, -1.0, 0.0,  0.0, 0.0,  // 1 - L D
         1.0, -1.0, 0.0,  1.0, 0.0,  // 2 - R D
         1.0,  1.0, 0.0,  1.0, 1.0,  // 3 - R U
         1.0,  1.0, 0.0,  1.0, 1.0,  // 3 - R U  
        -1.0,  1.0, 0.0,  0.0, 1.0,  // 4 - L U
        -1.0, -1.0, 0.0,  0.0, 0.0,   // 1 - L D
    };

    unsigned int VAOMoon;
    glGenVertexArrays(1, &VAOMoon);
    glBindVertexArray(VAOMoon);
    unsigned int VBOMoon;
    glGenBuffers(1, &VBOMoon);
    glBindBuffer(GL_ARRAY_BUFFER, VBOMoon);
    glBufferData(GL_ARRAY_BUFFER, sizeof(moonVertices), moonVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /////////////////////////////// NAME ////////////////////////////////////

    float nameVertices[] = {
     -0.75, -0.85,  0.0, 0.0,
     -0.47, -0.85,  1.0, 0.0,
     -0.75, -0.55,  0.0, 1.0,
     -0.47, -0.55,  1.0, 1.0
    };

    unsigned int VAOName;
    glGenVertexArrays(1, &VAOName);
    glBindVertexArray(VAOName);
    unsigned int VBOName;
    glGenBuffers(1, &VBOName);
    glBindBuffer(GL_ARRAY_BUFFER, VBOName);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nameVertices), nameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ////////////////////////////// TEXTURES ///////////////////////////////

    unsigned dashboardTexture = loadImageToTexture("res/dashboard.png");
    glBindTexture(GL_TEXTURE_2D, dashboardTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned roadTexture = loadImageToTexture("res/road.jpg");
    glBindTexture(GL_TEXTURE_2D, roadTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned roadSpecTexture = loadImageToTexture("res/road_spec.png");
    glBindTexture(GL_TEXTURE_2D, roadSpecTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned buildingTexture = loadImageToTexture("res/building.jpg");
    glBindTexture(GL_TEXTURE_2D, buildingTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned buildingSpecTexture = loadImageToTexture("res/building_spec.jpg");
    glBindTexture(GL_TEXTURE_2D, buildingSpecTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned moonTexture = loadImageToTexture("res/batsignal_moon.png");
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned nameTexture = loadImageToTexture("res/signature.png");
    glBindTexture(GL_TEXTURE_2D, nameTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    ///////////////////////////////////////////////////////////////////////

    Model wheel("res/wheel/13488_Steering_Column_V1_l3.obj");
    Shader wheelShader("shaders/wheel.vert", "shaders/wheel.frag");
    Shader textureShader("shaders/texture.vert", "shaders/texture.frag");
    Shader nameShader("shaders/name.vert", "shaders/name.frag");
    Shader streetShader("shaders/street.vert", "shaders/street.frag");
    Shader arrowShader("shaders/arrow.vert", "shaders/arrow.frag");
    Shader batShader("shaders/bat.vert", "shaders/basic.frag");
    Shader dashboardShader("shaders/dashboard.vert", "shaders/dashboard.frag");
    Shader dashboardLightsShader("shaders/basic.vert", "shaders/dashboardLights.frag");
    Shader gearShader("shaders/basic.vert", "shaders/gear.frag");
    Shader basicShader("shaders/basic.vert", "shaders/odometer.frag");

    ///Kamera ulice
    glm::vec3 streetCameraPosition = glm::vec3(0.0f, 0.0f, 9.5f);
    glm::vec3 streetCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 streetCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 streetPerspectiveProjection = glm::perspective(glm::radians(120.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    glm::mat4 streetView = glm::lookAt(streetCameraPosition, streetCameraPosition + streetCameraFront, streetCameraUp);

    glm::mat4 roadModelMatrix = glm::mat4(1.0f);
    roadModelMatrix = glm::scale(roadModelMatrix, glm::vec3(1.0f, 1.0f, 10.5f));

    glm::mat4 moonModelMatrix = glm::mat4(1.0f);
    moonModelMatrix = glm::scale(moonModelMatrix, glm::vec3(1.6f, 2.0f, 1.0f));
    moonModelMatrix = glm::translate(moonModelMatrix, glm::vec3(-4.0f, 10.0, -15.0));

    // Kamera instrument table
    glm::vec3 dashboardCameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 dashboardCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 dashboardCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 dashboardPerspectiveProjection = glm::perspective(glm::radians(10.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);    
    glm::mat4 dashboardView = glm::lookAt(dashboardCameraPosition, dashboardCameraPosition + dashboardCameraFront, dashboardCameraUp);


    glm::mat4 wheelModelMatrix = glm::mat4(1.0f);
    wheelModelMatrix = glm::scale(wheelModelMatrix, glm::vec3(0.015f, 0.028f, 0.028f));
    wheelModelMatrix = glm::rotate(wheelModelMatrix, glm::radians(-180.0f), glm::vec3(0.0f, -1.0f, 0.75f));
    wheelModelMatrix = glm::rotate(wheelModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wheelModelMatrix = glm::translate(wheelModelMatrix, glm::vec3(0.0f, 0.0, -30.0));

    glm::mat4 dashboardModelMatrix = glm::mat4(1.0f);
    dashboardModelMatrix = glm::scale(dashboardModelMatrix, glm::vec3(0.5f, 0.6f, 1.0f));
    dashboardModelMatrix = glm::translate(dashboardModelMatrix, glm::vec3(0.004f, 0.0, 0.0));

    glm::mat4 batModelMatrix = glm::mat4(1.0f);
    batModelMatrix = glm::scale(batModelMatrix, glm::vec3(0.02f, 0.04f, 1.0f));
    batModelMatrix = glm::translate(batModelMatrix, glm::vec3(-0.3, 3.0, 0.0));

    glm::mat4 checkEngineModelMatrix = glm::mat4(1.0f);
    checkEngineModelMatrix = glm::scale(checkEngineModelMatrix, glm::vec3(0.02f, 0.04f, 1.0f));
    checkEngineModelMatrix = glm::translate(checkEngineModelMatrix, glm::vec3(-1.0, -0.5, 0.0));
    
    glm::mat4 batteryModelMatrix = glm::mat4(1.0f);
    batteryModelMatrix = glm::scale(batteryModelMatrix, glm::vec3(0.02f, 0.04f, 1.0f));
    batteryModelMatrix = glm::translate(batteryModelMatrix, glm::vec3(0.8, -0.5, 0.0));

    glm::mat4 gearModelMatrix = glm::mat4(1.0f);
    gearModelMatrix = glm::scale(gearModelMatrix, glm::vec3(0.02f, 0.04f, 1.0f));
    gearModelMatrix = glm::translate(gearModelMatrix, glm::vec3(-0.1, -3.0, 0.0));

    float forwardCameraSpeed = 0.0; 
    float backwardCameraSpeed = 0.0;
    float currentSpeed = 0.0;   
    float sideCameraSpeed = 0.0001;
    bool upActive = false;
    bool downActive = false;
    float batX = 0, batY = 0;
    bool checkEngineActive = false;
    bool batteryActive = false;
    int gear = 0;
    int kilometersThreshold = 10.0;
    float kilometersInTotal = 0.0;
    bool reflectorActive = false;

    vector<glm::vec3> buildingTranslations = {
    glm::vec3( 1.4f, 0.25f,  0.0f),   
    glm::vec3(-1.4f, 0.25f,  0.0f),   
    glm::vec3( 1.4f, 0.25f, -1.3f),
    glm::vec3(-1.4f, 0.25f, -1.3f),
    glm::vec3( 1.4f, 0.25f,  1.3f),
    glm::vec3(-1.4f, 0.25f,  1.3f),
    glm::vec3( 1.4f, 0.25f,  2.5f),
    glm::vec3(-1.4f, 0.25f,  2.5f),
    glm::vec3( 1.4f, 0.25f, -2.5f),
    glm::vec3(-1.4f, 0.25f, -2.5f),
    glm::vec3( 1.4f, 0.25f,  3.7f),
    glm::vec3(-1.4f, 0.25f,  3.7f),
    glm::vec3( 1.4f, 0.25f, -3.7f),
    glm::vec3(-1.4f, 0.25f, -3.7f),
    };

    vector<glm::vec3> odometerTranslations = {
    glm::vec3(8.0, 2.5, 0.0),
    glm::vec3(9.2, 2.5, 0.0),
    glm::vec3(10.4, 2.5, 0.0),
    glm::vec3(11.6, 2.5, 0.0),
    glm::vec3(12.8, 2.5, 0.0),
    glm::vec3(14.0, 2.5, 0.0),
    glm::vec3(15.2, 2.5, 0.0),
    glm::vec3(16.4, 2.5, 0.0),
    glm::vec3(17.6, 2.5, 0.0),
    glm::vec3(18.8, 2.5, 0.0),
    };

    glm::vec3 lightA = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 lightD = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 lightS = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 pointLightCheckEngine = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 pointLightBattery = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 reflectorLight = glm::vec3(0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window))
    {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_CULL_FACE);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //////////////////////////////////////////// STREET VIEWPORT ///////////////////////////////////////////////
        glViewport(0, 0.45 * wHeight, wWidth, 0.55 * wHeight);

        HandleUpDown(window, upActive, downActive, streetCameraPosition, forwardCameraSpeed, streetCameraFront, backwardCameraSpeed, kilometersInTotal);
        HandleLeftRight(window, upActive, sideCameraSpeed, forwardCameraSpeed, streetCameraPosition, streetCameraFront, streetCameraUp, backwardCameraSpeed);
        HandleRotation(window, streetCameraFront);
        HandleNightVision(window, lightA, lightD, lightS);
        HandleGear(upActive, downActive, currentSpeed, forwardCameraSpeed, backwardCameraSpeed, gear);
        HandleSpotlight(window, reflectorLight);
            
        streetView = glm::lookAt(streetCameraPosition, streetCameraPosition + streetCameraFront, streetCameraUp);
        
        
        streetShader.use();
        streetShader.setMat4("projection", streetPerspectiveProjection);
        streetShader.setMat4("view", streetView);
        streetShader.setMat4("model", roadModelMatrix); 
        streetShader.setVec3("dirLight.direction", 3.0f, -10.0, -15.0);
        streetShader.setVec3("viewPos", streetCameraPosition);
        // direct light properties
        streetShader.setVec3("dirLight.ambient", lightA);
        streetShader.setVec3("dirLight.diffuse", lightD);
        streetShader.setVec3("dirLight.specular", lightS);
        // spot light properties
        streetShader.setVec3("spotLight.position", streetCameraPosition);
        streetShader.setVec3("spotLight.direction", streetCameraFront);
        streetShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        streetShader.setVec3("spotLight.diffuse", reflectorLight);
        streetShader.setVec3("spotLight.specular", reflectorLight);
        streetShader.setFloat("spotLight.constant", 1.0f);
        streetShader.setFloat("spotLight.linear", 0.09f);
        streetShader.setFloat("spotLight.quadratic", 0.032f);
        streetShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(50.0f)));  
        streetShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(60.0f)));;
        // material properties
        streetShader.setFloat("material.shininess", 20.0f);
        streetShader.setInt("material.diffuse", 0);
        streetShader.setInt("material.specular", 1);   
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, roadTexture);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, roadSpecTexture);
        glBindVertexArray(VAORoad);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);

        for (size_t i = 0; i < buildingTranslations.size(); ++i)
        {
            glm::mat4 buildingModelMatrix = glm::mat4(1.0f);
            buildingModelMatrix = glm::scale(buildingModelMatrix, glm::vec3(1.0f, 2.0f, 2.5f));
            buildingModelMatrix = glm::translate(buildingModelMatrix, buildingTranslations[i]);
            streetShader.use();
            streetShader.setMat4("projection", streetPerspectiveProjection);
            streetShader.setMat4("view", streetView);

            streetShader.setVec3("dirLight.direction", 3.0f, -10.0, 10.0);
            streetShader.setVec3("viewPos", streetCameraPosition);
            // direct light properties
            streetShader.setVec3("dirLight.ambient", lightA);
            streetShader.setVec3("dirLight.diffuse", lightD);
            streetShader.setVec3("dirLight.specular", lightS);
            // spot light properties
            streetShader.setVec3("spotLight.position", streetCameraPosition);
            streetShader.setVec3("spotLight.direction", streetCameraFront);
            streetShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            streetShader.setVec3("spotLight.diffuse", reflectorLight);
            streetShader.setVec3("spotLight.specular", reflectorLight);
            streetShader.setFloat("spotLight.constant", 1.0f);
            streetShader.setFloat("spotLight.linear", 0.09f);
            streetShader.setFloat("spotLight.quadratic", 0.032f);
            streetShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(30.0f)));
            streetShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(80.0f)));
            // material properties
            streetShader.setFloat("material.shininess", 100.0f);
            streetShader.setInt("material.diffuse", 0);
            streetShader.setInt("material.specular", 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, buildingTexture);
            // bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, buildingSpecTexture);
            glBindVertexArray(VAOBuilding);
            streetShader.setMat4("model", buildingModelMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, 0);
        }     

        textureShader.use();
        textureShader.setMat4("uP", streetPerspectiveProjection);
        textureShader.setMat4("uV", streetView);
        textureShader.setMat4("uM", moonModelMatrix);
        glBindVertexArray(VAOMoon);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);       

        //////////////////////////////////////////// DASHBOARD VIEWPORT ///////////////////////////////////////////////
        glViewport(0, 0, wWidth, 0.45 * wHeight);

        HandleBatMoving(window, batY, batX);
        HandleDashboardLights(window, checkEngineActive, pointLightCheckEngine, batteryActive, pointLightBattery);
    
        wheelShader.use();
        wheelShader.setVec3("uLightPos", 0.0, 10.0, 3.0);
        wheelShader.setVec3("uViewPos", 0.0f, 0.0f, 5.0f);
        wheelShader.setVec3("uLightColor", 0.5, 0.5, 0.5);
        wheelShader.setMat4("uP", dashboardPerspectiveProjection);
        wheelShader.setMat4("uV", dashboardView);
        wheelShader.setMat4("uM", wheelModelMatrix);
        wheel.Draw(wheelShader);

        dashboardShader.use();
        dashboardShader.setMat4("projection", dashboardPerspectiveProjection);
        dashboardShader.setMat4("view", dashboardView);
        dashboardShader.setMat4("model", dashboardModelMatrix);     
        dashboardShader.setVec3("viewPos", 0.0f, 0.0f, 5.0f);

        // directional light
        dashboardShader.setVec3("dirLight.direction", 0.f, 5.0f, 3.0f);
        dashboardShader.setVec3("dirLight.ambient", 0.5, 0.5, 0.5);
        dashboardShader.setVec3("dirLight.diffuse", 0.5, 0.5, 0.5);
        dashboardShader.setVec3("dirLight.specular", 0.5, 0.5, 0.5);
       
        // point light 1 check engine
        dashboardShader.setVec3("pointLights[0].position", -0.3, 0.0, -0.5);
        dashboardShader.setVec3("pointLights[0].ambient", pointLightCheckEngine);
        dashboardShader.setVec3("pointLights[0].diffuse", pointLightCheckEngine);
        dashboardShader.setVec3("pointLights[0].specular", pointLightCheckEngine);
        dashboardShader.setFloat("pointLights[0].constant", 1.0f);
        dashboardShader.setFloat("pointLights[0].linear", 0.09f);
        dashboardShader.setFloat("pointLights[0].quadratic", 0.032f);

        // point light batery
        dashboardShader.setVec3("pointLights[1].position", 0.3, 0.0, -0.5);
        dashboardShader.setVec3("pointLights[1].ambient", pointLightBattery);
        dashboardShader.setVec3("pointLights[1].diffuse", pointLightBattery);
        dashboardShader.setVec3("pointLights[1].specular", pointLightBattery);
        dashboardShader.setFloat("pointLights[1].constant", 1.0f);
        dashboardShader.setFloat("pointLights[1].linear", 0.09f);
        dashboardShader.setFloat("pointLights[1].quadratic", 0.032f);
        
        // material properties
        dashboardShader.setFloat("material.shininess",50.0f);
        dashboardShader.setInt("material.diffuse", 0);
        dashboardShader.setInt("material.specular", 1);

        glBindVertexArray(VAODashboard);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dashboardTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dashboardTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);

        arrowShader.use();
        if (upActive) {
            arrowShader.setFloat("uSpeed", forwardCameraSpeed);
        }
        else {
            arrowShader.setFloat("uSpeed", backwardCameraSpeed);
        }
        arrowShader.setVec3("uFixedPoint", glm::vec3(-0.72, -0.52, 0.1) );
        glLineWidth(5.0);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
       
        arrowShader.use();
        if (upActive) {
            arrowShader.setFloat("uSpeed", forwardCameraSpeed);
        }
        else {
            arrowShader.setFloat("uSpeed", backwardCameraSpeed);
        }
        arrowShader.setVec3("uFixedPoint", glm::vec3(0.68, -0.52, 0.1));
        glLineWidth(5.0);
        glDrawArrays(GL_LINE_STRIP, 0, 2);       
        
        batShader.use();
        batShader.setVec2("uPos", glm::vec2(batX, batY));
        batShader.setMat4("uP", dashboardPerspectiveProjection);
        batShader.setMat4("uV", dashboardView);
        batShader.setMat4("uM", batModelMatrix);
        glBindVertexArray(VAOBat);
        glDrawArrays(GL_TRIANGLES, 0, 12);


        dashboardLightsShader.use();
        dashboardLightsShader.setMat4("uP", dashboardPerspectiveProjection);
        dashboardLightsShader.setMat4("uV", dashboardView);
        dashboardLightsShader.setMat4("uM", checkEngineModelMatrix);
        dashboardLightsShader.setBool("uLightActive", checkEngineActive);
        dashboardLightsShader.setVec3("uLightColor", glm::vec3(0.5, 0.0, 0.0));
        glBindVertexArray(VAODashboardLights);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        dashboardLightsShader.use();
        dashboardLightsShader.setMat4("uP", dashboardPerspectiveProjection);
        dashboardLightsShader.setMat4("uV", dashboardView);
        dashboardLightsShader.setMat4("uM", batteryModelMatrix);
        dashboardLightsShader.setBool("uLightActive", batteryActive);
        dashboardLightsShader.setVec3("uLightColor", glm::vec3(0.5, 0.5, 0.0));
        glBindVertexArray(VAODashboardLights);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        gearShader.use();
        gearShader.setMat4("uP", dashboardPerspectiveProjection);
        gearShader.setMat4("uV", dashboardView);
        gearShader.setMat4("uM", gearModelMatrix);
        gearShader.setFloat("uTime", glfwGetTime());
        if (upActive) {
            gearShader.setInt("uGear", gear);
        }
        else {
            gearShader.setInt("uGear", gear);
        }
        glBindVertexArray(VAODashboardLights);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        kilometersThreshold = kilometersInTotal - (10 * floor(kilometersInTotal / 10));
        for (size_t i = 0; i < kilometersThreshold; ++i)
        {
            glm::mat4 odometerModelMatrix = glm::mat4(1.0f);
            odometerModelMatrix = glm::scale(odometerModelMatrix, glm::vec3(0.01f, 0.04f, 1.0f));
            odometerModelMatrix = glm::translate(odometerModelMatrix, odometerTranslations[i]);
            basicShader.use();
            basicShader.setMat4("uP", dashboardPerspectiveProjection);
            basicShader.setMat4("uV", dashboardView);
            basicShader.setMat4("uM", odometerModelMatrix);
            glBindVertexArray(VAODashboardLights);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        nameShader.use();
        glBindVertexArray(VAOName);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VAODashboard);
    glDeleteVertexArrays(1, &VBODashboard);
    glDeleteBuffers(1, &VAOBat);
    glDeleteVertexArrays(1, &VBOBat);
    glDeleteBuffers(1, &VAODashboardLights);
    glDeleteVertexArrays(1, &VBODashboardLights);
    glDeleteBuffers(1, &VAORoad);
    glDeleteVertexArrays(1, &VBORoad);
    glDeleteBuffers(1, &VAOBuilding);
    glDeleteVertexArrays(1, &VBOBuilding);
    glDeleteBuffers(1, &VAOMoon);
    glDeleteVertexArrays(1, &VBOMoon);
    glDeleteBuffers(1, &VAOName);
    glDeleteVertexArrays(1, &VBOName);

    glfwTerminate();
    return 0;
}

void HandleSpotlight(GLFWwindow* window, glm::vec3& reflectorLight)
{
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        reflectorLight = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_R) != GLFW_PRESS) {
        reflectorLight = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void HandleGear(bool upActive, bool downActive, float& currentSpeed, float forwardCameraSpeed, float backwardCameraSpeed, int& gear)
{
    if (upActive) {
        currentSpeed = forwardCameraSpeed;
    }
    else {
        currentSpeed = backwardCameraSpeed;
    }

    if (currentSpeed <= 0.0 ) {
        gear = 0;
    }
    else if (currentSpeed < 0.001) {
        gear = 1;
    }
    else if (currentSpeed < 0.002) {
        gear = 2;
    }
    else if (currentSpeed < 0.003) {
        gear = 3;
    }
    else if (currentSpeed < 0.004) {
        gear = 4;
    }
    else {
        gear = 5;
    }
}

void HandleDashboardLights(GLFWwindow* window, bool& checkEngineActive, glm::vec3& pointLightCheckEngine, bool& batteryActive, glm::vec3& pointLightBattery)
{
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        checkEngineActive = true;
        pointLightCheckEngine = glm::vec3(0.2f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS) {
        checkEngineActive = false;
        pointLightCheckEngine = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        batteryActive = true;
        pointLightBattery = glm::vec3(0.2f, 0.2f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_B) != GLFW_PRESS) {
        batteryActive = false;
        pointLightBattery = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void HandleBatMoving(GLFWwindow* window, float& batY, float& batX)
{
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (batY <= 1.6) {
            batY += 0.002;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (batX >= -2.4) {
            batX -= 0.002;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        if (batY >= -1.3) {
            batY -= 0.002;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (batX <= 2.3) {
            batX += 0.002;
        }
    }
}

void HandleNightVision(GLFWwindow* window, glm::vec3& lightA, glm::vec3& lightD, glm::vec3& lightS)
{
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        lightA = glm::vec3(0.0f, 0.5f, 0.0f);
        lightD = glm::vec3(0.0f, 0.5f, 0.0f);
        lightS = glm::vec3(0.0f, 0.5f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_N) != GLFW_PRESS)
    {
        lightA = glm::vec3(0.2f, 0.2f, 0.2f);
        lightD = glm::vec3(0.5f, 0.5f, 0.5f);
        lightS = glm::vec3(1.0f, 1.0f, 1.0f);
    }
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

void HandleUpDown(GLFWwindow* window, bool& upActive, bool& downActive, glm::vec3& streetCameraPosition, float& forwardCameraSpeed, glm::vec3& streetCameraFront, float& backwardCameraSpeed, float& kilometersInTotal)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        upActive = true;
        downActive = false;
        backwardCameraSpeed = 0.0;
        if (streetCameraPosition.z <= -9.4 || streetCameraPosition.x <= -0.7f || streetCameraPosition.x >= 0.7f) {
            forwardCameraSpeed = 0.0;
        }
        if (streetCameraPosition.z > -9.4f || streetCameraPosition.z <= 9.5f || streetCameraPosition.x > -0.7f || streetCameraPosition.x < 0.7f) {  // logicki obrnuto, zbog -1 kod cameraFront
            streetCameraPosition += forwardCameraSpeed * streetCameraFront;
            if (forwardCameraSpeed < 0.05) {
                forwardCameraSpeed += 0.0000005;
                kilometersInTotal += 0.0005;
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS)
    {
        if (upActive) {
            if (streetCameraPosition.z <= -9.4 || streetCameraPosition.x <= -0.7f || streetCameraPosition.x >= 0.7f) {
                forwardCameraSpeed = 0.0;
            }
            if (streetCameraPosition.z > -9.4f || streetCameraPosition.z <= 9.5f || streetCameraPosition.x > -0.7f || streetCameraPosition.x < 0.7f) {  // logicki obrnuto, zbog -1 kod cameraFront
                if (forwardCameraSpeed > 0.0) {
                    streetCameraPosition += forwardCameraSpeed * streetCameraFront;
                    forwardCameraSpeed -= 0.0000005;
                }
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        downActive = true;
        upActive = false;
        forwardCameraSpeed = 0.0;
        if (streetCameraPosition.z >= 9.4 || streetCameraPosition.x <= -0.7f || streetCameraPosition.x >= 0.7f) {
            backwardCameraSpeed = 0.0;
        }
        if (streetCameraPosition.z < 9.4f || streetCameraPosition.z >= -9.5f || streetCameraPosition.x >= -0.7f || streetCameraPosition.x <= 0.7f) { // logicki obrnuto, zbog -1 kod cameraFront
            streetCameraPosition -= backwardCameraSpeed * streetCameraFront;
            if (backwardCameraSpeed < 0.05) {
                backwardCameraSpeed += 0.0000005;
                kilometersInTotal += 0.0005;

            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS)
    {
        if (downActive) {
            if (streetCameraPosition.z >= 9.4 || streetCameraPosition.x <= -0.7f || streetCameraPosition.x >= 0.7f) {
                backwardCameraSpeed = 0.0;
            }
            if (streetCameraPosition.z < 9.4f || streetCameraPosition.z >= -9.5f || streetCameraPosition.x >= -0.7f || streetCameraPosition.x <= 0.7f) {
                if (backwardCameraSpeed > 0.0) {
                    streetCameraPosition -= backwardCameraSpeed * streetCameraFront;
                    backwardCameraSpeed -= 0.0000005;
                }
            }
        }
    }
}

void HandleLeftRight(GLFWwindow* window, bool upActive, float& sideCameraSpeed, float forwardCameraSpeed, glm::vec3& streetCameraPosition, glm::vec3& streetCameraFront, glm::vec3& streetCameraUp, float backwardCameraSpeed)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (upActive) {
            sideCameraSpeed = forwardCameraSpeed / 2;
            if (streetCameraPosition.x > -0.65f) {
                streetCameraPosition -= glm::normalize(glm::cross(streetCameraFront, streetCameraUp)) * sideCameraSpeed; //glm cross racuna vektor normalan na ova dva
            }
        }
        else {
            sideCameraSpeed = backwardCameraSpeed / 2;
            if (streetCameraPosition.x > -0.65f) {
                streetCameraPosition -= glm::normalize(glm::cross(streetCameraFront, streetCameraUp)) * sideCameraSpeed;
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (upActive) {
            sideCameraSpeed = forwardCameraSpeed / 2;
            if (streetCameraPosition.x < 0.65f) {
                streetCameraPosition += glm::normalize(glm::cross(streetCameraFront, streetCameraUp)) * sideCameraSpeed;
            }
        }
        else {
            sideCameraSpeed = backwardCameraSpeed / 2;
            if (streetCameraPosition.x < 0.65f) {
                streetCameraPosition += glm::normalize(glm::cross(streetCameraFront, streetCameraUp)) * sideCameraSpeed;
            }
        }
    }
}

void HandleRotation(GLFWwindow* window, glm::vec3& streetCameraFront)
{
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (streetCameraFront.x >= -0.5) {
            streetCameraFront.x -= 0.001;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (streetCameraFront.x <= 0.5) {
            streetCameraFront.x += 0.001;
        }
    }
}