#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class SrtObject
{
	
	private:
        glm::mat4 Lambda;
        glm::mat4 LambdaINV;
        float gamma;
        glm::vec4 Translation;

	public:
        SrtObject();
        SrtObject(glm::vec3 beta, glm::vec3 a);
        void setPoincare(glm::vec3 beta, glm::vec3 a);
        void setBeta(glm::vec3 beta);
        void setTranslation(glm::vec3 a);

        glm::mat4 getMat();
        glm::mat4 getMatINV();
        glm::vec4 getTrans();



};
