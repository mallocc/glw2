#include "glwcamera.h"


using glw::engine::GLWCamera;


GLWCamera::GLWCamera() {}

GLWCamera::GLWCamera(glm::vec3 pos, glm::vec3 vel, glm::vec3 dir, glm::vec3 up)
    :
    m_pos(pos),
    m_vel(vel),
    m_dir(dir),
    m_up(up)
{}

GLWCamera::~GLWCamera()
{}

void GLWCamera::update(float dt, float friction)
{
  m_vel *= friction;
	m_pos += m_vel * dt;
	m_dirVel *= friction;
	m_dir += m_dirVel;
	m_upVel *= friction;
	m_up += m_upVel;
}

void GLWCamera::applyForce(glm::vec3 force)
{
	m_vel += force;
}

void GLWCamera::applyForceForward(float mag)
{
	m_vel += m_dir * mag;
}

void GLWCamera::applyForceBackward(float mag)
{
	m_vel -= m_dir * mag;
}

void GLWCamera::applyForceRight(float mag)
{
	m_vel += glm::cross(m_dir, m_up) * mag;
}

void GLWCamera::applyForceLeft(float mag)
{
	m_vel -= glm::cross(m_dir, m_up) * mag;
}

void GLWCamera::applyForceUp(float mag)
{
  m_vel += m_up * mag;
}

void GLWCamera::applyForceDown(float mag)
{
  m_vel -= m_up * mag;
}

void GLWCamera::yawLeft(float deg)
{
	m_dir = glm::quat(glm::vec3(0, 1, 0) * glm::radians(deg)) * m_dir;
}

void GLWCamera::yawRight(float deg)
{
	m_dir = glm::quat(glm::vec3(0, -1, 0) * glm::radians(deg)) * m_dir;
}

void GLWCamera::pitchUp(float deg)
{
	glm::vec3 right = glm::cross(m_dir, m_up);
	//m_up = glm::quat(right * glm::radians(deg)) * m_up;
	m_dir = glm::quat(right * glm::radians(deg)) * m_dir;
}

void GLWCamera::pitchDown(float deg)
{
	glm::vec3 right = -glm::cross(m_dir, m_up);
	//m_up = glm::quat(right * glm::radians(deg)) * m_up;
	m_dir = glm::quat(right * glm::radians(deg)) * m_dir;
}

void GLWCamera::brake(float friction)
{
	m_vel *= friction;
}

glm::vec3 GLWCamera::getPosition()
{
  return m_pos;
}

glm::vec3 GLWCamera::getLookPosition()
{
  return m_pos + m_dir;
}

glm::vec3 GLWCamera::getUp()
{
  return m_up;
}
