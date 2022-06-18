/*!*****************************************************************************
 * @file    Camera.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    27 MAY 2022
 * @brief
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <Assignment/Camera.h>
#include <utility/matrixTransforms.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// nextafter only constexpr after C++23
const float MTU::Camera::s_RotYMin{ std::nextafterf(-glm::radians(89.9f), 0.0f) };
const float MTU::Camera::s_RotYMax{ std::nextafterf( glm::radians(89.9f), 0.0f) };
const glm::vec3 MTU::Camera::s_Up{ 0.0f, 1.0f, 0.0f };

void MTU::Camera::updateCursor(glm::ivec2 const& CursorCurr, bool shouldUpdateRotation)
{
  if (shouldUpdateRotation)
  {
    glm::vec2 cursorDelta{ CursorCurr - m_CursorPrev };
    cursorDelta *= m_Sensitivity;
    m_Rot.x -= cursorDelta.x;
    m_Rot.y = glm::clamp(m_Rot.y - cursorDelta.y, s_RotYMin, s_RotYMax);
  }
  m_CursorPrev = CursorCurr;
}

void MTU::Camera::updateForwardVector()
{
  m_Fwd =
  (
    MTU::axisAngleRotation(s_Up, m_Rot.x, nullptr) *
    glm::vec3{ cosf(m_Rot.y), sinf(m_Rot.y), 0.0f }
  );
}

void MTU::Camera::updateMatrix()
{
  m_LookMat = glm::lookAt(m_Pos, m_Pos + m_Fwd, s_Up);
  m_W2V = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far) * m_LookMat;
}

void MTU::Camera::moveForward(float dist)
{
  m_Pos += dist * m_Fwd;
}

void MTU::Camera::moveRight(float dist)
{ // lazy to think, right is expensive (^;
  m_Pos += dist * glm::normalize(glm::cross(m_Fwd, s_Up));
}

void MTU::Camera::moveUp(float dist)
{
  m_Pos += dist * s_Up;
}
