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
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/End.png");
    resman_.LoadResource(Texture, "End", filename.c_str());
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
    std::cout << "j" << ::std::endl;
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
    Skybox->Scale(glm::vec3(glm::vec3(9999,9999,9999)));
    
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
   
    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){

        // Animate the scene
        if (animating_ ){
            for (int i = 0; i < phases.size(); i++)
            {
                if (!display || phases[i]->projecting == true)
                {
                    phases[i]->SetPosition(glm::vec3(0, -100, 0));
                    phases[i]->projecting = false;
                }
            }
            for (int i = 0; i < items.size(); i++)
            {
                if (items[i]->lore==phase_name)
                {
                    items[i]->SetPosition(glm::vec3(0, -100, 0));
                }
            }
            static double last_time = 0;
            double current_time = glfwGetTime();
            phase = scene_.GetNode(phase_name);

            if ((current_time - last_time) > 0.01 )
			{
                scene_.Update();
                bool completion = true;
                if (display )
                {
                    showToScreen(phase);


                }

                for (int i = 0; i < items.size(); i++)
                {
                    glm::quat rotation = glm::angleAxis(0.95f * glm::pi<float>() / 180.0f, glm::vec3(0.0, 0.0, 1.0));
                    items[i]->Translate(glm::vec3(0, sin(current_time) / 100, 0));
                    items[i]->Rotate(rotation);
                    if (items[i]->collided(camera_.GetPosition()))
                    {
                        std::cout << "abba" << ::std::endl;
                        phase_name = items[i]->lore;
                        items[i]->have = true;
                    }
                        
                }

                // if there is a collision with a node that protects the door, this function moves the player back and shows a screen. 
                // Animate the scene
                if (scene_.GetNode("boar")->collided(camera_.GetPosition()))
                {
                    glfwSetWindowShouldClose(window_, true);
                    //end game
                }
                

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
       
        game->scene_.pause = !game->scene_.pause;
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {

        if (game->scene_.start)
        {
            game->scene_.GetNode("Title")->projecting = false;
            game->scene_.GetNode("Title")->SetPosition(glm::vec3(0, -100, 0));
            phase_name = "Intro";
            game->scene_.start = false;
         }
        else
        {
            game->display = !game->display;
        }
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
    factory->SetPosition(glm::vec3(-2000, 0, 0));
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

    glm::quat rot = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
 
    //page->Translate(glm::vec3(0))
    for (int i = 0; i < 8; i++)
    {
        float placerngx = rand() % (1 + 1) + (-50);
        float placerngz = rand() % (1000 + 1) + (-1000);
        
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "Page_" + index;
        game::SceneNode* page = CreateInstance(name, "paper", "Lighting","Flesh");
        items.push_back(page);
        
        switch (i) {
        case 0:
            page->Translate(glm::vec3(615.329, 10, -735.46));
            page->lore = resman_.GetResource("Title")->GetName();
            break;
        case 1:
            page->Translate(glm::vec3(605.7, 10, -655.79));
            page->lore = resman_.GetResource("Intro")->GetName();
            break;
        case 2:
            page->Translate(glm::vec3(336.04, 10, -677.69));
            page->lore = resman_.GetResource("Welcome")->GetName();
            break;
        case 3:
            page->Translate(glm::vec3(573.65, 10, -505.441));
            page->lore = resman_.GetResource("Letter")->GetName();
            break;

        case 4:
            page->Translate(glm::vec3(341.4, 10, -810.704));
            page->lore = resman_.GetResource("Peter")->GetName();
            break;
        case 5:
            page->Translate(glm::vec3(537.02, 10, -858.78));
            page->lore = resman_.GetResource("Results")->GetName();
            break;
        case 6:
            page->Translate(glm::vec3(725.504, 10, -861.7));
            page->lore = resman_.GetResource("Diary")->GetName();
            break;
        case 7:
            page->Translate(glm::vec3(645.593, 10, -716.61));
            page->lore = resman_.GetResource("Last")->GetName();
            break;
        }
       
        page->Attach(factor_int_2, 0);
    //    page->Translate(glm::vec3(placerngx, 10, placerngz));
        page->Scale(glm::vec3(100, 100, 100));
        page->Rotate(rot);
        ss << "X of " << i << ": " << page->GetPosition().x << "Z of " << i << ": " << page->GetPosition().z;
        std::string test = ss.str();
        std::cout << test << ::std::endl;
    }
    //ONE TIME ITEMS. Theses are one time items to be made 
    game::SceneNode* boar = CreateInstance("boar", "Boar", "Lighting", "Flesh");
    boar->SetPosition(glm::vec3(229.46, 5, -748.369));
    boar->Scale(glm::vec3(10, 10, 10));
    game::SceneNode* key = CreateInstance("key", "Key", "Lighting");
    key->SetPosition(glm::vec3(484.36, 20, -748.369));
    key->Scale(glm::vec3(5, 5, 5));
    key->Rotate(rot);
    
    // PARTICLES LOAD IN 
    //project particles, to be used in project, it is recommended that you comment out  'particles' to get a better view but they represent dripping water.

    game::SceneNode* particles_1 = CreateInstance("Drip_1", "SphereParticles1", "DripMaterial", "Water");
        particles_1->SetPosition(glm::vec3(423.27, 175.171, -702.455));
        particles_1->Scale(glm::vec3(5, 5, 5));
    game::SceneNode* particles_2= CreateInstance("Drip_2", "SphereParticles1", "DripMaterial", "Water");
    particles_2->SetPosition(glm::vec3(598.8, 71.058, -749.82));
    particles_2->Scale(glm::vec3(5, 5, 5));
    game::SceneNode* particles_3 = CreateInstance("Drip_3", "SphereParticles1", "DripMaterial", "Water");
    particles_3->SetPosition(glm::vec3(568.303, 29.4, -819.64));
    particles_3->Scale(glm::vec3(5, 5, 5));
    game::SceneNode* particles_4 = CreateInstance("Sparks", "SparkParticles", "SparkMaterial");
    particles_4->SetPosition(glm::vec3(761.572,17.756,-881.256));
    particles_4->Scale(glm::vec3(5, 5, 5));
    //particles to represent sparks for electricity or grinding metal

    // Creating the Phase screens, used to exposite onto the player+ act as our start and end screens. will put them under the game world so that they can come when they are needed 
    //Start screen
    game::SceneNode* title = CreateInstance("Title", "FlatSurface", "TextureShader", "Title");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //Intro screen
    title = CreateInstance("Intro", "FlatSurface", "TextureShader", "Intro");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //expo 1 screen
    title = CreateInstance("Welcome", "FlatSurface", "TextureShader", "Welcome");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //expo 2 screen
    title = CreateInstance("Letter", "FlatSurface", "TextureShader", "Letter");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //expo 3 screen
    title = CreateInstance("Peter", "FlatSurface", "TextureShader", "Peter");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //expo 4 screen
    title = CreateInstance("Results", "FlatSurface", "TextureShader", "Results");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //expo 5 screen
    title = CreateInstance("Diary", "FlatSurface", "TextureShader", "Diary");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //expo 6 screen
    title = CreateInstance("Last", "FlatSurface", "TextureShader", "Last");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);
    //end screen
    title = CreateInstance("Title", "FlatSurface", "TextureShader", "End");
    title->Translate(glm::vec3(0, -100, 0));
    title->SetScale(glm::vec3(7, 10, 10));
    phases.push_back(title);

}
void Game::showToScreen(SceneNode* phase) {

    phase->SetPosition(glm::vec3(camera_.GetPosition().x+(camera_.GetForward().x*15), camera_.GetPosition().y + 1, camera_.GetPosition().z + (camera_.GetForward().z * 15)));
    phase->SetOrientation(camera_.GetOrientation());
    phase->projecting = true;
    
}

} // namespace game
