//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "cube.h"
#include "stb_image.h"
#include "SpotLight.h"
#include  "directionLight.h"
#include "car.h"
#include <math.h>
#include <iostream>

using namespace std;

unsigned int loadCubemap(vector<std::string> faces);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void draw_cube(float rotationX, float rotationY, float rotationZ, glm::vec3 scale, glm::vec3 translate, glm::vec3 translateBack,
    float r, float g, float b, Cube cube, Shader ourShader);
void draw_cube_texture(float rotationX, float rotationY, float rotationZ, glm::vec3 scale, glm::vec3 translate,
    glm::vec3 translateBack, Cube cube, Shader ourShader);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
//void bed(Shader& lightingShader, glm::mat4 alTogether, Cube& cube);




//------------------------------functions for bezier----------------------------------
long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L);




// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;



#define NR_POINT_LIGHTS 40
#define NR_DIRECTIONAL_LIGHTS 5


// camera
Camera camera(glm::vec3(7, -4.1, -30));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = -1.0, lookAtY = 0.4, lookAtZ = 1.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

Car car(glm::vec3(-5.0f, 0.0f, 18.0f));



glm::vec3 pointLightPositions[NR_POINT_LIGHTS];
PointLight pointlight[NR_POINT_LIGHTS];
SpotLight  spotlight[1];
DirectionLight directionLight[NR_DIRECTIONAL_LIGHTS];


// light settings
bool directionlighton = true;
bool pointLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool spot_on = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float s_ambient = 1;
float s_diffuse = 1;
float s_specular = 1;
float rotationy = 0;
float rotation = 0.0f;
bool collision = false;
glm::vec3 movement = glm::vec3(20, -6.5, -3);
int gameover = 0;
float windMillRotation = 0.0f;
float dist = 100.0f;
float simulation_velocity = 0.15f;

float carvertices[] = {
    -0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, 0.1f, -0.1f,
    0.1f, 0.1f, -0.1f,
    -0.1f, 0.1f, -0.1f,
    -0.1f, -0.1f, -0.1f,

    -0.1f, -0.1f, 0.1f,
    0.1f, -0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f,
    -0.1f, -0.1f, 0.1f,

    -0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, -0.1f,
    -0.1f, -0.1f, -0.1f,
    -0.1f, -0.1f, -0.1f,
    -0.1f, -0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f,

    0.1f, 0.1f, 0.1f,
    0.1f, 0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,

    -0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, 0.1f,
    0.1f, -0.1f, 0.1f,
    -0.1f, -0.1f, 0.1f,
    -0.1f, -0.1f, -0.1f,

    -0.1f, 0.1f, -0.1f,
    0.1f, 0.1f, -0.1f,
    0.1f, 0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, -0.1f,
};

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

//to switch skybox
int skybox = 0;
//-----------------------bezier variable declarations---------
float wcsClkUp[24] = {
    -0.6000, 2.4450, 5.1000,
        -0.6100, 2.3450, 5.1000,
        -0.6450, 2.2250, 5.1000,
        -0.6600, 2.0350, 5.1000,
        -0.7100, 1.8300, 5.1000,
        -0.7350, 1.6100, 5.1000,
        -0.8050, 1.2400, 5.1000,
        -0.8250, 1.1150, 5.1000
};
vector <float> cntrlPoints;
vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;
unsigned int bezierVAO;

int main()

{
    glm::vec3 pointLightPositions[NR_POINT_LIGHTS] =
    { glm::vec3(13,-2,-9),
    glm::vec3(13,-2,-25),
    glm::vec3(13,-2,-50),
    glm::vec3(13,-2,-75),
    glm::vec3(13,-2,-100),
    glm::vec3(13,-2,-125),
    glm::vec3(13,-2,-150),
    glm::vec3(28,-2,-170),
    glm::vec3(38,-2,-180),
    glm::vec3(52,-2,-194),
    glm::vec3(65,-2,-208),
    glm::vec3(77,-2,-219),
    glm::vec3(93,-2,-235),
    glm::vec3(110,-2,-253),
    glm::vec3(131,-2,-274),
    glm::vec3(143,-2,-308),
    glm::vec3(144,-2,-329),
    glm::vec3(144,-2,-350),
    glm::vec3(144,-2,-379),
    glm::vec3(144,-2,-413),
    glm::vec3(144,-2,-436),
    glm::vec3(144,-2,-458),
    glm::vec3(140,-2,-477),
    glm::vec3(126,-2,-400),
    glm::vec3(112,-2,-504),
    glm::vec3(98,-2,-518),
    glm::vec3(83,-2,-532),
    glm::vec3(65,-2,-552),
    glm::vec3(50,-2,-566),
    glm::vec3(30,-2,-587) };

    glm::vec3 directionalLightPositions[NR_DIRECTIONAL_LIGHTS] = {
        glm::vec3(13, -6,-70),
        glm::vec3(80,-6,-222),
        glm::vec3(144, -6,-383),
        glm::vec3(85,-6,-532)
    };




    // point lights attribute set
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        pointlight[i] = PointLight(
            pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z,  // position
            0.6f, 0.6f, 0.6f,     // ambient
            0.8f, 0.8f, 0.8f,     // diffuse
            0.5, 0.5, 0.5,        // specular
            1.0f,   //k_c
            0.09f,  //k_l
            0.032f, //k_q
            i + 1       // light number
        );
    }



    //to copy the bezier vertices in controlpoint
    for (int i = 0; i < 24; i++) {
        cntrlPoints.push_back(wcsClkUp[i]);
    }

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");//for beziershader
    Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");



    bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1);//for bezier vao
    // texture map path
    string diffuseMapPath = "builiding.jpg";
    string specularMapPath = "builiding.jpg";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPathRoad = "road.jpg";
    string specularMapPathRoad = "road.jpg";
    unsigned int diffMapRoad = loadTexture(diffuseMapPathRoad.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapRoad = loadTexture(specularMapPathRoad.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cubeRoad = Cube(diffMapRoad, specMapRoad, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);



    diffuseMapPathRoad = "building2.png";
    specularMapPathRoad = "building2.png";
    unsigned int diffMapb2 = loadTexture(diffuseMapPathRoad.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapb2 = loadTexture(specularMapPathRoad.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cubeb2r = Cube(diffMapb2, specMapb2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "shop.png";
    specularMapPath = "shop.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cubes1l = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "shop2.png";
    specularMapPath = "shop2.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cubes2l = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "building3.png";
    specularMapPath = "building3.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cubeb3l = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //Sphere sphere = Sphere();

    diffuseMapPath = "desert.jpg";
    specularMapPath = "desert.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cubedesert = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "win.jpg";
    specularMapPath = "win.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube winEndCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    
    diffuseMapPath = "lose.jpg";
    specularMapPath = "lose.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube loseEndCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "water.jpg";
    specularMapPath = "water.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube waterCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "grass.jpg";
    specularMapPath = "grass.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grassCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    unsigned int carVBO, carVAO;
    glGenVertexArrays(1, &carVAO);
    glGenBuffers(1, &carVBO);
    glBindVertexArray(carVAO);
    glBindBuffer(GL_ARRAY_BUFFER, carVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(carvertices), carvertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------


    vector<std::string> faces
    {
     "skybox1/right.jpg",
    "skybox1/left.jpg",
    "skybox1/top.jpg",
    "skybox1/bottom.jpg",
    "skybox1/front.jpg",
    "skybox1/back.jpg"
    };


    vector<std::string> faces1
    {
  "skybox5/right.jpg",
    "skybox5/left.jpg",
    "skybox5/top.jpg",
    "skybox5/bottom.jpg",
    "skybox5/front.jpg",
    "skybox5/back.jpg"
    };


    unsigned int cubemapTexture = loadCubemap(faces);

    unsigned int cubemapTexture1 = loadCubemap(faces1);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);



    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // directional light
        for (int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++) {
            directionLight[i].direction = directionalLightPositions[i];
            if (skybox) {
                directionLight[i].ambient = glm::vec3(0.35, 0.35, 0.35);
                directionLight[i].diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
                directionLight[i].specular = glm::vec3(0.1f, 0.1f, 0.1f);
            }
            else
            {
                directionLight[i].ambient = glm::vec3(0.07, 0.07, 0.07);
                directionLight[i].diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
                directionLight[i].specular = glm::vec3(0.1f, 0.1f, 0.1f);

            }
            directionLight[i].Number = i + 1;
            directionLight[i].setUpDirectLight(lightingShaderWithTexture);
        }


        // point light
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            if (skybox) {   // day time
                pointlight[i].ambient = glm::vec3(0);
                pointlight[i].diffuse = glm::vec3(0);
                pointlight[i].specular = glm::vec3(0);
            }
            else {
                pointlight[i].ambient = glm::vec3(0.8f, 0.8f, 0.8f);
                pointlight[i].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
                pointlight[i].specular = glm::vec3(0.5, 0.5, 0.5);
            }
            pointlight[i].setUpPointLight(lightingShaderWithTexture);
        }


        // spot light
        spotlight[0].position = glm::vec3(camera.Position.x, 3, camera.Position.z);
        spotlight[0].Number = 0;
        //if (camera.yaw_abs >= 0 && camera.yaw_abs < 90) {
        //    spotlight[0].direction.x = camera.Position.x + ((cos(glm::radians(360 - camera.yaw_abs))) * 8);
        //    spotlight[0].direction.z = camera.Position.z + ((sin(glm::radians(360 - camera.yaw_abs))) * 8);

        //    //cout << 1 << endl;
        //}
        //if (camera.yaw_abs >= 90 && camera.yaw_abs < 180) {
        //    spotlight[0].direction.x = camera.Position.x - ((cos(glm::radians(360 - camera.yaw_abs))) * 8);
        //    spotlight[0].direction.z = camera.Position.z + ((sin(glm::radians(360 - camera.yaw_abs))) * 8);

        //    //std::cout << x <<" "<<y<< std::endl;
        //}
        //else if (camera.yaw_abs >= 180 && camera.yaw_abs < 270) {
        //    spotlight[0].direction.x = camera.Position.x - ((cos(glm::radians(360 - camera.yaw_abs))) * 8);
        //    spotlight[0].direction.z = camera.Position.z - ((sin(glm::radians(360 - camera.yaw_abs))) * 8);

        //    //rotation_angle =  yaw_abs -180;
        //    //cout << 3 << endl;
        //    //std::cout << "ok" << std::endl;
        //}
        //else if (camera.yaw_abs >= 270 && camera.yaw_abs < 360) {
        //    spotlight[0].direction.x = camera.Position.x + ((cos(glm::radians(360 - camera.yaw_abs))) * 8);
        //    spotlight[0].direction.z = camera.Position.z - ((sin(glm::radians(360 - camera.yaw_abs))) * 8);

        //    //cout << 4 << endl;
        //    //std::cout << "ok" << std::endl;
        //}
        spotlight[0].direction.y = -6.8;
        //cout << spotlight[0].direction.x << " " << spotlight[0].direction.y << " " << spotlight[0].direction.z << endl;
        //cout << spotlight[0].direction.z << " " << camera.Position.z << " " << camera.yaw_abs << endl;
        spotlight[0].direction = camera.Front;
        spotlight[0].direction.z = camera.Front.z * 5;
        spotlight[0].k_c = 0.1;
        spotlight[0].k_l = 0.01;
        spotlight[0].k_q = 0.002;
        spotlight[0].inner_circle = glm::cos(glm::radians(50.5f));
        spotlight[0].outer_circle = glm::cos(glm::radians(60.0f));
        spotlight[0].setUpspotLight(lightingShaderWithTexture);

        if (skybox) // daytime
        {
            spotlight[0].ambient = glm::vec3(0, 0, 0);
            spotlight[0].diffuse = glm::vec3(0, 0, 0);
            spotlight[0].specular = glm::vec3(0, 0, 0);
        }
        else  // night time
        {
            spotlight[0].ambient = glm::vec3(0.6, 0.6, 0.6);
            spotlight[0].diffuse = glm::vec3(0.4, 0.4, 0.4);
            spotlight[0].specular = glm::vec3(0.0f, 0.0f, 0.0f);
        }






        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, -2.2, -0.2));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 3, 2));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShaderWithTexture.use();




        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(model, glm::vec3(0.48, -0.18f, -2.8f));
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);



        // building 2 right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(35, -6.8, -80));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 5, 2));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        cubeb2r.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(35, -6.8, -85));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 5, 2));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        cubeb2r.drawCubeWithTexture(lightingShaderWithTexture, model);
        // building 2 right





        



        // school 2 left

        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(-4, -6.7, -9.2));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 1, 1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        cubes2l.drawCubeWithTexture(lightingShaderWithTexture, model);*/

        // school 2 left








        // road
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(160, 0.02, 30));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3, -6.7, translate_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        cubeRoad.drawCubeWithTexture(lightingShaderWithTexture, model);
        // road





        // model car cube
        draw_cube(0,camera.rotation_angle+1*camera.rotation_coefficient,0,glm::vec3(2,0.4,1),
            glm::vec3(camera.car_back_x,camera.y-1,camera.car_back_z),
            glm::vec3(-1,0,-0.5), 0.031, 0.42, 0.008, cube, ourShader);
        draw_cube(0, camera.rotation_angle + 1*camera.rotation_coefficient, 0, glm::vec3(1.6, 0.4, 0.8), 
            glm::vec3(camera.car_back_x, camera.y - 0.95, camera.car_back_z),
            glm::vec3(-0.8, 0, -0.4), 1, 0.992, 0.439, cube, ourShader);
        


        
        


        // road
        draw_cube_texture(0, 45.0f, 0, glm::vec3(200, 0.02, 30), glm::vec3(78, -6.701, -220), glm::vec3(-100, 0, -15),
            cubeRoad, lightingShaderWithTexture);

        draw_cube_texture(0, 90.0f, 0, glm::vec3(200, 0.02, 30), glm::vec3(144, -6.705, -378), glm::vec3(-100, 0, -15),
            cubeRoad, lightingShaderWithTexture);

        draw_cube_texture(0, -45.0f, 0, glm::vec3(200, 0.02, 30), glm::vec3(78, -6.701, -539), glm::vec3(-100, 0, -15),
            cubeRoad, lightingShaderWithTexture);
        //road

        // desert
        draw_cube_texture(0,0,0,glm::vec3(350, 0.02,-1200),glm::vec3(0,-6.82,0), glm::vec3(-150, 0, 200),
            cubedesert,lightingShaderWithTexture);
        // desert






        // simulation
        draw_cube(0.0f,rotationy,0.0f,glm::vec3(1,0.8,2),movement,glm::vec3(-0.5,0,-1), 0.012, 0.51, 0.098,cube,ourShader);

        if (movement.z > -145) {
            movement.x = movement.x;
            draw_cube(0.0f, rotationy, 0.0f, glm::vec3(1, 0.3, 1),
            glm::vec3(movement.x,movement.y+0.8,movement.z+0.5), glm::vec3(-0.5, 0, -0.5), 0, 0.788, 0.133, cube, ourShader);
        }
        else if (movement.z <= -145 && movement.z > -277) {
            rotationy = 135.0f;
            draw_cube(0.0f, rotationy, 0.0f, glm::vec3(1, 0.3, 1),
                glm::vec3(movement.x-0.25, movement.y + 0.8, movement.z + 0.25),
                glm::vec3(-0.5, 0, -0.5), 0, 0.788, 0.133, cube, ourShader);;
            movement.x = (movement.z - (-145 - tan(glm::radians(135.0f))*26))/tan(glm::radians(135.0f)) - 10;
        }
        else if (movement.z <= -277 && movement.z > -479) {
            rotationy = 180.0f;
            draw_cube(0.0f, rotationy, 0.0f, glm::vec3(1, 0.3, 1),
                glm::vec3(movement.x, movement.y + 0.8, movement.z + 0.5),
                glm::vec3(-0.5, 0, -0.5), 0, 0.788, 0.133, cube, ourShader);
        }
        else if (movement.z <= -479 && movement.z > -615) {
            rotationy = 45.0f;
            movement.x = (movement.z - (-479 - tan(glm::radians(45.0f)) * 157)) / tan(glm::radians(45.0f)) - 5;
            draw_cube(0.0f, rotationy, 0.0f, glm::vec3(1, 0.3, 1),
                glm::vec3(movement.x + 0.25, movement.y + 0.8, movement.z + 0.25),
                glm::vec3(-0.5, 0, -0.5), 0, 0.788, 0.133, cube, ourShader);
        }
        else {
            draw_cube(0.0f, rotationy, 0.0f, glm::vec3(1, 0.3, 1),
                glm::vec3(movement.x + 0.25, movement.y + 0.8, movement.z + 0.25),
                glm::vec3(-0.5, 0, -0.5), 0, 0.788, 0.133, cube, ourShader);
            movement.z += simulation_velocity;
        }
        movement.z -= simulation_velocity;
        //simulation





        // race end
        if (camera.Position.z <= -615 && camera.Position.x<20 && camera.Position.x>-6) {
            movement.z += 0.15;
            gameover = 1;
        }
        else if (movement.z <= -615) {
            camera.rotation_coefficient = 0;
            gameover = 2;
        }

        // race end







        // water body
        draw_cube_texture(0, 0, 0, 
            glm::vec3(20, 1, 150),
            glm::vec3(-7, - 7.7, -95),
            glm::vec3(-10, 0, -75),
            waterCube, lightingShaderWithTexture);

        draw_cube_texture(0, -45.0f, 0,
            glm::vec3(20, 1, 150),
            glm::vec3(30, -7.7, -160),
            glm::vec3(-10, 0, -75),
            waterCube, lightingShaderWithTexture);
        // water body






        // car collision
        dist = pow(camera.car_back_x - movement.x,2) + pow(camera.car_back_z - movement.z, 2);
        if (dist <= 2) {
            camera.Position.x -= 5;
            camera.Position.z += 5;
            camera.velocity = -50;
        }


        // car collision





        // wind mill
        glm::vec3 translate = glm::vec3(70,20,-260);
        model = glm::mat4(1.0f);
        identityMatrix = glm::mat4(1.0f);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(8, 1, 0.5));
        translateMatrix = glm::translate(identityMatrix, translate);
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(windMillRotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        cube.drawCube(ourShader, model, 0.7, 0.7, 0.7);





        model = glm::mat4(1.0f);
        identityMatrix = glm::mat4(1.0f);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(8, 1, 0.5));
        translateMatrix = glm::translate(identityMatrix, translate);
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(windMillRotation+120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        cube.drawCube(ourShader, model, 0.7, 0.7, 0.7);




        model = glm::mat4(1.0f);
        identityMatrix = glm::mat4(1.0f);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(8, 1, 0.5));
        translateMatrix = glm::translate(identityMatrix, translate);
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(windMillRotation+240.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        cube.drawCube(ourShader, model, 0.7, 0.7, 0.7);
 
        windMillRotation += 0.5f;

        draw_cube(0, 0, 0, glm::vec3(3, 28, 1), glm::vec3(71, -7, -262), glm::vec3(0, 0, 0), 0.4, 0.4, 0.4, cube, ourShader);
        // wind mill










        // grass
        draw_cube_texture(0, 0, 0, glm::vec3(10, 0.1, 120),
            glm::vec3(25, -6.8, -150), 
            glm::vec3(0, 0, 0), 
            grassCube,lightingShaderWithTexture);
        // grass










        
        // ending scene cube

        if (gameover == 1) {
            draw_cube_texture(0, 0, 0, glm::vec3(10, 5, 0.5),
                glm::vec3(camera.car_back_x-3, camera.y - 1, camera.car_back_z),
                glm::vec3(-5, -2.5, -0.25),
                winEndCube, lightingShaderWithTexture);
        }
        else if (gameover == 2) {
            draw_cube_texture(0, 0, 0, glm::vec3(4, 4, 0.5),
                glm::vec3(camera.car_back_x, camera.Position.y, camera.car_back_z),
                glm::vec3(-2, -2, -0.25),
                loseEndCube, lightingShaderWithTexture);
        }
        // ending scene cube


        //car.CarInit(glm::vec3(-5.0f, 0.0f, 18.0f));

      



        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.01f)); // Make it a smaller cube
            cube.drawCube(ourShader, model, 0.8f, 0.8f, 0.8f);
        }
        //---------------------------------bezier curve rendering start----------------
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setMat4("projection", projection);
        view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

       
        //transformation
        translateMatrix = glm::translate(identityMatrix, glm::vec3(13, -6.9, -180.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10, 30, 25));   //(scale_Y,scale_Z,scale_X)
        model = translateMatrix * rotateZMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;


        lightingShader.setMat4("model", model);
        lightingShader.setVec3("material.ambient", glm::vec3(0.3, 0.3, 0.3));
        lightingShader.setVec3("material.diffuse", glm::vec3(0.6, 0.6, 0.6));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("material.shininess", 10.0f);

        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);


        //---------------------------------bezier curve rendering end ----------------


        //skybox
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        if (skybox == 0) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        }
        else {
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture1);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    else {
        camera.ProcessKeyboard(NO_PRESS, deltaTime);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        simulation_velocity = 0.15;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        simulation_velocity = 0.25;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        simulation_velocity = 0.5;
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (skybox == 0) {
            skybox = 1;
            cout << "pressed" << endl;
        }
        else {
            skybox = 0;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //float xpos = static_cast<float>(xposIn);
    //float ypos = static_cast<float>(yposIn);

    //if (firstMouse)
    //{
    //    lastX = xpos;
    //    lastY = ypos;
    //    firstMouse = false;
    //}

    //float xoffset = xpos - lastX;
    //float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    //lastX = xpos;
    //lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void draw_cube(float rotationX,float rotationY,float rotationZ, glm::vec3 scale, glm::vec3 translate, glm::vec3 translateBack,
    float r,float g,float b,Cube cube,Shader ourShader) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateBackMatrix = glm::translate(identityMatrix, translateBack);
    glm::mat4 scaleMatrix = glm::scale(identityMatrix, scale);
    glm::mat4 translateMatrix = glm::translate(identityMatrix, translate);
    glm::mat4 rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotationY), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateYMatrix * translateBackMatrix * scaleMatrix;
    cube.drawCube(ourShader, model, r, g, b);
}


void draw_cube_texture(float rotationX, float rotationY, float rotationZ, glm::vec3 scale, glm::vec3 translate, 
    glm::vec3 translateBack, Cube cube, Shader ourShader) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateBackMatrix = glm::translate(identityMatrix, translateBack);
    glm::mat4 scaleMatrix = glm::scale(identityMatrix, scale);
    glm::mat4 translateMatrix = glm::translate(identityMatrix, translate);
    glm::mat4 rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotationY), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateYMatrix * translateBackMatrix * scaleMatrix;
    cube.drawCubeWithTexture(ourShader, model);
}



long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L)
{
   
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal
    

    const float dtheta = pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    return bezierVAO;
}
