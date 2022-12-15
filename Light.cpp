#include "Light.h"

namespace game {

Light::Light(){
    light_vector = glm::vec3(0, 0, 0);
}


Light::~Light(){
}


void Light::Update(glm::vec3 pos) {

    light_vector = -pos;
}
            
glm::vec3 Light::GetPosition()
{
    return light_vector;
}

} // namespace game
