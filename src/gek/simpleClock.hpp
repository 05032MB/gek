#ifndef GEK_SIMPLE_CLOCK_HPP
#define GEK_SIMPLE_CLOCK_HPP

#include <GLFW/glfw3.h>

namespace GEK
{

template <typename PrecisionType>
class simpleClockM
{
    PrecisionType deltaTime{0};
    PrecisionType lastFrame{0};

    public:
        simpleClockM() = default;

        void tick()
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }
        PrecisionType getDelta() const
        {
            return deltaTime;
        }
};

using simpleClock = simpleClockM<float>;

}

#endif