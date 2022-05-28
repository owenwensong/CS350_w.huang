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
#include <Assignment/Geometry.h>
#include <GameStateManager/GS_Misc.h>

// *****************************************************************************

MTU::GS_Misc::GS_Misc(GameStateManager& rGSM) : 
  GameState{ rGSM },
  inputs{ rGSM.getInput() },
  m_Cam{  },
  m_CamMoveSpeed{  },
  m_Positions{  },
  m_Models{  },
  m_Pipelines{  }
{
  std::cout << __FUNCSIG__ << std::endl;

  // Load models

  if (false == m_Models[0].load3DModelPositionOnly("../Assets/Meshes/sphere.obj"))
  {
    printWarning("Failed to load a model");
  }
  if (false == m_Models[1].load3DModelPositionOnly("../Assets/Meshes/bunny.obj"))
  {
    printWarning("Failed to load a model");
  }
  if (false == m_Models[2].load3DModelPositionOnly("../Assets/Meshes/starwars1.obj"))
  {
    printWarning("Failed to load a model");
  }

  // Load pipelines
  {
    auto& vkWin{ GSM.getVKWin() };
    {
      vulkanPipeline::setup pipelineSetup{ };
      pipelineSetup.m_VertexBindingMode = vulkanPipeline::E_VERTEX_BINDING_MODE::AOS_XYZ_F32;
      pipelineSetup.m_PathShaderVert = "../Assets/Shaders/vertBasic.spv";
      pipelineSetup.m_PathShaderFrag = "../Assets/Shaders/fragBasic.spv";
      //pipelineSetup.m_UniformsVert
      pipelineSetup.m_UniformsFrag = vulkanPipeline::createUniformInfo<glm::vec3>();
      pipelineSetup.m_PushConstantRangeVert = vulkanPipeline::createPushConstantInfo<glm::mat4>(VK_SHADER_STAGE_VERTEX_BIT);
      //pipelineSetup.m_PushConstantRangeFrag
      pipelineSetup.m_PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
      vkWin->createPipelineInfo(m_Pipelines[0], pipelineSetup);
    }
  }

}

void MTU::GS_Misc::Init()
{
  std::cout << __FUNCSIG__ << std::endl;
  m_Cam.m_AspectRatio = static_cast<float>(GSM.getVKWin()->m_windowsWindow.getWidth()) / GSM.getVKWin()->m_windowsWindow.getHeight();
  m_Cam.m_FOV = glm::radians(75.0f);
  // default constructed sensitivity
  // default constructed near
  // default constructed far
  m_Cam.m_Pos = glm::vec3{ 0.0f, 0.0f, 2.0f };
  m_Cam.m_Rot = glm::vec2{ glm::half_pi<float>(), 0.0f};
  {
    glm::ivec2 currCursorPos;// no need to initialize
    inputs.getCursorPos(currCursorPos.x, currCursorPos.y);
    m_Cam.updateCursor(currCursorPos, false);
  }
  m_Cam.updateMatrix();
  m_CamMoveSpeed = 2.5f;
  m_CamFastModifier = 2.0f;
  m_Positions[0] = glm::vec3{ 2.0f, 0.0f, 0.0f };
  m_Positions[1] = glm::vec3{ -2.0f, 0.0f, 0.0f };
  m_Positions[2] = glm::vec3{ 0.0f, -0.125f, 0.0f };
  m_Positions[3] = glm::vec3{ 50.0f, -25.0f, -200.0f };
  MTG::test();
}

void MTU::GS_Misc::Update(uint64_t dt)
{
  static_assert(MTU::Timer::clockFrequency > 0, "Clock frequency less than or equal to 0?");
  constexpr float reciprocalFrequency{ 1.0f / MTU::Timer::clockFrequency };
  float fdt{ dt * reciprocalFrequency };
  // ***************************************************************************
  // **************************************************************** ImGui ****
  ImGui::ShowMetricsWindow();

  if (ImGui::Begin("CS350Menu"))
  {
    ImGui::TextUnformatted("Camera controls (not fine tuned but works)");
    
    // SPEED CONTROL
    ImGui::DragFloat2("Speed / Shift multiplier", &m_CamMoveSpeed, 0.125f, 0.125f, 10.0f);

    // FOV CONTROL
    ImGui::DragFloat("FOV", &m_Cam.m_FOV, glm::radians(1.0f), glm::radians(50.0f), glm::radians(150.0f));
    ImGui::SameLine();
    ImGui::Text("%.2f degrees", glm::degrees(m_Cam.m_FOV));

    // SENSITIVITY CONTROL
    ImGui::DragFloat("Sensitivity", &m_Cam.m_Sensitivity, 0.0000125f, 0.00078125f, 0.0625f);

    // NEAR AND FAR PLANES
    ImGui::DragFloat2("Near/Far", &m_Cam.m_Near, 0.125f, 0.0000125f, 12500.0f);

    ImGui::TextUnformatted("Move stuff");
    ImGui::DragFloat3("Ball0Pos", &m_Positions[0].x, 0.125f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("Ball1Pos", &m_Positions[1].x, 0.125f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("BunnyPos", &m_Positions[2].x, 0.125f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("StarWars", &m_Positions[3].x, 0.125f, -1000.0f, 1000.0f);

  }
  ImGui::End();

  // **************************************************************** ImGui ****
  // ***************************************************************************
  // ******************************************************** Update camera ****

  { // update aspect ratio
    m_Cam.m_AspectRatio = static_cast<float>(GSM.getVKWin()->m_windowsWindow.getWidth()) / GSM.getVKWin()->m_windowsWindow.getHeight();
  }
  { // update camera orientation
    glm::ivec2 cursorCurr;
    inputs.getCursorPos(cursorCurr.x, cursorCurr.y);
    m_Cam.updateCursor(cursorCurr, inputs.isPressed(VK_RBUTTON));
    m_Cam.updateForwardVector();
  }
  { // update camera position
    float camMoveAmt{ inputs.isPressed(VK_SHIFT) ? m_CamFastModifier * fdt : fdt };
    if (inputs.isPressed(VK_W))
    {
      m_Cam.moveForward(m_CamMoveSpeed * camMoveAmt);
    }
    else if (inputs.isPressed(VK_S))
    {
      m_Cam.moveForward(m_CamMoveSpeed * -camMoveAmt);
    }
    if (inputs.isPressed(VK_D))
    {
      m_Cam.moveRight(m_CamMoveSpeed * camMoveAmt);
    }
    else if (inputs.isPressed(VK_A))
    {
      m_Cam.moveRight(m_CamMoveSpeed * -camMoveAmt);
    }
    if (inputs.isPressed(VK_SPACE))
    {
      m_Cam.moveUp(m_CamMoveSpeed * camMoveAmt);
    }
    else if (inputs.isPressed(VK_CONTROL))
    {
      m_Cam.moveUp(m_CamMoveSpeed * -camMoveAmt);
    }
  }
  { // update camera matrices
    m_Cam.updateMatrix();
  }

  // ******************************************************** Update camera ****
  // ***************************************************************************
}

void MTU::GS_Misc::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

  glm::vec3 tempColor{ 0.0f, 1.0f, 0.0f };
  if (inputs.isPressed(VK_LBUTTON))tempColor.r = 1.0f, tempColor.g = 0.0f;
  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[0]);
  GSM.getVKWin()->setUniform(m_Pipelines[0], 1, 0, &tempColor, sizeof(tempColor));
  { // first object
    glm::mat4 xform{ m_Cam.m_W2V * glm::translate(glm::identity<glm::mat4>(), m_Positions[0])};
    m_Pipelines[0].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Models[0].draw(FCB);
  }
  {
    glm::mat4 xform{ m_Cam.m_W2V * glm::translate(glm::identity<glm::mat4>(), m_Positions[1]) };
    GSM.getVKWin()->setUniform(m_Pipelines[0], 1, 0, &tempColor, sizeof(tempColor));
    m_Pipelines[0].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Models[0].draw(FCB);
  }
  {
    glm::mat4 xform{ m_Cam.m_W2V * glm::translate(glm::identity<glm::mat4>(), m_Positions[2]) };
    GSM.getVKWin()->setUniform(m_Pipelines[0], 1, 0, &tempColor, sizeof(tempColor));
    m_Pipelines[0].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Models[1].draw(FCB);
  }
  {
    glm::mat4 xform{ m_Cam.m_W2V * glm::translate(glm::identity<glm::mat4>(), m_Positions[3]) };
    GSM.getVKWin()->setUniform(m_Pipelines[0], 1, 0, &tempColor, sizeof(tempColor));
    m_Pipelines[0].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Models[2].draw(FCB);
  }
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
