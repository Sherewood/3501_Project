#ifndef Paper_H_
#define Paper_H_


#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <vector>


#include "resource.h"
#include "scene_node.h"
#include "camera.h"

namespace game {

    // Abstraction of an Paper
    class Paper : public SceneNode {

    public:
        // Create Paper from given resources
        Paper(const std::string name, const Resource* geometry, const Resource* material);

        // Destructor
        ~Paper();

        // Get/set attributes specific to Papers
        glm::quat GetAngM(void) const;
        void SetAngM(glm::quat angm);
        // Update geometry configuration
        void Update(void);
        void read();
      

    private:
        // Angular momentum of Paper
       // glm::quat angm_;
        float radius = 10.0f;
        std::string lore;


    }; // class Paper

} // namespace game

#endif // Paper_H_
