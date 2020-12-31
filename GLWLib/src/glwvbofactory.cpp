#include "glwvbofactory.h"
#include "glwvertex.h"
#include "glwcolor.h"
#include "stringformat.h"
#include "logger.h"


LDEFINECLASS("GLW", "GLWVGOFactory")


using glw::engine::buffers::GLWVBOFactory;
using glw::engine::buffers::GLWVertexArray;
using glw::engine::buffers::GLWVertex;
using glw::engine::buffers::GLWVec3Array;
using glw::engine::buffers::GLWVec2Array;

using glw::util::StringFormat;


namespace
{
  
  const float PI = 3.141596f;
  const float PI2 = PI * 2.0f;
  
  //Cube vertex data array
  const float CUBE_V_B[] = {
	  -1.0f,-1.0f,-1.0f,
	  -1.0f,-1.0f, 1.0f,
	  -1.0f, 1.0f, 1.0f,
	   1.0f, 1.0f,-1.0f,
	  -1.0f,-1.0f,-1.0f,
	  -1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f, 1.0f,
	  -1.0f,-1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	  -1.0f,-1.0f,-1.0f,
	  -1.0f,-1.0f,-1.0f,
	  -1.0f, 1.0f, 1.0f,
	  -1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f, 1.0f,
	  -1.0f,-1.0f, 1.0f,
	  -1.0f,-1.0f,-1.0f,
	  -1.0f, 1.0f, 1.0f,
	  -1.0f,-1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f, 1.0f,-1.0f,
	  -1.0f, 1.0f,-1.0f,
	   1.0f, 1.0f, 1.0f,
	  -1.0f, 1.0f,-1.0f,
	  -1.0f, 1.0f, 1.0f,
	   1.0f, 1.0f, 1.0f,
	  -1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f
  };
}

GLWVBOFactory::GLWVBOFactory()
{
}


GLWVBOFactory::~GLWVBOFactory()
{
}

void GLWVBOFactory::cube(GLWVec3Array& v)
{
  for (int i = 0; i < 36; i++)
	{
	  v.push_back(glm::vec3(CUBE_V_B[i * 3], CUBE_V_B[i * 3 + 1], CUBE_V_B[i * 3 + 2]));
	}
}
void GLWVBOFactory::cone(GLWVec3Array& v, int k)
{
	float step = PI2 / float(k);
	float c = 0.0f, s = 0.0f;
	float len = 2.0f;
	//cone
	for (float a = 0; a <= PI2; a += step)
	{
		v.push_back(glm::vec3());
		c = cos(a);
		s = sin(a);
		v.push_back(glm::vec3(c, s, len));
		c = cos(a - step);
		s = sin(a - step);
		v.push_back(glm::vec3(c, s, len));
	}
	//circle
	for (float a = 0; a <= PI2; a += step)
	{
		c = cos(a - step);
		s = sin(a - step);
		v.push_back(glm::vec3(c, s, len));
		c = cos(a);
		s = sin(a);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(0.0f, 0.0f, len));
	}
}
void GLWVBOFactory::circle(GLWVec3Array& v, int k)
{
	 float step = PI2 / float(k);
	 float c = 0.0f, s = 0.0f;
	 float len = 0.0f;
   for (float a = step; a <= PI2; a += step)
	 {
		 c = cos(a - step);
		 s = sin(a - step);
		 v.push_back(glm::vec3(c, s, len));
		 c = cos(a);
		 s = sin(a);
		 v.push_back(glm::vec3(c, s, len));
		 v.push_back(glm::vec3(0.0f, 0.0f, len));
	 }
}
void GLWVBOFactory::centeredCircle(GLWVec3Array& v, int k)
{
	 float step = PI2 / float(k);
	 float c = 0.0f, s = 0.0f;
	 float len = 0.0f;
	 for (float a = 0; a <= PI2; a += step)
	 {
		 c = cos(a - step);
		 s = sin(a - step);
		 v.push_back(glm::vec3(c, s, len) / 2.0f + glm::vec3(0.5f,0.5f,0));
		 c = cos(a);
		 s = sin(a);
		 v.push_back(glm::vec3(c, s, len) / 2.0f + glm::vec3(0.5f, 0.5f, 0));
		 v.push_back(glm::vec3(0.0f, 0.0f, len) / 2.0f + glm::vec3(0.5f, 0.5f, 0));
	 }
}
void GLWVBOFactory::cylinder(GLWVec3Array& v, int k, float len)
{
	glm::vec3 t1, t2;
	float step = PI2 / float(k);
	float Radius = 1., c = 0., s = 0.;
	for (float a = 0; a <= PI2; a += step)
	{
		v.push_back(glm::vec3(0., 0., 0));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(glm::vec3(c, s, 0));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(glm::vec3(c, s, 0));
	}
	for (float a = 0; a > -PI2; a -= step)
	{
		c = Radius * cos(a);
		s = Radius * sin(a);
		t1 = glm::vec3(c, s, 0);
		t2 = glm::vec3(c, s, len);
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);

		v.push_back(t1);
		v.push_back(t2);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(c, s, 0));
		v.push_back(t1);
	}
	for (float a = 0; a <= PI2; a += step)
	{
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(glm::vec3(c, s, len));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(0., 0., len));
	}
}

static glm::vec3 getCubeSpherePos(int n, int x, int y)
{
  const float DEG2RAD = acos(-1) / 180.0f;

  // normal for latitudinal plane
  // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
  // therefore, it is rotating (0,1,0) vector by latitude angle a2
  float a2 = DEG2RAD * (45.0f - 90.0f * x / ((float)n - 1));
  glm::vec3 n2 = glm::vec3(-sin(a2), cos(a2), 0);

  // normal for longitudinal plane
  // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
  // therefore, it is rotating (0,0,-1) vector by longitude angle a1
  float a1 = DEG2RAD * (-45.0f + 90.0f * y / ((float)n - 1));
  glm::vec3 n1 = glm::vec3(-sin(a1), 0, -cos(a1));

  // find direction vector of intersected line, n1 x n2
  glm::vec3 v1 = glm::vec3(n1.y * n2.z - n1.z * n2.y,
                n1.z * n2.x - n1.x * n2.z,
                n1.x * n2.y - n1.y * n2.x);

  // normalize direction vector
  v1 = glm::normalize(v1);

  return v1;
}

static void moldCubeSphereFace(GLWVec3Array& v, int subdivision)
{
  // compute the number of vertices per row, 2^n + 1
  int pointsPerRow = subdivision;

  // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
  for(unsigned int i = 0; i < pointsPerRow - 1; ++i)
  {
    // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
    for(unsigned int j = 0; j < pointsPerRow - 1; ++j)
    {
      // add a vertex into array
      v.push_back(getCubeSpherePos(pointsPerRow, i, j));
      v.push_back(getCubeSpherePos(pointsPerRow, i + 1, j));
      v.push_back(getCubeSpherePos(pointsPerRow, i, j + 1));

      v.push_back(getCubeSpherePos(pointsPerRow, i, j + 1));
      v.push_back(getCubeSpherePos(pointsPerRow, i + 1, j));
      v.push_back(getCubeSpherePos(pointsPerRow, i + 1, j + 1));
    }
  }
}
void GLWVBOFactory::cubeSphere(GLWVec3Array& v, int subdivision)
{
  GLWVec3Array meshFace;

  moldCubeSphereFace(meshFace, subdivision);
  v.insert(v.end(), meshFace.begin(), meshFace.end());
  meshFace.clear();

  moldCubeSphereFace(meshFace, subdivision);
  preRotate(meshFace, glm::vec3(0,0,PI/2));
  v.insert(v.end(), meshFace.begin(), meshFace.end());
  meshFace.clear();

  moldCubeSphereFace(meshFace, subdivision);
  preRotate(meshFace, glm::vec3(0,0,PI * 3.0f/2.0f));
  v.insert(v.end(), meshFace.begin(), meshFace.end());
  meshFace.clear();

  moldCubeSphereFace(meshFace, subdivision);
  preRotate(meshFace, glm::vec3(0,PI/2,0));
  v.insert(v.end(), meshFace.begin(), meshFace.end());
  meshFace.clear();

  moldCubeSphereFace(meshFace, subdivision);
  preRotate(meshFace, glm::vec3(0,PI * 3.0f/2.0f,0));
  v.insert(v.end(), meshFace.begin(), meshFace.end());
  meshFace.clear();

  moldCubeSphereFace(meshFace, subdivision);
  preRotate(meshFace, glm::vec3(0,PI,0));
  v.insert(v.end(), meshFace.begin(), meshFace.end());
  meshFace.clear();

}
void GLWVBOFactory::sphere(GLWVec3Array& v, int lats, int longs)
{
  float step_lats = PI / float(lats);
  float step_longs = PI2 / float(longs);
  for (float a = -PI; a <= PI; a += step_lats)
  {
    for (float b = 0; b <= PI2; b += step_longs)
    {
      v.push_back(polar_cart(a, b));
      v.push_back(polar_cart(a + step_lats, b));
      v.push_back(polar_cart(a + step_lats, b + step_longs));
      v.push_back(polar_cart(a + step_lats, b + step_longs));
      v.push_back(polar_cart(a, b + step_longs));
      v.push_back(polar_cart(a, b));
    }
  }
}
void GLWVBOFactory::sphereInvert(GLWVec3Array& v, int lats, int longs)
{
	float step_lats = PI2 / float(lats);
	float step_longs = PI2 / float(longs);
	float Radius = 1., x, y, z;
  for (float a = 0; a <= PI2; a += step_lats)
	{
    for (float b = 0; b <= PI2; b += step_longs)
		{
			v.push_back(polar_cart(a, b));
			v.push_back(polar_cart(a, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b));
			v.push_back(polar_cart(a, b));
		}
	}
}
void GLWVBOFactory::rect(GLWVec3Array& v)
{
	v.push_back(glm::vec3(1, 0, 1));
	v.push_back(glm::vec3(1, 0, -1));
	v.push_back(glm::vec3(-1, 0, -1));
	v.push_back(glm::vec3(-1, 0, -1));
	v.push_back(glm::vec3(-1, 0, 1));
	v.push_back(glm::vec3(1, 0, 1));
}
void GLWVBOFactory::rects(GLWVec3Array& v, int w, int h)
{
	glm::vec3
		s = glm::vec3(1 / (float)w, 0, 1 / (float)h),
		a = glm::vec3(1, 0, 1) * s,
		b = glm::vec3(1, 0, -1) * s,
		c = glm::vec3(-1, 0, -1) * s,
		d = glm::vec3(-1, 0, 1) * s;
	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < h; ++y)
		{
			glm::vec3 t = s * glm::vec3(x, 0, y) - glm::vec3(0.5f, 0, 0.5f);
			v.push_back(a + t);
			v.push_back(b + t);
			v.push_back(c + t);
			v.push_back(c + t);
			v.push_back(d + t);
			v.push_back(a + t);
		}
	}
}     
void GLWVBOFactory::squareMesh(GLWVec3Array& v, int w, int h)
{
	glm::vec3
		s = glm::vec3(1 / (float)w, 1 / (float)h, 0),
		a = glm::vec3(0, 0, 0) * s,
		b = glm::vec3(0, 1, 0) * s,
		c = glm::vec3(1, 1, 0) * s,
		d = glm::vec3(1, 0, 0) * s;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			glm::vec3 t = s * glm::vec3(x, y, 0);
			v.push_back(a + t);
			v.push_back(b + t);
			v.push_back(c + t);
			v.push_back(c + t);
			v.push_back(d + t);
			v.push_back(a + t);
		}
	}
}
void GLWVBOFactory::squareMeshes(GLWVec3Array& v, int n)
{
	glm::vec3
		a = glm::vec3(0, 0, 0),
		b = glm::vec3(0, 1, 0),
		c = glm::vec3(1, 1, 0),
		d = glm::vec3(1, 0, 0);
	for (int x = 0; x < n; ++x)
	{
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(c);
		v.push_back(d);
		v.push_back(a);
	}
}  
void GLWVBOFactory::centeredSquareMesh(GLWVec3Array& v, int w, int h)
{
  glm::vec3
    s = glm::vec3(1 / (float)w, 1 / (float)h, 0),
    a = glm::vec3(0, 0, 0) * s,
    b = glm::vec3(0, 1, 0) * s,
    c = glm::vec3(1, 1, 0) * s,
    d = glm::vec3(1, 0, 0) * s;
  for (int y = 0; y < h; ++y)
  {
    for (int x = 0; x < w; ++x)
    {
     glm::vec3 t = s * glm::vec3(x, y, 0) - glm::vec3(0.5f, 0.5f, 0);
     v.push_back(a + t);
     v.push_back(b + t);
     v.push_back(c + t);
     v.push_back(c + t);
     v.push_back(d + t);
     v.push_back(a + t);
    }
  }
}
void GLWVBOFactory::squareBorder(GLWVec3Array& v, float weight, float aspectRatio)
{
  glm::vec3 a, b, c, d;

  // left
  a = glm::vec3(0, 0, 0);
  b = glm::vec3(0, 1, 0);
  c = glm::vec3(weight / aspectRatio, 1, 0);
  d = glm::vec3(weight / aspectRatio, 0, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);

  //top
  a = glm::vec3(0, 1 - weight * aspectRatio, 0);
  b = glm::vec3(0, 1, 0);
  c = glm::vec3(1, 1, 0);
  d = glm::vec3(1, 1 - weight * aspectRatio, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);

  //right
  a = glm::vec3(1 - weight / aspectRatio, 0, 0);
  b = glm::vec3(1 - weight / aspectRatio, 1, 0);
  c = glm::vec3(1, 1, 0);
  d = glm::vec3(1, 0, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);

  //bottom
  a = glm::vec3(0, 0, 0);
  b = glm::vec3(0, weight * aspectRatio, 0);
  c = glm::vec3(1, weight * aspectRatio, 0);
  d = glm::vec3(1, 0, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);
}


void GLWVBOFactory::colourBuffer(GLWVec3Array& c, glm::vec3 colour, int n)
{
	for (int i = 0; i < n; i++)
	{
	  c.push_back(colour);
  }
}

void GLWVBOFactory::randomColourBuffer(GLWVec3Array& c, const glm::vec3& max, const int n)
{
	for (int i = 0; i < n; i++)
	{
	  c.push_back(glm::vec3(max.x*pg_randf(), max.y*pg_randf(), max.z*pg_randf()));
  }
}

void GLWVBOFactory::randomIntesityColourBuffer(GLWVec3Array& c, glm::vec3& colour, int n)
{
	float f;
	for (int i = 0; i < n; i++)
	{
		f = pg_randf();
		c.push_back(glm::vec3(colour.x*f, colour.y*f, colour.z*f));
	}
}

void GLWVBOFactory::nullUVs(GLWVec2Array& uv, int n)
{
	for (int i = 0; i < n; i++)
	{
		uv.push_back(glm::vec2());
  }
}

void GLWVBOFactory::polarUVs(GLWVec2Array& uv, GLWVec3Array& v)
{
  for (int i = 0; i < v.size(); i++)
	{
		uv.push_back(cart_polar(v[i]));
  }
}

void GLWVBOFactory::circleUVs(GLWVec2Array& uv, GLWVec3Array& v)
{
  for (int i = 0; i < v.size(); i++)
  {
    uv.push_back(glm::vec2(v[i].x, v[i].y));
  }
}

void GLWVBOFactory::sphereicalUVs(GLWVec2Array& uv, GLWVec3Array& v)
{
  glm::vec2 previous;
  glm::vec2 previous2;
  glm::vec2 next;
  glm::vec2 next2;

  for (int i = 0; i < v.size(); i++)
  {
    next2 = glm::vec2(
          atan2(v[i].z, v[i].x) / (glm::pi<float>()*2.0f) + 0.5f,
          asin(v[i].y)          / glm::pi<float>() - 0.5f);


    next = previous = next2;

    switch (i % 6)
    {
      case 2:
      case 5:
        if (next.x - previous.x > 0.7f)
        {
//          LDEBUG(StringFormat("moved").str());
          next.x = next.x + 1.0f;
        }
        break;
      case 3:
        if (previous2.x - next.x > 0.7f)
        {
//          LDEBUG(StringFormat("moved").str());
          next.x = next.x + 1.0f;
        }
        break;
      case 4:
        if (previous.x - next.x > 0.7f)
        {
//          LDEBUG(StringFormat("moved").str());
          next.x = next.x + 1.0f;
        }
      case 0:
        previous2 = next;
        break;
    }

    uv.push_back(next);
//    LDEBUG(StringFormat("UV = %0 %1").arg(next.x).arg(next.y).str());
  }
}

void GLWVBOFactory::rectUVs(GLWVec2Array& uv, GLWVec3Array& v)
{
  for (int i = 0; i < v.size(); i += 6)
  {
    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(1, 0));
    uv.push_back(glm::vec2(1, 1));
    uv.push_back(glm::vec2(1, 1));
    uv.push_back(glm::vec2(0, 1));
    uv.push_back(glm::vec2(0, 0));
  }
}

void GLWVBOFactory::squareMeshUVs(GLWVec2Array& uv, int w, int h)
{
  glm::vec2
    s = glm::vec2(1 / (float)w, 1 / (float)h),
    a = glm::vec2(0, 0) * s,
    b = glm::vec2(0, 1) * s,
    c = glm::vec2(1, 1) * s,
    d = glm::vec2(1, 0) * s;

  for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
      glm::vec2 t = s * glm::vec2(x, y);
      uv.push_back(a + t);
      uv.push_back(b + t);
      uv.push_back(c + t);
      uv.push_back(c + t);
      uv.push_back(d + t);
      uv.push_back(a + t);
    }
}

void GLWVBOFactory::squareMeshUVsGUI(GLWVec2Array& uv, int w, int h)
{
  glm::vec2
    s = glm::vec2(1 / (float)w, 1 / (float)h),
    a = glm::vec2(0, 1) * s,
    b = glm::vec2(0, 0) * s,
    c = glm::vec2(1, 0) * s,
    d = glm::vec2(1, 1) * s;

  for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
      glm::vec2 t = s * glm::vec2(x, y);
      uv.push_back(a + t);
      uv.push_back(b + t);
      uv.push_back(c + t);
      uv.push_back(c + t);
      uv.push_back(d + t);
      uv.push_back(a + t);
    }
}

void GLWVBOFactory::normals(GLWVec3Array& n, GLWVec3Array& v)
{
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3 nm = glm::normalize(glm::cross(v[i + 1] - v[i], v[i + 2] - v[i]));
		for (int j = 0; j < 3; ++j)
		{
			n.push_back(nm);
	  }
	}
}

void GLWVBOFactory::tangents(GLWVec3Array& t, GLWVec3Array& v)
{
	for (int i = 0; i < v.size(); i += 6)
	{
		glm::vec3 nm = glm::normalize(v[i + 2] - v[i]);
		for (int j = 0; j < 6; ++j)
			t.push_back(nm);
	}
}

void GLWVBOFactory::preRotate(GLWVec3Array& v, const glm::vec3& rotate)
{
	for (glm::vec3& V : v)
	{
    V = glm::quat(rotate) * V;
  }
}


// creates a vector of Vertices to pass to Obj
void GLWVBOFactory::packObject(GLWVertexArray& o, GLWVec3Array& v)
{
  GLWVec2Array uv;
  packObject(o, v, uv);
}

void GLWVBOFactory::packObject(GLWVertexArray& o, GLWVec3Array& v, GLWVec2Array& uv)
{
  if(o.size() > 0)
  {
    o.clear();
  }

  GLWVec3Array c, n, t;
  glm::vec3 color = glm::vec3(0.5f);

  if(uv.size() == 0)
  {
    randomColourBuffer(c, color, v.size());
  }
  else
  {
    colourBuffer(c, glm::vec3(0), v.size());
  }
  normals(n, v);
  tangents(t, v);

  packObject(o, v, c, n, t, uv);
}

void GLWVBOFactory::packObject(GLWVertexArray& o, GLWVec3Array& v,
                                   GLWVec3Array& c, GLWVec3Array& n,
                                   GLWVec3Array& t, GLWVec2Array& uv)
{
  for (int i = 0; i < v.size(); ++i)
	{
    GLWVertex vert;
		
		if (v.size() != 0)
		{
			vert.position = v[i];
		}
		else
		{
			vert.position = glm::vec3();
		}
		
		if (c.size() != 0)
		{
		  vert.color = c[i];
		}
		else
		{
			vert.color = glm::vec3();
		}
		
		if (n.size() != 0)
		{
		  vert.normal = n[i];
		}
		else
		{
			vert.normal = glm::vec3();
		}
		
		if (uv.size() != 0)
		{
			vert.uv = uv[i];
		}
		else
		{
			vert.uv = glm::vec2();
		}	
		
		if (t.size() != 0)
		{
			vert.tangent = t[i];
		}
		else
		{
			vert.tangent = glm::vec3();
		}
		
		o.push_back(vert);
	}
}

void GLWVBOFactory::glwSphere(GLWVertexArray& o,
                              const glm::vec3 colour,
                              const int lats,
                              const int longs)
{
  o.clear();

  GLWVec3Array v, c, n, t;
  GLWVec2Array uv;

  sphere(v, lats, longs);

  sphereicalUVs(uv, v);

  normals(n, v);
  tangents(t, v);

  randomColourBuffer(c, colour, v.size());

  packObject(o, v, c, n, t, uv);
}

void GLWVBOFactory::glwCubeSphere(GLWVertexArray& o,
                              const glm::vec3 colour,
                              const int subdivisions)
{
  o.clear();

  GLWVec3Array v, c, n, t;
  GLWVec2Array uv;

  cubeSphere(v, subdivisions);

  sphereicalUVs(uv, v);

  normals(n, v);
  tangents(t, v);

  randomColourBuffer(c, colour, v.size());

  packObject(o, v, c, n, t, uv);
}

void GLWVBOFactory::glwCube(GLWVertexArray& o,
                    const glm::vec3 colour)
{
  o.clear();

  GLWVec3Array v, c, n, t;
  GLWVec2Array uv;

  cube(v);

  sphereicalUVs(uv, v);

  normals(n, v);
  tangents(t, v);

  randomColourBuffer(c, colour, v.size());

  packObject(o, v, c, n, t, uv);
}

void GLWVBOFactory::glwRectangle(GLWVertexArray& o,
                                 const glm::vec3 colour)
{
  o.clear();

  GLWVec3Array v, c, n, t;
  GLWVec2Array uv;

  rect(v);

  preRotate(v, glm::vec3(0,0,PI/2.0f));

  rectUVs(uv, v);

  normals(n, v);
  tangents(t, v);

  colourBuffer(c, colour, v.size());

  packObject(o, v, c, n, t, uv);
}

void GLWVBOFactory::glwCenteredRectangle(GLWVertexArray& o,
                                         const glm::vec3 colour)
{
  o.clear();

  GLWVec3Array v, c, n, t;
  GLWVec2Array uv;

  centeredSquareMesh(v, 1, 1);

  squareMeshUVs(uv, 1, 1);

  normals(n, v);
  tangents(t, v);

  colourBuffer(c, colour, v.size());

  packObject(o, v, c, n, t, uv);
}

void GLWVBOFactory::glwCircle(GLWVertexArray& o,
                         const glm::vec3 colour)
{
  o.clear();

  GLWVec3Array v, c, n, t;
  GLWVec2Array uv;

  circle(v, 12);

  circleUVs(uv, v);

  normals(n, v);
  tangents(t, v);

  colourBuffer(c, colour, v.size());

  packObject(o, v, c, n, t, uv);
}

//Returns random float
float	GLWVBOFactory::pg_randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);                            
}
// converts polar to cartesian
glm::vec3 GLWVBOFactory::polar_cart( float polar, float alpha)
{
  return glm::vec3(cos(polar)*sin(alpha), cos(polar) * cos(alpha), sin(polar));
}
// converts cartesian to polar
glm::vec2 GLWVBOFactory::cart_polar(glm::vec3& v)
{
	float r = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
  return glm::vec2(atan(v.y / v.x) + 0.5, acos(v.z / r) - 0.5);
}
