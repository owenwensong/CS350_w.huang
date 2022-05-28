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

#include <Assignment/Camera.h>
#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>
#include <vulkanHelpers/vulkanModel.h>

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

    Camera m_Cam;
    float  m_CamMoveSpeed;
    float  m_CamFastModifier;
    
    std::array<glm::vec3, 4>      m_Positions;
    std::array<vulkanModel, 3>    m_Models;
    std::array<vulkanPipeline, 1> m_Pipelines;
  };
}

#endif//GAMESTATE_MISC_HEADER_GUARD
