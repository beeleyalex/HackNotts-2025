#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/string_cast.hpp>
#include <glm/glm/gtx/intersect.hpp>

#include "shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "Player.h"
#include "Tree.h"
#include "Particle.h"
#include "Enemy.h"

#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <math.h>

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_movement_callback(GLFWwindow*, double, double);
void processInput(GLFWwindow*, float);
unsigned int loadCubemap(std::vector<std::string>);

float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float floorVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

int sWidth = 1600, sHeight = 1400;

bool spacePressed = false;

Camera camera = Camera(glm::vec3(6.0f, 2.5f, 6.0f), sWidth, sHeight);
Player player = Player();
std::vector<Particle> particles;
std::vector<Enemy> enemies;

float mouseX, mouseY;
float prevX, prevY;

int main()
{
    mouseX = mouseY = prevX = prevY = 0;

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(sWidth, sHeight, "Wizzerds", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_movement_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, sWidth, sHeight);

    unsigned int EBO, VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int cubemapVAO, cubemapVBO;
    glGenVertexArrays(1, &cubemapVAO);
    glGenBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    // textures
    std::vector<std::string> cubemap;
    cubemap.push_back("../../resources/textures/advancededge.jpg");
    cubemap.push_back("../../resources/textures/advancededge.jpg");
    cubemap.push_back("../../resources/textures/advancedgrass.jpg");
    cubemap.push_back("../../resources/textures/advancedgrass.jpg");
    cubemap.push_back("../../resources/textures/advancededge.jpg");
    cubemap.push_back("../../resources/textures/advancededge.jpg");
    unsigned int cubemapTexture = loadCubemap(cubemap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("../../resources/textures/grassTex.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);

    player.init();

    Tree tree = Tree(glm::vec3(0.2f, 0.0f, 1.75f));
    tree.init();

    Shader shader = Shader("../../vertex.glsl", "../../fragment.glsl");
    Shader cubemapShader = Shader("../../cubemapVertex.glsl", "../../cubemapFragment.glsl");

    float prevTime = 0;
    float deltaTime;
    float cumulativeTimeSkeleton = 0;
    float cumulativeTimeDragon = 0;

    glm::vec3 mouseDirection;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.16f, 0.3f, 0.6f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();
        deltaTime = time - prevTime;
        cumulativeTimeSkeleton += deltaTime;
        cumulativeTimeDragon += deltaTime;
        processInput(window, deltaTime);

        glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 2.0f, -3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        float scalar = 1250.0f;
        glm::mat4 projection = glm::ortho((float)-sWidth / scalar, (float)sWidth / scalar, (float)-sHeight / scalar, (float)sHeight / scalar, -10.0f, 1000.0f);

        if (cumulativeTimeSkeleton >= 3.0f)
        {
            cumulativeTimeSkeleton = 0.0f;
            float r1 = (float) rand() / RAND_MAX;
            float r2 = (float) rand() / RAND_MAX;

            Enemy e = Enemy(Skeleton, glm::vec3(r1, 0.0f, r2));

            e.init();
            enemies.push_back(e);
        }

        if (cumulativeTimeDragon >= 10.0f)
        {
            cumulativeTimeDragon = 0.0f;
            float r1 = - (float) rand() / RAND_MAX;
            float r2 = (float) rand() / RAND_MAX;

            Enemy e = Enemy(Dragon, glm::vec3(r1, 0.0f, r2));

            e.init();
            enemies.push_back(e);
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1.5f, 0.35f, 1.5f));

        cubemapShader.activate();
        cubemapShader.setUniformMat4("model", model);
        cubemapShader.setUniformMat4("view", view);
        cubemapShader.setUniformMat4("projection", projection);

        //glBindTexture(GL_TEXTURE_2D, texture);
        //glActiveTexture(texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glBindVertexArray(cubemapVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader.activate();
        shader.setUniformMat4("view", view);
        shader.setUniformMat4("projection", projection);

        if (!player.dead)
        {
            shader.setUniformMat4("model", player.modelMatrix);
            player.draw();
        }

        shader.setUniformMat4("model", tree.modelMatrix);
        tree.draw();

        for (int i = 0; i < particles.size(); i++)
        {
            Particle& p = particles[i];
            glm::vec3 dir = player.direction;
            p.move((float) (1.2 * deltaTime) * dir);
            //p.move(glm::vec3(0.0f, 0.0f, deltaTime * 1.2));
            shader.setUniformMat4("model", p.modelMatrix);
            p.draw();
            
            if (pow(p.totalMoved, 2) >= 150)
            {
                std::cout << "hello" << std::endl;
                particles.erase(particles.begin() + i);
            }
        }

        for (int i = 0; i < enemies.size(); i++)
        {
            Enemy& e = enemies[i];
            glm::vec3 direction = glm::normalize(player.pos - e.pos);
            for (int j = 0; j < particles.size(); j++)
            {
                Particle& p = particles[j];
                if (glm::length(p.pos - e.pos) <= 0.225f)
                {
                    enemies.erase(enemies.begin() + i);
                    particles.erase(particles.begin() + i);
                }
            }

            e.move(0.175f * deltaTime * direction);
            if (glm::length(player.pos - e.pos) <= 0.1f)
            {
                player.health--;
                enemies.erase(enemies.begin() + i);
                if (player.health == 0)
                    player.dead = true;
            }

            shader.setUniformMat4("model", e.modelMatrix);
            e.draw();
        }

        prevTime = time;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float playerMoveScale = 0.25f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.move(deltaTime * glm::vec3(-playerMoveScale, 0.0f, playerMoveScale));
        player.setMoving(true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        player.setMoving(false);
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.move(deltaTime * glm::vec3(playerMoveScale, 0.0f, -playerMoveScale));
        player.setMoving(true);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player.move(deltaTime * glm::vec3(-playerMoveScale, 0.0f, -playerMoveScale));
        player.setMoving(true);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player.move(deltaTime * glm::vec3(playerMoveScale, 0.0f, playerMoveScale));
        player.setMoving(true);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed)
    {
        spacePressed = true;
        Particle p = Particle(player.pos);
        p.init();
        particles.push_back(p);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        spacePressed = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    sWidth = width;
    sHeight = height;
    camera.updateProjectionMatrix(width, height);
    
}

void mouse_movement_callback(GLFWwindow* window, double newMouseX, double newMouseY)
{
    mouseX = static_cast<float>(newMouseX);
    mouseY = static_cast<float>(newMouseY);

    // get new positions
    float xOffset = mouseX - prevX;
    float yOffset = mouseY - prevY;

    // set old positions for next call
    prevX = mouseX;
    prevY = mouseY;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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