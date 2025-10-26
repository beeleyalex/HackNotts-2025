#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <math.h>

#include "stb_image.h"

class Particle
{
public:
	glm::vec3 pos;
    glm::mat4 modelMatrix;
    glm::vec3 direction;
    float totalMoved = 1.0f;

	Particle(glm::vec3 position, glm::vec3 dir) : pos(position), direction(dir)
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle::vertices), Particle::vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Particle::indices), Particle::indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // still texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char* data = stbi_load("../../resources/textures/fireball.jpg", &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Failed to load texture" << std::endl;

        stbi_image_free(data);
    }

    void move(float speed)
    {
        totalMoved += glm::length((float) speed * direction);
        pos += (float) speed * direction;
        updateModelMatrix();
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
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, pos);
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.65f / pow(totalMoved, 1.5)));
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

};

#endif