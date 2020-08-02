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

using namespace GEK;

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    GEK::initGLFW();

    window win;
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

//////////
    glEnable(GL_DEPTH_TEST);  
    while(!glfwWindowShouldClose(win()))
    {
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        view  = glm::translate(view, glm::vec3(0.0f, 1.0f, -10.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)win.width() / (float)win.height(), 0.1f, 50.0f);

        processInput(win());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tex.use();
        
        shp->setUniform("model", (model));
        shp->setUniform("view", (view));
        shp->setUniform("projection", (projection));
        shp->setUniform("usesTexture", true);
        shp->setUniform("color", glm::vec4(1.0f,1.0f,1.0f,1.0f));

        br.draw();

        if(auto err = glGetError() != GL_NO_ERROR)std::cout<<err<<std::endl;

        glfwSwapBuffers(win());
        glfwPollEvents();
    }

    }catch(failExcept *e)
    {
        std::cout<<e->what<<std::endl;
        exit(2);
    }

    glfwTerminate();

    return 0;
}