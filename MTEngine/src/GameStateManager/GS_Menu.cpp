/*!*****************************************************************************
 * @file    GS_Menu.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    20 AUG 2022
 * @brief   Gamestate for menu
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <GameStateManager/GS_Menu.h>

#include <imgui.h>
#include <utility/matrixTransforms.h>

// *****************************************************************************
// ******************************************************* Helper Functions ****

namespace MenuHelper
{
  static inline constexpr ImGuiWindowFlags imguiWindowFlag
  {
    ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoSavedSettings
    | ImGuiWindowFlags_NoTitleBar
    | ImGuiWindowFlags_AlwaysAutoResize
    | ImGuiWindowFlags_NoBackground
  };
}

// *****************************************************************************
// **************************************************** Gamestate functions ****

MTU::GS_Menu::GS_Menu(GameStateManager& rGSM) : 
  GameState{ rGSM },
  inputs{ rGSM.getInput() },
  m_Cam{  },
  m_CamMoveSpeed{  },
  m_CamFastModifier{  },
  m_BasicPipeline{  },
  m_QuestionMark{  },
  m_ObjTimer{  },
  m_ExampleDragInt{  },
  m_ExampleDragFloat{  },
  m_bExampleCheckbox{ false },
  m_bExampleColorCheckboxR{ false },
  m_bExampleColorCheckboxG{ false },
  m_bExampleColorCheckboxB{ false }
{
  {
    vulkanPipeline::setup pipelineSetup{ };
    pipelineSetup.m_VertexBindingMode = vulkanPipeline::E_VERTEX_BINDING_MODE::AOS_XYZ_RGB_F32;
    pipelineSetup.m_PathShaderVert = "../Assets/Shaders/vert_BasicLight_Ass3.spv";
    pipelineSetup.m_PathShaderFrag = "../Assets/Shaders/frag_BasicLight_Ass3.spv";
    //pipelineSetup.m_UniformsVert
    //pipelineSetup.m_UniformsFrag = vulkanPipeline::createUniformInfo<glm::vec3>();
    pipelineSetup.m_PushConstantRangeVert = vulkanPipeline::createPushConstantInfo<glm::mat4>(VK_SHADER_STAGE_VERTEX_BIT);
    // using vec4 here to maintain 16B alignment, there is essentially 4B of dead space here
    pipelineSetup.m_PushConstantRangeFrag = vulkanPipeline::createPushConstantInfo<glm::vec4, glm::vec3>(VK_SHADER_STAGE_FRAGMENT_BIT);
    pipelineSetup.m_PolygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;

    if (false == rGSM.getVKWin()->createPipelineInfo(m_BasicPipeline, pipelineSetup))
    {
      printWarning("Failed to load pipeline: vert_Wireframe_Ass2");
    }
  }
  {
    std::vector<glm::vec3> throwAway;
    if (false == m_QuestionMark.load3DNmlModel("../Assets/Meshes/""QuestionMark.obj", throwAway, true))
    {
      printWarning("Failed to load model: ""QuestionMark.obj");
    }
  }
}

void MTU::GS_Menu::Init()
{
  m_Cam.m_AspectRatio = static_cast<float>(GSM.getVKWin()->m_windowsWindow.getWidth()) / GSM.getVKWin()->m_windowsWindow.getHeight();
  m_Cam.m_FOV = glm::radians(75.0f);
  // default constructed sensitivity
  m_Cam.m_Near = 0.000625f;
  m_Cam.m_Far = 25000.0f;
  m_Cam.m_Pos = glm::vec3{ -1.25f, 0.25f, 1.5f };
  m_Cam.m_Rot = glm::vec2{ glm::half_pi<float>(), 0.0f};
  {
    glm::ivec2 currCursorPos;// no need to initialize
    inputs.getCursorPos(currCursorPos.x, currCursorPos.y);
    m_Cam.updateCursor(currCursorPos, false);
  }
  m_Cam.updateForwardVector();
  m_Cam.updateMatrix();
  m_CamMoveSpeed = 2.5f;
  m_CamFastModifier = 2.0f;

  m_ObjTimer = 0.0f;

  m_bExampleCheckbox = true;
  m_bExampleColorCheckboxR = true;
  m_bExampleColorCheckboxG = true;
  m_bExampleColorCheckboxB = true;

  m_ExampleDragInt = 69;
  m_ExampleDragFloat = 420.0f;
}

void MTU::GS_Menu::Update(uint64_t dt)
{
  if (inputs.isTriggered(VK_F1))GSM.setNextGameState(GS::E_ASSIGNMENT_1);
  if (inputs.isTriggered(VK_F2))GSM.setNextGameState(GS::E_ASSIGNMENT_2);
  if (inputs.isTriggered(VK_F3))GSM.setNextGameState(GS::E_ASSIGNMENT_3);
  if (inputs.isTriggered(VK_F4))GSM.setNextGameState(GS::E_RESTART);
  // ***************************************************************************
  // **************************************************************** ImGui ****

#define IMGUI_SAMELINE_TOOLTIP_HELPER(strA) ImGui::SameLine(); ImGui::TextUnformatted("(?)"); if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::TextUnformatted(strA); ImGui::EndTooltip(); }
#define IMGUI_SAMELINE_TOOLTIPV_HELPER(strA, ...) ImGui::SameLine(); ImGui::TextUnformatted("(?)"); if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::Text(strA, __VA_ARGS__); ImGui::EndTooltip(); }
#define IMGUI_COLOR_CHECKBOX_HELPER(strA, boolB, colorR, colorG, colorB) ImGui::ColorButton("color: " strA, ImVec4{ colorR, colorG, colorB, 1.0f }); ImGui::SameLine(); ImGui::Checkbox(strA, &boolB)

  if (ImGui::Begin("CS350 Demo Main Menu", nullptr, MenuHelper::imguiWindowFlag))
  {
    ImGui::TextUnformatted("****************************************************************************************************\nWelcome to CS350 Demo by Owen Huang Wensong.\n");
    ImGui::Separator();
    // *************************************************************************
    // ************************************************************* README ****
    if (ImGui::TreeNode("About this program"))
    {
      ImGui::Separator();
      ImGui::TextUnformatted("This Demo showcases various submissions for CS350 Advanced Computer Graphics II.\nThe framework behind this program is written by me, it uses the Vulkan graphics API.\nThe framework was originally written for C++20 but has been rewritten to target C++17.");
      ImGui::Separator();
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("How to use the GUI"))
    {
      ImGui::Separator();
      ImGui::Spacing();
      ImGui::TextUnformatted("Tooltips: Hover over bracketed question marks for tooltips ->");
      IMGUI_SAMELINE_TOOLTIPV_HELPER("These tooltips contain information of the subject on the same line. This may come in the form of\n\ta) How to use it\n\tb) How it works\n\tc) Why it was done in a certain way");
      ImGui::Separator();
      ImGui::Spacing();

      ImGui::TextUnformatted("Checkboxes: Checkboxes set the setting described by the text on it's right.");
      IMGUI_SAMELINE_TOOLTIP_HELPER("Special color checkboxes show the color that will be drawn when when that setting is checked on the left of the checkbox.")
      ImGui::Spacing();
      ImGui::Checkbox("Example Checkbox: Object rotation", &m_bExampleCheckbox);
      IMGUI_SAMELINE_TOOLTIP_HELPER("Toggles the rotation of the question mark object in the bottom right.");
      IMGUI_COLOR_CHECKBOX_HELPER("Example Color Checkbox R", m_bExampleColorCheckboxR, 1.0f, 0.0f, 0.0f);
      IMGUI_SAMELINE_TOOLTIPV_HELPER("%s Color Light component shining on the question mark object in the bottom right.", "Red");
      IMGUI_COLOR_CHECKBOX_HELPER("Example Color Checkbox G", m_bExampleColorCheckboxG, 0.0f, 1.0f, 0.0f);
      IMGUI_SAMELINE_TOOLTIPV_HELPER("%s Color Light component shining on the question mark object in the bottom right.", "Green");
      IMGUI_COLOR_CHECKBOX_HELPER("Example Color Checkbox B", m_bExampleColorCheckboxB, 0.0f, 0.0f, 1.0f);
      IMGUI_SAMELINE_TOOLTIPV_HELPER("%s Color Light component shining on the question mark object in the bottom right.", "Blue");
      ImGui::Separator();
      ImGui::Spacing();

      ImGui::TextUnformatted("Sliders: Sliders allow you to set a value by dragging or double clicking then typing");
      ImGui::Spacing();
      ImGui::DragInt("Integer example", &m_ExampleDragInt, 1.0f, 0, 100);
      IMGUI_SAMELINE_TOOLTIP_HELPER("These sliders may come with limits, [0, 100] in this case.\nDouble clicking allows keyboard inputs, which may allow overriding these limits.\nIn sensitive cases, the limits will be respected even when double clicking.");
      ImGui::DragFloat("Float example", &m_ExampleDragFloat, 0.125f, -6.9f, 420.0f);
      IMGUI_SAMELINE_TOOLTIP_HELPER("These sliders may be more or less sensitive, less sensitive in this case.");
      ImGui::Separator();
      ImGui::Spacing();

      ImGui::TextUnformatted("Buttons: Click on buttons to trigger an action");
      IMGUI_SAMELINE_TOOLTIP_HELPER("The action is typically determined by the text on the button or the tooltip next to it.");
      ImGui::Spacing();
      if (ImGui::Button("Example Button: Print slider values to console"))printf_s("Example button: %d, %f\n", m_ExampleDragInt, m_ExampleDragFloat);
      IMGUI_SAMELINE_TOOLTIP_HELPER("The console is the other window with text on it that starts up with this program.");
      ImGui::Separator();

      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Common Camera controls"))
    {
      IMGUI_SAMELINE_TOOLTIP_HELPER("controls are not active in main menu.");
      ImGui::Separator();
      ImGui::Text("Right Mouse Button (hold): look around\nW: Move Forward\nA: Move Left\nS: Move Back\nD: Move Right\nSPACE: Move Upwards\nCONTROL: Move Downwards\nSHIFT (hold): Use speed multiplier");
      ImGui::Separator();
      ImGui::TreePop();
    }
    if (ImGui::TreeNode("Assignment descriptions"))
    {
      if (ImGui::TreeNode("Assignment 1: Geometry Toolbox"))
      {
        ImGui::Separator();
        ImGui::TextUnformatted("The purpose of this assignment is to be able to construct and use core bounding\nvolumes (Bounding Spheres and AABBs) for simple optimizations.\nThis includes using them for bounding volume spatial partitions\nto optimize ray-casting, frustumcasting, and pair-queries.");
        ImGui::TextUnformatted("The following primitive tests have been implemented:\n\tSphere vs Sphere\n\tAABB vs Sphere\n\tSphere vs AABB\n\tAABB vs AABB\n\tPoint vs Sphere\n\tPoint vs AABB\n\tPoint vs Triangle (not visualized)\n\tPoint vs Plane (not visualized)\n\tRay vs Plane (not visualized)\n\tRay vs AABB (not visualized)\n\tRay vs Sphere (not visualized)\n\tRay vs Triangle (not visualized)\n\tPlane vs AABB (not visualized)\n\tPlane vs Sphere (not visualized)");
        ImGui::Separator();
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Assignment 2: Bounding Volumes Representation and Bounding Volume Hierarchies"))
      {
        ImGui::Separator();
        ImGui::TextUnformatted("This assignment builds the bounding volume hierarchy framework for the CS350 class.");
        ImGui::TextUnformatted("The following Bounding Volumes are employed:\n\tAxis Aligned Bounding Box (AABB)\n\tJack Ritter's Bounding Sphere\n\tModified Thomas Larsson's Bounding Spheres using\n\t\tExtremal Points Optimal Sphere (EPOS) Algorithm\n\t\t\tEPOS-6\n\t\t\tEPOS-14\n\t\t\tEPOS-26\n\t\t\tEPOS-98\n\tKarl Pearson's Principal Component Analysis (PCA) Bounding Sphere\n\nThe following Bounding Volume Hierarchies have been implemented\n\tTop-Down AABB\n\tTop-Down Bounding Spheres (PCA)\n\tBottom-Up AABB\n\tBottom-Up Bounding Spheres (PCA)");
        ImGui::Separator();
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Assignment 3: Spatial Partitioning using Adaptive OctTrees and BSP Trees"))
      {
        ImGui::Separator();
        ImGui::TextUnformatted("This assignment performs spatial partitioning on a scene on a Triangle level.");
        ImGui::TextUnformatted("The following are implemented:\n\tTop-down creation of adaptive OctTree\n\tTop-Down creation of BSPTree\n\nTo visualize the splits, each partition will be drawn in a different wireframe color.\nPrecomputed OctTree and BSPTree meshes are available for quick comparisons.");
        ImGui::Separator();
        ImGui::TreePop();
      }
      ImGui::TreePop();
    }
    // *************************************************************************
    // ************************************************************ Buttons ****
    
    if (ImGui::Button("Start Assignment 1"))GSM.setNextGameState(GS::E_ASSIGNMENT_1);
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::Text("%s\n\nView Assignment Descriptions for more information.", "Geometry Toolbox");
      ImGui::EndTooltip();
    }
    ImGui::SameLine();
    if (ImGui::Button("Start Assignment 2"))GSM.setNextGameState(GS::E_ASSIGNMENT_2);
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::Text("%s\n\nView Assignment Descriptions for more information.", "Bounding Volumes Representation and Bounding Volume Hierarchies");
      ImGui::EndTooltip();
    }
    ImGui::SameLine();
    if (ImGui::Button("Start Assignment 3"))GSM.setNextGameState(GS::E_ASSIGNMENT_3);
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::Text("%s\n\nView Assignment Descriptions for more information.", "Spatial Partitioning using Adaptive OctTrees and BSP Trees\nThis scene may take awhile to start up");
      ImGui::EndTooltip();
    }
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    if (ImGui::Button("Quit Demo"))GSM.setNextGameState(GS::E_QUIT);

    // *************************************************************************
  }
  ImGui::End();//CS350 Demo Main Menu

#undef IMGUI_SAMELINE_TOOLTIP_HELPER
#undef IMGUI_SAMELINE_TOOLTIPV_HELPER
#undef IMGUI_COLOR_CHECKBOX_HELPER

  // ***************************************************************************
  // *************************************************************** Camera ****

  m_Cam.m_AspectRatio = static_cast<float>(GSM.getVKWin()->m_windowsWindow.getWidth()) / GSM.getVKWin()->m_windowsWindow.getHeight();
  m_Cam.updateMatrix();

  // ***************************************************************************
  // ******************************************************* Bunny rotation ****
  
  if (true == m_bExampleCheckbox)
  {
    static constexpr float dtmul{ 0.75f / MTU::Timer::clockFrequency };
    m_ObjTimer += dt * dtmul;
    if (m_ObjTimer > glm::two_pi<float>())m_ObjTimer -= glm::two_pi<float>();
  }

  // ***************************************************************************

}

void MTU::GS_Menu::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

  GSM.getVKWin()->createAndSetPipeline(m_BasicPipeline);

  {
    glm::mat3 bunnyW2M, bunnyM2W{ MTU::axisAngleRotation(m_Cam.s_Up, std::cosf(m_ObjTimer), &bunnyW2M)};
    glm::mat4 xform{ m_Cam.m_W2V * glm::mat4{ bunnyM2W } };
    glm::vec3 lightPos{ bunnyW2M * m_Cam.m_Pos };
    glm::vec3 lightCol
    {
      m_bExampleColorCheckboxR ? 1.0f : 0.0f,
      m_bExampleColorCheckboxG ? 1.0f : 0.0f,
      m_bExampleColorCheckboxB ? 1.0f : 0.0f
    };
    m_BasicPipeline.pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &lightPos);
    m_BasicPipeline.pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 16, sizeof(glm::vec3), &lightCol);
    m_BasicPipeline.pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
    m_QuestionMark.draw(FCB);
  }

}

void MTU::GS_Menu::Free()
{

}

MTU::GS_Menu::~GS_Menu()
{
  m_QuestionMark.destroyModel();
  GSM.getVKWin()->destroyPipelineInfo(m_BasicPipeline);
}

// *****************************************************************************
