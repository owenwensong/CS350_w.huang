/*!*****************************************************************************
 * @file    GS_Misc.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   QOL GameState misc class
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_MISC_HEADER_GUARD
#define GAMESTATE_MISC_HEADER_GUARD

#include <GameStateManager/GameStateBase.h>

#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>
#include <vulkanHelpers/vulkanModel.h>

struct originCamera   // struct just for this implementation always facing origin
{                     // todo: change to freecam
  float     m_AspectRatio;
  float     m_Dist;
  glm::vec3 m_Pos;
  glm::vec2 m_Rot;

  glm::mat4 m_LookMat;
  glm::mat4 m_W2V;

  glm::ivec2 m_CursorPrev;

  static constexpr glm::vec2 s_DistLimits{ 0.25f, 25.0f };
  static constexpr float s_ScrollSpeedMul{ 0.125f };
  static constexpr float s_CamFOV{ glm::radians(75.0f) };
  static constexpr float s_Near{ 0.0625f };
  static constexpr float s_Far{ s_DistLimits.y * 1.5f };
  static const float s_RotYMin;
  static const float s_RotYMax;
  static const glm::vec3 s_Tgt;
  static const glm::vec3 s_Up;
};

namespace MTU
{
  // Quick and dirty GameState class for CS350
  class GS_Misc : public GameState
  {
  public:

    // Constructor will act as Load

    GS_Misc(GameStateManager& rGSM);

    virtual void Init() override;
    virtual void Update(uint64_t dt) override;
    virtual void Draw() override;
    virtual void Free() override;

    // Destructor will act as Unload
    virtual ~GS_Misc();

  private:
    windowsInput& inputs;

    originCamera  m_Cam;
    
    std::array<vulkanModel, 1>    m_Models;
    std::array<vulkanPipeline, 1> m_Pipelines;
  };
}

#endif//GAMESTATE_MISC_HEADER_GUARD
