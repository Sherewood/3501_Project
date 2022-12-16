#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <vector>
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "camera.h"
#include "Light.h"

namespace game {

    // Class that manages one object in a scene 
    class SceneNode {

        public:
            // Create scene node from given resources
            SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);

            // Destructor
            ~SceneNode();
            
            // Get name of node
            const std::string GetName(void) const;

            // Get node attributes
            glm::vec3 GetPosition(void) const;
            glm::quat GetOrientation(void) const;
            glm::vec3 GetScale(void) const;

            // Set node attributes
            void SetPosition(glm::vec3 position);
            void SetOrientation(glm::quat orientation);
            void SetScale(glm::vec3 scale);
            
            // Perform transformations on node
            void Translate(glm::vec3 trans);
            void Rotate(glm::quat rot);
            void Scale(glm::vec3 scale);

            // Draw the node according to scene parameters in 'camera'
            // variable
            virtual void Draw(Camera *camera, SceneNode *l);

            // Update the node
            virtual void Update(void);

            // OpenGL variables
            GLenum GetMode(void) const;
            GLuint GetArrayBuffer(void) const;
            GLuint GetElementArrayBuffer(void) const;
            GLsizei GetSize(void) const;
            GLuint GetMaterial(void) const;

            //attachment 
            void Attach(SceneNode* parent, float level);

            //Collision function USE THIS FOR COLLISION
            bool collided(glm::vec3); // function for collision. May need extra details 
            // phases booleans
            std::string lore = "";
            bool projecting = false;
            bool have=false; //boolean to be used whenever a node object is "picked up" by the player
            //radius
            float radius = 1000.0f;

        private:
            std::string name_; // Name of the scene node
            GLuint array_buffer_; // References to geometry: vertex and array buffers
            GLuint element_array_buffer_;
            GLenum mode_; // Type of geometry
            GLsizei size_; // Number of primitives in geometry
            GLuint material_; // Reference to shader program
            GLuint texture_; // Reference to texture resource
            glm::vec3 position_; // Position of node
            glm::quat orientation_; // Orientation of node
            glm::vec3 scale_; // Scale of node
            std::vector<SceneNode*> children;
            glm::mat4 parent; //reference to Parent transformation of the node
            glm::vec3 joint; //position where the node rotates on 
            glm::mat4 world_transformation; // value which saves the world_transformation to be used later 

            
            


            // Set matrices that transform the node in a shader program
            void SetupShader(GLuint program,SceneNode *l);

    }; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
