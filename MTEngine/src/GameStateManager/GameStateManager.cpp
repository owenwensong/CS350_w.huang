/*!*****************************************************************************
 * @file    GameStateManager.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   QOL GameStateManager class implementation
 *          Quick and dirty for use in CS350
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <memory>
#include <vulkanHelpers/printWarnings.h>
#include <GameStateManager/GameStateManager.h>

#include <imguiHelper.h>
bool MTU::GameStateManager::s_bImGuiInUse{ false };

// *****************************************************************************
// ************************************************************* GameStates ****

#include <GameStateManager/GameStateBase.h>
#include <GameStateManager/GS_Assignment_1.h>
#include <GameStateManager/GS_Assignment_2.h>
#include <GameStateManager/GS_Assignment_3.h>

// return value is whether it should continue
bool updateGameState(MTU::GameStateManager& rGSM, std::unique_ptr<MTU::GameState>& upGS, MTU::GS nextGS)
{
  switch (nextGS)
  {
#define RC_HELPER(a) reinterpret_cast<MTU::GameState*>(a)
  case MTU::GS::E_ASSIGNMENT_1:
    upGS.reset(RC_HELPER(new MTU::GS_Assignment_1{ rGSM }));
    return true;
  case MTU::GS::E_ASSIGNMENT_2:
    upGS.reset(RC_HELPER(new MTU::GS_Assignment_2{ rGSM }));
    return true;
  case MTU::GS::E_ASSIGNMENT_3:
    upGS.reset(RC_HELPER(new MTU::GS_Assignment_3{ rGSM }));
    return true;
#undef RC_HELPER
  default:
    printWarning("Unknown GameState enum provided"sv, true);
    __fallthrough;
  case MTU::GS::E_QUIT:
    upGS.reset(nullptr);
    return false;
  case MTU::GS::E_RESTART:
    return true;
  }
}

// *****************************************************************************
// ********************************************************** GSM Functions ****

MTU::GameStateManager::GameStateManager(windowSetup const& windowSetup) :
  m_VKWindow{ windowHandler::getPInstance()->createWindow(windowSetup) },
  m_Timer{ MTU::Timer::getCurrentTP() },
  m_FCB{ VK_NULL_HANDLE },
  m_gsCurr{ GS::E_QUIT },
  m_gsNext{ GS::E_QUIT },
  m_bUsingImGui{ false }
{
  // let it crash if window handler wasn't created properly
  
}

MTU::GameStateManager::~GameStateManager()
{
  m_VKWindow->m_Device->waitForDeviceIdle();
  if (m_bUsingImGui)
  {
    imguiHelper::ShutDown();
    s_bImGuiInUse = false;
  }
}

void MTU::GameStateManager::setNextGameState(GS nextGS) noexcept
{
  m_gsNext = nextGS;
}

void MTU::GameStateManager::Run()
{
  for (std::unique_ptr<GameState> upGS{ nullptr }; updateGameState(*this, upGS, m_gsNext); )
  {
    if (upGS == nullptr)break;// should never happen (quit returns false)
    if (m_gsNext == MTU::GS::E_RESTART)
    {
      m_gsNext = m_gsCurr;
    }
    else
    {
      m_gsCurr = m_gsNext;
    }

    upGS->Init();

    windowsInput& inputs{ m_VKWindow->m_windowsWindow.m_windowInputs };
    while (m_gsCurr == m_gsNext)
    {
      if (windowHandler* pWH{ windowHandler::getPInstance() }; false == pWH->processInputEvents())
      {
        m_gsNext = GS::E_QUIT;
        break;// exit from message
      }
      inputs.update();

      if (inputs.isTriggered(VK_F11))m_VKWindow->toggleFullscreen();

      uint64_t prevElapsed{ m_Timer.getElapsedCount() };
      m_Timer.stop();

      m_FCB = m_VKWindow->FrameBegin();
      if (m_FCB != VK_NULL_HANDLE)
      {
        if (m_bUsingImGui)imguiHelper::Update();
        upGS->Update(m_Timer.getElapsedCount() - prevElapsed);
        upGS->Draw();

        if (m_bUsingImGui)imguiHelper::Draw(m_FCB);
        m_VKWindow->FrameEnd();
        m_VKWindow->PageFlip();

        m_FCB = VK_NULL_HANDLE;
      }

      
    }
    m_VKWindow->m_Device->waitForDeviceIdle();
    upGS->Free();

  }
}

bool MTU::GameStateManager::initImgui()
{
  if (s_bImGuiInUse)return false;
  windowHandler* pWH{ windowHandler::getPInstance() };
  if (pWH == nullptr)return false;
  {
    ImGui_ImplVulkan_InitInfo initInfo{};
    auto l_vkDevice{ m_VKWindow->m_Device };
    std::scoped_lock Lk0{ l_vkDevice->m_VKMainQueue };
    std::scoped_lock Lk1{ l_vkDevice->m_LockedVKDescriptorPool };

    initInfo.Instance = l_vkDevice->m_pVKInst->m_VkHandle;
    initInfo.PhysicalDevice = l_vkDevice->m_VKPhysicalDevice;
    initInfo.Device = l_vkDevice->m_VKDevice;
    initInfo.QueueFamily = l_vkDevice->m_MainQueueIndex;
    initInfo.Queue = l_vkDevice->m_VKMainQueue.get();
    initInfo.PipelineCache = l_vkDevice->m_VKPipelineCache;
    initInfo.DescriptorPool = l_vkDevice->m_LockedVKDescriptorPool.get();
    initInfo.Subpass = 0;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = m_VKWindow->m_ImageCount;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    initInfo.Allocator = l_vkDevice->m_pVKInst->m_pVKAllocator;
    initInfo.CheckVkResultFn = nullptr;

    imguiHelper::initIMGUI(initInfo, m_VKWindow->m_VKRenderPass, m_VKWindow->m_windowsWindow.getSystemWindowHandle());
  }
  // FCB this time is Font Create Buffer lmao
  if (VkCommandBuffer FCB{ pWH->beginOneTimeSubmitCommand(true) }; FCB != nullptr)
  {
    imguiHelper::createFont(FCB);
    pWH->endOneTimeSubmitCommand(FCB, true);
  }
  else
  {
    printWarning("Failed to create command buffer for imgui font creation"sv, true);
    return false;
  }
  s_bImGuiInUse = m_bUsingImGui = true;
  return true;
}

VkCommandBuffer MTU::GameStateManager::getFCB() const noexcept
{
  return m_FCB;
}

std::unique_ptr<vulkanWindow>& MTU::GameStateManager::getVKWin() noexcept
{
  return m_VKWindow;
}

windowsInput& MTU::GameStateManager::getInput() noexcept
{
  return m_VKWindow->m_windowsWindow.m_windowInputs;
}

// *****************************************************************************
