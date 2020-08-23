#ifndef GEK_COLLISION_SPHERE
#define GEK_COLLISION_SPHERE

#include <glm/glm.hpp>

namespace GEK
{
    template <typename precisionType>
    class collisionSphereT
    {
        precisionType radius;
        glm::vec3 pos;

        static auto dist3d(glm::vec3 a, glm::vec3 b)
        {
            return glm::distance(a, b);
        }

        public:

        collisionSphereT(glm::vec3 pos, precisionType radius = 1)
        {
            this->pos = pos;
            this->radius = radius;
        }

        bool collidesOrIsWithin(const collisionSphereT & other)
        {
            auto dist = abs(dist3d(this->pos, other.getPosition()));
            if(this->radius + other.radius < dist)
            {
                return false;
            }
            return true;
        }

        void updatePosition(glm::vec3 pos)
        {
            this->pos = pos;
        }

        glm::vec3 getPosition() const
        {
            return this->pos;
        }

        precisionType getRadius() const
        {
            return this->radius;
        }

    };

using collisionSphere = collisionSphereT<float>;
}

#endif