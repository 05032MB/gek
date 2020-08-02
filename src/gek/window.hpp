#ifndef GEK_WINDOW_HPP
#define GEK_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>

#include <gek/except.hpp>

namespace GEK
{

class window
{
    struct GLFWwinDestroyer{
        void operator()(GLFWwindow* ptr){
            glfwDestroyWindow(ptr);
        }
    };

    std::unique_ptr<GLFWwindow, GLFWwinDestroyer> win;
    int w, h;

    void hint()
    {}
    void hintStr()
    {}

    public:

    window()
    {}

    template <typename opt, typename what, typename ... rest>
    void hint(opt o, what w, rest ... r)
    {
        glfwWindowHint(o, w);
        hint(r...);
    }

    template <typename opt, typename what, typename ... rest>
    void hintStr(opt o, what w, rest ... r)
    {
        glfwWindowHintString(o, w);
        hintStr(r...);
    }

    void setCurrent()
    {
        glfwMakeContextCurrent(win.get());
    }

    GLFWwindow * operator()() //should only be passed to openGL functions that are not implemented to work with what is in here
    {
        return win.get();
    }

    const GLFWwindow* createWindow(int w, int h, std::string title)
    {
        if(win)
        {
            throw new recoverableExcept("Window exists for this handler");
        }
        
        win.reset(glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr));

        if(!win)
        {
            throw new recoverableExcept("Window init failed");
        }

        this->w = w;
        this->h = h;

        glfwSetWindowUserPointer(win.get(), reinterpret_cast<void *>(this));
        glViewport(0, 0, w, h);
        glfwSetFramebufferSizeCallback(win.get(), resizeCallback);

        return win.get();
    }

    static void resizeCallback(GLFWwindow* gwin, int width, int height)
    {
        window * handler = reinterpret_cast<window *>(glfwGetWindowUserPointer(gwin));

        if(handler == nullptr)
        {
            throw new failExcept("Resize callback epic fail");
        }

        handler->w = width;
        handler->h = height;
        glViewport(0, 0, width, height);
    }

    const auto width() const
    {
        return w;
    }
    
    const auto height() const
    {
        return h;
    }
};

}
#endif