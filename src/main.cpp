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
#include <gek/objModel.hpp>
#include <gek/simpleClock.hpp>

using namespace GEK;

window win;
camera cam({0,0,8});
simpleClock cl;

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    auto deltaTime = cl.getDelta();

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

    //cam.moveWithMouse(xoffset, -yoffset);
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

    auto sha3D = std::make_shared<shader>("src/shaderz/vertex/vertex3dWithLightning.glsl", GL_VERTEX_SHADER);

    try{
        shp->enslaveShader( sha3D,
                          std::make_shared<shader>("src/shaderz/fragment/fragmentWithLightning.glsl", GL_FRAGMENT_SHADER));

        shp->compile();
        shp->releaseShaders();

        shp->activate();

    shp->activate();

    texture tex("../Downloads/Andorian (1).png", true);
    texture tex2("../Downloads/diffuse.jpg", false);
    texture tex3("../Downloads/Asteroid/10464_Asteroid_v1_diffuse.jpg", true);

    std::cout<<"-----"<<std::endl;
    objPrimitive pr(/*"tinyobjloader/models/cube.obj"*/"../Downloads/Quarren Coyote Ship.obj", "../Downloads/"/*"tinyobjloader/models/"*/, 0.01, 1 << 0);
    objPrimitive zr(/*"tinyobjloader/models/cube.obj"*/"../Downloads/backpack.obj", "../Downloads/"/*"tinyobjloader/models/"*/, 1);
    std::cout<<"#####"<<std::endl;
    pr.bind();
    zr.bind();
    tex.createTexture();
    tex2.createTexture();
    tex3.createTexture();
    //camera cam;

    objPrimitive cu("../Downloads/Asteroid/10464_Asteroid_v1_Iterations-2.obj", "../Downloads/Asteroid", 0.01);
    cu.bind();

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 1.0f,  0.5f,  0.7f),
        glm::vec3( 5.0f,  5.5f,  -20.0f),
    };

//////////
    win.enableZBuffer(); 
    win.setClearScreenColor(0.2f, 0.3f, 0.3f, 1.0f);
    while(!win.shouldClose())
    {

        cl.tick();

        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        processInput(win());
        processMouse(win());

        win.clearScreen();

        view  = cam.getViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)win.width() / (float)win.height(), 0.1f, cam.zoom);
        shp->setUniform("actCameraPos", cam.getPosition());
        //cam.cameraSpeed = 17;

        //shp->setUniform("ambientLight", 0.25f);

        tex.use();

            model = glm::translate(model, cubePositions[1]);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));  
            
            shp->setUniform("model", (model));
            shp->setUniform("view", (view));
            shp->setUniform("projection", (projection));
            //shp->setUniform("usesTexture", true);
            shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));
            
        pr.draw();

        tex2.use();

            model = glm::translate(model, cubePositions[0]);
            //cubePositions[0][1] += sin(glfwGetTime()) * 0.0001;
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));  
            
            shp->setUniform("model", (model));
            shp->setUniform("view", (view));
            shp->setUniform("projection", (projection));
            //shp->setUniform("usesTexture", true);
            shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));

        zr.draw();

        tex3.use();
            model = glm::translate(glm::mat4(1.0f), cubePositions[2]);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
            
            shp->setUniform("model", (model));
            shp->setUniform("view", (view));
            shp->setUniform("projection", (projection));
            //shp->setUniform("usesTexture", true);
            shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));

        cu.draw();

        if(auto err = glGetError() != GL_NO_ERROR)std::cout<<"[GL Error]: "<<err<<std::endl;

        win.swapBuffers();
        win.pollEvents();
    }

    }catch(except *e)
    {
        std::cout<<"[GEK ERROR]: "<<e->what<<std::endl;
        glfwTerminate();
        exit(2);
    }

    glfwTerminate();

    return 0;
}