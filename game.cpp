#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Tree Field";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 60.0; // Field-of-view of camera (degrees)
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_g(0.0, -200.0, 900.0);
glm::vec3 camera_look_at_g(0.0, 0.0, -1);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

float  num_stumps = 5;
float num_trunks = 5;
float num_branches = 10;
//innner counter for Bullets 


// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;


Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current 
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){

    // Set up z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    // Create a simple sphere to represent the Trees
    resman_.CreateTorus("SimpleTorus", 2.0, 1.0, 10, 10);
    resman_.CreateCylinder("Stump", 30, 40, 90, 45);
    resman_.CreateCylinder("Trunk", 10, 10, 90, 45);
    resman_.CreateCylinder("Branch", 30, 5, 90, 45);



    // Load material to be applied to Trees
    std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
    resman_.LoadResource(Material, "ObjectMaterial", filename.c_str());
}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create a Tree
    PaintATree();

	

}


void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_){
            static double last_time = 0;
            double current_time = glfwGetTime();
            if ((current_time - last_time) > 0.05){
                scene_.Update();
                last_time = current_time;
            }
        }
     // test print pls ignore   std::cout << "easdasd" << std::endl;
        //start of Bullet updating
        // Draw the scene
        scene_.Draw(&camera_);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    // Get user data with a pointer to the game class
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;

    // Quit game if 'q' is pressed
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    // Stop animation if space bar is pressed
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        game->animating_ = (game->animating_ == true) ? false : true;
    }

    // View control
    float rot_factor(glm::pi<float>() / 180);
    float trans_factor = 1.0;
    if (key == GLFW_KEY_UP){
        game->camera_.Pitch(rot_factor);
    }
    if (key == GLFW_KEY_DOWN){
        game->camera_.Pitch(-rot_factor);
    }
    if (key == GLFW_KEY_LEFT){
        game->camera_.Yaw(rot_factor);
    }
    if (key == GLFW_KEY_RIGHT){
        game->camera_.Yaw(-rot_factor);
    }
    if (key == GLFW_KEY_S){
        game->camera_.Roll(-rot_factor);
    }
    if (key == GLFW_KEY_X){
        game->camera_.Roll(rot_factor);
    }
    if (key == GLFW_KEY_A){
        game->camera_.Translate(game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_Z){
        game->camera_.Translate(-game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_J){
        game->camera_.Translate(-game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_L){
        game->camera_.Translate(game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_I){
        game->camera_.Translate(game->camera_.GetUp()*trans_factor);
    }
    if (key == GLFW_KEY_K){
        game->camera_.Translate(-game->camera_.GetUp()*trans_factor);
    }
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height){

    // Set up viewport and camera projection based on new window size
    glViewport(0, 0, width, height);
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
    game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


Game::~Game(){
    
    glfwTerminate();
}

void Game::PaintATree() {
    // Get resources
   Resource* geom = resman_.GetResource("Stump");
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + "Stump" + std::string("\"")));
    }
    Resource* geom_tr = resman_.GetResource("Trunk");
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + "Trunk" + std::string("\"")));
    }
    Resource* geom_br = resman_.GetResource("Branch");
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + "Stump" + std::string("\"")));
    }
    Resource* mat = resman_.GetResource("ObjectMaterial");
    if (!mat) {
        throw(GameException(std::string("Could not find resource \"") + "ObjectMaterial" + std::string("\"")));
    }
    //Creates the root node and gives it a position
     SceneNode *Base= new SceneNode("Root",geom,mat);
     Base->SetPosition(glm::vec3(0,-100,0));
     Base->SetScale(glm::vec3(1, 1, 1));
     Base->SetJoint(glm::vec3(0, 0, 0));
     
   for (int i = 0; i<num_stumps; i++)
    {
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "Stump" + index;
        SceneNode *create= new SceneNode(name,geom,mat);
        create->SetPosition(glm::vec3(0,30,0));
        create->SetScale(glm::vec3(1, 1, 1));
        create->SetJoint(glm::vec3(0, 30, 0));
        Base->Add(Base,create);
    }
        //should create several stump objects, give them position, scale,and a joint and then add them to the SceneNode

    for (int j = 0; j<num_trunks; j++)
    {
        std::stringstream ss;
        ss << j;
        std::string index = ss.str();
        std::string name = "Trunk" + index;
         SceneNode *trunk=new SceneNode(name,geom,mat);
        trunk->SetPosition(glm::vec3(0,30, 0));
        trunk->SetJoint(glm::vec3(0, 30, 0));
        trunk->SetScale(glm::vec3(1,1,1));
        Base->Add(Base,trunk);
    }
        //should create several trunk objects, give them the same stuff as what is above, and add them to the SceneNode
   for (int z = 0; z<num_branches; z++)
    {
        std::stringstream ss;
        ss << z;
        std::string index = ss.str();
        std::string name = "Branch" + index;
        SceneNode* branch = new SceneNode(name, geom_br, mat);
        if (z<4){ 
            branch->SetPosition(glm::vec3(-30+(20*z), 30, 0));
            branch->SetJoint(glm::vec3(-30 + (20 * z), 30, 0));

        }
        else {
            if (z == 4)
            {
                branch->SetPosition(glm::vec3(-30, 30, 0));
               // float angle = 20*(z);
                branch->SetJoint(glm::vec3(-30, 30, 0));
               // glm::quat myquat = glm::angleAxis(angle, glm::normalize(glm::vec3(5,5,0)));
               // branch->SetOrientation(myquat);
            }
            else if (z == 5)
            {
                branch->SetPosition(glm::vec3(-10, 30, 0));
                branch->SetJoint(glm::vec3(0, 30, 0));

            }
            else if (z == 6)
            {
                branch->SetPosition(glm::vec3(0, 30, 0));
                branch->SetJoint(glm::vec3(0,30, 0));
               // float angle = 20 * (1 + z);
                //branch->SetJoint(glm::vec3(-30, 300, -20));
                //glm::quat myquat = glm::angleAxis(angle, glm::normalize(glm::vec3(0, -5, 0)));
                //branch->SetOrientation(myquat);
            }
            else if (z == 7)
            {
                branch->SetPosition(glm::vec3(0,30, 0));
                branch->SetJoint(glm::vec3(0, 30, 0));
                //float angle = 20 * (1 + z);
                //branch->SetJoint(glm::vec3(-30, 300, -20));
                //glm::quat myquat = glm::angleAxis(angle, glm::normalize(glm::vec3(0, -5, 0)));
                //branch->SetOrientation(myquat);
            }
            else if (z == 8)
            {
                branch->SetPosition(glm::vec3(0, 30, 0));
                branch->SetJoint(glm::vec3(0, 30, 0));
                //float angle = 20 * (1 + z);
                //branch->SetJoint(glm::vec3(-30, 300, -20));
                //glm::quat myquat = glm::angleAxis(angle, glm::normalize(glm::vec3(0, -5, 0)));
                ///branch->SetOrientation(myquat);
            }
            else if (z == 9)
            {
                branch->SetPosition(glm::vec3(10, 30, 0));
                branch->SetJoint(glm::vec3(10, 30, 0));
                //float angle = 20 * (1 + z);
                //branch->SetJoint(glm::vec3(-30, 150, -20));
                //glm::quat myquat = glm::angleAxis(angle, glm::normalize(glm::vec3(0, 5, 0)));
                //branch->SetOrientation(myquat);
               
            }
           
        }
        
        branch->SetScale(glm::vec3(1, 1.5, 1));
        Base->Add(Base, branch);
    }///should create several branch objects which should be placed near to the tree to look as if it is connected
    scene_.AddNode(Base);
}

} // namespace game
