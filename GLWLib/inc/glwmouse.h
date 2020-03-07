#pragma once

#include "opengl.h"
#include "glm.h"


namespace glw
{
  namespace engine
  {
    class GMouse
    {
    public:
      
      GMouse()
      {}

      GMouse(GLFWwindow * window,
             GLFWmousebuttonfun mouse_button_callback = NULL,
             GLFWcursorposfun cursor_pos_callback = NULL,
             GLFWscrollfun scroll_callback = NULL)
        : m_window(window),
          m_scrollDelta(0.0f),
          m_mouseDelta(),
          m_mousePosition()
      {
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_pos_callback);
        glfwSetScrollCallback(window, scroll_callback);
      }
      
      ~GMouse()
      {
      }
      
      bool isLeftButtonDown()
      {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
      }

      bool isRightButtonDown()
      {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
      }

      bool isMiddleButtonDown()
      {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
      }

      glm::vec2 getMousePosition()
      {
        return m_mousePosition;
      }

      glm::vec2 getMouseDelta()
      {
        return m_mouseDelta;
      }

      float getScrollDelta()
      {
        return m_scrollDelta;
      }

      glm::vec2 popMouseDelta()
      {
        glm::vec2 delta = m_mouseDelta;
        m_mouseDelta = glm::vec2();
        return delta;
      }

      float popScrollDelta()
      {
        float delta = m_scrollDelta;
        m_scrollDelta = 0.0f;
        return delta;
      }

      void updatePosition(double xpos, double ypos)
      {
        glm::vec2 pos(xpos, ypos);
        glm::vec2 delta = pos - m_mousePosition;
        m_mouseDelta = delta;
        m_mousePosition = pos;
      }

      void updateScroll(double xoffset, double yoffset)
      {
        m_scrollDelta += (float)yoffset;
      }


      void updateMouse()
      {
        if (NULL != m_window)
        {
          double xpos, ypos;
          glfwGetCursorPos(m_window, &xpos, &ypos);
          glm::vec2 pos(xpos, ypos);
          glm::vec2 delta = pos - m_mousePosition;
          m_mouseDelta = delta;
          m_mousePosition = pos;
        }
      }

    private:
      GLFWwindow * m_window;

      glm::vec2 m_mousePosition;
      glm::vec2 m_mouseDelta;
      float m_scrollDelta;
    };
  }
}
