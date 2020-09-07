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
    collisionSphere colsp{1};

    void move(float deltaTime)
    {
        auto fin = bullet.getPosition() + dir;
        bullet.setPosition( {fin.x + deltaTime * speed, fin.y + deltaTime * speed, fin.z +  + deltaTime * speed});
        colsp.updatePosition(bullet.getPosition());
    }
};

struct asteroida
{
    collisionSphere dlaKuli{4.5};
    collisionSphere dlaGracza{7};
    object internal;
};
//zmienne globalne aktualnego przyspieszenia w danym kierunku
float forwardsTime = 0;
float backwardsTime = 0;
float leftsTime = 0;
float rightsTime = 0;
float upsTime = 0;
float downsTime = 0;
float pitchupsTime = 0;
float pitchdownsTime = 0;
float yawupsTime = 0;
float yawdownsTime = 0;
float rollupsTime = 0;
float rolldownsTime = 0;

//modyfikatory przyspieszenia
float scaleGlobal = 20.0;
float scaleUpDown = 10.0;
float scalePitchYawRoll = 20.0;
float scaleStabilize = 1.01;

//funkcja ruchu w bezwładności
void moveInertia(GLFWwindow* window, camera &cam){
	cam.moveWithKbd(kwaCamera::movement::forwards, forwardsTime);
	cam.moveWithKbd(kwaCamera::movement::backwards, backwardsTime);
	cam.moveWithKbd(kwaCamera::movement::lefts, leftsTime);
	cam.moveWithKbd(kwaCamera::movement::rights, rightsTime);
	cam.moveWithKbd(kwaCamera::movement::ups, upsTime);
	cam.moveWithKbd(kwaCamera::movement::downs, downsTime);
	cam.moveWithKbd(kwaCamera::movement::pitchups, pitchupsTime);
	cam.moveWithKbd(kwaCamera::movement::pitchdowns, pitchdownsTime);
	cam.moveWithKbd(kwaCamera::movement::yawups, yawupsTime);
	cam.moveWithKbd(kwaCamera::movement::yawdowns, yawdownsTime);
	//cam.moveWithKbd(kwaCamera::movement::rollups, rollupsTime);
	//cam.moveWithKbd(kwaCamera::movement::rolldowns, rolldownsTime);
}

void processInput(window &win, camera &cam, simpleClock &cl, object &bull)
{
    auto window = win();
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    auto deltaTime = cl.getDelta();
	deltaTime /= scaleGlobal;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		forwardsTime += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        backwardsTime += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        leftsTime += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        rightsTime += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        upsTime += deltaTime/scaleUpDown;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        downsTime += deltaTime/scaleUpDown;
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        pitchupsTime += deltaTime/scalePitchYawRoll;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        pitchdownsTime += deltaTime/scalePitchYawRoll;
   
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        yawupsTime += deltaTime/scalePitchYawRoll;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        yawdownsTime += deltaTime/scalePitchYawRoll;
	/*
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		rollupsTime += deltaTime/scalePitchYawRoll;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		rolldownsTime += deltaTime/scalePitchYawRoll;
     */
	
	//ratunkowa stabilizacja ruchu aka hamulce
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
		forwardsTime /= scaleStabilize;
		backwardsTime /= scaleStabilize;
		leftsTime /= scaleStabilize;
		rightsTime /= scaleStabilize;
		upsTime /= scaleStabilize;
		downsTime /= scaleStabilize;
		pitchupsTime /= scaleStabilize;
		pitchdownsTime /= scaleStabilize;
		yawupsTime /= scaleStabilize;
		yawdownsTime /= scaleStabilize;
		//rollupsTime /= scaleStabilize;
		//rolldownsTime /= scaleStabilize;
	}

}

std::pair<bool,bullet> shouldMakeBullet(window &win, camera &cam, simpleClock &cl, object &bull)
{
    auto window = win();
    constexpr float fireDelay{1};

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

        auto dir = glm::normalize(cam.position - (cam.position - cam.antidirection) ); //camera direction hack

        ret.bullet = bull;
        ret.bullet.setPosition( cam.getPosition() );
        ret.bullet.setRotationAngle(object::pitch, cam.pitch);
        ret.bullet.setRotationAngle(object::yaw, -cam.yaw + 270);
        ret.bullet.setRotationAngle(object::roll, 180);
        ret.colsp.updatePosition( cam.getPosition() );
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

    srand(glfwGetTime());

//////////

    camera cam({0,0,8});
    simpleClock cl;
    simpleClock shootDelayer;

    const unsigned numAst = 10;

    auto shp = std::make_shared<shaderProgram>();
    shp->enslaveShader(std::make_shared<shader>("src/shaderz/vertex/vertex3dWithLightning.glsl", GL_VERTEX_SHADER),
                        std::make_shared<shader>("src/shaderz/fragment/fragmentWithLightning.glsl", GL_FRAGMENT_SHADER));
    shp->compile();
    shp->cull();
    shp->activate();

    object bakPak, ship;

    collisionSphere camSphere(cam.getPosition(), 1);

    std::cout<<"---Loading Texs---"<<std::endl;
    auto shiptex = std::make_shared<texture>("../Downloads/Andorian (1).png", true);
    auto missletex = std::make_shared<texture>("../Downloads/AIM-9 SIDEWINDER texture2.png", true);
    auto shiptexspec = std::make_shared<texture>("../Downloads/Andorian (4).png", true, texture::specular);
    auto tex2 = std::make_shared<texture>("../Downloads/diffuse.jpg", false);
    auto tex2spe = std::make_shared<texture>("../Downloads/baspecular.jpg", false, texture::specular);
    auto asteroidtex = std::make_shared<texture>("../Downloads/Asteroid/10464_Asteroid_v1_diffuse.jpg", true);
    std::cout<<"###Texs done###"<<std::endl;

    std::cout<<"---Loading Models---"<<std::endl;
    auto shipmodel = std::make_shared<objPrimitive>("../Downloads/Quarren Coyote Ship.obj", "./media/", 0.01, 1 << 0);
    auto zr = std::make_shared<objPrimitive>("../Downloads/backpack.obj", "./media/", 1);
    auto asteroidmodel = std::make_shared<objPrimitive>("../Downloads/Asteroid/10464_Asteroid_v1_Iterations-2.obj", "./media/", 0.008);
    auto testBullet = std::make_shared<objPrimitive>("../Downloads/AIM-9 SIDEWINDER.obj", "./media/", 0.01);
    testBullet->bind();
    std::cout<<"###Models done###"<<std::endl;

    std::cout<<"---Generating texs and models---"<<std::endl;
    shipmodel->bind();
    zr->bind();
    asteroidmodel->bind();
    shiptex->createTexture(GL_RGBA);
    tex2->createTexture();
    asteroidtex->createTexture();
    tex2spe->createTexture();
    missletex->createTexture(GL_RGB);
    std::cout<<"###Gen done###"<<std::endl;

    bakPak.enslaveModel(zr);
    bakPak.enslaveTex(tex2, tex2spe);

    ship.enslaveModel(shipmodel);
    ship.enslaveTex(shiptex, shiptexspec);

    object bull;
    bull.enslaveModel(testBullet);
    bull.enslaveTex(missletex);

    std::vector<bullet> bullets;
    std::vector<asteroida> asteroids;

    int basepos = 0;
    for(auto f = 0; f < numAst; f++)
    {
        asteroida tmp;

        tmp.internal.enslaveModel(asteroidmodel);
        tmp.internal.enslaveTex(asteroidtex);
        tmp.internal.setPosition({basepos + rand() % 10, basepos + rand() % 10, basepos + rand() % 10 });
        tmp.internal.setRotationAngle(static_cast<object::whichAngle>(rand() % 3), rand() % 360);
        tmp.dlaKuli.updatePosition(tmp.internal.getPosition());
        //tmp.dlaGracza.setRadius(6);
        asteroids.push_back(tmp);

        basepos += rand() % 3 + 5;
    }

    bakPak.setPosition(glm::vec3( 0.0f,  0.0f,  0.0f));
    bakPak.setRotationAngle(object::whichAngle::roll, 21);

    //bullet bu1;

//////////
    win.enableZBuffer(); 
    win.setClearScreenColor(1.0f, 1.0f, 1.0f, 1.0f);
    while(!win.shouldClose())
    {

        cl.tick();

        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        glm::mat4 model    = glm::mat4(1.0f);

        processInput(win, cam, cl, bull);
        processMouse(win, cam);
		moveInertia(win(), cam);
        //processMouse(win());

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

        shp->setUniform("model", bull.getModelMatrix());
        bull.draw();

        shp->setUniform("hasSpecularTex", true);
            /*model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  -8.0f));
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-1.0f, -1.0f, -1.0f));  
            
            shp->setUniform("model", (model));
            shp->setUniform("view", (glm::mat4(1.0f))); //XXXX
            shp->setUniform("projection", (projection));
            //shp->setUniform("usesTexture", true);
            shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));*/
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
            i.bullet.draw();
        }

        /////asteroidy
        asteroidtex->use();

        for (auto &i : asteroids)
        {
            shp->setUniform("model", i.internal.getModelMatrix() );
            shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));
            i.internal.draw();
        }
        ///////

        ///////kolizje
        for(auto &i : asteroids)
        {
            if(i.dlaGracza.collidesOrIsWithin(camSphere))
            {
                //std::cout<<"Kolizja z asteroidą"<<std::endl;
                //exit(42);
            }
        }


        /*
        Ograniczenie - wyłapuje na raz tylko jedną parę. Ale i tak nie ma dużo strzelania, więc jest ok.
        */
        auto astBullCollCb = [&bullets, &asteroids]() -> std::pair<int, int>
        {
            for (auto i = 0; i < bullets.size(); i++)
            {
                auto col = bullets[i].colsp;
                for(auto f = 0; f < asteroids.size(); f++)
                {
                    if(col.collidesOrIsWithin(asteroids[f].dlaKuli))
                    {
                        return {i, f};
                    }
                }
            }
            return {-1, -1};
        };

        auto isThere = astBullCollCb();

        if(isThere.first != -1)
        {
            bullets.erase(bullets.begin() + isThere.first);
            asteroids.erase(asteroids.begin() + isThere.second);
        }

        ////////

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