#ifndef GLWCONTEXT_H
#define GLWCONTEXT_H

#include "opengl.h"
#include "glm.h"
#include "glwreturncode.h"
#include "glwcamera.h"

#include <string>
#include <chrono>
#include <stdio.h>


namespace glw
{
  namespace engine
  {
    class GLWContext
    {
    public:

      GLWContext();

      ~GLWContext();

      GLWReturnCode createWindow();
      GLFWwindow* getWindow();

      void swapBuffers();
      int shouldExit();
      void destroyWindow();

      void load3DPerspective();
      void loadExternalOrtho();
      void loadOrtho();
      void loadPseudoIsometric();
      void loadHyperPerspective();

      glm::vec3* getEyePos();
      glm::vec3* getEyeLookPos();
      glm::vec3* getUp();

      glm::mat4* getModelMat();
      glm::mat4* getViewMat();
      glm::mat4* getProjMat();

      void setClearColor(const glm::vec3& color);
      void setClearColor(const glm::vec4& color);

      void clearAlpha();
      void clearDepthBuffer();
      void clearColorBuffer();
      void clearColor();
      void clearBuffers();
      void clearAll();

      void getWindowSize(glm::vec2& windowSize) const;
      glm::vec2& getWindowSize();
      void setWindowSize(glm::vec2 windowSize);

      void setEyePos(const glm::vec3& pos);
      void setEyeLookPos(const glm::vec3& pos);
      void setUp(const glm::vec3& up);

      void setCamera(GLWCamera& camera);

      float getIsometricDepth() const;
      void setIsometricDepth(const float depth);

      void startFrameTimer();
      void finishFrameTimer();

      int getFrames() const;

      void setPrintFps(bool print);
      void setFpsCap(int fpsCap);
      int getFpsCap();

      float getFps();

      void exit();

      bool isValid();

    private:

      static void FRAMEBUFFER_SIZE_CALLBACK(GLFWwindow* window, int width, int height)
      {
          glViewport(0, 0, width, height);
      }

      glm::mat4 getExternalOrtho() const;
      glm::mat4 getExternalOrthoView() const;

      glm::mat4 getOrtho() const;
      glm::mat4 getOrthoView() const;

      glm::mat4 getOrthoIsometric() const;
      glm::mat4 getOrthoIsometricView() const;

      glm::mat4 getPseudoIsometric() const;
      glm::mat4 getPseudoIsometricView() const;

      glm::mat4 getPerspective() const;
      glm::mat4 getPerspectiveView() const;

      glm::mat4 getHyperPerspective() const;
      glm::mat4 getHyperPerspectiveView() const;


      // Attributes //

      GLFWwindow * m_window;

      glm::vec2 m_windowSize;

      glm::vec3 m_eyePos;
      glm::vec3 m_eyeLookPos;
      glm::vec3 m_up;

      glm::vec4 m_clearColor;

      glm::mat4 m_model;
      glm::mat4 m_view;
      glm::mat4 m_projection;

      float m_fov;
      float m_aspectRatio;
      float m_nearZ;
      float m_farZ;
      float m_isometricDepth;

      int m_frames = 0;

      bool m_printFps = false;
      int m_fpsCap = 62;
      float m_fps = 0;

      bool m_isValid = false;

      std::chrono::system_clock::time_point m_time;

    };
  }
}




#endif // GLWCONTEXT_H
