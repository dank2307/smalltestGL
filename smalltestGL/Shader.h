#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader {

    private:

    unsigned int shaderProgram;
    
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string GeometrySource;
        std::string FragmentSource;

    };
    
    ShaderProgramSource ParseShader(const std::string& filepath);
    void compTest(unsigned int& totest, std::string type);


    public:
   
	Shader(const std::string& filepath);
    void use();
    unsigned int getProgramId();
       
};



Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, GEOMETRY=1, FRAGMENT = 2
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("geometry") != std::string::npos)
                type = ShaderType::GEOMETRY;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;


        }

        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str() , ss[1].str(), ss[2].str() };

}

void Shader::compTest(unsigned int& totest, std::string type)
{
    //test compilation
    int success;
    char infoLog[512];

    if (type != "PROGRAM")
    {
        glGetShaderiv(totest, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(totest, 512, NULL, infoLog);
            std::cout << "Error compilation shader: \n" << infoLog << std::endl;

        }
    }
    else
    {
        glGetProgramiv(totest, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(totest, 512, NULL, infoLog);
            std::cout << "Error compilation Program\n" << infoLog << std::endl;

        }
    }



}

Shader::Shader(const std::string& filepath)
{


    //get shader code from file:
    ShaderProgramSource source = ParseShader(filepath);
    const char* vertexShaderSource = source.VertexSource.c_str();
    const char* geometryShaderSource = source.GeometrySource.c_str();
    const char* fragmentShaderSource = source.FragmentSource.c_str();



    //initialize vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    compTest(vertexShader, "VERTEX");

    
    //initialize geometry shader
    unsigned int geometryShader;
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
    glCompileShader(geometryShader);

    compTest(geometryShader, "GEOMETRY");


    //initialize fregment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    compTest(fragmentShader, "FRAGMENT");


    //link program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    this->shaderProgram = shaderProgram;

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    compTest(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);


}

void Shader::use()
{
    glUseProgram(this->shaderProgram);
}

unsigned int Shader::getProgramId()
{
    return this->shaderProgram;
}