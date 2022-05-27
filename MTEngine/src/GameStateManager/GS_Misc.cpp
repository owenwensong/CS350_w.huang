/*!*****************************************************************************
 * @file    GS_Misc.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   QOL GameState misc class
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <imgui.h>
#include <iostream>
#include <GameStateManager/GS_Misc.h>

struct originCamera  // struct just for this implementation always facing origin
{
  float     m_Dist;
  glm::vec3 m_Pos;
  glm::vec2 m_Rot;

  glm::mat4 m_LookMat;
  glm::mat4 m_W2V;

  glm::ivec2 m_CursorPrev;

  static constexpr glm::vec2 s_DistLimits{ 2.5f, 25.0f };
  static constexpr float s_ScrollSpeedMul{ 0.125f };
  static constexpr float s_CamFOV{ glm::radians(75.0f) };
  static constexpr float s_Near{ 0.125f };
  static constexpr float s_Far{ s_DistLimits.y * 1.5f };
  static const float s_RotYMin;
  static const float s_RotYMax;
  static const glm::vec3 s_Tgt;
  static const glm::vec3 s_Up;
};

// nextafter only constexpr after C++23
const float originCamera::s_RotYMin{ std::nextafterf(-glm::half_pi<float>(), 0.0f) };
const float originCamera::s_RotYMax{ std::nextafterf(glm::half_pi<float>(), 0.0f) };
const glm::vec3 originCamera::s_Tgt{ 0.0f, 0.0f, 0.0f };
const glm::vec3 originCamera::s_Up{ 0.0f, 1.0f, 0.0f };

// *****************************************************************************

MTU::GS_Misc::GS_Misc(GameStateManager& rGSM) : 
  GameState{ rGSM },
  inputs{ rGSM.getInput() }
{
  std::cout << __FUNCSIG__ << std::endl;
}

void MTU::GS_Misc::Init()
{
  std::cout << __FUNCSIG__ << std::endl;
}

void MTU::GS_Misc::Update(uint64_t dt)
{
  ImGui::ShowDemoWindow();
}

void MTU::GS_Misc::Draw()
{

}

void MTU::GS_Misc::Free()
{
  std::cout << __FUNCSIG__ << std::endl;
}

MTU::GS_Misc::~GS_Misc()
{
  std::cout << __FUNCSIG__ << std::endl;
}
