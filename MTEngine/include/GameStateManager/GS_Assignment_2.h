/*!*****************************************************************************
 * @file    GS_Assignment_2.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    16 JUN 2022
 * @brief   Gamestate for assignment 2
 * 
 *          The bounding sphere types have been named after the creator.
 *          The Larsson's sphere algorithm has been modified however.
 * 
 *          BS_Ritter:  Jack Ritter [1990]
 *                      An Efficient Bounding Sphere
 * 
 *          BS_Larsson: Thomas Larsson [2008]
 *                      Fast and Tight fitting bounding spheres
 * 
 *          BS_Pearson: Karl Pearson [1901]
 *                      Principal Component Analysis
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
    E_MODEL_DRAGON,
    E_MODEL_ARMADILLO,
    E_MODEL_BLENDER_MONKEY,
    E_MODEL_DISPLAY_CASE,
    E_MODEL_QUESTION_MARK,
    E_MODEL_TRASH_BIN,

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
    "4Sphere [?]",
    "Bunny [Stanford]",
    "Lucy Princeton [Stanford]",
    "Star Destroyer [?]",
    "Dragon  [Stanford]",
    "Armadillo [Stanford]",
    "Monkey [Blender]",
    "Display Stand [Owen Huang]",
    "Question Mark [Owen Huang]",
    "Trash Bin [Owen Huang]"
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
  using LBS = std::array<MTG::Sphere, E_NUM_EPOS>;        // Larsson's Bounding Spheres

// *****************************************************************************
// ************************************************ STRUCTS FOR CONVENIENCE ****

  struct Object
  {
    glm::vec3 m_Pos;
    glm::vec3 m_Rot;  // stored Pitch, Yaw, Roll, calculated Roll->Yaw->Pitch
    glm::vec3 m_Scale;

    glm::mat4 m_M2W;
    glm::mat4 m_W2M;

    MTG::AABB   m_AABB;
    MTG::Sphere m_BS_Ritter;
    LBS         m_BS_Larsson;
    MTG::Sphere m_BS_Pearson;

    enumAss2Models m_Model;
    int m_EposK;

    void updateMatrices();
    void computeBoundingVolumes(MVA const& inModelVertexArray);

    using Proxy = size_t; // ID (offset of object into OV)

  };

  struct TreeNode
  {
    union nodeType
    {
      struct asLeaf
      {
        Object::Proxy* m_pProxies;// pointer to an Object::Proxy vector element
        size_t         m_Size;    // number of elements being pointed to
      } asLeaf;
      struct asInternal
      {
        TreeNode* m_LChild;
        TreeNode* m_RChild;
      } asInternal;
    } m_Union;

    union bvType
    {
      MTG::AABB   asAABB;
      MTG::Sphere asSphere;
    } m_BV;

    bool m_bIsLeaf;
  };

// *****************************************************************************
// *****************************************************************************
// *********************************************** TYPEDEFS FOR CONVENIENCE ****

  using OPV = std::vector<Object::Proxy>;        // Object Proxy Vector

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

    // get the number of vertices of all objects in the scene
    size_t getNumSceneVertices() const noexcept;

    size_t getObjectsRam() const noexcept;
    
    size_t getModelsRAM() const noexcept;

    size_t getModelsVRAM() const noexcept;

    void computeBVHs();

    void destroyBVHs();

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
    A2H::OPV m_ObjectProxies;// object proxies for BVH

    A2H::TreeNode* m_pBVH_AABB;

    unsigned char m_EPOS;

    bool m_bDrawAABB;
    bool m_bDrawBS_Ritter;
    bool m_bDrawBS_Larsson;
    bool m_bDrawBS_Pearson;

    bool m_bDrawBVH_AABB;
    
  };
}

#endif//GAMESTATE_ASSIGNMENT_2_HEADER_GUARD
