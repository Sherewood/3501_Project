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
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 20.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0, 255, 255);
glm::vec3 camera_position_g(0., 20.0, -1000.0);
glm::vec3 camera_look_at_g(0.0, 0.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

//world state booleans
bool dooropen=false;
bool start=true;
bool end=false;
bool game=false;
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

    // Create geometry of the objects
	resman_.CreateTorus("SimpleTorusMesh", 0.8, 0.35, 30, 30);
	resman_.CreateSeamlessTorus("SeamlessTorusMesh", 0.8, 0.35, 80, 80);
	resman_.CreateWall("FlatSurface");
	resman_.CreateCylinder("SimpleCylinderMesh", 2.0, 0.4, 30, 30);
    resman_.CreateSphere("SimpleSphere");
    //MATERIALS
    // Load shader for texture mapping
	std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/textured_material");
	resman_.LoadResource(Material, "TextureShader", filename.c_str());

	// shader for corona effect
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/corona");
	resman_.LoadResource(Material, "Procedural", filename.c_str());

	// shader for checkerboard effect
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/rectangle");
	resman_.LoadResource(Material, "Blocks", filename.c_str());

    //shader for checkerboard
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/procedural");
    resman_.LoadResource(Material, "prod", filename.c_str());

    //new shader
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/new_shd");
    resman_.LoadResource(Material, "Nova", filename.c_str());
    //shaders for the dripping effect
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/dripping");
    resman_.LoadResource(Material, "DripMaterial", filename.c_str());
    //particles
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/duststorm");
    resman_.LoadResource(Material, "DustMaterial", filename.c_str());

	// shader for 3-term lighting effect
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/lit");
	resman_.LoadResource(Material, "Lighting", filename.c_str());
    //Lore materials
  /*  filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Title.png");
    resman_.LoadResource(Material, "Title_1", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Intro Lore.png");
    resman_.LoadResource(Material, "Intro_2", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Welcome message.png");
    resman_.LoadResource(Material, "Welcome_3", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/letter home.png");
    resman_.LoadResource(Material, "letter_4", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/peter's message.png");
    resman_.LoadResource(Material, "peter_5", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/results.png");
    resman_.LoadResource(Material, "results_6", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Diary.png");
    resman_.LoadResource(Material, "Diary_7", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/LORE/Last.png");
    resman_.LoadResource(Material, "Last_8", filename.c_str());
    * Doesn't work yet so I don't think we can use screen effects, Unless we make a weirdass shader module*/
	// Load texture to be used on the object
    //TEXTURES
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/rocky.png");
	resman_.LoadResource(Texture, "RockyTexture", filename.c_str());
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


    //particles
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/spark");
    resman_.LoadResource(Material, "SparkMaterial", filename.c_str());
    // Create particles for explosion
    resman_.CreateSphereDustParticles("SphereParticles", 100);
    resman_.CreateSphereParticles("SphereParticles1", 25);
    resman_.CreateSparkParticles("SparkParticles", camera_.GetPosition());

}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create an object for showing the texture
	// instance contains identifier, geometry, shader, and texture
    game::SceneNode* light = CreateInstance("Lightbulb", "SimpleSphere", "Lighting", "WoodTexture");
    light->Translate(glm::vec3(0, 10000, -10));
    light->Scale(glm::vec3(100, 100,100));
    scene_.Setlight(light);
//    game::SceneNode* ground = CreateInstance("Wall", "FlatSurface", "Lighting", "Grass");
//    glm::quat rot = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
//    ground->Translate(glm::vec3(0, -5, 0));
//    ground->Scale(glm::vec3(500, 500, 500));
//     ground->Rotate(rot);

    //facto->Scale(glm::vec3(10, 10,10));
    //facto->Translate(glm::vec3(0, -2, -20));
    

    // Create particles
    //environmental effect, takes form of a massive sand/dust storm. Holds around 10000 particles and basically fills the screen. 
    game::SceneNode* particles = CreateInstance("ParticleInstance", "SphereParticles", "DustMaterial");
    //project particles, to be used in project, it is recommended that you comment out  'particles' to get a better view but they represent dripping water.
    game::SceneNode* particles_1 = CreateInstance("ParticleInstance_Project1", "SphereParticles1", "DripMaterial", "Water");
    particles_1->SetPosition(glm::vec3(camera_.GetPosition().x - 1, camera_.GetPosition().y, 0));
    game::SceneNode* particles_2 = CreateInstance("ParticleInstance_Project2", "SphereParticles1", "DripMaterial", "Water");
    particles_2->SetPosition(glm::vec3(camera_.GetPosition().x + 1, camera_.GetPosition().y, 0));
    //particles to represent sparks for electricity or grinding metal
    game::SceneNode* particles_3 = CreateInstance("ParticleInstance_3", "SparkParticles", "SparkMaterial", "Water");
    initalizeMap();
    /*	game::SceneNode* mytorus = CreateInstance("MyTorus1", "SeamlessTorusMesh", "Lighting", "RockyTexture");
  
    
    game::SceneNode* newShader = CreateInstance("Object", "SeamlessTorusMesh", "Nova", "Blocks");
	mytorus->Translate(glm::vec3(-5, 0, -10));
    canvas->Translate(glm::vec3(0, 0, -10));
    newShader->Translate(glm::vec3(5, 0, -10));
    
    */
		
}


void Game::MainLoop(void){
	float bleh = 0;
    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_){
            static double last_time = 0;
            double current_time = glfwGetTime();
            if ((current_time - last_time) > 0.01)
			{
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
                }
                // Animate the scene
          //      SceneNode *node = scene_.GetNode("MyTorus1");
	//			glm::quat rotation = glm::angleAxis(0.95f*glm::pi<float>()/180.0f, glm::vec3(0.0, 1.0, 0.0));
     //          node->Rotate(rotation);

                last_time = current_time;
				
            }
        }

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
    SceneNode* halo = game->scene_.GetNode("ParticleInstance");
    // View control
    float rot_factor(glm::pi<float>() / 180);
    float trans_factor = 10.0;
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
    if (key == GLFW_KEY_Z) {
        game->camera_.Roll(-rot_factor);
    }
    if (key == GLFW_KEY_X) {
        game->camera_.Roll(rot_factor);
    }
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
    //create a flesh object and move it into place
    game::SceneNode* factory = CreateInstance("Area1", "Factory", "Lighting", "Steel"); //creates the main factory building

    factory->Scale(glm::vec3(.1, .1, .1));
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
    float placerng = (-100 + rand() % (100 - (-100) + 1));
    float placerngz = (-10 + rand() % (10 - (-10) + 1));
    page->Translate(glm::vec3(0, 5, 0));
    page->Scale(glm::vec3(.09, .09, .09));
    glm::quat rot = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
 
    //page->Translate(glm::vec3(0))
    for (int i = 0; i < 8; i++)
    {
        float placerngx = (-100 + rand() % (100 - (-100) + 1));
        placerngz = (-10 + rand() % (10 - (-10) + 1));
        items.push_back(page);
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "Page_" + index;
        page = CreateInstance(name, "paper", "Lighting","Flesh");
        page->Translate(glm::vec3(placerngx, 5, placerngz));
        page->Scale(glm::vec3(.09, .09, .09));
        page->Rotate(rot);
    }

    game::SceneNode* boar = CreateInstance("boar", "Boar", "Lighting", "Flesh");
    boar->Translate(glm::vec3(3, 0, 0));
    boar->Attach(factory, 0);


}

} // namespace game
