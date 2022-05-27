/*!*****************************************************************************
 * @file    GameStateManager.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   QOL GameStateManager class interface
 *          Quick and dirty for use in CS350
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_MANAGER_HEADER_GUARD
#define GAMESTATE_MANAGER_HEADER_GUARD

#include <vulkan/vulkan.h>
#include <utility/Timer.h>
#include <handlers/windowHandler.h>
#include <GameStateManager/GameStates.h>

namespace MTU
{
  class GameStateManager
  {
  public:

    // One unique window per GSM
    GameStateManager(windowSetup const& windowSetup);
    ~GameStateManager();

    void setNextGameState(GS nextGS) noexcept;

    void Run();

    bool initImgui();

    // To be called by gamestate to get Frame Command Buffer
    VkCommandBuffer getFCB() const noexcept;
    std::unique_ptr<vulkanWindow>& getVKWin() noexcept;
    windowsInput& getInput() noexcept;

  private:

    std::unique_ptr<vulkanWindow> m_VKWindow;
    MTU::Timer                    m_Timer;
    VkCommandBuffer               m_FCB;
    GS                            m_gsCurr, m_gsNext;

    bool                          m_bUsingImGui;
    static bool                   s_bImGuiInUse;
  };
}

#endif//GAMESTATE_MANAGER_HEADER_GUARD
