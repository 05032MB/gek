#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

#include <iostream>

#include <gek/misc.hpp>
#include <gek/shaderProgram.hpp>
#include <gek/primitivez/primitives.hpp>

using namespace GEK;

void resizeWindowCb(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = GEK::createWindow(800, 600, "pdoomu");

    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, resizeWindowCb);

    GEK::initGLEW();

    errno = 0; //gLfw NiE usTaWia ErrNo aLe wYwoŁujE dUżO fUnkcJi poŚreDnicH

//////////
    shaderProgram shp;

    auto shader1 = std::make_shared<shader>("src/shaderz/vertexTrivial.glsl", GL_VERTEX_SHADER);

    try{
        shp.enslaveShader( shader1,
                          std::make_shared<shader>("src/shaderz/fragmentTrivial.glsl", GL_FRAGMENT_SHADER));

        shp.compile();
        shp.releaseShaders();

    }catch(failExcept *e)
    {
        std::cout<<e->what<<std::endl;
        exit(2);
    }

    triangle tr;
    tr.bind();

    shp.activate();
//////////
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        tr.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}