#include "Camera.h"

Camera::Camera()
{
    Model = glm::mat4(1.0f);
    View = glm::mat4(1.0f);
    Projection = glm::mat4(1.0f);

}

void Camera::setModel() {
    Model = glm::mat4(1.0f);
}

void Camera::setView(glm::vec3 ceye, glm::vec3 ctarget)
{ 
    //crate camera up first
    glm::vec3 cdirection = glm::normalize(-ceye + ctarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cright = glm::normalize(glm::cross(cdirection, up));
    glm::vec3 cup = glm::normalize(glm::cross(cright, cdirection));

    View = glm::lookAt(ceye,ctarget,cup);
}

void Camera::setProjection(float fovangle, float windowwidth, float windowheight, float nearplane, float farplane)
{
    Projection = glm::perspective(glm::radians(fovangle), windowwidth / windowheight, nearplane, farplane);
}

glm::mat4 Camera::getModel()
{
    return Model;
}

glm::mat4 Camera::getView()
{
    return View;
}

glm::mat4 Camera::getProjection()
{
    return Projection;
}

