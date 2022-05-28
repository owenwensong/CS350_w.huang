/*!*****************************************************************************
 * @file    Camera.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    27 MAY 2022
 * @brief   
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef ASSIGNMENT_CAMERA_HEADER_GUARD
#define ASSIGNMENT_CAMERA_HEADER_GUARD

#include <glm/glm.hpp>

namespace MTU
{
  struct Camera   // struct just for this implementation always facing origin
  {                     // todo: change to freecam
    float     m_AspectRatio;
    float     m_FOV;
    float     m_Sensitivity{ 0.0078125f };
    float     m_Near{ 0.000625f };
    float     m_Far{ 1250.0f };
    glm::vec3 m_Pos;
    glm::vec3 m_Fwd;// Forward vector
    glm::vec2 m_Rot;

    glm::mat4 m_LookMat;
    glm::mat4 m_W2V;

    glm::ivec2 m_CursorPrev;

    static const float s_RotYMin;
    static const float s_RotYMax;
    static const glm::vec3 s_Up;

    void updateCursor(glm::ivec2 const& CursorCurr, bool shouldUpdateRotation);
    void updateForwardVector();
    void updateMatrix();
    void moveForward(float dist);
    void moveRight(float dist);
    void moveUp(float dist);

  };

}

#endif//ASSIGNMENT_CAMERA_HEADER_GUARD
