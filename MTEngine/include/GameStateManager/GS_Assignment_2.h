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
#include <Assignment/Geometry.h>
#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>

namespace A2H // Assignment 2 Helper namespace
{

// *****************************************************************************
// ************************************************** ENUMS FOR CONVENIENCE ****

  enum enumAss2Pipelines
  {
    E_PIPELINE_FIRST = 0,
    E_PIPELINE_WIREFRAME = E_PIPELINE_FIRST,
    E_PIPELINE_BASICLIGHT,

    E_NUM_PIPELINES
  };

  enum enumAss2DebugModels
  {
    E_DEBUGMODEL_FIRST = 0,
    E_DEBUGMODEL_SPHERE = E_DEBUGMODEL_FIRST,
    E_DEBUGMODEL_CUBE,
    E_DEBUGMODEL_POINT,

    E_NUM_DEBUGMODELS
  };

  enum enumAss2Models
  {
    E_MODEL_FIRST = 0,
    E_MODEL_4SPHERE = E_MODEL_FIRST,
    E_MODEL_BUNNY,
    E_MODEL_LUCY_PRINCETON,
    E_MODEL_STAR_DESTROYER,

    E_NUM_MODELS
  };

  enum enumAss2EPOS
  {
    E_EPOS_FIRST = 0,
    E_POS_6 = E_EPOS_FIRST,
    E_EPOS_14,
    E_EPOS_26,
    E_EPOS_98,

    E_EPOS_LAST = E_EPOS_98,
    E_NUM_EPOS
  };

// *****************************************************************************
// *********************************************** LITERALS FOR CONVENIENCE ****

  constexpr const char* namesAss2Models[E_NUM_MODELS]
  {
    "4Sphere",
    "Bunny",
    "Lucy Princeton",
    "Star Destroyer"
  };

  constexpr size_t eposNumVecs[E_NUM_EPOS]{ 3, 7, 13, 49 };

  constexpr const char* eposNames[E_NUM_EPOS]{ "EPOS-6", "EPOS-14", "EPOS-26", "EPOS-98"};

// Larsson Copy Paste Helper (to copy paste straight from the paper)
#define LCPH(a, b, c) glm::vec3{ a, b, c }
  constexpr glm::vec3 eposVecs[]// this size should align with eposNumVecs[E_EPOS_LAST]
  {
    LCPH(1, 0, 0), LCPH(0, 1, 0),  LCPH(0, 0, 1),
    LCPH(1, 1, 1), LCPH(1, 1, -1), LCPH(1, -1, 1), LCPH(1, -1, -1),
    LCPH(1, 1, 0), LCPH(1, -1, 0), LCPH(1, 0, 1),  LCPH(1, 0, -1), LCPH(0, 1, 1),  LCPH(0, 1, -1),
    LCPH(0, 1, 2), LCPH(0, 2, 1),  LCPH(1, 0, 2),  LCPH(2, 0, 1),  LCPH(1, 2, 0),  LCPH(2, 1, 0),   LCPH(0, 1, -2), LCPH(0, 2, -1), LCPH(1, 0, -2),  LCPH(2, 0, -1), LCPH(1, -2, 0), LCPH(2, -1, 0),
    LCPH(1, 1, 2), LCPH(2, 1, 1),  LCPH(1, 2, 1),  LCPH(1, -1, 2), LCPH(1, 1, -2), LCPH(1, -1, -2), LCPH(2, -1, 1), LCPH(2, 1, -1), LCPH(2, -1, -1), LCPH(1, -2, 1), LCPH(1, 2, -1), LCPH(1, -2, -1),
    LCPH(2, 2, 1), LCPH(1, 2, 2),  LCPH(2, 1, 2),  LCPH(2, -2, 1), LCPH(2, 2, -1), LCPH(2, -2, -1), LCPH(1, -2, 2), LCPH(1, 2, -2), LCPH(1, -2, -2), LCPH(2, -1, 2), LCPH(2, 1, -2), LCPH(2, -1, -2)
  };
#undef LCPH

// *****************************************************************************
// **************************** STRUCT FORWARD DECLARATIONS FOR CONVENIENCE ****

  struct Object;

// *****************************************************************************
// *********************************************** TYPEDEFS FOR CONVENIENCE ****

  using PLA = std::array<vulkanPipeline, E_NUM_PIPELINES>;// PipeLine Array
  using DMA = std::array<vulkanModel, E_NUM_DEBUGMODELS>; // Debug Model Array
  using  MA = std::array<vulkanModel, E_NUM_MODELS>;      // Model Array
  using  VV = std::vector<glm::vec3>;                     // Vertex vector
  using MVA = std::array<VV, E_NUM_MODELS>;               // Model Vertices Array
  using  OV = std::vector<Object>;                        // Object vector

// *****************************************************************************
// ************************************************ STRUCTS FOR CONVENIENCE ****

  struct Object
  {
    glm::vec3 m_Pos;
    glm::vec3 m_Rot;  // stored Pitch, Yaw, Roll, calculated Roll->Yaw->Pitch
    glm::vec3 m_Scale;

    glm::mat4 m_M2W;
    glm::mat4 m_W2M;

    MTG::AABB m_AABB;
    MTG::Sphere m_BS_Ritter;
    std::array<MTG::Sphere, E_NUM_EPOS> m_BS_Larsson;

    enumAss2Models m_Model;
    int m_EposK;

    void updateMatrices();
    void computeBoundingVolumes(MVA const& inModelVertexArray);

  };

// *****************************************************************************
// ********************************************** FUNCTIONS FOR CONVENIENCE ****

  glm::mat4 getAABBMat(MTG::AABB const& inAABB) noexcept;
  glm::mat4 getBSMat(MTG::Sphere const& inBS) noexcept;

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
    A2H::OV  m_Objects;      // objects

    unsigned char m_EPOS;

    bool m_bDrawAABB;
    bool m_bDrawBS_Ritter;
    bool m_bDrawBS_Larsson;
    
  };
}

#endif//GAMESTATE_ASSIGNMENT_2_HEADER_GUARD
