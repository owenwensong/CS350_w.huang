/*!*****************************************************************************
 * @file    GS_Assignment_2.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    16 JUN 2022
 * @brief   Gamestate for assignment 2
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_ASSIGNMENT_2_HEADER_GUARD
#define GAMESTATE_ASSIGNMENT_2_HEADER_GUARD

#include <GameStateManager/GameStateBase.h>

#include <Assignment/Camera.h>
#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>

namespace A2H // Assignment 2 Helper namespace
{

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

// ************************************************ STRUCTS FOR CONVENIENCE ****



// *****************************************************************************
// *********************************************** TYPEDEFS FOR CONVENIENCE ****

  using PLA = std::array<vulkanPipeline, E_NUM_PIPELINES>;      // PipeLine Array
  using DMA = std::array<vulkanModel, E_NUM_DEBUGMODELS>;       // Debug Model Array
  using  MA = std::array<vulkanModel, E_NUM_MODELS>;            // Model Array
  using MVA = std::array<std::vector<glm::vec3>, E_NUM_MODELS>; // Model Vertices Array

// *****************************************************************************


}

namespace MTU
{
  // Quick and dirty GameState class for CS350 assignment 2
  class GS_Assignment_2 : public GameState
  {
  public:

    // Constructor will act as Load

    GS_Assignment_2(GameStateManager& rGSM);

    virtual void Init() override;
    virtual void Update(uint64_t dt) override;
    virtual void Draw() override;
    virtual void Free() override;

    // Destructor will act as Unload
    virtual ~GS_Assignment_2();

  private:



  private:
    windowsInput& inputs;

    Camera    m_Cam;
    glm::vec3 m_LightColor;
    float     m_CamMoveSpeed;
    float     m_CamFastModifier;

    A2H::PLA m_Pipelines;    // rendering pipelines
    A2H::DMA m_DebugModels;  // debug meshes
    A2H::MVA m_Vertices;     // model raw vertices
    A2H::MA  m_Models;       // assignment models
    
  };
}

#endif//GAMESTATE_ASSIGNMENT_2_HEADER_GUARD
