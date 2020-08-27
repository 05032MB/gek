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
#include <gek/clockz/simpleClock.hpp>
#include <gek/collisionSphere.hpp>
#include <gek/object.hpp>

using namespace GEK;

struct bullet
{
    glm::vec3 dir;
    object bullet;
    float speed{0.005};

    void move(float deltaTime)
    {
        auto fin = bullet.getPosition() + dir;
        bullet.setPosition( {fin.x + deltaTime * speed, fin.y + deltaTime * speed, fin.z +  + deltaTime * speed});
    }
};

void processInput(window &win, camera &cam, simpleClock &cl, object &bull)
{
    auto window = win();
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

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::rollups, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.moveWithKbd(camera::movement::rolldowns, deltaTime);
}

std::pair<bool,bullet> shouldMakeBullet(window &win, camera &cam, simpleClock &cl, object &bull)
{
    auto window = win();
    constexpr float fireDelay{5};

    cl.tick();

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if(fireDelay > cl.getDiff())
        {
            std::cout<<"Insuff diff "<<cl.getDiff()<<std::endl;
            return {false, bullet()};
        }
        cl.freezeTimestamp();

        bullet ret;

        auto dir = glm::normalize(cam.position - (cam.position + -cam.antidirection) ); //camera direction hack

        ret.bullet = bull;
        ret.bullet.setPosition( cam.getPosition() );
        ret.dir = dir;

        return {true, ret};
    }
    return {false, bullet()};
}

void processMouse(window &win, iCameraStandardOps &cam)
{
    double x, y;
    glfwGetCursorPos(win(), &x, &y);

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
    try
    {
    GEK::initGLFW();

    window win;

    win.hint(GLFW_CONTEXT_VERSION_MAJOR, 3, GLFW_CONTEXT_VERSION_MINOR, 3, GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win.createWindow(800, 600, "asteroids");
    win.setCurrent();

    GEK::initGLEW();

    errno = 0; //gLfw NiE usTaWia ErrNo aLe wYwoŁujE dUżO fUnkcJi poŚreDnicH

//////////

    camera cam({0,0,8});
    simpleClock cl;
    simpleClock shootDelayer;

    const unsigned numAst = 10;

    auto shp = std::make_shared<shaderProgram>();
    shp->enslaveShader(std::make_shared<shader>("src/shaderz/vertex/vertex3dWithLightning.glsl", GL_VERTEX_SHADER),
                        std::make_shared<shader>("src/shaderz/fragment/fragmentWithLightning.glsl", GL_FRAGMENT_SHADER));
    shp->compile();
    shp->releaseShaders();
    shp->activate();

    object bakPak, asteroids[numAst], ship;

    collisionSphere camSphere(cam.getPosition(), 1);
    collisionSphere astSpheres[numAst] = {1}; //changeme

    auto shiptex = std::make_shared<texture>("../Downloads/Andorian (1).png", true);
    auto shiptexspec = std::make_shared<texture>("../Downloads/Andorian (4).png", true, texture::specular);
    auto tex2 = std::make_shared<texture>("../Downloads/diffuse.jpg", false);
    auto tex2spe = std::make_shared<texture>("../Downloads/baspecular.jpg", false, texture::specular);
    auto asteroidtex = std::make_shared<texture>("../Downloads/Asteroid/10464_Asteroid_v1_diffuse.jpg", true);

    std::cout<<"-----"<<std::endl;
    auto shipmodel = std::make_shared<objPrimitive>("../Downloads/Quarren Coyote Ship.obj", "../Downloads/", 0.01, 1 << 0);
    auto zr = std::make_shared<objPrimitive>(/*"tinyobjloader/models/cube.obj"*/"../Downloads/backpack.obj", "../Downloads/"/*"tinyobjloader/models/"*/, 1);
    auto asteroidmodel = std::make_shared<objPrimitive>("../Downloads/Asteroid/10464_Asteroid_v1_Iterations-2.obj", "../Downloads/Asteroid", 0.008);

    auto testBullet = std::make_shared<cube>();
    testBullet->bind();
    std::cout<<"#####"<<std::endl;

    shipmodel->bind();
    zr->bind();
    asteroidmodel->bind();
    shiptex->createTexture();
    tex2->createTexture();
    asteroidtex->createTexture();
    tex2spe->createTexture();

    bakPak.enslaveModel(zr);
    bakPak.enslaveTex(tex2, tex2spe);

    ship.enslaveModel(shipmodel);
    ship.enslaveTex(shiptex, shiptexspec);

    object bull;
    bull.enslaveModel(testBullet);

    std::vector<bullet> bullets;

    int basepos = 0;
    for(auto f = 0; f < numAst; f++)
    {
        auto &i = asteroids[f];

        i.enslaveModel(asteroidmodel);
        i.enslaveTex(asteroidtex);
        i.setPosition({basepos + rand() % 10, basepos + rand() % 10, basepos + rand() % 10 });
        i.setRotationAngle(static_cast<object::whichAngle>(rand() % 3), rand() % 360);
        astSpheres[f].updatePosition(i.getPosition());
        basepos += rand() % 3 + 5;
    }

    bakPak.setPosition(glm::vec3( 0.0f,  0.0f,  0.0f));
    bakPak.setRotationAngle(object::whichAngle::roll, 21);

    bullet bu1;

//////////
    win.enableZBuffer(); 
    win.setClearScreenColor(1.0f, 1.0f, 1.0f, 1.0f);
    while(!win.shouldClose())
    {

        cl.tick();

        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        processInput(win, cam, cl, bull);
        processMouse(win, cam);

        win.clearScreen();

        view  = cam.getViewMatrix();
        projection = glm::perspective(glm::radians(cam.zoom), (float)win.width() / (float)win.height(), 0.1f, 500.0f);
        shp->setUniform("actCameraPos", cam.getPosition());
        shp->setUniform("staticLightPos", glm::vec3(10,0,0));
        shp->setUniform("view", (view));
        shp->setUniform("projection", (projection));

        camSphere.updatePosition(cam.getPosition());

        //ship.setPosition({cam.position.x + cam.antidirection.x + 1, cam.position.y + cam.antidirection.y + 1, cam.position.z + cam.antidirection.z + 1});

        glm::mat4 shipModelMat = glm::mat4(1.0f);
        shipModelMat = glm::translate(shipModelMat, {0,0,0});
        shp->setUniform("model", ship.getModelMatrix());
        shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));

        shp->setUniform("hasSpecularTex", true);
        ship.draw();
        shp->setUniform("hasSpecularTex", false);

            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));  
            
        //test model
        shp->setUniform("model", bakPak.getModelMatrix());
        shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));
        shp->setUniform("hasSpecularTex", true);

        bakPak.draw();

        shp->setUniform("hasSpecularTex", false);
        /////

        auto potentialBullet = shouldMakeBullet(win, cam, shootDelayer, bull);
        if(potentialBullet.first)
        {
            auto fu = potentialBullet.second.bullet.getPosition();
            std::cout<<fu.x<<" "<<fu.y<<" "<<fu.z<<std::endl;
            bullets.push_back(potentialBullet.second);
        }

        for(auto &i : bullets)
        {
            shp->setUniform("model", i.bullet.getModelMatrix());
            i.move(cl.getDelta());
            //bu1.bullet.enslaveModel(testBullet);
            i.bullet.draw();
        }

        /////
        asteroidtex->use();

        for (auto &i : asteroids)
        {
            shp->setUniform("model", i.getModelMatrix() );
            shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));

            i.draw();
        }

        for(auto &i : astSpheres)
        {
            if(i.collidesOrIsWithin(camSphere))
            {
                std::cout<<"Kolizja z asteroidą"<<std::endl;
                exit(42);
            }
        }

        if(auto err = glGetError() != GL_NO_ERROR)std::cout<<"[GL Error]: "<<err<<std::endl;

        win.swapBuffers();
        win.pollEvents();
    }

    }
    catch(except *e)
    {
        std::cout<<"[GEK ERROR]: "<<e->what<<std::endl;
        glfwTerminate();
        exit(2);
    }

    glfwTerminate();

    return 0;
}