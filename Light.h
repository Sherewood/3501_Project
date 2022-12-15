#ifndef Light_H_
#define Light_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"


namespace game {

    // Abstraction of an Light
    class Light {

    public:
        // Create Light from given resources
        Light();

        // Destructor
        ~Light();
        void Update(glm::vec3 pos);
        glm:: vec3 GetPosition();

    private:
        // Angular momentum of Light
        
        glm::vec3 light_vector;

    }; // class Light

} // namespace game

#endif // Light_H_
