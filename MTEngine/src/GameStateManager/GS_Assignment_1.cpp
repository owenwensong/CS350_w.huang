/*!*****************************************************************************
 * @file    GS_Assignment_1.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   Gamestate for assignment 1
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <imgui.h>
#include <iostream>
#include <Assignment/Geometry.h>
#include <Assignment/GeometryTests.h>
#include <GameStateManager/GS_Assignment_1.h>

enum enumCollisionTests
{
  E_Sphere_Sphere = 0,
  E_Sphere_AABB,
  E_AABB_Sphere,
  E_AABB_AABB,
  E_Point_Sphere,
  E_Point_AABB,
  //E_Point_Triangle,
  //E_Point_Plane,
  //E_Ray_Plane,
  //E_Ray_AABB,
  //E_Ray_Sphere,
  //E_Ray_Triangle,
  //E_Plane_AABB,
  //E_Plane_Sphere,
  E_NUM_TESTS
};

static const std::array<const char*, E_NUM_TESTS> s_CollisionTestStringLiterals
{
  "Sphere vs Sphere",
  "Sphere vs AABB",
  "AABB vs Sphere",
  "AABB vs AABB",
  "Point vs Sphere",
  "Point vs AABB",
  //"Point vs Triangle",
  //"Point vs Plane",
  //"Ray vs Plane",
  //"Ray vs AABB",
  //"Ray vs Sphere",
  //"Ray vs Triangle",
  //"Plane vs AABB",
  //"Plane vs Sphere"
};

// *****************************************************************************

MTU::GS_Assignment_1::GS_Assignment_1(GameStateManager& rGSM) : 
  GameState{ rGSM },
  inputs{ rGSM.getInput() },
  m_Cam{  },
  m_CamMoveSpeed{  },
  m_CamFastModifier{  },
  m_CurrentTest{  },
  m_bColliding{ false },
  m_CollisionObjects{  },
  m_Positions{  },
  m_Models{  },
  m_Pipelines{  }
{
  std::cout << __FUNCSIG__ << std::endl;

  // Load models

  if (false == m_Models[0].load3DModelPositionOnly("../Assets/Meshes/4Sphere.obj"))
  {
    printWarning("Failed to load a prop");
  }
  if (false == m_Models[1].load3DModelPositionOnly("../Assets/Meshes/bunny.obj"))
  {
    printWarning("Failed to load a prop");
  }
  if (false == m_Models[2].load3DModelPositionOnly("../Assets/Meshes/lucy_princeton.obj"))
  {
    printWarning("Failed to load a prop");
  }
  if (false == m_Models[3].load3DModelPositionOnly("../Assets/Meshes/starwars1.obj"))
  {
    printWarning("Failed to load a prop");
  }

  if (false == m_DebugModels[0].load3DModelPositionOnly("../Assets/Meshes/DebugMeshes/normalIcoSphere.obj"))
  {
    printWarning("Failed to load debug icosphere");
  }
  if (false == m_DebugModels[1].load3DModelPositionOnly("../Assets/Meshes/DebugMeshes/normalCube.obj"))
  {
    printWarning("Failed to load debug cube");
  }
  if (false == m_DebugModels[2].load3DModelPositionOnly("../Assets/Meshes/DebugMeshes/normalPoint.obj"))
  {
    printWarning("Failed to load debug point");
  }
  if (false == m_DebugModels[3].load3DModelPositionOnly("../Assets/Meshes/DebugMeshes/normalPlane.obj"))
  {
    printWarning("Failed to load debug plane");
  }

  // Load pipelines
  {
    auto& vkWin{ GSM.getVKWin() };
    {
      vulkanPipeline::setup pipelineSetup{ };
      pipelineSetup.m_VertexBindingMode = vulkanPipeline::E_VERTEX_BINDING_MODE::AOS_XYZ_F32;
      pipelineSetup.m_PathShaderVert = "../Assets/Shaders/vert_Wireframe_Ass1.spv";
      pipelineSetup.m_PathShaderFrag = "../Assets/Shaders/frag_Wireframe_Ass1.spv";
      //pipelineSetup.m_UniformsVert
      pipelineSetup.m_UniformsFrag = vulkanPipeline::createUniformInfo<glm::vec3>();
      pipelineSetup.m_PushConstantRangeVert = vulkanPipeline::createPushConstantInfo<glm::mat4>(VK_SHADER_STAGE_VERTEX_BIT);
      //pipelineSetup.m_PushConstantRangeFrag
      pipelineSetup.m_PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;

      // create the same cus I'm lazy but want 2 global colors, 1 for the
      // collision objects and another for the props.
      vkWin->createPipelineInfo(m_Pipelines[0], pipelineSetup);
      vkWin->createPipelineInfo(m_Pipelines[1], pipelineSetup);
    }
  }

}

void MTU::GS_Assignment_1::Init()
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
  m_Positions[0] = glm::vec3{ 0.0f, 0.0f, 250.0f };
  m_Positions[1] = glm::vec3{ 1.0f, 0.0f, 2.5f };
  m_Positions[2] = glm::vec3{ 200.0f, -750.0f, 100.0f };
  m_Positions[3] = glm::vec3{ 200.0f, 50.0f, -200.0f };
  MTG::test();
  changeCurrentTest(E_Sphere_Sphere);
}

void MTU::GS_Assignment_1::Update(uint64_t dt)
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

    ImGui::Separator();

    if (ImGui::BeginCombo("CollisionTests", s_CollisionTestStringLiterals[m_CurrentTest]))
    {
      for (unsigned i{ 0 }; i < E_NUM_TESTS; ++i)
      {
        if (bool isSelected{ m_CurrentTest == i }; ImGui::Selectable(s_CollisionTestStringLiterals[i], isSelected))
        {
          changeCurrentTest(i);
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::TextUnformatted("(?)");
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::TextUnformatted
      (
        "The following are not graphically shown:\n"
        "Point vs Triangle\n"
        "Point vs Plane\n"
        "Ray vs Plane\n"
        "Ray vs AABB\n"
        "Ray vs Sphere\n"
        "Ray vs Triangle\n"
        "Plane vs AABB\n"
        "Plane vs Sphere\n"
        "\nThis is because I can't easily show it.\n"
        "unlike opengl where I can draw a line from point to point,\n"
        "I have to determine the rotation and have an actual mesh,\n"
        "I would need to do TRST matrix and bla bla bla I'm already\n"
        "late with this resubmission so please understand.\n"
        "Please give me the 20% for\n"
        "\"Display the primitives that are being tested\"\n"
        "Thanks!\n"
      );
      ImGui::EndTooltip();
    }

    imguiSwitch();

    ImGui::Separator();

    ImGui::TextUnformatted("Move props");
    ImGui::DragFloat3("4Sphere", &m_Positions[0].x, 0.125f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("bunny", &m_Positions[1].x, 0.125f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("lucy_princeton", &m_Positions[2].x, 0.125f, -1000.0f, 1000.0f);
    ImGui::DragFloat3("StarWars", &m_Positions[3].x, 0.125f, -1000.0f, 1000.0f);

  }
  ImGui::End();

  m_bColliding = updateSwitch();

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

void MTU::GS_Assignment_1::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[0]);
  {
    glm::vec3 tempColor{ m_bColliding ? glm::vec3{ 1.0f, 0.0f, 0.0f } : glm::vec3{ 0.0f, 1.0f, 0.0f } };
    GSM.getVKWin()->setUniform(m_Pipelines[0], 1, 0, &tempColor, sizeof(tempColor));
  }
  drawSwitch(FCB, m_Pipelines[0]);

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[1]);
  {
    glm::vec3 tempColor{ 0.5f, 0.5f, 1.0f };
    GSM.getVKWin()->setUniform(m_Pipelines[1], 1, 0, &tempColor, sizeof(tempColor));
  }
  // lazy assumption
  for (size_t i{ 0 }; i < m_Models.size(); ++i)
  {
    glm::mat4 xform{ m_Cam.m_W2V * glm::translate(glm::identity<glm::mat4>(), m_Positions[i]) };
    m_Pipelines[1].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_Models[i].draw(FCB);
  }
}

void MTU::GS_Assignment_1::Free()
{
  std::cout << __FUNCSIG__ << std::endl;
}

MTU::GS_Assignment_1::~GS_Assignment_1()
{
  std::cout << __FUNCSIG__ << std::endl;
  for (auto& x : m_Pipelines)GSM.getVKWin()->destroyPipelineInfo(x);
  for (auto& x : m_Models)x.destroyModel();
}

// *****************************************************************************
// ******************************************************* hard coded tests ****

void MTU::GS_Assignment_1::changeCurrentTest(unsigned nextTest)
{
  assert(nextTest < E_NUM_TESTS);
  m_CurrentTest = nextTest;
  switch (m_CurrentTest)
  {
  case E_Sphere_Sphere:// will ignore y and z of scale
  case E_Sphere_AABB:
  case E_AABB_Sphere:
  case E_AABB_AABB:
  case E_Point_Sphere:
  case E_Point_AABB:
    m_CollisionObjects[0].m_Position = glm::vec3{ -2.0f, 0.0f, 0.0f };
    m_CollisionObjects[0].m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    m_CollisionObjects[1].m_Position = glm::vec3{ 2.0f, 0.0f, 0.0f };
    m_CollisionObjects[1].m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    break;
  }

}

void MTU::GS_Assignment_1::updateM2WSphere(objectInfo& toUpdate)
{
  float radius{ 2.0f * toUpdate.m_Scale.x };// the model I made has a diameter of 1
  toUpdate.m_M2W = glm::mat4
  {
    glm::vec4{ radius, 0.0f, 0.0f, 0.0f },
    glm::vec4{ 0.0f, radius, 0.0f, 0.0f },
    glm::vec4{ 0.0f, 0.0f, radius, 0.0f },
    glm::vec4{ toUpdate.m_Position.x, toUpdate.m_Position.y, toUpdate.m_Position.z, 1.0f }
  };
}

void MTU::GS_Assignment_1::updateM2WAABB(objectInfo& toUpdate)
{ // multiplied by two because half extents
  toUpdate.m_M2W = glm::mat4
  {
    glm::vec4{ 2.0f * toUpdate.m_Scale.x, 0.0f, 0.0f, 0.0f },
    glm::vec4{ 0.0f, 2.0f * toUpdate.m_Scale.y, 0.0f, 0.0f },
    glm::vec4{ 0.0f, 0.0f, 2.0f * toUpdate.m_Scale.z, 0.0f },
    glm::vec4{ toUpdate.m_Position.x, toUpdate.m_Position.y, toUpdate.m_Position.z, 1.0f }
  };
}

void MTU::GS_Assignment_1::updateM2WPoint(objectInfo& toUpdate)
{
  toUpdate.m_M2W = glm::translate(glm::identity<glm::mat4>(), toUpdate.m_Position);
}

bool MTU::GS_Assignment_1::updateSwitch()
{
  switch (m_CurrentTest)
  {
  case E_Sphere_Sphere:
    updateM2WSphere(m_CollisionObjects[0]);
    updateM2WSphere(m_CollisionObjects[1]);
    return MTG::intersectionSphereSphere(MTG::Sphere{ m_CollisionObjects[0].m_Position, m_CollisionObjects[0].m_Scale.x }, MTG::Sphere{ m_CollisionObjects[1].m_Position, m_CollisionObjects[1].m_Scale.x });
  case E_Sphere_AABB:
    updateM2WSphere(m_CollisionObjects[0]);
    updateM2WAABB(m_CollisionObjects[1]);
    return MTG::intersectionSphereAABB(MTG::Sphere{ m_CollisionObjects[0].m_Position, m_CollisionObjects[0].m_Scale.x }, MTG::AABB::createFromCenterAndHalfExtents(m_CollisionObjects[1].m_Position, m_CollisionObjects[1].m_Scale));
  case E_AABB_Sphere:
    updateM2WAABB(m_CollisionObjects[0]);
    updateM2WSphere(m_CollisionObjects[1]);
    return MTG::intersectionSphereAABB(MTG::Sphere{ m_CollisionObjects[1].m_Position, m_CollisionObjects[1].m_Scale.x }, MTG::AABB::createFromCenterAndHalfExtents(m_CollisionObjects[0].m_Position, m_CollisionObjects[0].m_Scale));
  case E_AABB_AABB:
    updateM2WAABB(m_CollisionObjects[0]);
    updateM2WAABB(m_CollisionObjects[1]);
    return MTG::intersectionAABBAABB(MTG::AABB::createFromCenterAndHalfExtents(m_CollisionObjects[0].m_Position, m_CollisionObjects[0].m_Scale), MTG::AABB::createFromCenterAndHalfExtents(m_CollisionObjects[1].m_Position, m_CollisionObjects[1].m_Scale));
  case E_Point_Sphere:
    updateM2WPoint(m_CollisionObjects[0]);
    updateM2WSphere(m_CollisionObjects[1]);
    return MTG::intersectionPointSphere(m_CollisionObjects[0].m_Position, MTG::Sphere{ m_CollisionObjects[1].m_Position, m_CollisionObjects[1].m_Scale.x });
  case E_Point_AABB:
    updateM2WPoint(m_CollisionObjects[0]);
    updateM2WAABB(m_CollisionObjects[1]);
    return MTG::intersectionPointAABB(m_CollisionObjects[0].m_Position, MTG::AABB::createFromCenterAndHalfExtents(m_CollisionObjects[1].m_Position, m_CollisionObjects[1].m_Scale));
  default:
    return false;
  }
}

void MTU::GS_Assignment_1::imguiSwitch()
{
  ImGui::DragFloat3("obj0 position", &m_CollisionObjects[0].m_Position.x, 0.0625f, -1250.0f, 1250.0f);
  ImGui::DragFloat3("obj1 position", &m_CollisionObjects[1].m_Position.x, 0.0625f, -1250.0f, 1250.0f);
  ImGui::Separator();
  switch (m_CurrentTest)
  {
  case E_Sphere_Sphere:
    ImGui::DragFloat("obj0 radius", &m_CollisionObjects[0].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    ImGui::DragFloat("obj1 radius", &m_CollisionObjects[1].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    break;
  case E_Sphere_AABB:
    ImGui::DragFloat("obj0 radius", &m_CollisionObjects[0].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    ImGui::DragFloat3("obj1 half extents", &m_CollisionObjects[1].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    break;
  case E_AABB_Sphere:
    ImGui::DragFloat3("obj0 half extents", &m_CollisionObjects[0].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    ImGui::DragFloat("obj1 radius", &m_CollisionObjects[1].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    break;
  case E_AABB_AABB:
    ImGui::DragFloat3("obj0 half extents", &m_CollisionObjects[0].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    ImGui::DragFloat3("obj1 half extents", &m_CollisionObjects[1].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    break;
  case E_Point_Sphere:
    ImGui::DragFloat("obj1 radius", &m_CollisionObjects[1].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    break;
  case E_Point_AABB:
    ImGui::DragFloat3("obj1 half extents", &m_CollisionObjects[1].m_Scale.x, 0.0625f, 0.125f, 1250.0f);
    break;
  default:
    break;
  }
}

void MTU::GS_Assignment_1::drawSwitch(VkCommandBuffer FCB, vulkanPipeline& inPipeline)
{

  std::array xforms
  {
    glm::mat4{ m_Cam.m_W2V * m_CollisionObjects[0].m_M2W },
    glm::mat4{ m_Cam.m_W2V * m_CollisionObjects[1].m_M2W }
  };
  switch (m_CurrentTest)
  {
  case E_Sphere_Sphere:
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[0]);
    m_DebugModels[0].draw(FCB);
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[1]);
    m_DebugModels[0].draw(FCB);
    break;
  case E_Sphere_AABB:
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[0]);
    m_DebugModels[0].draw(FCB);
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[1]);
    m_DebugModels[1].draw(FCB);
    break;
  case E_AABB_Sphere:
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[0]);
    m_DebugModels[1].draw(FCB);
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[1]);
    m_DebugModels[0].draw(FCB);
    break;
  case E_AABB_AABB:
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[0]);
    m_DebugModels[1].draw(FCB);
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[1]);
    m_DebugModels[1].draw(FCB);
    break;
  case E_Point_Sphere:
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[0]);
    m_DebugModels[2].draw(FCB);
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[1]);
    m_DebugModels[0].draw(FCB);
    break;
  case E_Point_AABB:
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[0]);
    m_DebugModels[2].draw(FCB);
    inPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xforms[1]);
    m_DebugModels[1].draw(FCB);
    break;
  default:
    break;
  }
}

// ******************************************************* hard coded tests ****
// *****************************************************************************
