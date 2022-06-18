/*!*****************************************************************************
 * @file    GS_Assignment_2.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    16 JUN 2022
 * @brief   Gamestate for assignment 2
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_ASSIGNMENT_2_HEADER_GUARD
#define GAMESTATE_ASSIGNMENT_2_HEADER_GUARD

#include <GameStateManager/GameStateBase.h>

#include <Assignment/Camera.h>
#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>

namespace MTU
{
  // Quick and dirty GameState class for CS350 assignment 2
  class GS_Assignment_2 : public GameState
  {
  public:

    // Constructor will act as Load

    GS_Assignment_2(GameStateManager& rGSM);

    virtual void Init() override;
    virtual void Update(uint64_t dt) override;
    virtual void Draw() override;
    virtual void Free() override;

    // Destructor will act as Unload
    virtual ~GS_Assignment_2();

  private:



  private:
    windowsInput& inputs;

    Camera    m_Cam;
    glm::vec3 m_LightColor;
    float     m_CamMoveSpeed;
    float     m_CamFastModifier;

    std::array<vulkanPipeline, 2> m_Pipelines;
    std::array<vulkanModel, 3>    m_DebugModels;
    std::array<vulkanModel, 2>    m_Models;
    
  };
}

#endif//GAMESTATE_ASSIGNMENT_2_HEADER_GUARD
