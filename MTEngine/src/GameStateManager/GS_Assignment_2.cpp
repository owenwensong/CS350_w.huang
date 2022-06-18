/*!*****************************************************************************
 * @file    GS_Assignment_2.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    16 JUN 2022
 * @brief   Gamestate for assignment 2
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <imgui.h>
#include <iostream>
#include <utility/ThreadTask.h>
#include <Assignment/Geometry.h>
#include <GameStateManager/GS_Assignment_2.h>

// *****************************************************************************
// ************************************************** ENUMS FOR CONVENIENCE ****

enum enumAss2Pipelines
{
  E_PIPELINE_WIREFRAME = 0,
  E_PIPELINE_BASICLIGHT,

  E_NUM_PIPELINES
};

enum enumAss2DebugModels
{
  E_DEBUGMODEL_SPHERE = 0,
  E_DEBUGMODEL_CUBE,
  E_DEBUGMODEL_POINT,

  E_NUM_DEBUGMODELS
};

enum enumAss2Models
{
  E_MODEL_BUNNY = 0,
  E_MODEL_LUCY_PRINCETON,
  
  E_NUM_MODELS
};

// *****************************************************************************

MTU::GS_Assignment_2::GS_Assignment_2(GameStateManager& rGSM) :
  GameState{ rGSM },
  inputs{ rGSM.getInput() },
  m_Cam{  },
  m_LightColor{  },
  m_CamMoveSpeed{  },
  m_CamFastModifier{  },
  m_Pipelines{  },
  m_DebugModels{  },
  m_Models{  }
{
  GS_PRINT_FUNCSIG();

  bool shouldStopTaskLoading{ false };
  MTU::ThreadTask isLoadingTask{ MTU::taskLoading, &shouldStopTaskLoading };

  // ***************************************************************************
  // ************************************************************** LOADING ****

  // Load debug models
  {
    if (false == m_DebugModels[E_DEBUGMODEL_SPHERE].load3DModelPositionOnly("../Assets/Meshes/DebugMeshes/normalIcoSphere.obj"))
    {
      printWarning("Failed to load debug icosphere");
    }
    if (false == m_DebugModels[E_DEBUGMODEL_CUBE].load3DModelPositionOnly("../Assets/Meshes/DebugMeshes/normalCube.obj"))
    {
      printWarning("Failed to load debug cube");
    }
    if (false == m_DebugModels[E_DEBUGMODEL_POINT].load3DModelPositionOnly("../Assets/Meshes/DebugMeshes/normalPoint.obj"))
    {
      printWarning("Failed to load debug point");
    }
  }

  // Load models
  {
    if (false == m_Models[E_MODEL_BUNNY].load3DNmlModel("../Assets/Meshes/bunny.obj"))
    {
      printWarning("Failed to load bunny model");
    }
    if (false == m_Models[E_MODEL_LUCY_PRINCETON].load3DNmlModel("../Assets/Meshes/lucy_princeton.obj"))
    {
      printWarning("Failed to load lucy princeton model");
    }
  }

  // Load pipelines
  {
    auto& vkWin{ GSM.getVKWin() };
    {
      vulkanPipeline::setup pipelineSetup{ };
      pipelineSetup.m_VertexBindingMode = vulkanPipeline::E_VERTEX_BINDING_MODE::AOS_XYZ_F32;
      pipelineSetup.m_PathShaderVert = "../Assets/Shaders/vert_Wireframe_Ass2.spv";
      pipelineSetup.m_PathShaderFrag = "../Assets/Shaders/frag_Wireframe_Ass2.spv";
      //pipelineSetup.m_UniformsVert
      //pipelineSetup.m_UniformsFrag = vulkanPipeline::createUniformInfo<glm::vec3>();
      pipelineSetup.m_PushConstantRangeVert = vulkanPipeline::createPushConstantInfo<glm::mat4>(VK_SHADER_STAGE_VERTEX_BIT);
      pipelineSetup.m_PushConstantRangeFrag = vulkanPipeline::createPushConstantInfo<glm::vec3>(VK_SHADER_STAGE_FRAGMENT_BIT);
      pipelineSetup.m_PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
      //pipelineSetup.m_CullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;

      vkWin->createPipelineInfo(m_Pipelines[E_PIPELINE_WIREFRAME], pipelineSetup);
    }
    {
      vulkanPipeline::setup pipelineSetup{ };
      pipelineSetup.m_VertexBindingMode = vulkanPipeline::E_VERTEX_BINDING_MODE::AOS_XYZ_RGB_F32;
      pipelineSetup.m_PathShaderVert = "../Assets/Shaders/vert_BasicLight_Ass2.spv";
      pipelineSetup.m_PathShaderFrag = "../Assets/Shaders/frag_BasicLight_Ass2.spv";
      //pipelineSetup.m_UniformsVert
      //pipelineSetup.m_UniformsFrag = vulkanPipeline::createUniformInfo<glm::vec3>();
      pipelineSetup.m_PushConstantRangeVert = vulkanPipeline::createPushConstantInfo<glm::mat4>(VK_SHADER_STAGE_VERTEX_BIT);
      // using vec4 here to maintain 16B alignment, there is essentially 4B of dead space here
      pipelineSetup.m_PushConstantRangeFrag = vulkanPipeline::createPushConstantInfo<glm::vec4, glm::vec3>(VK_SHADER_STAGE_FRAGMENT_BIT);
      pipelineSetup.m_PolygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;

      vkWin->createPipelineInfo(m_Pipelines[E_PIPELINE_BASICLIGHT], pipelineSetup);
    }
  }

  // ************************************************************** LOADING ****
  // ***************************************************************************

  shouldStopTaskLoading = true;
  for (MTU::Timer lazyTimer{ MTU::Timer::getCurrentTP() }; false == isLoadingTask.isDone(); lazyTimer.stop())
  { // Error if 1s goes by after the loading finishes but thread doesn't end
    if (lazyTimer.getElapsedCount() / MTU::Timer::clockFrequency)
    {
      printf_s("Something has gone wrong! Attempting to continue...\n");
      break;
    }
  }

}

void MTU::GS_Assignment_2::Init()
{
  GS_PRINT_FUNCSIG();
  m_Cam.m_AspectRatio = static_cast<float>(GSM.getVKWin()->m_windowsWindow.getWidth()) / GSM.getVKWin()->m_windowsWindow.getHeight();
  m_Cam.m_FOV = glm::radians(75.0f);
  // default constructed sensitivity
  m_Cam.m_Near = 0.000625f;
  m_Cam.m_Far = 25000.0f;
  m_Cam.m_Pos = glm::vec3{ 0.0f, 0.0f, 2.0f };
  m_Cam.m_Rot = glm::vec2{ glm::half_pi<float>(), 0.0f };
  {
    glm::ivec2 currCursorPos;// no need to initialize
    inputs.getCursorPos(currCursorPos.x, currCursorPos.y);
    m_Cam.updateCursor(currCursorPos, false);
  }
  m_Cam.updateMatrix();
  m_LightColor = glm::vec3{ 0.5f, 0.5f, 0.75f };
  m_CamMoveSpeed = 2.5f;
  m_CamFastModifier = 2.0f;
}

void MTU::GS_Assignment_2::Update(uint64_t dt)
{
  if (inputs.isTriggered(VK_F1))GSM.setNextGameState(GS::E_ASSIGNMENT_1);
  else if (inputs.isTriggered(VK_F2))GSM.setNextGameState(GS::E_RESTART);
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
    ImGui::DragFloat2("Near/Far", &m_Cam.m_Near, 0.125f, 0.0000125f, 25000.0f);

    // CAMERA LIGHT COLOR
    ImGui::DragFloat3("Camera Light Color", &m_LightColor.x, 0.0125f, 0.0f, 1.0f);

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

void MTU::GS_Assignment_2::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[E_PIPELINE_WIREFRAME]);

  { // centered draw test
    glm::vec3 tempColor{ 0.0f, 1.0f, 0.0f };
    m_Pipelines[E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &m_Cam.m_W2V);
    m_Pipelines[E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &tempColor);
    m_DebugModels[E_DEBUGMODEL_CUBE].draw(FCB);
  }
  { // offset red draw test
    glm::vec3 tempColor{ 1.0f, 0.0f, 0.0f };
    glm::mat4 xform{ m_Cam.m_W2V * glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 2.0f, 0.0f, 0.0f }) };
    m_Pipelines[E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Pipelines[E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &tempColor);
    m_DebugModels[E_DEBUGMODEL_SPHERE].draw(FCB);
  }

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[E_PIPELINE_BASICLIGHT]);
  {
    // TODO W2M Matrix done on a per object level
    m_Pipelines[E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &m_Cam.m_Pos);
    m_Pipelines[E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 16, sizeof(glm::vec3), &m_LightColor);
    
    m_Pipelines[E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &m_Cam.m_W2V);
    m_Models[E_MODEL_BUNNY].draw(FCB);

    m_Models[E_MODEL_LUCY_PRINCETON].draw(FCB);
  }
  

}

void MTU::GS_Assignment_2::Free()
{
  GS_PRINT_FUNCSIG();
}

MTU::GS_Assignment_2::~GS_Assignment_2()
{
  GS_PRINT_FUNCSIG();

  for (auto& x : m_Pipelines)GSM.getVKWin()->destroyPipelineInfo(x);
  for (auto& x : m_DebugModels)x.destroyModel();
}
