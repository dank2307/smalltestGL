#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{

	private:
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Projection;


	public:
        Camera();
        void setModel();
        void setView(glm::vec3 ceye, glm::vec3 ctarget);
        void setProjection(float fovangle, float windowwidth, float windowheight, float nearplane, float farplane);
        glm::mat4 getModel();
        glm::mat4 getView();
        glm::mat4 getProjection();

            



};