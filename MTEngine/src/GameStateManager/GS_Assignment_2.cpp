/*!*****************************************************************************
 * @file    GS_Assignment_2.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    16 JUN 2022
 * @brief   Gamestate for assignment 2
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <random>
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
    A2H_LOAD_MODEL_HELPER(E_MODEL_4SPHERE, "4Sphere");
    A2H_LOAD_MODEL_HELPER(E_MODEL_BUNNY, "bunny");
    A2H_LOAD_MODEL_HELPER(E_MODEL_LUCY_PRINCETON, "lucy_princeton");
    A2H_LOAD_MODEL_HELPER(E_MODEL_STAR_DESTROYER, "starwars1");
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
      pipelineSetup.m_CullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;

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

  template <typename T>
  static inline float sqrLen(T const& glmVec) noexcept { return glm::dot(glmVec, glmVec); }

  template <typename T>
  static inline float sqrDist(T const& glmVecA, T const& glmVecB) noexcept { T tmp{ glmVecB - glmVecA }; return glm::dot(tmp, tmp); }

  /// @brief  Get opposite point from a sphere and a point.
  ///         This function assumes the sphere has a squared radius.
  ///         This function also ignores the possibility of p1 being s0.m_Center
  ///         because it should only be called if dist is greater than radius.
  /// @param s0 sphere input with square radius to save some time
  /// @param p1 new extreme point of the sphere
  /// @return a point on s0's circumference in the direction of s0 center from p1
  static glm::vec3 getOppositePoint(MTG::Sphere const& s0, glm::vec3 p1) noexcept
  {
    p1 = s0.m_Center - p1;  // p1 is now the vector from the point to the center
    return s0.m_Center + std::sqrtf(s0.m_Radius / sqrLen(p1)) * p1; // own optimization, ask me for more info.
  }

  /// @brief  create a new sphere with radius data member being squared as this 
  ///         is not the final sphere. It will be used for further computation.
  ///         The arguments are taken by copy to be used as center and radius. 
  /// @param inA one of 2 opposing sides of a sphere (the opposite of inB)
  /// @param inB one of 2 opposing sides of a sphere (the opposite of inA)
  /// @return new bounding sphere with a squared radius component
  static MTG::Sphere createNewSquaredSphere(glm::vec3 inA, glm::vec3 inB) noexcept
  {
    inA = 0.5f * (inA + inB); // inA now holds the center
    inB -= inA;               // inB now holds the radius vector
    return MTG::Sphere{ inA, sqrLen(inB) };
  }

  /// @brief first pass to generate the ritter bounding sphere
  /// @param inMins minimum points, 0, 1, 2 for x, y, z respectively
  /// @param inMaxs maximum points, 0, 1, 2 for x, y, z respectively 
  /// @return first squared sphere (radius component is squared)
  static MTG::Sphere ritterFirstPass(glm::mat3 const& inMins, glm::mat3 const& inMaxs) noexcept
  {
    glm::vec3 SqrDistances{ sqrDist(inMins[0], inMaxs[0]), sqrDist(inMins[1], inMaxs[1]), sqrDist(inMins[2], inMaxs[2]) };
    return
    (
      (SqrDistances.x > SqrDistances.y) ?
      (SqrDistances.x > SqrDistances.z ? createNewSquaredSphere(inMins[0], inMaxs[0]) : createNewSquaredSphere(inMins[2], inMaxs[2])) :
      (SqrDistances.y > SqrDistances.z ? createNewSquaredSphere(inMins[1], inMaxs[1]) : createNewSquaredSphere(inMins[2], inMaxs[2]))
    );
  }

  static MTG::Sphere larsonFirstPass(MTG::AABB const* pData, MTG::AABB const* pEnd, MTG::AABB const*& pLargest, float& LargestLen)
  {
    for (; pData < pEnd; ++pData)
    {
      if (float currLen{ sqrDist(pData->m_Min, pData->m_Max) }; currLen > LargestLen)
      {
        pLargest = pData;
        LargestLen = currLen;
      }
    }
    return createNewSquaredSphere(pLargest->m_Min, pLargest->m_Max);
  }

  // ***************************************************************************

  static const glm::vec3 AABB_WireColor{ 0.0625f, 1.0f, 0.0625f };
  static const glm::vec3 BS_Ritter_WireColor{ 0.0625f, 0.0625f, 1.0f };
  static const glm::vec3 BS_Larsson_WireColor{ 1.0f, 1.0f, 0.0625f };

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
  m_Objects{  },
  m_EPOS{ A2H::E_EPOS_LAST },
  m_bDrawAABB{ false },
  m_bDrawBS_Ritter{ false },
  m_bDrawBS_Larsson{ false }
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
  m_Objects.reserve(3); // reserve memory for basic scene

  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -1.0f, 0.0f, 0.0f };
    //obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A2H::E_MODEL_4SPHERE;
  }

  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 0.0f, 0.0f, -1.0f };
    //obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A2H::E_MODEL_BUNNY;
  }

  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 1.0f, 0.0f, 0.0f };
    //obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A2H::E_MODEL_LUCY_PRINCETON;
  }

  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 0.0f, 1.0f, 0.0f };
    //obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, 0.0f, 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A2H::E_MODEL_STAR_DESTROYER;
  }

  for (auto& x : m_Objects)
  {
    x.m_EposK = static_cast<int>(m_Vertices[x.m_Model].size()); // full range by default
    x.updateMatrices();                   // first matrix update
    x.computeBoundingVolumes(m_Vertices); // first bounding volume calc
  }

  // ***************************************************************************
  // *************************************************** SET BOOLS AND MORE ****

  m_EPOS = A2H::E_EPOS_LAST;

  m_bDrawAABB = false;
  m_bDrawBS_Ritter = false;
  m_bDrawBS_Larsson = false;

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

#define IMGUI_SAMELINE_TOOLTIP_HELPER(strA) ImGui::SameLine(); ImGui::TextUnformatted("(?)"); if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::TextUnformatted(strA); ImGui::EndTooltip(); }
#define IMGUI_SAMELINE_TOOLTIPV_HELPER(strA, ...) ImGui::SameLine(); ImGui::TextUnformatted("(?)"); if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::Text(strA, __VA_ARGS__); ImGui::EndTooltip(); }
#define IMGUI_COLOR_CHECKBOX_HELPER(strA, boolB, colorC) ImGui::ColorButton("color: " strA, ImVec4{ colorC.r, colorC.g, colorC.b, 1.0f }); ImGui::SameLine(); ImGui::Checkbox(strA, &boolB)

  if (ImGui::Begin("CS350Menu"))
  {
    ImGui::TextUnformatted("Camera controls [HOLD RIGHT CLICK TO LOOK AROUND]");

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

    // LARSSON EPOS SELECTOR
    if (ImGui::BeginCombo("BS Larsson EPOS", A2H::eposNames[m_EPOS]))
    {
      for (unsigned char i{ 0 }; i < A2H::E_NUM_EPOS; ++i)
      {
        if (ImGui::Selectable(A2H::eposNames[i], m_EPOS == i))
        {
          m_EPOS = i;
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("recompute Bounding Volumes"))
    {
      for (auto& x : m_Objects)
      {
        //x.updateMatrices(); // updates on change, can't click and change in this case
        x.computeBoundingVolumes(m_Vertices); // compute all bounding volumes
      }
    }
    IMGUI_SAMELINE_TOOLTIP_HELPER("Please generate bounding volumes before checking any draw boxes.\nBounding volumes only update upon user request.\nLarsson's bounding spheres will change EPOS only on recomputation\nInstead of choosing K fixed vertices, I use K random vertices,\nso recomputation of Larsson's sphere with the same K value may vary");

    // AABB draw checkbox
    IMGUI_COLOR_CHECKBOX_HELPER("draw AABB", m_bDrawAABB, A2H::AABB_WireColor);
    IMGUI_COLOR_CHECKBOX_HELPER("draw BS Ritter", m_bDrawBS_Ritter, A2H::BS_Ritter_WireColor);
    IMGUI_COLOR_CHECKBOX_HELPER("draw BS Larsson", m_bDrawBS_Larsson, A2H::BS_Larsson_WireColor);
    IMGUI_SAMELINE_TOOLTIP_HELPER("Use the selector to choose which EPOS level to use,\nadjust EPOS K on an a per object level\nEPOS K will choose a random set of contiguous vertices.\nFor consistency, the default value is max so no random values used.");

    // TODO: add checkboxes here for AABB/OBB/BS/BVH

    // SCENE OBJECTS EDITOR (Add only, lazy to support remove)
    ImGui::Separator();
    if (ImGui::BeginChild("Objects", ImVec2{ 0.0f, 7.5f * ImGui::GetFrameHeightWithSpacing() }, true))
    {

      for (size_t i{ 0 }, t{ m_Objects.size() }; i < t; ++i)
      {

        bool shouldUpdateTransform{ false };

        // quick and dirty ptr_id, idk if it will cause any leaks with
        // imgui when the vector is resized. Hopefully not, but this
        // isn't meant to be a long term framework, just for assignment 2.
        A2H::Object* pObject{ m_Objects.data() + i };
        
        if (ImGui::TreeNode(pObject, "Object %d", static_cast<int>(i)))
        {
          // Object Position
          if (ImGui::DragFloat3("Pos", &pObject->m_Pos.x, 0.125f, -1000.0f, 1000.0f))
          {
            shouldUpdateTransform = true;
          }

          // Object Rotation
          if (ImGui::DragFloat3("Rot", &pObject->m_Rot.x, 0.0125f, -glm::pi<float>(), glm::pi<float>()))
          {
            shouldUpdateTransform = true;
          }
          IMGUI_SAMELINE_TOOLTIP_HELPER("Rotation display: Pitch->Yaw->Roll\nEvaluation order: Roll->Yaw->Pitch");

          // Object Scale
          if (ImGui::DragFloat3("Scale", &pObject->m_Scale.x, 0.0125f, 0.125f, 1000.0f))
          {
            shouldUpdateTransform = true;
          }

          // Object Model
          if (ImGui::BeginCombo("Model", A2H::namesAss2Models[pObject->m_Model]))
          {

            for (size_t j{ 0 }; j < A2H::E_NUM_MODELS; ++j)
            {
              if (ImGui::Selectable(A2H::namesAss2Models[j], j == pObject->m_Model))
              {
                pObject->m_Model = static_cast<A2H::enumAss2Models>(j);
                pObject->m_EposK = static_cast<int>(m_Vertices[pObject->m_Model].size());
                break;
              }
            }

            ImGui::EndCombo();// objModel combo
          }

          // Object eposK for Larsson's BS
          if (int v_max{ static_cast<int>(m_Vertices[pObject->m_Model].size()) }; ImGui::DragInt("EPOS K", &pObject->m_EposK, 1.0f, 0, v_max))
          {
            if (pObject->m_EposK > v_max)pObject->m_EposK = v_max;// hard clamp
          }

          ImGui::TreePop();// testy node pop
        }

        if (shouldUpdateTransform)pObject->updateMatrices();

      }

    }
    ImGui::EndChild();// Objects child
    
  }
  ImGui::End();

#undef IMGUI_SAMELINE_TOOLTIP_HELPER
#undef IMGUI_SAMELINE_TOOLTIPV_HELPER
#undef IMGUI_COLOR_CHECKBOX_HELPER

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

  //for (auto& x : m_Objects)x.updateMatrices();// done only on imgui update

  // ***************************************************************************

}

void MTU::GS_Assignment_2::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

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

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[A2H::E_PIPELINE_WIREFRAME]);

  if (true == m_bDrawAABB)
  {
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A2H::AABB_WireColor);
    for (auto& x : m_Objects)
    {
      glm::mat4 xform{ m_Cam.m_W2V * A2H::getAABBMat(x.m_AABB) };

      // TODO color based on BVH level
      m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
      
      m_DebugModels[A2H::E_DEBUGMODEL_CUBE].draw(FCB);
    }
  }

  if (true == m_bDrawBS_Ritter)
  {
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A2H::BS_Ritter_WireColor);
    for (auto& x : m_Objects)
    {
      glm::mat4 xform{ m_Cam.m_W2V * A2H::getBSMat(x.m_BS_Ritter) };

      // TODO color based on BVH level
      m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);

      m_DebugModels[A2H::E_DEBUGMODEL_SPHERE].draw(FCB);
    }
  }

  if (true == m_bDrawBS_Larsson)
  {
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A2H::BS_Larsson_WireColor);
    for (auto& x : m_Objects)
    {
      glm::mat4 xform{ m_Cam.m_W2V * A2H::getBSMat(x.m_BS_Larsson[m_EPOS])};

      // TODO color based on BVH level
      m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);

      m_DebugModels[A2H::E_DEBUGMODEL_SPHERE].draw(FCB);
    }
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

void A2H::Object::computeBoundingVolumes(MVA const& inModelVertexArray)
{
  // setup AABB to be computed at the same time as transformation
  glm::mat3 cardinalMin // 0, 1, 2 is x, y, z min vecs respectively
  {
    glm::vec3{ std::numeric_limits<float>::max() },
    glm::vec3{ std::numeric_limits<float>::max() },
    glm::vec3{ std::numeric_limits<float>::max() }
  };
  glm::mat3 cardinalMax // 0, 1, 2 is x, y, z max vecs respectively
  {
    glm::vec3{ std::numeric_limits<float>::lowest() },
    glm::vec3{ std::numeric_limits<float>::lowest() },
    glm::vec3{ std::numeric_limits<float>::lowest() }
  };

  // transform vertices into a copy to compute all the BVs in world coords
  VV vertices;
  vertices.reserve(inModelVertexArray.size());
  std::transform
  (
    inModelVertexArray[m_Model].begin(),
    inModelVertexArray[m_Model].end(),
    std::back_inserter(vertices),
    [&](glm::vec3 const& inVertex)
    {
      glm::vec3 retval{ m_M2W * glm::vec4{ inVertex, 1.0f } };

      // setup cardinal min max while we at it to use with ritter sphere and AABB
      if (retval.x < cardinalMin[0].x)cardinalMin[0] = retval;
      if (retval.x > cardinalMax[0].x)cardinalMax[0] = retval;
      if (retval.y < cardinalMin[1].y)cardinalMin[1] = retval;
      if (retval.y > cardinalMax[1].y)cardinalMax[1] = retval;
      if (retval.z < cardinalMin[2].z)cardinalMin[2] = retval;
      if (retval.z > cardinalMax[2].z)cardinalMax[2] = retval;

      return retval;// NRVO since we want to do more with 1 loop
    }
  );

  // AABB from cardinal min max
  m_AABB.m_Min.x = cardinalMin[0].x;
  m_AABB.m_Min.y = cardinalMin[1].y;
  m_AABB.m_Min.z = cardinalMin[2].z;
  m_AABB.m_Max.x = cardinalMax[0].x;
  m_AABB.m_Max.y = cardinalMax[1].y;
  m_AABB.m_Max.z = cardinalMax[2].z;

  // Ritter sphere from cardinal min max
  m_BS_Ritter = ritterFirstPass(cardinalMin, cardinalMax);
  for (auto const& x : vertices)
  {
    if (sqrDist(x, m_BS_Ritter.m_Center) > m_BS_Ritter.m_Radius)
    {
      m_BS_Ritter = createNewSquaredSphere(getOppositePoint(m_BS_Ritter, x), x);
    }
  }
  m_BS_Ritter.m_Radius = std::sqrtf(m_BS_Ritter.m_Radius);

  // Modified Larsson's sphere
  {
    std::mt19937 randomEngine{ std::random_device{}() };
    size_t r{ std::uniform_int_distribution<size_t>{ 0, static_cast<size_t>(vertices.size() - m_EposK) }(randomEngine) };
    size_t rt{ r + m_EposK };
    // r to rt find support points, 0 to r and rt to end expand circle

    std::array<MTG::AABB, eposNumVecs[E_EPOS_LAST]> checkedExtents;
    {
      std::array<glm::vec2, eposNumVecs[E_EPOS_LAST]> projExtents;
      std::fill(projExtents.begin(), projExtents.end(), glm::vec2{ std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest() });
      for (size_t ri{ r }; ri < rt; ++ri)// finding support points off k random verts
      {
        glm::vec3 const& x{ vertices[ri] };
        // j is specially to access checkedExtents
        for (size_t i{ 0 }, t{ eposNumVecs[E_EPOS_LAST] }; i < t; ++i)
        {
          float projDist{ glm::dot(x, eposVecs[i]) };
          if (projDist < projExtents[i].x)
          {
            checkedExtents[i].m_Min = x;
            projExtents[i].x = projDist;
          }
          if (projDist > projExtents[i].y)
          {
            checkedExtents[i].m_Max = x;
            projExtents[i].y = projDist;
          }
        }
        // done checking all directions for this vertex (^_;
      }
    }
    // got rid of reuse because it makes EPOS K calculations bias towards x y z
    //checkedExtents[0].m_Min = cardinalMin[0];
    //checkedExtents[0].m_Max = cardinalMax[0];
    //checkedExtents[1].m_Min = cardinalMin[1];
    //checkedExtents[1].m_Max = cardinalMax[1];
    //checkedExtents[2].m_Min = cardinalMin[2];
    //checkedExtents[2].m_Max = cardinalMax[2];

    // initial setup, reuse previous EPOS calculations
    {
      MTG::AABB const* pLargest{ checkedExtents.data() };
      float largestLen{ sqrDist(pLargest->m_Min, pLargest->m_Max) };
      MTG::AABB const* pBegin{ checkedExtents.data() + 1 };
      

      for (size_t i{ 0 }; i < E_NUM_EPOS; ++i)
      {
        MTG::AABB const* pEnd{ checkedExtents.data() + eposNumVecs[i] };
        m_BS_Larsson[i] = larsonFirstPass(pBegin, pEnd, pLargest, largestLen);
        pBegin = pEnd;
      }
    }

    for (size_t ri{ 0 }; ri < rt; ++ri)
    {
      glm::vec3 const& x{ vertices[ri] };
      for (auto& BS : m_BS_Larsson)
      {
        if (sqrDist(x, BS.m_Center) > BS.m_Radius)
        {
          BS = createNewSquaredSphere(getOppositePoint(BS, x), x);
        }
      }
    }
    for (size_t ri{ rt }, re{ vertices.size() }; ri < re; ++ri)
    {
      glm::vec3 const& x{ vertices[ri] };
      for (auto& BS : m_BS_Larsson)
      {
        if (sqrDist(x, BS.m_Center) > BS.m_Radius)
        {
          BS = createNewSquaredSphere(getOppositePoint(BS, x), x);
        }
      }
    }

    for (auto& BS : m_BS_Larsson)BS.m_Radius = std::sqrtf(BS.m_Radius);
  }

  // Compute other BVs?

}

// *****************************************************************************
// ******************************************************** OTHER FUNCTIONS ****

glm::mat4 A2H::getAABBMat(MTG::AABB const& inAABB) noexcept
{ // extent is the scale, debug cube is normalized [-0.5f, 0.5f], thus full extent for scale.
  glm::vec3 extent{ inAABB.m_Max - inAABB.m_Min }, center{ inAABB.m_Min + 0.5f * extent };
  return glm::mat4{ extent.x, 0.0f, 0.0f, 0.0f, 0.0f, extent.y, 0.0f, 0.0f, 0.0f, 0.0f, extent.z, 0.0f, center.x, center.y, center.z, 1.0f };
}

glm::mat4 A2H::getBSMat(MTG::Sphere const& inBS) noexcept
{ // normalized sphere model, diameter is 1 so take 2 * radius for matrix scale
  float uniformScale{ 2.0f * inBS.m_Radius };
  return glm::mat4{ uniformScale, 0.0f, 0.0f, 0.0f, 0.0f, uniformScale, 0.0f, 0.0f, 0.0f, 0.0f, uniformScale, 0.0f, inBS.m_Center.x, inBS.m_Center.y, inBS.m_Center.z, 1.0f };
}

// *****************************************************************************
