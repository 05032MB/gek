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
    PrecisionType all{0};

    PrecisionType timestamp{0};

    public:
        simpleClockM() = default;

        void reset()
        {
            lastFrame = glfwGetTime();
            deltaTime = 0;
            timestamp = 0;
            all = 0;
        }
        void tick()
        {
            auto currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            all += deltaTime;
        }
        PrecisionType getDelta() const
        {
            return deltaTime;
        }
        PrecisionType getLifetime() const
        {
            return all;
        }
        void freezeTimestamp()
        {
            timestamp = glfwGetTime();
        }
        PrecisionType getDiff()
        {
            return glfwGetTime() - timestamp;
        }
};

using simpleClock = simpleClockM<float>;

}

#endif