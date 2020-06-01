#include "SrtObject.h"

SrtObject::SrtObject()
{
    Lambda = glm::mat4(1.0f);
    LambdaINV = glm::mat4(1.0f);
    gamma = 1.0f;
    Translation = glm::vec4(0.0f);
}

SrtObject::SrtObject(glm::vec3 beta, glm::vec3 a)
{
    setBeta(beta);
    setTranslation(a);
}

void SrtObject::setPoincare(glm::vec3 beta, glm::vec3 a)
{
    setBeta(beta);
    setTranslation(a);
}


void SrtObject::setBeta(glm::vec3 beta)
{
    gamma = 1.0f / (glm::sqrt(1.0f - (glm::pow(beta.x, 2) + glm::pow(beta.y, 2) + glm::pow(beta.z, 2))));

    LambdaINV = glm::mat4(glm::vec4(gamma, -gamma * beta.x, -gamma * beta.y, -gamma * beta.z),
            glm::vec4(-gamma * beta.x, 1 + ((gamma * gamma) / (gamma + 1)) * beta.x * beta.x, ((gamma * gamma) / (gamma + 1)) * beta.y * beta.x, ((gamma * gamma) / (gamma + 1)) * beta.z * beta.x),
            glm::vec4(-gamma * beta.y, ((gamma * gamma) / (gamma + 1)) * beta.x * beta.y, 1 + ((gamma * gamma) / (gamma + 1)) * beta.y * beta.y, ((gamma * gamma) / (gamma + 1)) * beta.z * beta.y),
            glm::vec4(-gamma * beta.z, ((gamma * gamma) / (gamma + 1)) * beta.x * beta.z, ((gamma * gamma) / (gamma + 1)) * beta.y * beta.z, 1 + ((gamma * gamma) / (gamma + 1)) * beta.z * beta.z));
    
    
    Lambda = glm::mat4(glm::vec4(gamma, gamma * beta.x, gamma * beta.y, gamma * beta.z), 
            glm::vec4(gamma * beta.x, 1 + ((gamma * gamma) / (gamma + 1)) * beta.x * beta.x, ((gamma * gamma) / (gamma + 1)) * beta.y * beta.x, ((gamma * gamma) / (gamma + 1)) * beta.z * beta.x),
            glm::vec4(gamma * beta.y, ((gamma * gamma) / (gamma + 1)) * beta.x * beta.y, 1 + ((gamma * gamma) / (gamma + 1)) * beta.y * beta.y, ((gamma * gamma) / (gamma + 1)) * beta.z * beta.y),
            glm::vec4(gamma * beta.z, ((gamma * gamma) / (gamma + 1)) * beta.x * beta.z, ((gamma * gamma) / (gamma + 1)) * beta.y * beta.z, 1 + ((gamma * gamma) / (gamma + 1)) * beta.z * beta.z));
    
}

void SrtObject::setTranslation(glm::vec3 a)
{
    Translation = glm::vec4(0,a);
}

glm::mat4 SrtObject::getMat()
{
    return Lambda;
}

glm::mat4 SrtObject::getMatINV()
{
    return LambdaINV;
}

glm::vec4 SrtObject::getTrans()
{
    return Translation;
}
