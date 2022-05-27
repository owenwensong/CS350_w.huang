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

// nextafter only constexpr after C++23
const float originCamera::s_RotYMin{ std::nextafterf(-glm::half_pi<float>(), 0.0f) };
const float originCamera::s_RotYMax{ std::nextafterf(glm::half_pi<float>(), 0.0f) };
const glm::vec3 originCamera::s_Tgt{ 0.0f, 0.0f, 0.0f };
const glm::vec3 originCamera::s_Up{ 0.0f, 1.0f, 0.0f };

void updateCamMatrices(originCamera& inCam)
{
  inCam.m_LookMat = glm::lookAt(inCam.m_Pos, inCam.s_Tgt, inCam.s_Up);
  inCam.m_W2V = glm::perspective(originCamera::s_CamFOV, inCam.m_AspectRatio, originCamera::s_Near, originCamera::s_Far) * inCam.m_LookMat;
}

// *****************************************************************************

MTU::GS_Misc::GS_Misc(GameStateManager& rGSM) : 
  GameState{ rGSM },
  inputs{ rGSM.getInput() },
  m_Cam{  },
  m_Models{  },
  m_Pipelines{  }
{
  std::cout << __FUNCSIG__ << std::endl;
  m_Cam.m_AspectRatio = static_cast<float>(GSM.getVKWin()->m_windowsWindow.getWidth()) / GSM.getVKWin()->m_windowsWindow.getHeight();
  m_Cam.m_Dist = 0.5f * (originCamera::s_DistLimits.x + originCamera::s_DistLimits.y);
  m_Cam.m_Pos = { 0.0f, 0.0f, m_Cam.m_Dist };
  m_Cam.m_Rot = { -glm::half_pi<float>(), 0.0f };
  updateCamMatrices(m_Cam);

  // Load models

  if (false == m_Models[0].load3DModelPositionOnlySolidColor("../Assets/Meshes/sphere.obj"))
  {
    printWarning("Failed to load model");
  }

  // Load pipelines
  {
    auto& vkWin{ GSM.getVKWin() };
    {
      vulkanPipeline::setup pipelineSetup{ };
      pipelineSetup.m_VertexBindingMode = vulkanPipeline::E_VERTEX_BINDING_MODE::AOS_XYZ_RGB_F32;
      pipelineSetup.m_PathShaderVert = "../Assets/Shaders/vertBasic.spv";
      pipelineSetup.m_PathShaderFrag = "../Assets/Shaders/fragBasic.spv";
      pipelineSetup.m_PushConstantRangeVert = vulkanPipeline::createPushConstantInfo<glm::mat4>(VK_SHADER_STAGE_VERTEX_BIT);
      pipelineSetup.m_PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
      vkWin->createPipelineInfo(m_Pipelines[0], pipelineSetup);
    }
  }

}

void MTU::GS_Misc::Init()
{
  std::cout << __FUNCSIG__ << std::endl;
}

void MTU::GS_Misc::Update(uint64_t dt)
{
  ImGui::ShowDemoWindow();
  
  // ***************************************************************************
  // ************************************ Update camera with mouse controls ****

  glm::ivec2 cursorCurr;
  inputs.getCursorPos(cursorCurr.x, cursorCurr.y);
  if (int scroll{ inputs.getScrollSteps() }; scroll || inputs.isPressed(VK_RBUTTON))
  {
    // update cam rotation based on cursor delta (speed based on pixels)
    glm::vec2 cursorDelta{ (cursorCurr - m_Cam.m_CursorPrev) };
    cursorDelta *= 0.0078125f;
    m_Cam.m_Rot.x -= cursorDelta.x;
    m_Cam.m_Rot.y = glm::clamp(m_Cam.m_Rot.y + cursorDelta.y, m_Cam.s_RotYMin, m_Cam.s_RotYMax);

    // update cam distance from origin based on scroll
    m_Cam.m_Dist = glm::clamp(m_Cam.m_Dist - m_Cam.s_ScrollSpeedMul * scroll, m_Cam.s_DistLimits.x, m_Cam.s_DistLimits.y);

    // update lookat
    m_Cam.m_Pos =
    {
      MTU::axisAngleRotation(m_Cam.s_Up, m_Cam.m_Rot.x, nullptr) *
      glm::vec3{ cosf(m_Cam.m_Rot.y), sinf(m_Cam.m_Rot.y), 0.0f} * m_Cam.m_Dist
    };

    updateCamMatrices(m_Cam);
  }
  m_Cam.m_CursorPrev = cursorCurr;

  // ************************************ Update camera with mouse controls ****
  // ***************************************************************************
}

void MTU::GS_Misc::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[0]);
  m_Pipelines[0].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &m_Cam.m_W2V);
  m_Models[0].draw(FCB);
}

void MTU::GS_Misc::Free()
{
  std::cout << __FUNCSIG__ << std::endl;
}

MTU::GS_Misc::~GS_Misc()
{
  std::cout << __FUNCSIG__ << std::endl;
  for (auto& x : m_Pipelines)GSM.getVKWin()->destroyPipelineInfo(x);
  for (auto& x : m_Models)x.destroyModel();
}
