#ifndef ENEMY_H
#define ENEMY_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <math.h>

#include "stb_image.h"

enum EnemyType
{
    Skeleton, Dragon
};

class Enemy
{
public:
    glm::vec3 pos;
    glm::mat4 modelMatrix;

    Enemy(EnemyType eType, glm::vec3 position) : pos(position), type(eType)
    {
        updateModelMatrix();
    }

    void init()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Enemy::vertices), Enemy::vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Enemy::indices), Enemy::indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // still texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        std::string filepath = "";
        if (type == Dragon)
            filepath = "../../resources/textures/dragon.jpg";
        else if (type == Skeleton)
            filepath = "../../resources/textures/skelton-sword.jpg";


        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Failed to load texture" << std::endl;

        stbi_image_free(data);
    }

    void draw()
    {
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void updateModelMatrix()
    {
        float scaleFactor = 0.4f;
        if (type == Dragon)
            scaleFactor = 0.75f;

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
    }

    void move(glm::vec3 vector)
    {
        pos += vector;
        updateModelMatrix();
    }

    static constexpr float vertices[] = {
        // positions        // texture coords
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left 
    };

    static constexpr int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

private:
    unsigned int VBO, VAO, EBO, texture;
    EnemyType type;
};

#endif