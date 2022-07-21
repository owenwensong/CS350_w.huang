/*!*****************************************************************************
 * @file    GS_Assignment_3.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    14 JUL 2022
 * @brief   Gamestate for assignment 3
 * 
 *          For convenience, I have chosen the PCA sphere as the representative
 *          bounding sphere for assignment 3.
 * 
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_ASSIGNMENT_3_HEADER_GUARD
#define GAMESTATE_ASSIGNMENT_3_HEADER_GUARD

#include <GameStateManager/GameStateBase.h>

#include <variant>
#include <Assignment/Camera.h>
#include <Assignment/Geometry.h>
#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>

namespace A3H // Assignment 3 Helper namespace
{

// *****************************************************************************
// ************************************************** ENUMS FOR CONVENIENCE ****

  enum enumAss3Pipelines
  {
    E_PIPELINE_FIRST = 0,
    E_PIPELINE_WIREFRAME = E_PIPELINE_FIRST,
    E_PIPELINE_BASICLIGHT,

    E_NUM_PIPELINES
  };

  enum enumAss3DebugModels
  {
    E_DEBUGMODEL_FIRST = 0,
    E_DEBUGMODEL_SPHERE = E_DEBUGMODEL_FIRST,
    E_DEBUGMODEL_CUBE,
    E_DEBUGMODEL_POINT,

    E_NUM_DEBUGMODELS
  };

  enum enumAss3Models
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

// *****************************************************************************
// *********************************************** LITERALS FOR CONVENIENCE ****

  constexpr const char* namesAss3Models[E_NUM_MODELS]
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

// *****************************************************************************
// **************************** STRUCT FORWARD DECLARATIONS FOR CONVENIENCE ****

  struct Object;

// *****************************************************************************
// *********************************************** TYPEDEFS FOR CONVENIENCE ****

  using PLA = std::array<vulkanPipeline, E_NUM_PIPELINES>;// PipeLine Array
  using DMA = std::array<vulkanModel, E_NUM_DEBUGMODELS>; // Debug Model Array
  using  MA = std::array<vulkanModel, E_NUM_MODELS>;      // Model Array
  using  VV = std::vector<glm::vec3>;                     // Vertex Vector
  using  IV = std::vector<uint32_t>;                      // Index Vector
  using  FV = std::vector<VV>;                            // Face Vector
  using MVA = std::array<VV, E_NUM_MODELS>;               // Model Vertices Array
  using MFA = std::array<FV, E_NUM_MODELS>;               // Model Faces Array
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

    MTG::AABB   m_AABB;
    //MTG::Sphere m_BS_Pearson;

    enumAss3Models m_Model;

    void updateMatrices();
    void computeBoundingVolumes(MVA const& inModelVertexArray);

    using Proxy = size_t; // ID (offset of object into OV)

  };

  struct OctTreeNode
  {

    struct internalType
    {
      std::array<OctTreeNode*, 8> m_pChild
      {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
      };// make sure it initializes to nullptr fully all the time
    };

    struct leafType
    {
      FV pGeometry;  // Geometry as a vector of faces
    };

    glm::vec3 m_Center{  };   // center of this
    float     m_HalfWidth{  };// half extents of this node

    std::vector<Object::Proxy> m_Objects; // objects contained at this level

    std::variant<internalType, leafType> m_Data;// defaults to index 0, internal

    bool isLeaf() const noexcept { return 1 == m_Data.index(); }

  };

  struct BSPNode
  {

    struct internalType
    {
      MTG::Plane              m_SplitPlane{  };
      std::array<BSPNode*, 2> m_pChild{ nullptr, nullptr };
    };

    struct leafType
    {
      FV pGeometry;
    };

    std::variant<internalType, leafType> m_Data;

    bool isLeaf() const noexcept { return 1 == m_Data.index(); };

  };

// *****************************************************************************
// *****************************************************************************
// *********************************************** TYPEDEFS FOR CONVENIENCE ****



// *****************************************************************************
// ********************************************** FUNCTIONS FOR CONVENIENCE ****

  glm::mat4 getAABBMat(MTG::AABB const& inAABB) noexcept;
  glm::mat4 getBSMat(MTG::Sphere const& inBS) noexcept;
  glm::mat4 getOctTreeAABBMat(OctTreeNode const* inNode) noexcept;

// *****************************************************************************
}

namespace MTU
{
  // Quick and dirty GameState class for CS350 assignment 2
  class GS_Assignment_3 : public GameState
  {
  public:

    // Constructor will act as Load

    GS_Assignment_3(GameStateManager& rGSM);

    virtual void Init() override;
    virtual void Update(uint64_t dt) override;
    virtual void Draw() override;
    virtual void Free() override;

    // Destructor will act as Unload
    virtual ~GS_Assignment_3();

  private:

    // get the number of vertices of all objects in the scene
    size_t getNumSceneVertices() const noexcept;
    
    //size_t getModelsRAM() const noexcept;

    size_t getModelsVRAM() const noexcept;

    void CreateOctTree();
    void DestroyOctTree(bool keepModel = false);

    void CreateBSPTree();
    void DestroyBSPTree(bool keepModel = false);

  private:
    windowsInput& inputs;

    Camera    m_Cam;
    glm::vec3 m_LightColor;
    float     m_CamMoveSpeed;
    float     m_CamFastModifier;

    A3H::PLA m_Pipelines;   // rendering pipelines
    A3H::DMA m_DebugModels; // debug meshes
    A3H::MVA m_Vertices;    // model raw vertices
    A3H::MFA m_Faces;       // model raw faces
    A3H::MA  m_Models;      // assignment models
    A3H::OV  m_Objects;     // objects

    A3H::OctTreeNode* m_OctTree;  // OctTree
    A3H::BSPNode*     m_BSPTree;  // BSP

    vulkanModel m_OctTreeModel; // Model of OctTree
    A3H::IV     m_OctTreeObjectIndexCounts; // Object index for drawing differently

    vulkanModel m_BSPTreeModel;
    A3H::IV     m_BSPTreeObjectIndexCounts;

    int m_Octree_TriPerCell;  // termination criteria for the octree
    static constexpr int s_OctTreeMinTriPerCell{ 300 };
    static constexpr int s_OctTreeDefTriPerCell{ 2048 };
    static constexpr int s_OctTreeMaxTriPerCell{ 30000 };

    int m_BSPTree_TriPerPart;
    static constexpr int s_BSPTreeMinTriPerPart{ 300 };
    static constexpr int s_BSPTreeDefTriPerPart{ 2048 };
    static constexpr int s_BSPTreeMaxTriPerCell{ 30000 };

    bool m_bEditMode;
    bool m_bDrawObj;
    bool m_bDrawAABB;
    //bool m_bDrawBS_Pearson;

    bool m_bDrawOctTreeBounds;
    bool m_bDrawOctTreeTris;
    bool m_bKeepOctTreeModel;

    bool m_bDrawBSPTreeTris;
    bool m_bKeepBSPTreeModel;

  };
}

#endif//GAMESTATE_ASSIGNMENT_3_HEADER_GUARD
