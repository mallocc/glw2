#ifndef GLWMANAGER_H
#define GLWMANAGER_H


#include <string>
#include <stdio.h>

#include "opengl.h"
#include "glm.h"
#include "glwreturncode.h"
#include "glwcontext.h"
#include "glwshaderprogrammanager.h"
#include "glwvbomanager.h"
#include "glwfbomanager.h"
#include "glwkeyboard.h"
#include "glwmouse.h"


namespace glw
{

  //Error callback
  static void error_callback(int error, const char* description);

  typedef GLWReturnCode(*GLWLoop)(engine::GLWContext& context,
                                  engine::glsl::GLWShaderProgramManager& shaderProgramManager,
                                  engine::buffers::GLWVBOManager& vboManager,
                                  engine::buffers::GLWFBOManager& fboManager);
  typedef GLWReturnCode(*GLWInit)(engine::GLWContext& context,
                                  engine::glsl::GLWShaderProgramManager& shaderProgramManager,
                                  engine::buffers::GLWVBOManager& vboManager,
                                  engine::buffers::GLWFBOManager& fboManager);

  class GLWManager
  {
  public:

    static GLWManager& getInstance()
    {
        static GLWManager instance;
        return instance;
    }

    static GLWManager* getInstancePtr()
    {
        static GLWManager instance;
        return &instance;
    }

    GLWManager();

    ~GLWManager();

    GLWReturnCode run(
        GLWLoop loop,
        GLWInit init = NULL,
        GLFWkeyfun key_callback = NULL,
        GLFWmousebuttonfun mouse_button_callback = NULL);

    engine::glsl::GLWShaderProgramManager& getShaderProgramManager();

    engine::GLWContext& getContext();

    glw::engine::GKeyboard& getKeyboard();

    glw::engine::GMouse& getMouse();

    GLFWkeyfun& getKeyCallback();

    GLFWmousebuttonfun& getMouseCallback();

    void stop();

   private:




    static void KEY_CALLBACK(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
      if(GLFW_PRESS == action || GLFW_REPEAT == action)
      {
        getInstance().getKeyboard().updateKey(key);
      }
      (getInstance().getKeyCallback())(window, key, scancode, action, mods);
    }

    static void CHARACTER_CALLBACK(GLFWwindow* window, unsigned int codepoint)
    {
      getInstance().getKeyboard().updateChar(codepoint);
    }

    static void CURSOR_POSITION_CALLBACK(GLFWwindow* window, double xpos, double ypos)
    {
      getInstance().getMouse().updatePosition(xpos, ypos);
    }

    static void SCROLL_CALLBACK(GLFWwindow* window, double xoffset, double yoffset)
    {
      getInstance().getMouse().updateScroll(xoffset, yoffset);
    }

    static void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action, int mods)
    {
      (getInstance().getMouseCallback())(window, button, action, mods);
    }

    //GL graphics loop
    GLWReturnCode glLoop(GLWLoop loop);
    //GL window initialise
    GLWReturnCode initWindow(glw::GLWInit init);

    engine::GLWContext m_context;

    engine::glsl::GLWShaderProgramManager m_shaderProgramManager;
    engine::buffers::GLWVBOManager m_vboManager;
    engine::buffers::GLWFBOManager m_fboManager;

    GLFWkeyfun m_keyfunc;
    GLFWmousebuttonfun m_mousebuttonfunc;

    glw::engine::GKeyboard m_keyboard;
    glw::engine::GMouse m_mouse;

  };

}



#endif // GLWMANAGER_H
