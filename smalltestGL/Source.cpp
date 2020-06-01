#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Shader.h"
#include "Camera.h"
#include "SrtObject.h"

#define LOG(x) std::cout << x << std::endl

//Define Window size:
#define W_WIDTH (int)900
#define W_HEIGHT (int)600

#define POLY_ON true


int main(void){

    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "smol shader test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //glewInit after creating valid context:
    if (glewInit() != GLEW_OK)
        LOG("Error");
    
    //use modern GL from here:
    //___________________________________________________

    LOG(glGetString(GL_VERSION)); //print GL version




    /*DATA*/
    float vertices[] = {
        /*
        -0.5f, -0.5f, -0.5f,   //hinten
         0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,  //vorne
         0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f
         */

    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        3,4,5
    };

    /*DATA*/


    //VBO: vertex buffer object ; VAO: vertex array object; EBO element buffer object
    unsigned int VBO,VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    //bind all objects and set data
    glBindVertexArray(VAO);
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Buffer Array
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    //generate Attribute Pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    //Shader:

    Shader myShader("res/shaders/trianglediv.glsl");
    myShader.use();

    //all uniform Locations: -------------
    
    //4d geometry

    //Beobachter System:
    int utimelocation = glGetUniformLocation(myShader.getProgramId(), "u_time"); //observer time

    int a1Loc = glGetUniformLocation(myShader.getProgramId(), "a1");
    int beta1Loc = glGetUniformLocation(myShader.getProgramId(), "beta1");
    int Lambda1Loc = glGetUniformLocation(myShader.getProgramId(), "Lambda1");
    int Lambda1INVLoc = glGetUniformLocation(myShader.getProgramId(), "Lambda1INV");

    
    //Gegenstandssystem:
    int a2Loc = glGetUniformLocation(myShader.getProgramId(), "a2");
    int beta2Loc = glGetUniformLocation(myShader.getProgramId(), "beta2");
    int Lambda2Loc = glGetUniformLocation(myShader.getProgramId(), "Lambda2");
    int Lambda2INVLoc = glGetUniformLocation(myShader.getProgramId(), "Lambda2INV");



    //3d Grafik:
    int modelLoc = glGetUniformLocation(myShader.getProgramId(), "model");
    int viewLoc = glGetUniformLocation(myShader.getProgramId(), "view");
    int projectionLoc = glGetUniformLocation(myShader.getProgramId(), "projection");

    
    //uniform Locations] -------------



    if(POLY_ON)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //nur Kanten zeichnen



    //Beobachter System:
    glm::vec3 beta1 = glm::vec3(0.0f, 0.0f,0.0f);
    glm::vec3 a1 = glm::vec3(0.0f, 0.0f, 8.0f);
    
    SrtObject srtBeobachter;
    srtBeobachter.setPoincare(beta1, a1);


    //Gegenstandssystem:
    glm::vec3 beta2 = glm::vec3(0.5f, -0.3f, 0.0f);
    glm::vec3 a2 = glm::vec3(0.0f, 0.0f, 0.0f);

    SrtObject srtWuerfel;
    srtWuerfel.setPoincare(beta2, a2);
    
    
    //set uniform to shader:

    glUniform4fv(a1Loc, 1, glm::value_ptr(srtBeobachter.getTrans()));
    glUniform4fv(a2Loc, 1, glm::value_ptr(srtWuerfel.getTrans()));

    glUniform3fv(beta1Loc, 1, glm::value_ptr(beta1));
    glUniform3fv(beta2Loc, 1, glm::value_ptr(beta2));

    glUniformMatrix4fv(Lambda1Loc, 1, GL_FALSE, glm::value_ptr(srtBeobachter.getMat()));
    glUniformMatrix4fv(Lambda1INVLoc, 1, GL_FALSE, glm::value_ptr(srtBeobachter.getMatINV()));
    glUniformMatrix4fv(Lambda2Loc, 1, GL_FALSE, glm::value_ptr(srtWuerfel.getMat()));
    glUniformMatrix4fv(Lambda2INVLoc, 1, GL_FALSE, glm::value_ptr(srtWuerfel.getMatINV()));



    //4d -----------------------------------------------------------------]



    //Kamera initialisieren:
    Camera myCamera;
    myCamera.setModel();
    myCamera.setView(a1,glm::vec3(0.0f,0.0f,0.0f));
    myCamera.setProjection(30.0f, W_WIDTH, W_HEIGHT, 0.1f, 100.0f);


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getModel()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getView()));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getProjection()));


    //_______________________________________________________
    glEnable(GL_DEPTH_TEST);
   
    float timeValue = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    
        glUniform1f(utimelocation, timeValue);
        
        if (timeValue <= 20) {
            timeValue += 0.0005f; // glfwGetTime();
        }
        

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        glDrawArrays(GL_TRIANGLES, 0, 12*3);

        //glBindVertexArray(0);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    //test log for program end:
    LOG("---program end---");

    glfwTerminate();
    return 0;
}