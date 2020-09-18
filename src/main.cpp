#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
//#include <thread>

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
#include <gek/skybox_vertices.h>

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
    short tty{2};
    short rotCnt{0};
    glm::vec3 movAnimDir;
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
float scaleStabilize = 1.02;

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

bool isFlashlight{true};

void processInput(window &win, iCameraStandardOps &cam, simpleClock &cl, object &bull)
{
    auto window = win();
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if(0.333f < cl.getDiff())
        {
            isFlashlight = !isFlashlight;
            cl.freezeTimestamp();
        }
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
   
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        yawupsTime += deltaTime/scalePitchYawRoll;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
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

//glm::vec3 adjustBulletStart = glm::vec3(0.0f,0.0f,9.0f);
//trzeba dobrac odpowiednie parametry

std::pair<bool,bullet> shouldMakeBullet(window &win, camera &cam, simpleClock &cl, object &bull)
{
    auto window = win();
    constexpr float fireDelay{1};

    cl.tick();

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if(fireDelay > cl.getDiff())
        {
            //std::cout<<"Insuff diff "<<cl.getDiff()<<std::endl;
            return {false, bullet()};
        }
        cl.freezeTimestamp();

        bullet ret;

        auto dir = glm::normalize(cam.position - (cam.position - cam.antidirection) ); //camera direction hack

        ret.bullet = bull;

        glm::vec3 adjustBulletStart = glm::normalize(cam.antidirection); //przesunięcie odpalenia pocisku pod statek
        adjustBulletStart *= 12; //strzał z przodu statku
        glm::vec3 adjustBulletDown = glm::normalize(cam.up); //przesunięcie pod statek
        adjustBulletDown *= -1.5; //i bardziej na dole po lewej/prawej

        ret.bullet.setPosition( cam.getPosition() + adjustBulletStart + adjustBulletDown);
        
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

unsigned int loadSkybox(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 3);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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

    camera cam({0,3,-6});
    simpleClock cl;
    simpleClock shootDelayer;

    const unsigned numAst = 50;	//zwiększona liczba asteroid
	
	//regular shader
    auto shp = std::make_shared<shaderProgram>();
    shp->enslaveShader(std::make_shared<shader>("src/shaderz/vertex/vertex3dWithLightning.glsl", GL_VERTEX_SHADER),
                        std::make_shared<shader>("src/shaderz/fragment/fragmentWithLightning2.glsl", GL_FRAGMENT_SHADER));
    shp->compile();
    shp->cull();
    shp->activate();
	
	//sky shader
	auto sky_shp = std::make_shared<shaderProgram>();
    sky_shp->enslaveShader(std::make_shared<shader>("src/shaderz/vertex/vertexSky.glsl", GL_VERTEX_SHADER),
                        std::make_shared<shader>("src/shaderz/fragment/fragmentSky.glsl", GL_FRAGMENT_SHADER));
    sky_shp->compile();
    sky_shp->cull();
    sky_shp->activate();

    object bakPak, ship;

    collisionSphere camSphere(cam.getPosition(), 1);
    collisionSphere megaSphere9(cam.getPosition(), 213742); //absolutny kraniec mapy

    std::cout<<"---Loading Texs---"<<std::endl;
    auto shiptex = std::make_shared<texture>("media/Andorian (1).png", true);
    auto missletex = std::make_shared<texture>("media/AIM-9 SIDEWINDER texture2.png", true);
    auto shiptexspec = std::make_shared<texture>("media/Andorian (4).png", true, texture::specular);
    auto tex2 = std::make_shared<texture>("media/SpaceStation/dockmaintexture1.jpg", false);
    //auto tex2spe = std::make_shared<texture>("media/baspecular.jpg", false, texture::specular);
    auto asteroidtex = std::make_shared<texture>("media/Asteroid/10464_Asteroid_v1_diffuse.jpg", true);
	
	//skybox
	std::vector<std::string> sky_faces = {"media/Skybox/right.png", 
									"media/Skybox/left.png",
									"media/Skybox/top.png",
									"media/Skybox/bottom.png",
									"media/Skybox/front.png",
									"media/Skybox/back.png"};
	unsigned int skytex = loadSkybox(sky_faces);
	
    std::cout<<"###Texs done###"<<std::endl;

    std::cout<<"---Loading Models---"<<std::endl;
    auto shipmodel = std::make_shared<objPrimitive>("media/Quarren Coyote Ship.obj", "./media/", 0.01, 1 << 0);
    auto zr = std::make_shared<objPrimitive>("media/SpaceStation/spacedock.obj", "./media/", 2.2);
    auto asteroidmodel = std::make_shared<objPrimitive>("media/Asteroid/10464_Asteroid_v1_Iterations-2.obj", "./media/", 0.0088);
    auto testBullet = std::make_shared<objPrimitive>("media/AIM-9 SIDEWINDER.obj", "./media/", 0.01);
    testBullet->bind();
	
	//skybox
	unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    std::cout<<"###Models done###"<<std::endl;

    std::cout<<"---Generating texs and models---"<<std::endl;
    shipmodel->bind();
    zr->bind();
    asteroidmodel->bind();
    shiptex->createTexture(GL_RGBA);
    tex2->createTexture();
    asteroidtex->createTexture();
    //tex2spe->createTexture();
    missletex->createTexture(GL_RGB);
    std::cout<<"###Gen done###"<<std::endl;

    bakPak.enslaveModel(zr);
    bakPak.enslaveTex(tex2/*, tex2spe*/);

    ship.enslaveModel(shipmodel);
    ship.enslaveTex(shiptex, shiptexspec);

    object bull;
    bull.enslaveModel(testBullet);
    bull.enslaveTex(missletex);

    std::vector<bullet> bullets;
    std::vector<asteroida> asteroids;

    int basepos = -10;
    for(auto f = 0; f < numAst; f++)
    {
        asteroida tmp;

        tmp.internal.enslaveModel(asteroidmodel);
        tmp.internal.enslaveTex(asteroidtex);
        tmp.internal.setPosition({basepos + rand() % 111, basepos + rand() % 111, basepos - rand() % 111 }); //korekta początkowego ułożenia asteroid
        tmp.internal.setRotationAngle(static_cast<object::whichAngle>(rand() % 3), rand() % 360);
        tmp.dlaKuli.updatePosition(tmp.internal.getPosition());
        //tmp.dlaGracza.setRadius(6);
        asteroids.push_back(tmp);

        basepos -= ((rand() % 15) + 5);
    }

    bakPak.setPosition(glm::vec3( 0.0f,  0.0f,  0.0f));
    //bakPak.setRotationAngle(object::whichAngle::roll, 21);	

//////////
    win.enableZBuffer(); 
    win.setClearScreenColor(0.0f, 0.0f, 0.0f, 1.0f);

    shp->activate();

    shp->setUniform("flashlight.innerCutOff", glm::cos(glm::radians(10.0f)) );
    shp->setUniform("flashlight.outerCutOff", glm::cos(glm::radians(15.0f)) );

    shp->setUniform("flashlight.constant", 1.0f);
    shp->setUniform("flashlight.linear", 0.09f);
    shp->setUniform("flashlight.quadratic", 0.032f);

    shp->setUniform("flashlight.diffuse", glm::vec3(0.2, 0.2, 0.2));
    shp->setUniform("flashlight.ambient", glm::vec3(1.0, 1.0, 1.0));
    shp->setUniform("flashlight.specular", glm::vec3(0.8, 0.8, 0.8));

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
		
		//skybox
		glDepthMask(GL_FALSE);
		sky_shp->activate();
		sky_shp->setUniform("projection", (projection));
		sky_shp->setUniform("view", (glm::mat4(glm::mat3(cam.getViewMatrix()))));
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skytex);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		shp->activate();
		
        shp->setUniform("actCameraPos", cam.getPosition());
        shp->setUniform("staticLightPos", glm::vec3(100,0,0));
        
        //shp->setUniform("dirlight.dir", -cam.antidirection);

        shp->setUniform("view", (view));
        shp->setUniform("projection", (projection));

        shp->setUniform("usesFlashlight", isFlashlight);

        auto flashlightPosOffseted = glm::normalize(cam.antidirection);
        flashlightPosOffseted *= 8;
        flashlightPosOffseted += cam.getPosition();

        shp->setUniform("flashlight.pos",  flashlightPosOffseted);  //latarka przed statkiem
        shp->setUniform("flashlight.dir", cam.antidirection);

        camSphere.updatePosition(cam.getPosition());

        
        shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));
        shp->setUniform("model", bull.getModelMatrix());
        bull.draw();

        shp->setUniform("hasSpecularTex", true);	
		shp->setUniform("view", (glm::mat4(1.0f))); //przyklejenie statku do kamery poprzez 1 w view matrix
		glm::mat4 shipModelMat = glm::mat4(1.0f);
        shipModelMat = glm::translate(shipModelMat, glm::vec3( 0.0f,  0.0f,  -7.0f));
        shp->setUniform("model", shipModelMat);
        ship.draw();
		shp->setUniform("view", (view)); //przywrócenie normalnego view matrix
		
        shp->setUniform("hasSpecularTex", false);
            
        //test spaceport/spacestation/spacedock model
        shp->setUniform("model", bakPak.getModelMatrix());
        shp->setUniform("lightColor", glm::vec3(1.0f,1.0f,1.0f));
        bakPak.draw();
		
        /////

        auto potentialBullet = shouldMakeBullet(win, cam, shootDelayer, bull);
        if(potentialBullet.first)
        {
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

        ///////animacje asteroid
        for(auto &i : asteroids)
        {
            if(i.rotCnt > 0)
            {
                auto selAngle = static_cast<object::whichAngle>(rand() % 3);
                i.internal.setRotationAngle( selAngle, i.internal.getRotationAngle(selAngle) + cl.getDelta() * 50 );
                i.rotCnt--;
            }

        }

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

        std::pair<int, int> isThere;
        do{
            isThere = astBullCollCb();

            if(unlikely(isThere.first != -1))   //do strzelania dochodzi statystycznie żadko
            {
                auto spawn = rand() % 3 + 2;
                auto &delCandidate = asteroids[isThere.second];

                while(spawn-- && delCandidate.tty > 0)
                {
                    asteroida tmp;

                    tmp.internal.enslaveModel(asteroidmodel);
                    tmp.internal.enslaveTex(asteroidtex);
                    tmp.internal.setPosition({delCandidate.internal.getPosition().x + 1 + rand() % 3, 
                                              delCandidate.internal.getPosition().y + 1 + rand() % 3, 
                                              delCandidate.internal.getPosition().z + 1 + rand() % 3 });

                    tmp.internal.setRotationAngle(static_cast<object::whichAngle>(rand() % 3), rand() % 360);
                    tmp.tty = delCandidate.tty - 1;
                    tmp.internal.setScale(0.2 + 0.1 * tmp.tty);
                    tmp.rotCnt = 30000;	//zwiększona liczba obrotów po trafieniu
                    tmp.dlaKuli.updatePosition(tmp.internal.getPosition());
                    tmp.dlaKuli.setRadius(tmp.dlaKuli.getRadius() * tmp.internal.getScale());
                    asteroids.push_back(tmp);
                }

                bullets.erase(bullets.begin() + isThere.first);
                asteroids.erase(asteroids.begin() + isThere.second);
            }
        }while(isThere.first != -1);

        std::remove_if(bullets.begin(), bullets.end(), [&megaSphere9](bullet & b) -> bool
        {
            return b.colsp.collidesOrIsWithin(megaSphere9);
        }); //usuń pociski daleko od środka mapy

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