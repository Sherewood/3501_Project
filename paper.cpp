#include "Paper.h"
#include <sstream>
#include <iostream>
namespace game {

    Paper::Paper(const std::string name, const Resource* geometry, const Resource* material) : SceneNode(name, geometry, material)
    {

    }
    

    Paper::~Paper() {
    }
   

    glm::quat Paper::GetAngM(void) const {

        return angm_;
    }

    void Paper::SetAngM(glm::quat angm) {

        angm_ = angm;
    }


    void Paper::Update(void) {

        
    }
    bool Paper::read() {

    }


} // namespace game
