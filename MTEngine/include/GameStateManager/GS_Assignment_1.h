/*!*****************************************************************************
 * @file    GS_Assignment_1.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   Gamestate for assignment 1
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_ASSIGNMENT_1_HEADER_GUARD
#define GAMESTATE_ASSIGNMENT_1_HEADER_GUARD

#include <GameStateManager/GameStateBase.h>

#include <Assignment/Camera.h>
#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>

namespace MTU
{
  // Quick and dirty GameState class for CS350 assignment 1
  class GS_Assignment_1 : public GameState
  {
  public:

    // Constructor will act as Load

    GS_Assignment_1(GameStateManager& rGSM);

    virtual void Init() override;
    virtual void Update(uint64_t dt) override;
    virtual void Draw() override;
    virtual void Free() override;

    // Destructor will act as Unload
    virtual ~GS_Assignment_1();

  private:

    struct objectInfo
    {
      glm::vec3 m_Position;
      glm::vec3 m_Scale;
      glm::mat4 m_M2W;
    };

    void changeCurrentTest(unsigned);
    static void updateM2WSphere(objectInfo& toUpdate);
    static void updateM2WAABB(objectInfo& toUpdate);
    static void updateM2WPoint(objectInfo& toUpdate);

    // updates matrices and returns collision info
    bool updateSwitch();
    void imguiSwitch();
    void drawSwitch(VkCommandBuffer FCB, vulkanPipeline& inPipeline);

  private:
    windowsInput& inputs;

    Camera m_Cam;
    float  m_CamMoveSpeed;
    float  m_CamFastModifier;

    unsigned m_CurrentTest;
    bool     m_bColliding;

    std::array<objectInfo, 2>     m_CollisionObjects;
    std::array<glm::vec3, 4>      m_Positions;
    std::array<vulkanModel, 4>    m_Models;       // random models
    std::array<vulkanModel, 4>    m_DebugModels;  // normalized debug models
    std::array<vulkanPipeline, 2> m_Pipelines;    // pipelines (collision and props)
  };
}

#endif//GAMESTATE_ASSIGNMENT_1_HEADER_GUARD
