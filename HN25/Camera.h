#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <stdio.h>

class Camera
{
public:
	glm::vec3 pos;
	glm::mat4 view;
	glm::mat4 projection;

	int screenWidth, screenHeight;

	Camera(glm::vec3 position, int sWidth, int sHeight) : pos(position), screenWidth(sWidth), screenHeight(sHeight)
	{
		updateViewMatrix();
	}

	void updateViewMatrix()
	{
		view = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void updateProjectionMatrix(float width, float height)
	{
		projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.1f, 1000.0f);
		printf("hello\n");
	}

private:

};

#endif