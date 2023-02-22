////
////#ifndef CAMERA_H
////#define CAMERA_H
////
//////#include "glad/glad.h"
////#include "glm/glm.hpp"
////#include "glm/gtc/matrix_transform.hpp"
////#include <glm.hpp>
////#include <vector>
////
////// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
////enum Camera_Movement {
////	FORWARD,
////	BACKWARD,
////	LEFT,
////	RIGHT
////};
////
////// Default camera values
////const float YAW = -90.0f;
////const float PITCH = 0.0f;
////const float SPEED = 2.5f;
////const float SENSITIVITY = 0.1f;
////const float ZOOM = 45.0f;
////
////
////// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
////class Camera
////{
////public:
////	// camera Attributes
////	glm::vec3 Position;
////	glm::vec3 Front;
////	glm::vec3 Up;
////	glm::vec3 Right;
////	glm::vec3 WorldUp;
////	// euler Angles
////	float Yaw;
////	float Pitch;
////	// camera options
////	float MovementSpeed;
////	float MouseSensitivity;
////	float Zoom;
////
////	// constructor with vectors
////	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
////	{
////		Position = position;
////		WorldUp = up;
////		Yaw = yaw;
////		Pitch = pitch;
////		updateCameraVectors();
////	}
////	// constructor with scalar values
////	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
////	{
////		Position = glm::vec3(posX, posY, posZ);
////		WorldUp = glm::vec3(upX, upY, upZ);
////		Yaw = yaw;
////		Pitch = pitch;
////		updateCameraVectors();
////	}
////
////	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
////	glm::mat4 GetViewMatrix()
////	{
////		return glm::lookAt(Position, Position + Front, Up);
////	}
////
////	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
////	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
////	{
////		float velocity = MovementSpeed * deltaTime;
////		if (direction == FORWARD)
////			Position += Front * velocity;
////		if (direction == BACKWARD)
////			Position -= Front * velocity;
////		if (direction == LEFT)
////			Position -= Right * velocity;
////		if (direction == RIGHT)
////			Position += Right * velocity;
////	}
////
////	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
////	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
////	{
////		xoffset *= MouseSensitivity;
////		yoffset *= MouseSensitivity;
////
////		Yaw += xoffset;
////		Pitch += yoffset;
////
////		// make sure that when pitch is out of bounds, screen doesn't get flipped
////		if (constrainPitch)
////		{
////			if (Pitch > 89.0f)
////				Pitch = 89.0f;
////			if (Pitch < -89.0f)
////				Pitch = -89.0f;
////		}
////
////		// update Front, Right and Up Vectors using the updated Euler angles
////		updateCameraVectors();
////	}
////
////	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
////	void ProcessMouseScroll(float yoffset)
////	{
////		Zoom -= (float)yoffset;
////		if (Zoom < 1.0f)
////			Zoom = 1.0f;
////		if (Zoom > 45.0f)
////			Zoom = 45.0f;
////	}
////
////private:
////	// calculates the front vector from the Camera's (updated) Euler Angles
////	void updateCameraVectors()
////	{
////		// calculate the new Front vector
////		glm::vec3 front;
////		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
////		front.y = sin(glm::radians(Pitch));
////		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
////		Front = glm::normalize(front);
////		// also re-calculate the Right and Up vector
////		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
////		Up = glm::normalize(glm::cross(Right, Front));
////	}
////};
////#endif
////
////
//////
//////#ifndef CAMERA_H
//////#define CAMERA_H
//////#include <cmath>
//////#include <iostream>
//////#include "SDL.h"
//////#include <GL/gl.h>
//////#include <GL/glu.h>
//////#include "vector3d.h"
//////
//////class camera {
//////	vector3d loc;
//////	float camPitch, camYaw;
//////	float movevel;
//////	float mousevel;
//////	bool mi, ismoved;
//////	void lockCamera();
//////	void moveCamera(float dir);
//////	void moveCameraUp(float dir);
//////public:
//////	camera();
//////	camera(vector3d loc);
//////	camera(vector3d loc, float yaw, float pitch);
//////	camera(vector3d loc, float yaw, float pitch, float mv, float mov);
//////	void Control();
//////	void UpdateCamera();
//////	vector3d getVector();
//////	vector3d getLocation();
//////	float getPitch();
//////	float getYaw();
//////	float getMovevel();
//////	float getMousevel();
//////	bool isMouseIn();
//////
//////	void setLocation(vector3d vec);
//////	void lookAt(float pitch, float yaw);
//////	void mouseIn(bool b);
//////	void setSpeed(float mv, float mov);
//////
//////	bool isMoved();
//////};
//////
//////#endif
//#include<iostream>
//
//#include<glew.h>
//#include<glfw3.h>
//
//#include<glm.hpp>
//#include<vec3.hpp>
//#include<mat4x4.hpp>
//#include<gtc\matrix_transform.hpp>
//
//enum direction { FORWARD = 0, BACKWARD, LEFT, RIGHT };
//
//class Camera
//{
//private:
//	glm::mat4 ViewMatrix;
//
//	GLfloat movementSpeed;
//	GLfloat sensitivity;
//
//	glm::vec3 worldUp;
//	glm::vec3 position;
//	glm::vec3 front;
//	glm::vec3 right;
//	glm::vec3 up;
//
//	GLfloat pitch;
//	GLfloat yaw;
//	GLfloat roll;
//
//	void updateCameraVectors()
//	{
//		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
//		this->front.y = sin(glm::radians(this->pitch));
//		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
//
//		this->front = glm::normalize(this->front);
//		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
//		this->up = glm::normalize(glm::cross(this->right, this->front));
//	}
//
//public:
//	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
//	{
//		this->ViewMatrix = glm::mat4(1.f);
//
//		this->movementSpeed = 3.f;
//		this->sensitivity = 5.f;
//
//		this->worldUp = worldUp;
//		this->position = position;
//		this->right = glm::vec3(0.f);
//		this->up = worldUp;
//
//		this->pitch = 0.f;
//		this->yaw = -90.f;
//		this->roll = 0.f;
//
//		this->updateCameraVectors();
//	}
//
//	~Camera() {}
//
//	//Accessors
//	const glm::mat4 getViewMatrix()
//	{
//		this->updateCameraVectors();
//
//		this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
//
//		return this->ViewMatrix;
//	}
//
//	const glm::vec3 getPosition() const
//	{
//		return this->position;
//	}
//
//	//Functions
//	void move(const float& dt, const int direction)
//	{
//		//Update position vector
//		switch (direction)
//		{
//		case FORWARD:
//			this->position += this->front * this->movementSpeed * dt;
//			break;
//		case BACKWARD:
//			this->position -= this->front * this->movementSpeed * dt;
//			break;
//		case LEFT:
//			this->position -= this->right * this->movementSpeed * dt;
//			break;
//		case RIGHT:
//			this->position += this->right * this->movementSpeed * dt;
//			break;
//		default:
//			break;
//		}
//	}
//
//	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
//	{
//		//Update pitch yaw and roll
//		this->pitch += static_cast<GLfloat>(offsetY) * this->sensitivity * dt;
//		this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * dt;
//
//		if (this->pitch > 80.f)
//			this->pitch = 80.f;
//		else if (this->pitch < -80.f)
//			this->pitch = -80.f;
//
//		if (this->yaw > 360.f || this->yaw < -360.f)
//			this->yaw = 0.f;
//	}
//
//	void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
//	{
//		this->updateMouseInput(dt, offsetX, offsetY);
//	}
//
//};