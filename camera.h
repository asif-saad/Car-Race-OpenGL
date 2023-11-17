//
//  camera.h
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include <vector>
using namespace std;
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    NO_PRESS
};

// Default camera values
const float YAW = -90;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float vm = 0.1;
    float kf = 0.2;
    float kb = 0.05;
    float time = 0;
    double e = 2.71828;
    float yaw_abs;
    float x, z, y;
    float car_back_x, car_back_z;
    float rotation_angle;
    float max = 2.5;      // maximum forward speed
    float acc = 0.05;    // acceleration
    float dec = 0.001;   // deceleration
    float decc = 0.008;  // deceleration
    float velocity = 0; // speed
    float min = -0.008;     // maximum backward speed
    float rotation_coefficient = 0;
    bool collision = false;
    bool collision_car = false;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, -2, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        yaw_abs = normalizeAngle((int)Yaw);
        Pitch = pitch;
        x = Position.x;
        y = Position.y;
        z = Position.z - 5;
        car_back_x = x;
        car_back_z = z + 0.1;
        rotation_angle = yaw_abs;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, 
        float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), 
        Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        yaw_abs = normalizeAngle((int)Yaw);
        x = Position.x;
        y = Position.y - 0.5;
        z = Position.z - 2;
        car_back_x = x;
        car_back_z = z + 0.1;
        rotation_angle = yaw_abs;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        rotation_coefficient = 1;
        if (direction == FORWARD)
        {
            if (velocity < max) {
                velocity += acc;
            }
            if (collision_detect(Position + Front * (velocity / 10))) {
                velocity *= 0.1;
            }
            Position += Front * (velocity / 10);
            car_rotation();

        }
        /*if (direction == FORWARD) {
            velocity = 0.2;
            if (collision_detect(Position + Front * velocity)) {
                velocity *= 0.1;
            }
            Position += Front * velocity;
            car_rotation();
        }*/
        if (direction == BACKWARD)
        {
            if (velocity > min) {
                velocity -= decc;
            }
            if (collision_detect(Position + Front * velocity)) {
                velocity *= 0.1;
            }
            Position += Front * velocity;
            updateCameraVectors();
            car_rotation();
        }
        /*if (direction == BACKWARD)
        {
            velocity -= 0.1;
            if (collision_detect(Position + Front * velocity)) {
                velocity *= 0.1;
            }
            Position += Front * velocity;
            car_rotation();
        }*/
        if (direction == LEFT) {
            rotation_coefficient = 375;
            Yaw -= 1;           
            yaw_abs = normalizeAngle((int)Yaw);
            car_rotation();

        }
        if (direction == RIGHT) {
            rotation_coefficient = -15;
            Yaw += 1;
            yaw_abs = normalizeAngle((int)Yaw);
            car_rotation();

        }
        /*else {
            if (velocity > 0) {
                velocity -= dec;
            }
            else {
                velocity = 0;
            }
            Position += Front * velocity;
            car_rotation();
        }*/
        updateCameraVectors();
    }

    bool collision_detect(glm:: vec3 temp) {
        float temp_x;
        if (temp.z > -145) {
            if (temp.x >= 26 || temp.x < -2) {
                return true;
             }
            return false;
        }
        else if (temp.z <= -145 && temp.z > -277) {
            temp_x=(temp.z - (-145 - tan(glm::radians(135.0f)) * 26)) / tan(glm::radians(135.0f));
            if (temp.x > temp_x || temp.x < (temp_x - 1.414 * 28)) {
                return true;
            }
            return false;
        }
        else if (temp.z <= -277 && temp.z > -479) {
            temp_x = 156;
            if (temp.x >= temp_x || temp.x < temp_x - 28) {
                return true;
            }
            return false;

        }
        else if (temp.z <= -479 && temp.z > -615) {
            temp_x = (temp.z - (-479 - tan(glm::radians(45.0f)) * 157)) / tan(glm::radians(45.0f));

            if (temp.x > temp_x || temp.x < (temp_x - 1.414 * 28)) {
                return true;
            }
            return false;
        }
    }

    int normalizeAngle(int angle) {
        // Normalize the angle to the range [0, 359]
        angle = angle % 360;

        // Make sure the result is non-negative
        if (angle < 0) {
            angle += 360;
        }

        return angle;
    }

    void car_rotation() {
        if (yaw_abs >= 0 && yaw_abs < 90) {
            x = Position.x + cos(glm::radians(yaw_abs)) * 5;
            z = Position.z + sin(glm::radians(yaw_abs)) * 5;
            car_back_x = x;
            car_back_z = z-0.1;

            //cout << 1 << endl;
        }
        if (yaw_abs >= 90 && yaw_abs < 180) {
            x = Position.x - cos(glm::radians(180 - yaw_abs)) * 5;
            z = Position.z + sin(glm::radians(180 - yaw_abs)) * 5;
            car_back_x = x;
            car_back_z = z - 0.1;

            //cout << 2 << endl;
            //std::cout << x <<" "<<y<< std::endl;
        }
        else if (yaw_abs >= 180 && yaw_abs < 270) {
            x = Position.x - cos(glm::radians(yaw_abs - 180)) * 5;
            z = Position.z - sin(glm::radians(yaw_abs - 180)) * 5;
            car_back_x = x;
            car_back_z = z + 0.1;

            //rotation_angle =  yaw_abs -180;
            //cout << 3 << endl;
            //std::cout << "ok" << std::endl;
        }
        else if (yaw_abs >= 270 && yaw_abs < 360) {
            x = Position.x + cos(glm::radians(360 - yaw_abs)) * 5;
            z = Position.z - sin(glm::radians(360 - yaw_abs)) * 5;
            car_back_x = x;
            car_back_z = z + 0.1;

            //cout << 4 << endl;
            //std::cout << "ok" << std::endl;
        }
        rotation_angle = 360 - yaw_abs;
        //std::cout << Yaw << std::endl;
        //std::cout << x << " " << z <<" "<<yaw_abs<< std::endl;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
#pragma once
