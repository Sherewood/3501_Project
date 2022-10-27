#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "scene_node.h"

namespace game {

    SceneNode::SceneNode(const std::string name, const Resource* geometry, const Resource* material) {

        // Set name of scene node
        name_ = name;
        

        // Set geometry
        if (geometry->GetType() == PointSet) {
            mode_ = GL_POINTS;
        }
        else if (geometry->GetType() == Mesh) {
            mode_ = GL_TRIANGLES;
        }
        else {
            throw(std::invalid_argument(std::string("Invalid type of geometry")));
        }

        array_buffer_ = geometry->GetArrayBuffer();
        element_array_buffer_ = geometry->GetElementArrayBuffer();
        size_ = geometry->GetSize();

        // Set material (shader program)
        if (material->GetType() != Material) {
            throw(std::invalid_argument(std::string("Invalid type of material")));
        }

        material_ = material->GetResource();

        // Other attributes
        scale_ = glm::vec3(1.0, 1.0, 1.0);
    }


    SceneNode::~SceneNode() {
    }


    const std::string SceneNode::GetName(void) const {

        return name_;
    }


    glm::vec3 SceneNode::GetPosition(void) const {

        return position_;
    }

    glm::vec3 SceneNode::GetJoint(void) const {
        return joint;
    }
    glm::quat SceneNode::GetOrientation(void) const {

        return orientation_;
    }


    glm::vec3 SceneNode::GetScale(void) const {

        return scale_;
    }


    void SceneNode::SetPosition(glm::vec3 position) {

        position_ = position;
    }


    void SceneNode::SetOrientation(glm::quat orientation) {

        orientation_ = orientation;
    }


    void SceneNode::SetScale(glm::vec3 scale) {

        scale_ = scale;
    }


    void SceneNode::Translate(glm::vec3 trans) {

        position_ += trans;
    }


    void SceneNode::Rotate(glm::quat rot) {

        orientation_ *= rot;
        orientation_ = glm::normalize(orientation_);
    }


    void SceneNode::Scale(glm::vec3 scale) {

        scale_ *= scale;
    }


    GLenum SceneNode::GetMode(void) const {

        return mode_;
    }


    GLuint SceneNode::GetArrayBuffer(void) const {

        return array_buffer_;
    }


    GLuint SceneNode::GetElementArrayBuffer(void) const {

        return element_array_buffer_;
    }


    GLsizei SceneNode::GetSize(void) const {

        return size_;
    }


    GLuint SceneNode::GetMaterial(void) const {

        return material_;
    }
    void SceneNode::SetJoint(glm::vec3 Newjoint) { joint = Newjoint; }

    void SceneNode::Draw(Camera* camera) {

        // Select proper material (shader program)
        glUseProgram(material_);

        // Set geometry to draw
        glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

        // Set globals for camera
        camera->SetupShader(material_);

        // Set world matrix and other shader input variables
        SetupShader(material_);
        
        // Draw geometry
        if (mode_ == GL_POINTS) {
            glDrawArrays(mode_, 0, size_);
        }
        else {
            glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
        }

    //tree searching functionality
        if (children.size() > 0)
        {
            for (int i = 0; i < children.size(); i++)
            {
                children[i]->parent = world_transformation; 
                children[i]->Draw(camera);
            }
        }
    }


    void SceneNode::Update(void) {

        // Do nothing for this generic type of scene node
    }
    void SceneNode::Add(SceneNode* Base, SceneNode* addon) //Adds a node to the tree 
    {
        //THIS IS MADNESS!!! YOU CAN"T HARDCODE AN ENTIRE TREE!
        //hehe, if statements go brrrr
        //In all seriousness, the add function is hardcoded to fit the model exactly as it is done. 
        //The function gets the name of the model, cuts off its identification, then places it in a predesignated spot 
        if (addon->GetName().substr(0, addon->GetName().size() - 1) == "Stump")
        {

            if (addon->GetName() == "Stump0")
            {

                Base->children.push_back(addon);


            }
            else if (addon->GetName() == "Stump1")
            {

                Base->children[0]->children.push_back(addon);
            }

            else if (addon->GetName() == "Stump2")
            {
                addon->parent = Base->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Stump3")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Stump4")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
        }

    
        else if (addon->GetName().substr(0, addon->GetName().size() - 1) == "Trunk")
        {
            if (addon->GetName() == "Trunk0")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Trunk1") 
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Trunk2")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Trunk3")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Trunk4")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
        }

        else if (addon->GetName().substr(0, addon->GetName().size()-1) == "Branch")
        {
            if (addon->GetName() == "Branch0")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch1")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch2")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch3")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch4")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch5")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch6")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch6")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[1]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[1]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch7")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[2]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[2]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch8")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[3]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[3]->children.push_back(addon);
            }
            else if (addon->GetName() == "Branch9")
            {
                addon->parent = Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[3]->world_transformation;
                Base->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[0]->children[3]->children.push_back(addon);
            }
         }
         
}

void SceneNode::SetupShader(GLuint program){

    // Set attributes for shaders
    GLint vertex_att = glGetAttribLocation(program, "vertex");
    glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vertex_att);

    GLint normal_att = glGetAttribLocation(program, "normal");
    glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(normal_att);

    GLint color_att = glGetAttribLocation(program, "color");
    glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (6*sizeof(GLfloat)));
    glEnableVertexAttribArray(color_att);

    GLint tex_att = glGetAttribLocation(program, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (9*sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_att);

    // World transformation
    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
    glm::mat4 rotation = glm::mat4_cast(orientation_);
    glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);
    float angle = sin(glfwGetTime())+cos(2*glfwGetTime()); //angle factor that increases over time
    glm::quat myquat = glm::angleAxis(6*angle * glm::pi<float>() / 180.0f, glm::normalize(glm::vec3(0,0,5)));
    glm::mat4 orbit = (glm::translate(glm::mat4(1.0), -joint) * glm::mat4_cast(myquat) * glm::translate(glm::mat4(1.0), joint)); //orbit equation 
    glm::mat4 transf = translation *orbit*rotation * scaling;
    transf =  parent* transf; 
    world_transformation = transf;
    

    GLint world_mat = glGetUniformLocation(program, "world_mat");
    glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

    // Timer
    GLint timer_var = glGetUniformLocation(program, "timer");
    double current_time = glfwGetTime();
    glUniform1f(timer_var, (float) current_time); 

}

} // namespace game;
