#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"

#include "path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.001;
float camera_far_clip_distance_g = 10000.0;
float camera_fov_g = 20.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0, 0, 0);
glm::vec3 camera_position_g(-362.7216, 64.4653, -777.982);
glm::vec3 camera_look_at_g(-400.7216, 0.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

std::string phase_name = "Title"; // string used to find the right phase. defaults to title 
game::SceneNode* phase; //default pointer to phase
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
    glfwSetCursorPos(window_, 400, 300);

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

    // Make the window's context the current one
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
    glfwSetCursorPosCallback(window_, CursorCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    scene_.SetupDrawToTexture();
    std::cout << "a" << ::std::endl;
    // Create geometry of the objects
    resman_.CreateCylinder("Square",1.0, 0.5, 4, 4);
    std::cout << "b" << ::std::endl;
    resman_.CreateCylinder("FlatSurface", 1.0, 0.5, 2, 2);
    std::cout << "c" << ::std::endl;
    resman_.CreateTorus("SimpleTorusMesh", 0.8, 0.35, 30, 30);
	resman_.CreateSeamlessTorus("SeamlessTorusMesh", 0.8, 0.35, 80, 80);
	resman_.CreateCylinder("SimpleCylinderMesh", 2.0, 0.4, 30, 30);
    resman_.CreateSphere("SimpleSphere");
    std::cout << "d" << ::std::endl;
    //MATERIALS
    // Load shader for texture mapping
	std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/textured_material");
	resman_.LoadResource(Material, "TextureShader", filename.c_str());
    std::cout << "e" << ::std::endl;
	// shader for corona effect
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/corona");
	resman_.LoadResource(Material, "Procedural", filename.c_str());
    std::cout << "f" << ::std::endl;
	// shader for checkerboard effect
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/rectangle");
	resman_.LoadResource(Material, "Blocks", filename.c_str());
    std::cout << "g" << ::std::endl;
    //shader for checkerboard
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/procedural");
    resman_.LoadResource(Material, "prod", filename.c_str());
    std::cout << "h" << ::std::endl;
    //new shader
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/new_shd");
    resman_.LoadResource(Material, "Nova", filename.c_str());
    //shaders for the dripping effect
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/dripping");
    resman_.LoadResource(Material, "DripMaterial", filename.c_str());
    //particles
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/duststorm");
    resman_.LoadResource(Material, "DustMaterial", filename.c_str());
    std::cout << "i" << ::std::endl;
	// shader for 3-term lighting effect
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/lit");
	resman_.LoadResource(Material, "Lighting", filename.c_str());
    //custom effect for project 
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/screen");
    resman_.LoadResource(Material, "Screen", filename.c_str());
    //Lore materials
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Title.png");
    resman_.LoadResource(Texture, "Title", filename.c_str());
    //I don't know why but literally only title is working fml 
   filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Intro.png");
    resman_.LoadResource(Texture, "Intro", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Welcome message.png");
    resman_.LoadResource(Texture, "Welcome", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/letter home.png");
    resman_.LoadResource(Texture, "Letter", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/peter's message.png");
    resman_.LoadResource(Texture, "Peter", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/results.png");
    resman_.LoadResource(Texture, "Results", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Diary.png");
    resman_.LoadResource(Texture, "Diary", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Last.png");
    resman_.LoadResource(Texture, "Last", filename.c_str());
    // Doesn't work yet so I don't think we can use screen effects, Unless we make a weirdass shader module
	// Load texture to be used on the object
    //TEXTURES
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/rocky.png");
	resman_.LoadResource(Texture, "RockyTexture", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/Day.jpg");
    resman_.LoadResource(Texture, "CubeTest", filename.c_str());
	// Load texture to be used on the object
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/download.jpg");
	resman_.LoadResource(Texture, "WoodTexture", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/water.jpeg");
    resman_.LoadResource(Texture, "Water", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/grasslands.jpeg");
    resman_.LoadResource(Texture, "Vine", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/flesh.jpg");
    resman_.LoadResource(Texture, "Flesh", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/stainless.png");
    resman_.LoadResource(Texture, "Steel", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/Yellow cement.jpg");
    resman_.LoadResource(Texture, "YSteel", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/cement.jpg");
    resman_.LoadResource(Texture, "Concrete", filename.c_str());
 
    std::cout << "j" << ::std::endl;

    //World objects 
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/Factory_main.obj");//texture steel
    resman_.LoadResource(Mesh, "Factory", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/grasslands.obj");//texture grasslands
    resman_.LoadResource(Mesh, "Field", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/parking terrain.obj");//texture Yellow steel
    resman_.LoadResource(Mesh, "Parking", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/Factory_int.obj");//texture CEMENT
    resman_.LoadResource(Mesh, "Fact_int_1", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/Factory_int_2.obj");//texture YELLOW CEMENT
    resman_.LoadResource(Mesh, "Fact_int_2", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/Reactor Detail.obj");//texture stainless
    resman_.LoadResource(Mesh, "React_detail", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/paper1.obj");//texture stainless
    resman_.LoadResource(Mesh, "paper", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/Boar.obj");//texture stainless
    resman_.LoadResource(Mesh, "Boar", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Meshes/MemoryCard.obj");//no texutre
    resman_.LoadResource(Mesh, "Key", filename.c_str());

    std::cout << "k" << ::std::endl;
    //particles
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/spark");
    resman_.LoadResource(Material, "SparkMaterial", filename.c_str());
    // Create particles for explosion
    resman_.CreateSphereDustParticles("SphereParticles", 1000);
    resman_.CreateSphereParticles("SphereParticles1", 25);
    resman_.CreateSparkParticles("SparkParticles", camera_.GetPosition());

    //SCREEN EFFECTS
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Radsuit");
    resman_.LoadResource(Material, "Radsuit", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/screen_space");
    resman_.LoadResource(Material, "ScreenSpaceMaterial", filename.c_str());
    std::cout << "l" << ::std::endl;
}


void Game::SetupScene(void){
    
    std::cout << "j" << ::std::endl;
    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);
    game::SceneNode* Skybox = CreateInstance("Skybox", "Square", "Lighting", "CubeTest");
    Skybox->SetPosition(glm::vec3(0, 0, 0));
    Skybox->Scale(glm::vec3(glm::vec3(9000,9000,9000)));
    std::cout << "j1" << ::std::endl;
    // Create an object for showing the texture
	// instance contains identifier, geometry, shader, and texture
    game::SceneNode* light = CreateInstance("Lightbulb", "SimpleSphere", "Lighting", "WoodTexture");
    light->Translate(glm::vec3(0, 10000, -10));
    light->Scale(glm::vec3(100, 100,100));
    scene_.Setlight(light);
    
    // Create particles
    //environmental effect, takes form of a massive sand/dust storm. Holds around 10000 particles and basically fills the screen. 
    game::SceneNode* particles = CreateInstance("ParticleInstance", "SphereParticles", "DustMaterial");
    

    // Create an instance of the torus mesh
    game::SceneNode* torus = CreateInstance("TorusInstance1", "SimpleTorusMesh", "Lighting");
    // Scale the instance
    torus->Scale(glm::vec3(1.5, 1.5, 1.5));

    initalizeMap();
   
  
     
  
}


void Game::MainLoop(void){

	float bleh = 0;
    std::cout << "m" << ::std::endl;
    phase = scene_.GetNode(phase_name);
    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_ ){
            
            static double last_time = 0;
            double current_time = glfwGetTime();

            if ((current_time - last_time) > 0.01 )
			{
                if (scene_.start )
                {
                    
                    phase->SetPosition(glm::vec3(camera_.GetPosition().x, camera_.GetPosition().y + 1, camera_.GetPosition().z + 15));
                    phase->projecting = true;

                }
                scene_.Update();
                
                for (int i = 0; i < items.size(); i++)
                {
                        std::stringstream ss;
                        ss << i;
                        std::string index = ss.str();
                        std::string name = "Page_" + index;
                        SceneNode *node = scene_.GetNode(name);
                        glm::quat rotation = glm::angleAxis(0.95f * glm::pi<float>() / 180.0f, glm::vec3(0.0, 0.0, 1.0));
                        node->Translate(glm::vec3(0, sin(current_time)/100, 0));
                        node->Rotate(rotation);
                        if (node->collided(camera_.GetPosition()))
                        {
                           
                            
                            /* 
                            * If A trigger has been pressed in camera, the code here should take one of the instances of the game screens and bring it in front of the player 
                            * depending on what is contained in the lore value in node, the screen loads up the right one */
                           
                        }
                        // if the items have all been aquired, writes a call to spawn a key item


                        
                }

                // if there is a collision with a node that protects the door, this function moves the player back and shows a screen. 
                // Animate the scene
                

                last_time = current_time;
				
            }
            
        }

        // Draw the scene
        //scene_.Draw(&camera_);
        //screen effect?
        scene_.DrawToTexture(&camera_);
        if (scene_.suited)
        {

            scene_.DisplayTexture(resman_.GetResource("Radsuit")->GetResource());

        }
        else
        {
            scene_.Draw(&camera_);
        }
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
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        game->controlCursor_ = !game->controlCursor_;
    }
    // Stop animation if space bar is pressed
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        game->animating_ = (game->animating_ == true) ? false : true;
    }
    SceneNode* halo = game->scene_.GetNode("ParticleInstance");
    SceneNode* skybox = game->scene_.GetNode("Skybox");
    // View control
    float rot_factor(glm::pi<float>() / 180);
    float trans_factor = 5.0;
    if (key == GLFW_KEY_UP){
        game->camera_.Pitch(rot_factor);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        std::cout << "whumpus" << std::endl;
        game->scene_.pause = !game->scene_.pause;
    }
   if (key == GLFW_KEY_DOWN) {
        game->camera_.Pitch(-rot_factor);
    }
    if (key == GLFW_KEY_LEFT){
        game->camera_.Yaw(rot_factor);
    }
    if (key == GLFW_KEY_RIGHT){
        game->camera_.Yaw(-rot_factor);
    }
    if (key == GLFW_KEY_Z) {
        game->camera_.Roll(-rot_factor);
    }
    if (key == GLFW_KEY_X) {
        game->camera_.Roll(rot_factor);
    }
    if (game->scene_.pause==false)
    { 
        if (key == GLFW_KEY_W) {
            game->camera_.Translate(game->camera_.GetForward() * trans_factor);
            halo->SetPosition(glm::vec3(game->camera_.GetPosition().x, game->camera_.GetPosition().y + 3, game->camera_.GetPosition().z));

        }
        if (key == GLFW_KEY_S) {
            game->camera_.Translate(-game->camera_.GetForward() * trans_factor);
            halo->SetPosition(glm::vec3(game->camera_.GetPosition().x, game->camera_.GetPosition().y + 3, game->camera_.GetPosition().z));

        }
        if (key == GLFW_KEY_A) {
            game->camera_.Translate(-game->camera_.GetSide() * trans_factor);
            halo->SetPosition(glm::vec3(game->camera_.GetPosition().x, game->camera_.GetPosition().y + 3, game->camera_.GetPosition().z));

        }
        if (key == GLFW_KEY_D) {
            game->camera_.Translate(game->camera_.GetSide() * trans_factor);
            halo->SetPosition(glm::vec3(game->camera_.GetPosition().x, game->camera_.GetPosition().y + 3, game->camera_.GetPosition().z));

        }
        // Do we need these controls?

        if (key == GLFW_KEY_R) {
            game->camera_.Translate(game->camera_.GetUp() * trans_factor);
            halo->SetPosition(glm::vec3(game->camera_.GetPosition().x, game->camera_.GetPosition().y + 3, game->camera_.GetPosition().z));

        }
        if (key == GLFW_KEY_E) {
            game->camera_.Translate(-game->camera_.GetUp() * trans_factor);
            halo->SetPosition(glm::vec3(game->camera_.GetPosition().x, game->camera_.GetPosition().y + 3, game->camera_.GetPosition().z));

        }
        if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
            game->controlCursor_ = !game->controlCursor_;
        }
        if (key == GLFW_KEY_M && action == GLFW_RELEASE) {
            std::stringstream ss;
            ss << "X coord :" << game->camera_.GetPosition().x << " Y coord :" << game->camera_.GetPosition().y << " Z coord :" << game->camera_.GetPosition().z;
            std::cout << ss.str() << std::endl;
        }
        if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
            game->scene_.suited = !game->scene_.suited;
            game->scene_.Savedtimer = glfwGetTime();

        }
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


Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Asteroid *ast = new Asteroid(entity_name, geom, mat);
    scene_.AddNode(ast);
    return ast;
}
void Game::CursorCallback(GLFWwindow* window, double xpos, double ypos) {
    void* ptr = glfwGetWindowUserPointer(window);
    Game* game = (Game*)ptr;
    if (game->controlCursor_) {
        game->camera_.Yaw((400 - xpos) / 1000);
        game->camera_.Pitch((300 - ypos) / 1000);
        glfwSetCursorPos(game->window_, 400, 300);
    }
}

void Game::CreateAsteroidField(int num_asteroids){

    // Create a number of asteroid instances
    for (int i = 0; i < num_asteroids; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;

        // Create asteroid instance
        Asteroid *ast = CreateAsteroidInstance(name, "SimpleSphereMesh", "ObjectMaterial");

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum
        ast->SetPosition(glm::vec3(-300.0 + 600.0*((float) rand() / RAND_MAX), -300.0 + 600.0*((float) rand() / RAND_MAX), 600.0*((float) rand() / RAND_MAX)));
        ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
        ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
    }
}


SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name){

    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = NULL;
    if (texture_name != ""){
        tex = resman_.GetResource(texture_name);
        if (!tex){
            throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
        }
    }

    SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, tex);
    return scn;
}

void Game::initalizeMap() {
    //inital map
    //OBJECTS LOADING IN
    game::SceneNode* factory = CreateInstance("Area1", "Factory", "Lighting", "Steel"); //creates the main factory building

    factory->Scale(glm::vec3(.5, .5, .3));
    factory->Translate(glm::vec3(0, -2, -20));
    game::SceneNode* land = CreateInstance("Area1", "Field", "Lighting", "Vine"); //creates the environment where the factory is located 
    land->Attach(factory, 0);
    game::SceneNode* parking = CreateInstance("parking", "Parking", "Lighting", "Vine");
    parking->Attach(factory, 0);
    game::SceneNode* factor_int = CreateInstance("interior_1", "Fact_int_1", "Lighting", "Concrete");
    factor_int->Attach(factory, 0);
    game::SceneNode* factor_int_2 = CreateInstance("interior_2", "Fact_int_2", "Lighting", "YSteel");
    factor_int_2->Attach(factory, 0);
   game::SceneNode* reactor = CreateInstance("ReactorDetail", "React_detail", "Lighting", "YSteel");
    reactor->Attach(factory, 0);
    game::SceneNode* page = CreateInstance("Page_0", "paper", "Lighting","Flesh");
    page->Attach(factor_int_2, 0);
    float placerng = rand() % rand() % (2 + 1) + (-25);
    float placerngz = rand() % (100 + 1) + (-100);
    page->Translate(glm::vec3(placerng, 10, placerngz));
    page->Scale(glm::vec3(.5, 1, 1));


    glm::quat rot = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
 
    //page->Translate(glm::vec3(0))
    for (int i = 0; i < 8; i++)
    {
        float placerngx = rand() % (1 + 1) + (-50);
        float placerngz = rand() % (1000 + 1) + (-1000);
        items.push_back(page);
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "Page_" + index;
        page = CreateInstance(name, "paper", "Lighting","Flesh");
        switch (i) {
        case 0:
            page->Translate(glm::vec3(473.632, 10, -78));
            page->lore = resman_.GetResource("Title")->GetName();
            break;
        case 1:
            page->Translate(glm::vec3(177.107, 10, -141.28));
            page->lore = resman_.GetResource("Intro")->GetName();
            break;
        case 2:
            page->Translate(glm::vec3(31.592, 10, -72.2487));
            page->lore = resman_.GetResource("Welcome")->GetName();
            break;
        case 3:
            page->Translate(glm::vec3(309.57, 10, -30.59));
            page->lore = resman_.GetResource("Letter")->GetName();
            break;

        case 4:
            page->Translate(glm::vec3(33.63, 10, 57.78));
            page->lore = resman_.GetResource("Peter")->GetName();
            break;
        case 5:
            page->Translate(glm::vec3(330.1, 10, 83.51));
            page->lore = resman_.GetResource("Results")->GetName();
            break;
        case 6:
            page->Translate(glm::vec3(141.23, 10, 135.13));
            page->lore = resman_.GetResource("Diary")->GetName();
            break;
        case 7:
            page->Translate(glm::vec3(162.4, 10, 5.98));
            page->lore = resman_.GetResource("Last")->GetName();
            break;
        }
      //  page->Attach(factor_int_2, 0);
    //    page->Translate(glm::vec3(placerngx, 10, placerngz));
        page->Scale(glm::vec3(.1, .1, .1));
        page->Rotate(rot);
        ss << "X of " << i << ": " << page->GetPosition().x << "Z of " << i << ": " << page->GetPosition().z;
        std::string test = ss.str();
        std::cout << test << ::std::endl;
    }

    game::SceneNode* boar = CreateInstance("boar", "Boar", "Lighting", "Flesh");
    boar->Translate(glm::vec3(6, 5, 0));
    boar->Scale(glm::vec3(10, 10, 10));
    boar->Attach(factory, 0);
    // PARTICLES LOAD IN 
    //project particles, to be used in project, it is recommended that you comment out  'particles' to get a better view but they represent dripping water.
    for (int i = 0; i < 8; i++)
    {
        float placerngx = rand() % (25 + 1) + (-500);
        float placerngz = rand() % (1000 + 1) + (-1000);
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "Drip_" + index;
        game::SceneNode* particles_1 = CreateInstance(name, "SphereParticles1", "DripMaterial", "Water");
        particles_1->Attach(factory, 0);
        particles_1->Translate(glm::vec3(i+50, 500, i));
        particles_1->Scale(glm::vec3(5, 5, 5));
    }
    for (int i = 0; i < 8; i++)
    {
        float placerngx = rand() % (25 + 1) + (-500);
        float placerngz = rand() % (1000 + 1) + (-1000);
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "spark_" + index;
        game::SceneNode* particles_3 = CreateInstance(name, "SparkParticles", "SparkMaterial", "Water");
        particles_3->Attach(factor_int_2, 0);
        particles_3->Translate(glm::vec3(placerngx, 10, placerngz));
        particles_3->Scale(glm::vec3(5, 5, 5));
    }
    //particles to represent sparks for electricity or grinding metal

    // Creating the Phase screens, used to exposite onto the player+ act as our start and end screens. will put them under the game world so that they can come when they are needed 
    //Start screen
    game::SceneNode* title = CreateInstance("Title", "FlatSurface", "TextureShader", "Title");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //Intro screen
    title = CreateInstance("Intro", "FlatSurface", "TextureShader", "Intro");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //expo 1 screen
    title = CreateInstance("Welcome", "FlatSurface", "TextureShader", "Welcome");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //expo 2 screen
    title = CreateInstance("Letter", "FlatSurface", "TextureShader", "Letter");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //expo 3 screen
    title = CreateInstance("Peter", "FlatSurface", "TextureShader", "Peter");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //expo 4 screen
    title = CreateInstance("Results", "FlatSurface", "TextureShader", "Results");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //expo 5 screen
    title = CreateInstance("Diary", "FlatSurface", "TextureShader", "Diary");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //expo 6 screen
    title = CreateInstance("Last", "FlatSurface", "TextureShader", "Last");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    //end screen
 //   game::SceneNode* title = CreateInstance("Title", "FlatSurface", "TextureShader", "Title");
//    title->Translate(glm::vec3(0, -100, 0));
 //   title->SetScale(glm::vec3(7, 10, 10));

}
void Game::showToScreen(SceneNode* phase) {
    phase->Translate(glm::vec3(camera_.GetPosition().x, camera_.GetPosition().y + 1, camera_.GetPosition().z + 15));
    phase->projecting = true;
    
}

} // namespace game
