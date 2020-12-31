#pragma once

#include "glm.h"

#include "glwvertex.h"
#include "glwcolor.h"

#include <vector>

#define GLW_GEN_SPHERE_DEFAULT_LATS   32U
#define GLW_GEN_SPHERE_DEFAULT_LONGS  32U
#define GLW_GEN_SPHERE_DEFAULT_SUBDIVISIONS 10

using glw::engine::buffers::GLWVertex;

//TODO comment

namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      typedef std::vector<GLWVertex> GLWVertexArray;
      typedef std::vector<glm::vec3> GLWVec3Array;
      typedef std::vector<glm::vec2> GLWVec2Array;
    	
      class GLWVBOFactory
      {
      public:
        
        GLWVBOFactory();
        
        ~GLWVBOFactory();
        
        ///shape generators non indexed triangles
        
        static void cube(GLWVec3Array& v);
        static void	cone(GLWVec3Array& v, int k);
        static void	circle(GLWVec3Array& v, int k);
        static void	centeredCircle(GLWVec3Array& v, int k);
        static void	cylinder(GLWVec3Array& v, int k, float len);
        static void cubeSphere(GLWVec3Array& v, int subdivision);
        static void	sphere(GLWVec3Array& v, int lats, int longs);
        static void	sphereInvert(GLWVec3Array& v, int lats, int longs);
        static void	rect(GLWVec3Array& v);
        static void	rects(GLWVec3Array& v, int w, int h);
        static void squareMesh(GLWVec3Array& v, int w, int h);
        static void squareMeshes(GLWVec3Array& v, int n);
        static void	centeredSquareMesh(GLWVec3Array& v, int w, int h);
        static void squareBorder(GLWVec3Array& v, float weight, float aspectRatio);

			
        static void colourBuffer(GLWVec3Array& c, glm::vec3 colour, int n);
        static void randomColourBuffer(GLWVec3Array& c, const glm::vec3& max, const int n);
        static void randomIntesityColourBuffer(GLWVec3Array& c, glm::vec3& colour, int n);

        static void nullUVs(GLWVec2Array& uv, int n);
        static void polarUVs(GLWVec2Array& uv, GLWVec3Array& v);
        static void circleUVs(GLWVec2Array& uv, GLWVec3Array& v);
        static void sphereicalUVs(GLWVec2Array& uv, GLWVec3Array& v);
        static void rectUVs(GLWVec2Array& uv, GLWVec3Array& v);
        static void squareMeshUVs(GLWVec2Array& uv, int w, int h);
        static void squareMeshUVsGUI(GLWVec2Array& uv, int w, int h);

        static void normals(GLWVec3Array& n, GLWVec3Array& v);
        static void tangents(GLWVec3Array& t, GLWVec3Array& v);
			
        static void preRotate(GLWVec3Array& v, const glm::vec3& rotate);
			
				// creates a vector of Vertices to pass to Obj
        static void packObject(GLWVertexArray& o, GLWVec3Array& v);
        static void packObject(GLWVertexArray& o, GLWVec3Array& v, GLWVec2Array& uv);
        static void packObject(GLWVertexArray& o, GLWVec3Array& v, GLWVec3Array& n);
        static void packObject(GLWVertexArray& o, GLWVec3Array& v,
                               GLWVec3Array& c, GLWVec3Array& n,
                               GLWVec3Array& t, GLWVec2Array& uv);


        static void glwSphere(GLWVertexArray& o,
                              const glm::vec3 colour  = glw::BLACK,
                              const int lats          = GLW_GEN_SPHERE_DEFAULT_LATS,
                              const int longs         = GLW_GEN_SPHERE_DEFAULT_LONGS);

        static void glwCubeSphere(GLWVertexArray& o,
                              const glm::vec3 colour  = glw::BLACK,
                              const int subdivsions   = GLW_GEN_SPHERE_DEFAULT_SUBDIVISIONS);

        static void glwCube(GLWVertexArray& o,
                            const glm::vec3 colour  = glw::BLACK);

        static void glwRectangle(GLWVertexArray& o,
                                 const glm::vec3 colour  = glw::BLACK);

        static void glwCenteredRectangle(GLWVertexArray& o,
                                         const glm::vec3 colour  = glw::BLACK);

        static void glwCircle(GLWVertexArray& o,
                                 const glm::vec3 colour  = glw::BLACK);


        //Returns random float
        static float pg_randf();
      private:
        

			  // converts polar to cartesian
        static glm::vec3 polar_cart(float theta, float phi);
			  // converts cartesian to polar
        static glm::vec2 cart_polar(glm::vec3& v);
      };
    }
  }
}
