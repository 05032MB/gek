#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>

#include <iostream>

#include <gek/misc.hpp>
#include <gek/shaderProgram.hpp>
#include <gek/primitivez/primitives.hpp>
#include <gek/window.hpp>
#include <gek/transform.hpp>
#include <gek/texture.hpp>
#include <gek/cameraz/cameras.hpp>

using namespace GEK;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

window win;
camera cam({0,0,8});

//#define camera kwaCamera

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::forwards, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::backwards, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::lefts, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::rights, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::ups, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::downs, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::pitchups, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::pitchdowns, deltaTime);
   
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::yawups, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::yawdowns, deltaTime);

    /*if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::rollups, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::rolldowns, deltaTime);*/
}

#undef camera

void processMouse(GLFWwindow* window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    float xoffset = x;
    float yoffset = y; 

    xoffset+=1;
    yoffset+=1;

    xoffset = log10(xoffset);
    yoffset = log10(yoffset);

    if(x < win.width()/2 + 100 && x > win.width()/2 - 100)xoffset = 0;
    if(y < win.height()/2 + 100 && y > win.height()/2 - 100)yoffset = 0;

    if(x < win.width()/2 - 100)xoffset *= -1;
    if(y < win.height()/2 - 100)yoffset *= -1;

    if(isnan(xoffset) || isinf(xoffset))xoffset = 0;
    if(isnan(yoffset) || isinf(yoffset))yoffset = 0;

    //std::cout<<"Mysz na"<<x<<" "<<y<<"off "<<xoffset<<" "<<yoffset<<std::endl;

    cam.moveWithMouse(xoffset, -yoffset);
}  

int main()
{
    GEK::initGLFW();

    win.hint(GLFW_CONTEXT_VERSION_MAJOR, 3, GLFW_CONTEXT_VERSION_MINOR, 3, GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win.createWindow(800, 600, "assteroids");
    win.setCurrent();

    GEK::initGLEW();

    errno = 0; //gLfw NiE usTaWia ErrNo aLe wYwoŁujE dUżO fUnkcJi poŚreDnicH

//////////
    auto shp = std::make_shared<shaderProgram>();

    auto sha3D = std::make_shared<shader>("src/shaderz/vertex/vertex3d.glsl", GL_VERTEX_SHADER);

    try{
        shp->enslaveShader( sha3D,
                          std::make_shared<shader>("src/shaderz/fragment/fragment.glsl", GL_FRAGMENT_SHADER));

        shp->compile();
        shp->releaseShaders();

        shp->activate();

    cube br;
    br.bind();

    shp->activate();

    texture tex("../Downloads/wall.jpg");
    tex.createTexture();

    //camera cam;

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 1.0f,  0.5f,  0.7f),
    };

//////////
    glEnable(GL_DEPTH_TEST);  
    while(!glfwWindowShouldClose(win()))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        processInput(win());
        processMouse(win());
        //glfwSetCursorPosCallback(win(), processMouse);  

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tex.use();

        for (unsigned int i = 0; i < 2; i++) 
        {
            model = glm::translate(model, cubePositions[i]);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));  
            view  = cam.getViewMatrix();
            projection = glm::perspective(glm::radians(45.0f), (float)win.width() / (float)win.height(), 0.1f, cam.zoom);
            
            shp->setUniform("model", (model));
            shp->setUniform("view", (view));
            shp->setUniform("projection", (projection));
            shp->setUniform("usesTexture", true);
            shp->setUniform("color", glm::vec4(1.0f,1.0f,1.0f,1.0f));
            
            br.draw();
        }

        if(auto err = glGetError() != GL_NO_ERROR)std::cout<<err<<std::endl;

        glfwSwapBuffers(win());
        glfwPollEvents();
    }

    }catch(except *e)
    {
        std::cout<<"[ERROR]: "<<e->what<<std::endl;
        exit(2);
    }

    glfwTerminate();

    return 0;
}