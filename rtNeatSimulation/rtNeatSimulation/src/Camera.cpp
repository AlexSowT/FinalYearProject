#include <Camera.h>

// Constructor with vectors
Camera::Camera(glm::vec3 position/*, glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f) */, float width, float height ) : m_movementSpeed(SPEED), m_zoom(ZOOM)
{
    m_width = width;
    m_height = height;

    m_position = position;
    m_forward = glm::vec3(0, 1, 0);
    m_right = glm::vec3(1, 0, 0);
    m_projection = glm::ortho(0.0f + m_zoom, (GLfloat)width - m_zoom, 0.0f + m_zoom, (GLfloat)height - m_zoom, -1000.f, 1000.0f);

    UpdateViewMatrix();
    //m_up = glm::vec3(0, 0, 1);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = m_movementSpeed * deltaTime;

    if (direction == FORWARD)
    {
        m_position -= m_forward * velocity;
    }

    if (direction == BACKWARD)
    {
        m_position += m_forward * velocity;
    }

    if (direction == LEFT)
    {
        m_position += m_right * velocity;
    }

    if (direction == RIGHT)
    {
        m_position -= m_right * velocity;
    }

    UpdateViewMatrix();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(GLfloat yOffset)
{
    if (m_zoom >= m_zoomMin && m_zoom <= m_zoomMax)
    {
        m_zoom -= yOffset;
    }

    if (m_zoom <= m_zoomMin)
    {
        m_zoom = m_zoomMin;
    }

    if (m_zoom >= m_zoomMax)
    {
        m_zoom = m_zoomMax;
    }

    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    m_view = glm::translate(glm::mat4(1), m_position);
    m_projection = glm::ortho(0.0f + m_zoom, (GLfloat)m_height - m_zoom, 0.0f + m_zoom, (GLfloat)m_width - m_zoom, -1000.f, 1000.0f);
}