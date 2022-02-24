#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat SPEED = 0.5f;
const GLfloat ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Constructor with vectors
    Camera(glm::vec3 position/*, glm::vec3 up*/, float width, float height );

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yOffset);

    void UpdateViewMatrix();

    glm::mat4 getViewMat() { return m_view; }
    glm::mat4 getProjectionMat() { return m_projection; }

private:
    // Camera Attributes
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_position;

    // Camera options
    GLfloat m_movementSpeed;
    GLfloat m_zoom;

    glm::mat4 m_view;
    glm::mat4 m_projection;

    float m_zoomMax = 300;
    float m_zoomMin = 1;

    float m_width;
    float m_height;
};