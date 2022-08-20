/*!*****************************************************************************
 * @file    GS_Menu.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    20 AUG 2022
 * @brief   Gamestate for menu
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_MENU_HEADER_GUARD
#define GAMESTATE_MENU_HEADER_GUARD

#include <GameStateManager/GameStateBase.h>
#include <Assignment/Camera.h>

#include <vulkanHelpers/vulkanPipeline.h>
#include <vulkanHelpers/vulkanModel.h>

#include <glm/glm.hpp>

namespace MTU
{
  class GS_Menu : public GameState
  {
  public:

    // Constructor will act as Load

    GS_Menu(GameStateManager& rGSM);

    virtual void Init() override;
    virtual void Update(uint64_t dt) override;
    virtual void Draw() override;
    virtual void Free() override;

    // Destructor will act as Unload
    virtual ~GS_Menu();

  private:



  private:
    windowsInput& inputs;

    Camera    m_Cam;
    float     m_CamMoveSpeed;
    float     m_CamFastModifier;

    vulkanPipeline  m_BasicPipeline;
    vulkanModel     m_QuestionMark;

    float m_ObjTimer;

    int   m_ExampleDragInt;
    float m_ExampleDragFloat;

    bool m_bExampleCheckbox;
    bool m_bExampleColorCheckboxR;
    bool m_bExampleColorCheckboxG;
    bool m_bExampleColorCheckboxB;

  };
}

#endif//GAMESTATE_MENU_HEADER_GUARD