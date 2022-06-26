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
#include <glm/gtc/quaternion.hpp>

namespace A2H
{

  // ***************************************************************************
  // **************************************************** LOADING FUNCTIONS ****

  static bool loadDebugModels(DMA& DebugModelArray)
  {
    bool retval{ true };
#define A2H_LOAD_MODEL_HELPER(enumA, strB) if (false == DebugModelArray[enumA].load3DModelPositionOnly("../Assets/Meshes/" strB ".obj", true)) { printWarning("Failed to load model: " strB); retval = false; }
    A2H_LOAD_MODEL_HELPER(E_DEBUGMODEL_SPHERE, "DebugMeshes/normalIcoSphere");
    A2H_LOAD_MODEL_HELPER(E_DEBUGMODEL_CUBE, "DebugMeshes/normalCube");
    A2H_LOAD_MODEL_HELPER(E_DEBUGMODEL_POINT, "DebugMeshes/normalPoint");
#undef A2H_LOAD_MODEL_HELPER
    return retval;
  }

  static bool loadModels(MVA& ModelVerticesArray, MA& ModelArray)
  {
    bool retval{ true };
#define A2H_LOAD_MODEL_HELPER(enumA, strB) if (false == ModelArray[enumA].load3DNmlModel("../Assets/Meshes/" strB ".obj", ModelVerticesArray[enumA], true)) { printWarning("Failed to load model: " strB); retval = false; }
    A2H_LOAD_MODEL_HELPER(E_MODEL_BUNNY, "bunny");
    A2H_LOAD_MODEL_HELPER(E_MODEL_LUCY_PRINCETON, "lucy_princeton");
#undef A2H_LOAD_MODEL_HELPER
    return retval;
  }

  static bool loadPipelines(PLA& PipelineArray, std::unique_ptr<vulkanWindow>& vkWin)
  {
    bool retval{ true };

    // COLORED WIREFRAME
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

      if (false == vkWin->createPipelineInfo(PipelineArray[A2H::E_PIPELINE_WIREFRAME], pipelineSetup))
      {
        retval = false;
      }
    }

    // SOLID COLOR, COLORED CAMERA LIGHT
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

      if (false == vkWin->createPipelineInfo(PipelineArray[A2H::E_PIPELINE_BASICLIGHT], pipelineSetup))
      {
        retval = false;
      }
    }

    return retval;
  }

  // ***************************************************************************
  // ***************************************************** HELPER FUNCTIONS ****



  // ***************************************************************************

}

MTU::GS_Assignment_2::GS_Assignment_2(GameStateManager& rGSM) :
  GameState{ rGSM },
  inputs{ rGSM.getInput() },
  m_Cam{  },
  m_LightColor{  },
  m_CamMoveSpeed{  },
  m_CamFastModifier{  },
  m_Pipelines{  },
  m_DebugModels{  },
  m_Vertices{  },
  m_Models{  },
  m_Objects{  }
{
  GS_PRINT_FUNCSIG();

  bool shouldStopTaskLoading{ false };
  MTU::ThreadTask isLoadingTask{ MTU::taskLoading, &shouldStopTaskLoading };

  // ***************************************************************************
  // ************************************************************** LOADING ****

  // Load debug models
  if (false == A2H::loadDebugModels(m_DebugModels))
  {
    printWarning("Failed to load one or more models, please check file structure!");
  }

  // Load models
  if (false == A2H::loadModels(m_Vertices, m_Models))
  {
    printWarning("Failed to load one or more models, please check file structure!");
  }

  // Load pipelines
  if (false == A2H::loadPipelines(m_Pipelines, GSM.getVKWin()))
  {
    printWarning("Failed to load one or more pipelines, please check file structure!");
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

  // ***************************************************************************
  // ****************************************** SCENE OBJECT INITIALIZATION ****

  m_Objects.clear();    // clear existing objects
  m_Objects.reserve(2); // reserve memory for basic scene

  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 0.0f, 0.0f, 0.0f };
    //obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.updateMatrices();
    obj.m_Model = A2H::E_MODEL_BUNNY;
  }

  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 2.0f, 2.0f, 2.0f };
    //obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Rot = glm::vec3{ glm::radians(45.0f), glm::radians(45.0f), glm::radians(45.0f) };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.updateMatrices();
    obj.m_Model = A2H::E_MODEL_LUCY_PRINCETON;
  }

  // ***************************************************************************
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

    // TODO: add checkboxes here for AABB/OBB/BS/BVH

    // TODO: add compute bounding volumes & compute BVH buttons

    // SCENE OBJECTS EDITOR (Add only, lazy to support remove)
    ImGui::Separator();
    if (ImGui::BeginChild("Objects", ImVec2{ 0.0f, 7.5f * ImGui::GetFrameHeightWithSpacing() }, true))
    {

      for (size_t i{ 0 }, t{ m_Objects.size() }; i < t; ++i)
      {
        // quick and dirty ptr_id, idk if it will cause any leaks with
        // imgui when the vector is resized. Hopefully not, but this
        // isn't meant to be a long term framework, just for assignment 2.
        A2H::Object* pObject{ m_Objects.data() + i };
        
        if (ImGui::TreeNode(pObject, "Object %d", static_cast<int>(i)))
        {
          // Object Position
          ImGui::DragFloat3("Pos", &pObject->m_Pos.x, 0.125f, -1000.0f, 1000.0f);

          // Object Rotation
          ImGui::DragFloat3("Rot", &pObject->m_Rot.x, 0.0125f, -glm::pi<float>(), glm::pi<float>());
          ImGui::SameLine();
          ImGui::TextUnformatted("(?)");
          if (ImGui::IsItemHovered())
          {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted("Rotation display: Pitch->Yaw->Roll\nEvaluation order: Roll->Yaw->Pitch");
            ImGui::EndTooltip();
          }

          // Object Scale
          ImGui::DragFloat3("Scale", &pObject->m_Scale.x, 0.0125f, 0.125f, 1000.0f);

          // Object Model
          if (ImGui::BeginCombo("Model", A2H::namesAss2Models[pObject->m_Model]))
          {

            for (size_t j{ 0 }; j < A2H::E_NUM_MODELS; ++j)
            {
              if (ImGui::Selectable(A2H::namesAss2Models[j]))
              {
                pObject->m_Model = static_cast<A2H::enumAss2Models>(j);
                break;
              }
            }

            ImGui::EndCombo();// objModel combo
          }

          ImGui::TreePop();// testy node pop
        }
      }

    }
    ImGui::EndChild();// Objects child
    
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
  // ******************************************************* Update objects ****

  for (auto& x : m_Objects)x.updateMatrices();// check if imgui checkbox?

  // ***************************************************************************

}

void MTU::GS_Assignment_2::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[A2H::E_PIPELINE_WIREFRAME]);

  { // centered draw test
    glm::vec3 tempColor{ 0.0f, 1.0f, 0.0f };
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &m_Cam.m_W2V);
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &tempColor);
    m_DebugModels[A2H::E_DEBUGMODEL_SPHERE].draw(FCB);
  }
  { // offset red draw test
    glm::vec3 tempColor{ 1.0f, 0.0f, 0.0f };
    glm::mat4 xform{ m_Cam.m_W2V * glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 2.0f, 2.0f, 2.0f }) };
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &tempColor);
    m_DebugModels[A2H::E_DEBUGMODEL_CUBE].draw(FCB);
  }

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[A2H::E_PIPELINE_BASICLIGHT]);
  m_Pipelines[A2H::E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 16, sizeof(glm::vec3), &m_LightColor);
  for (auto& x : m_Objects)
  {
    glm::mat4 xform{ m_Cam.m_W2V * x.m_M2W };
    glm::vec3 lightpos{ x.m_W2M * glm::vec4{ m_Cam.m_Pos, 1.0f } };

    m_Pipelines[A2H::E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &lightpos);
    m_Pipelines[A2H::E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Models[x.m_Model].draw(FCB);

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
  for (auto& x : m_Models)x.destroyModel();
}

// *****************************************************************************
// ****************************************** HELPER CLASS/STRUCT FUNCTIONS ****

void A2H::Object::updateMatrices()
{
  glm::mat3 rotMat{ glm::mat3_cast(glm::quat{ m_Rot }) };

  m_M2W = glm::mat4{ rotMat * glm::mat3{ m_Scale.x, 0.0f, 0.0f, 0.0f, m_Scale.y, 0.0f, 0.0f, 0.0f, m_Scale.z } };
  m_M2W[3].x = m_Pos.x;
  m_M2W[3].y = m_Pos.y;
  m_M2W[3].z = m_Pos.z;
  m_W2M = glm::translate(glm::mat4{ glm::mat3{(m_Scale.x ? 1.0f / m_Scale.x : 0.0f), 0.0f, 0.0f, 0.0f, (m_Scale.y ? 1.0f / m_Scale.y : 0.0f), 0.0f, 0.0f, 0.0f, (m_Scale.z ? 1.0f / m_Scale.z : 0.0f) } *glm::transpose(rotMat) }, -m_Pos);
}

// *****************************************************************************

