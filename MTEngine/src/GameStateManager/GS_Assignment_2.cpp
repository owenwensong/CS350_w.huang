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
#include <algorithm>
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
    A2H_LOAD_MODEL_HELPER(E_DEBUGMODEL_SPHERE, "DebugMeshes/normalUVSphere");
    A2H_LOAD_MODEL_HELPER(E_DEBUGMODEL_CUBE, "DebugMeshes/normalCube");
    A2H_LOAD_MODEL_HELPER(E_DEBUGMODEL_POINT, "DebugMeshes/normalPoint");
#undef A2H_LOAD_MODEL_HELPER
    return retval;
  }

  static bool loadModels(MVA& ModelVerticesArray, MA& ModelArray)
  {
    bool retval{ true };
#define A2H_LOAD_MODEL_HELPER(enumA, strB) if (false == ModelArray[enumA].load3DNmlModel("../Assets/Meshes/" strB, ModelVerticesArray[enumA], true)) { printWarning("Failed to load model: " strB); retval = false; }
    A2H_LOAD_MODEL_HELPER(E_MODEL_4SPHERE, "4Sphere.obj");
    A2H_LOAD_MODEL_HELPER(E_MODEL_BUNNY, "bunny.obj");
    A2H_LOAD_MODEL_HELPER(E_MODEL_LUCY_PRINCETON, "lucy_princeton.obj");
    A2H_LOAD_MODEL_HELPER(E_MODEL_STAR_DESTROYER, "starwars1.obj");
    A2H_LOAD_MODEL_HELPER(E_MODEL_DRAGON, "dragon_vrip_res2.ply");
    A2H_LOAD_MODEL_HELPER(E_MODEL_ARMADILLO, "Armadillo.ply");
    A2H_LOAD_MODEL_HELPER(E_MODEL_BLENDER_MONKEY, "BlenderMonkey.obj");
    A2H_LOAD_MODEL_HELPER(E_MODEL_DISPLAY_CASE, "DisplayStand.obj");
    A2H_LOAD_MODEL_HELPER(E_MODEL_QUESTION_MARK, "QuestionMark.obj");
    A2H_LOAD_MODEL_HELPER(E_MODEL_TRASH_BIN, "Trashbin.obj");
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
  // ***************************************************** BOUNDING VOLUMES ****

  LBS createLarssonSpheres(std::vector<glm::vec3> const& inPoints, size_t eposK)
  {
    eposK = std::min(eposK, inPoints.size());
    LBS retval;

    std::mt19937 randomEngine{ std::random_device{}() };
    size_t r{ std::uniform_int_distribution<size_t>{ 0, static_cast<size_t>(inPoints.size() - eposK) }(randomEngine) };
    size_t rt{ r + eposK };
    // r to rt find support points, 0 to r and rt to end expand circle

    std::array<MTG::AABB, eposNumVecs[E_EPOS_LAST]> checkedExtents;
    {
      std::array<glm::vec2, eposNumVecs[E_EPOS_LAST]> projExtents;
      std::fill(projExtents.begin(), projExtents.end(), glm::vec2{ std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest() });
      for (size_t ri{ r }; ri < rt; ++ri)// finding support points off k random verts
      {
        glm::vec3 const& x{ inPoints[ri] };
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

    // initial setup, reuse previous EPOS calculations
    {
      MTG::AABB const* pLargest{ checkedExtents.data() };
      float largestLen{ sqrDist(pLargest->m_Min, pLargest->m_Max) };
      MTG::AABB const* pBegin{ checkedExtents.data() + 1 };


      for (size_t i{ 0 }; i < E_NUM_EPOS; ++i)
      {
        MTG::AABB const* pEnd{ checkedExtents.data() + eposNumVecs[i] };
        retval[i] = larsonFirstPass(pBegin, pEnd, pLargest, largestLen);
        pBegin = pEnd;
      }
    }

    for (size_t ri{ 0 }; ri < rt; ++ri)
    {
      glm::vec3 const& x{ inPoints[ri] };
      for (auto& BS : retval)
      {
        if (sqrDist(x, BS.m_Center) > BS.m_Radius)
        {
          BS = createNewSquaredSphere(getOppositePoint(BS, x), x);
        }
      }
    }
    for (size_t ri{ rt }, re{ inPoints.size() }; ri < re; ++ri)
    {
      glm::vec3 const& x{ inPoints[ri] };
      for (auto& BS : retval)
      {
        if (sqrDist(x, BS.m_Center) > BS.m_Radius)
        {
          BS = createNewSquaredSphere(getOppositePoint(BS, x), x);
        }
      }
    }

    for (auto& BS : retval)BS.m_Radius = std::sqrtf(BS.m_Radius);

    return retval;
  }

  // ***************************************************************************
  // ****************************************** BOUNDING VOLUME HIERARCHIES ****
  
  MTG::AABB computeSetAABB(OV const& objVec, Object::Proxy* pObjects, size_t numObjects)
  {
    MTG::AABB retval{ glm::vec3{ std::numeric_limits<float>::max() }, glm::vec3{ std::numeric_limits<float>::lowest() } };
    for (size_t i{ 0 }; i < numObjects; ++i)
    { // assume all AABB are computed correctly
      MTG::AABB const& iAABB{ objVec[pObjects[i]].m_AABB };
#define LAZYHELPER(a, b) if (iAABB. a b retval. a)retval. a = iAABB. a;
      LAZYHELPER(m_Min.x, < );
      LAZYHELPER(m_Min.y, < );
      LAZYHELPER(m_Min.z, < );
      LAZYHELPER(m_Max.x, > );
      LAZYHELPER(m_Max.y, > );
      LAZYHELPER(m_Max.z, > );
#undef LAZYHELPER
    }
    return retval;
  }

  void ComputeNodeAABB(TreeNode* pNode, OV const& objVec, Object::Proxy* pObjects, size_t numObjects)
  {
    if (0 != numObjects)pNode->m_BV.asAABB = computeSetAABB(objVec, pObjects, numObjects);
  }

  template <size_t maxDepth>
  bool floorFnTopDown(size_t numObjects, size_t currDepth)
  {
    if constexpr (0 == maxDepth)return 1 == numObjects;// unbound
    if constexpr (0 != maxDepth)return 1 == numObjects || maxDepth < currDepth;
  }

  template <typename T>
  T looseMedians(std::vector<T>& inVec)// will partially sort the vec, lazy 100
  {
    auto nth{ inVec.begin() + inVec.size() / 2 };
    std::nth_element(inVec.begin(), nth, inVec.begin() + inVec.size());
    return *nth;
  }

  int getAxisLargestSpreadFromAABB(MTG::AABB const& inAABB)
  {
    glm::vec3 diff{ inAABB.m_Max - inAABB.m_Min };
    return (diff.x > diff.y) ? (diff.z > diff.x ? 2 : 0) : (diff.z > diff.y ? 2 : 1);
  }

  //glm::vec3 getMedianCardinalsFromAABB(OV const& objVec, Object::Proxy* pObjects, size_t numObjects, int inAxis)
  //{
  //  assert(inAxis >= 0 && inAxis < 3);

  //  std::vector<float> floatVec;
  //  floatVec.reserve(2 * numObjects);// twice for AABB min max
  //  for (size_t i{ 0 }; i < numObjects; ++i)
  //  {
  //    MTG::AABB const& iAABB{ objVec[pObjects[i]].m_AABB };
  //    floatVec.emplace_back(iAABB.m_Min.x);
  //    floatVec.emplace_back(iAABB.m_Max.x);
  //  }
  //  glm::vec3 retval{ 0.0f, 0.0f, 0.0f };
  //  retval[inAxis] = looseMedians(floatVec);
  //  return retval;
  //}

  size_t topDownSplitPlanePartitionAABB(TreeNode* pNode, OV const& objVec, Object::Proxy* pObjects, size_t numObjects)
  {
    if (2 == numObjects)return 1;// early skip since only 2

    // Step 1: Calculate the axis to split the volumes
    int axisLargestSpread{ getAxisLargestSpreadFromAABB(pNode->m_BV.asAABB) };

    // Step 2: Sort the volumes based on the axis to split (I sort by AABB min here)
    std::sort
    (
      pObjects,
      pObjects + numObjects,
      [&](Object::Proxy inA, Object::Proxy inB)
      {
        return objVec[inA].m_AABB.m_Min[axisLargestSpread] < objVec[inB].m_AABB.m_Min[axisLargestSpread];
      }
    );

    // Step 3: Divide the space into two subsets
    std::vector<float> heuristicCosts;
    heuristicCosts.reserve(numObjects - 1);

    //float reciprocalParentVo{ pNode->m_BV.asAABB.getVolume() };
    //if (reciprocalParentVo)reciprocalParentVo = 1.0f / reciprocalParentVo;
    float reciprocalParentSA{ pNode->m_BV.asAABB.getSurfaceArea() };
    if (reciprocalParentSA)reciprocalParentSA = 1.0f / reciprocalParentSA;

    for (size_t i{ 1 }, t{ numObjects }; i < t; ++i)
    {
      MTG::AABB LSet{ computeSetAABB(objVec, pObjects, i) };
      MTG::AABB RSet{ computeSetAABB(objVec, pObjects + i, numObjects - i) };

      // leaving them separate for now in case I want to reference/change

      // Volume Heuristic (affects it only a bit but for the better)
      float VOCost{ 1.0f - LSet.getOverlapPercent(RSet) };

      // Surface Area Heuristic
      float SACost{ reciprocalParentSA * (i * LSet.getSurfaceArea() + (numObjects - i) * RSet.getSurfaceArea()) };

      heuristicCosts.emplace_back(VOCost + SACost);
    }

    size_t retval{ 0 };
    for (size_t i{ 1 }, t{ heuristicCosts.size() }; i < t; ++i)
    {
      if (heuristicCosts[i] < heuristicCosts[retval])retval = i;
    }

    return retval + 1;// +1 because the heuristic split starts at 1
  }

  template <typename computeBVFnType, typename floorFnType, typename partitionFnType>
  TreeNode* constructTopDown(OV const& objVec, Object::Proxy* pObjects, size_t numObjects, computeBVFnType computeBVFn, floorFnType floorFn, partitionFnType partitionFn, size_t currDepth = 0)
  {
    TreeNode* pNode{ new TreeNode };

    computeBVFn(pNode, objVec, pObjects, numObjects);
    pNode->m_bIsLeaf = floorFn(numObjects, currDepth);

    if (pNode->m_bIsLeaf)
    {
      pNode->m_Union.asLeaf.m_pProxies = pObjects;
      pNode->m_Union.asLeaf.m_Size = numObjects;
    }
    else
    {
      size_t k{ partitionFn(pNode, objVec, pObjects, numObjects) };
      pNode->m_Union.asInternal.m_LChild = constructTopDown(objVec, pObjects, k, computeBVFn, floorFn, partitionFn, currDepth + 1);
      pNode->m_Union.asInternal.m_RChild = constructTopDown(objVec, pObjects + k, numObjects - k, computeBVFn, floorFn, partitionFn, currDepth + 1);
    }

    return pNode;
  }

  void destroyTree(TreeNode* pTree)
  {
    if (nullptr == pTree)return;
    if (false == pTree->m_bIsLeaf)
    {
      destroyTree(pTree->m_Union.asInternal.m_LChild);
      destroyTree(pTree->m_Union.asInternal.m_RChild);
    }
    delete pTree;
  }

  template <typename funcType>
  void depthFirst(TreeNode* pTree, funcType pFn, size_t currDepth = 0)
  {
    if (nullptr == pTree)return;

    pFn(pTree, currDepth);

    if (pTree->m_bIsLeaf)return;
    depthFirst(pTree->m_Union.asInternal.m_LChild, pFn, currDepth + 1);
    depthFirst(pTree->m_Union.asInternal.m_RChild, pFn, currDepth + 1);
  }
  
  // ***************************************************************************

  static const glm::vec3 AABB_WireColor{ 0.0625f, 1.0f, 0.0625f };
  static const glm::vec3 BS_Ritter_WireColor{ 0.0625f, 0.0625f, 1.0f };
  static const glm::vec3 BS_Larsson_WireColor{ 1.0f, 1.0f, 0.0625f };
  static const glm::vec3 BS_Pearson_WireColor{ 1.0f, 0.0625f, 0.0625f };
  static const std::array<glm::vec3, 8> BVH_WireColors  // 7 + 1 (any farther levels just use the +1)
  {
    glm::vec3{ 0.0f, 0.75f, 0.5f },
    glm::vec3{ 1.0f, 0.25f, 0.25f },
    glm::vec3{ 1.0f, 0.75f, 0.125f },
    glm::vec3{ 0.75f, 0.75f, 0.0f },
    glm::vec3{ 0.5f, 0.75f, 0.25f },
    glm::vec3{ 0.75f, 0.25f, 0.75f },
    glm::vec3{ 0.25f, 0.25f, 0.75f },
    glm::vec3{ 0.5f, 0.75f, 1.0f }  // This will repeat for a lot of levels
  };

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
  m_pBVH_AABB{ nullptr },
  m_EPOS{ A2H::E_EPOS_LAST },
  m_bDrawAABB{ false },
  m_bDrawBS_Ritter{ false },
  m_bDrawBS_Larsson{ false },
  m_bDrawBS_Pearson{ false },
  m_bDrawBVH_AABB{ false }
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
  m_Cam.m_Pos = glm::vec3{ 0.0f, 1.0f, 3.5f };
  m_Cam.m_Rot = glm::vec2{ glm::half_pi<float>(), -0.25f };
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
  m_Objects.reserve(256); // reserve enough memory for anyone to play around with

  for (int i{ 0 }; i < 3; ++i)
  {
    float zPos{ i * -2.0f };
    // Left side display stands
    {
      A2H::Object& obj{ m_Objects.emplace_back() };
      obj.m_Pos = glm::vec3{ -1.5f, -1.0f, zPos };
      obj.m_Rot = glm::vec3{ 0.0f, glm::half_pi<float>() , 0.0f };
      obj.m_Scale = glm::vec3{ 3.0f, 3.0f, 3.0f };
      obj.m_Model = A2H::E_MODEL_DISPLAY_CASE;
    }
    // Right side display stands
    {
      A2H::Object& obj{ m_Objects.emplace_back() };
      obj.m_Pos = glm::vec3{ 1.5f, -1.0f, zPos };
      obj.m_Rot = glm::vec3{ 0.0f, -glm::half_pi<float>() , 0.0f };
      obj.m_Scale = glm::vec3{ 3.0f, 3.0f, 3.0f };
      obj.m_Model = A2H::E_MODEL_DISPLAY_CASE;
    }
  }

  // adjust R3 display stand to be tipped over
  {
    A2H::Object& obj{ m_Objects.back() };
    obj.m_Pos = glm::vec3{ 3.0f, -2.25f, -3.00f };
    obj.m_Rot = glm::vec3{ 0.5f, 0.0f , -glm::half_pi<float>() };
  }

  // L1 Armadillo
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -1.5f, 0.25f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, -glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A2H::E_MODEL_ARMADILLO;
  }

  // R1 Bunny
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 1.5f, 0.25f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, -glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A2H::E_MODEL_BUNNY;
  }

  // L2 4Sphere
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -1.5f, 0.25f, -2.0f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A2H::E_MODEL_4SPHERE;
  }

  // R2 Star Destroyer
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 1.475f, 0.125f, -2.0f };
    obj.m_Rot = glm::vec3{ 0.325f, -glm::quarter_pi<float>(), 0.0f};
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A2H::E_MODEL_STAR_DESTROYER;
  }

  // L3 Blender Monkey
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -1.5f, 0.25f, -4.0f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A2H::E_MODEL_BLENDER_MONKEY;
  }

  // R3 Tipped over Question Mark
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 4.0f, -2.2625f, -2.375f };
    obj.m_Rot = glm::vec3{ 0.0f, -0.55f, -0.4f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A2H::E_MODEL_QUESTION_MARK;
  }

  // C4 Lucy Princeton
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 0.0f, 0.0f, -6.0f };
    obj.m_Rot = glm::vec3{ 0.0f, 0.0f , 0.0f };
    obj.m_Scale = glm::vec3{ 5.0f, 5.0f, 5.0f };
    obj.m_Model = A2H::E_MODEL_LUCY_PRINCETON;
  }

  // L4 Dragon (not on a stand to be respectful)
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -6.375f, -0.5f, -4.125f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::quarter_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 5.0f, 5.0f, 5.0f };
    obj.m_Model = A2H::E_MODEL_DRAGON;
  }

  // Misc Trash bin
  {
    A2H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 2.375f, -2.0f, 0.125f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::pi<float>(), 0.0f};
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A2H::E_MODEL_TRASH_BIN;
  }

  for (auto& x : m_Objects)
  {
    x.m_EposK = static_cast<int>(m_Vertices[x.m_Model].size()); // full range by default
    x.updateMatrices();                   // first matrix update
    x.computeBoundingVolumes(m_Vertices); // first bounding volume calc
  }

  computeBVHs();

  // ***************************************************************************
  // *************************************************** SET BOOLS AND MORE ****

  m_EPOS = A2H::E_EPOS_LAST;

  m_bDrawAABB = false;
  m_bDrawBS_Ritter = false;
  m_bDrawBS_Larsson = false;
  m_bDrawBS_Pearson = false;

  m_bDrawBVH_AABB = false;

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
    ImGui::TextUnformatted("Hover tooltips:");
    IMGUI_SAMELINE_TOOLTIPV_HELPER("These (?) tooltips contain more information to use the program as intended\n\nExtra info:\n\nVertices: %d (total number of vertices in the scene)\nObjects RAM: %d B (Memory used to store per object pos, rot, scale, matrices, bounding volumes, etc)\nModels RAM:  %d B (Memory used to store positions to calculate BVs)\nModels VRAM: %d B (GPU Memory used to store model data)", static_cast<int>(getNumSceneVertices()), static_cast<int>(getObjectsRam()), static_cast<int>(getModelsRAM()), static_cast<int>(getModelsVRAM()));
    ImGui::TextUnformatted("Window controls");
    IMGUI_SAMELINE_TOOLTIPV_HELPER("F11: Fullscreen\n\n%s", "F1: Go to Assignment 1 state\nF2: Restart Assignment 2 state");
    ImGui::TextUnformatted("Camera controls");
    IMGUI_SAMELINE_TOOLTIP_HELPER("Right Mouse Button (hold): look around\nW: Move Forward\nA: Move Left\nS: Move Back\nD: Move Right\nSPACE: Move Upwards\nCONTROL: Move Downwards\nSHIFT (hold): Use speed multiplier");

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

    ImGui::Separator();

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

    if (ImGui::Button("recompute BVs and BVHs"))
    {
      for (auto& x : m_Objects)
      {
        //x.updateMatrices(); // updates on change, can't click and change in this case
        x.computeBoundingVolumes(m_Vertices); // compute all bounding volumes
      }
      computeBVHs();
    }
    IMGUI_SAMELINE_TOOLTIP_HELPER("Recomputes the Bounding Volumes and Bounding Volume Heirarchies.\nPlease recompute after making changes to the scene.\nBVs and BVHs only update upon user request.\nInstead of choosing K fixed vertices, I use K random vertices,\nso recomputation of Larsson's sphere with the same K value may vary");

    // AABB draw checkbox
    IMGUI_COLOR_CHECKBOX_HELPER("draw AABB", m_bDrawAABB, A2H::AABB_WireColor);
    IMGUI_COLOR_CHECKBOX_HELPER("draw BS Ritter", m_bDrawBS_Ritter, A2H::BS_Ritter_WireColor);
    IMGUI_COLOR_CHECKBOX_HELPER("draw BS Larsson", m_bDrawBS_Larsson, A2H::BS_Larsson_WireColor);
    IMGUI_SAMELINE_TOOLTIP_HELPER("Use the selector to choose which EPOS level to use,\nadjust EPOS K on an a per object level\nEPOS K will choose a random set of contiguous vertices.\nFor consistency, the default value is max so no random values used.");
    IMGUI_COLOR_CHECKBOX_HELPER("draw BS Pearson (PCA)", m_bDrawBS_Pearson, A2H::BS_Pearson_WireColor);

    // BVH draw checkboxes
    ImGui::Checkbox("draw AABB BVHs", &m_bDrawBVH_AABB);

    // SCENE OBJECTS EDITOR (Add only, lazy to support remove)
    ImGui::Separator();
    ImGui::Text("Scene editor [%d objects]", static_cast<int>(m_Objects.size()));
    IMGUI_SAMELINE_TOOLTIP_HELPER("You can edit the scene here.\nEdits are on a per object level.\nPlease recompute BV and BVH after making changes.");
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
          if (ImGui::DragFloat3("Rot", &pObject->m_Rot.x, 0.0125f, -glm::two_pi<float>(), glm::two_pi<float>()))
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
          if (int v_max{ static_cast<int>(m_Vertices[pObject->m_Model].size()) }; ImGui::DragInt("EPOS K", &pObject->m_EposK, 1.0f, 1, v_max))
          {
            pObject->m_EposK = glm::clamp(pObject->m_EposK, 1, v_max);// hard clamp
          }

          ImGui::TreePop();// testy node pop
        }

        if (shouldUpdateTransform)pObject->updateMatrices();

      }

    }
    ImGui::EndChild();// Objects child

    {
      bool shouldAddObject{ ImGui::Button("Add object") || inputs.isPressed(VK_SHIFT) ? inputs.isPressed(VK_E) : inputs.isTriggered(VK_E) };
      IMGUI_SAMELINE_TOOLTIP_HELPER("HOTKEY E (hold shift to spam add)\nAdds an object directly in front of the camera.\nDo note that Bounding Volumes and Bounding Volume Hierarchies only update on user request.\nAs such, make sure to recompute both after adding an object.");
      if (shouldAddObject)
      {
        std::mt19937 randomEngine{ std::random_device{}() };
        std::uniform_real_distribution<float> distribution{ -glm::pi<float>(), glm::pi<float>() };

        A2H::Object& obj{ m_Objects.emplace_back() };
        obj.m_Pos = m_Cam.m_Pos + m_Cam.m_Fwd;
        obj.m_Rot = glm::vec3{ distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
        obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
        obj.m_Model = A2H::E_MODEL_QUESTION_MARK;
        obj.updateMatrices();
        obj.m_EposK = static_cast<int>(m_Vertices[obj.m_Model].size());
      }
    }
    
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

      m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);

      m_DebugModels[A2H::E_DEBUGMODEL_SPHERE].draw(FCB);
    }
  }

  if (true == m_bDrawBS_Pearson)
  {
    m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A2H::BS_Pearson_WireColor);
    for (auto& x : m_Objects)
    {
      glm::mat4 xform{ m_Cam.m_W2V * A2H::getBSMat(x.m_BS_Pearson) };

      m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);

      m_DebugModels[A2H::E_DEBUGMODEL_SPHERE].draw(FCB);
    }
  }

  if (true == m_bDrawBVH_AABB)
  {
    A2H::depthFirst
    (
      m_pBVH_AABB,
      [&](A2H::TreeNode* pNode, size_t currDepth)// color based on BVH level
      {
        if (nullptr == pNode || pNode->m_bIsLeaf)return;

        glm::mat4 xform{ m_Cam.m_W2V * A2H::getAABBMat(pNode->m_BV.asAABB) };

        currDepth = std::min(currDepth, A2H::BVH_WireColors.size() - 1);
        m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A2H::BVH_WireColors[currDepth]);
        m_Pipelines[A2H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);

        m_DebugModels[A2H::E_DEBUGMODEL_CUBE].draw(FCB);
      }
    );

  }

}

void MTU::GS_Assignment_2::Free()
{
  GS_PRINT_FUNCSIG();

  destroyBVHs();
}

MTU::GS_Assignment_2::~GS_Assignment_2()
{
  GS_PRINT_FUNCSIG();

  for (auto& x : m_Pipelines)GSM.getVKWin()->destroyPipelineInfo(x);
  for (auto& x : m_DebugModels)x.destroyModel();
  for (auto& x : m_Models)x.destroyModel();
}

void MTU::GS_Assignment_2::computeBVHs()
{
  destroyBVHs();
  m_ObjectProxies.reserve(m_Objects.size());
  for (size_t i{ 0 }, t{ m_Objects.size() }; i < t; ++i)m_ObjectProxies.emplace_back(i);
  m_pBVH_AABB = A2H::constructTopDown(m_Objects, m_ObjectProxies.data(), m_ObjectProxies.size(), A2H::ComputeNodeAABB, A2H::floorFnTopDown<7>, A2H::topDownSplitPlanePartitionAABB);
}

void MTU::GS_Assignment_2::destroyBVHs()
{
  A2H::destroyTree(m_pBVH_AABB);
  m_pBVH_AABB = nullptr;
  m_ObjectProxies.clear();
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
  m_BS_Larsson = createLarssonSpheres(vertices, m_EposK);

  // Principal Component Analysis
  
  // TODO: calculate it, this is just to make sure it draws something
  m_BS_Pearson = m_BS_Larsson[E_EPOS_98];

}

size_t MTU::GS_Assignment_2::getNumSceneVertices() const noexcept
{
  size_t retval{ 0 };
  for (auto const& x : m_Objects)retval += m_Vertices[x.m_Model].size();
  return retval;
}

size_t MTU::GS_Assignment_2::getObjectsRam() const noexcept
{
  return m_Objects.capacity() * sizeof(m_Objects);
}

size_t MTU::GS_Assignment_2::getModelsRAM() const noexcept
{
  size_t retval{ 0 };
  for (auto const& x : m_Vertices)retval += x.size() * sizeof(decltype(m_Vertices)::value_type::value_type);
  return retval;
}

size_t MTU::GS_Assignment_2::getModelsVRAM() const noexcept
{
  size_t retval{ 0 };
  for (auto const& x : m_Models)
  {
    retval +=
    (
      static_cast<size_t>(x.m_Buffer_Vertex.m_Settings.m_Count) * static_cast<size_t>(x.m_Buffer_Vertex.m_Settings.m_ElemSize) + 
      static_cast<size_t>(x.m_Buffer_Index.m_Settings.m_Count) * static_cast<size_t>(x.m_Buffer_Index.m_Settings.m_ElemSize)
    );
  }
  return retval;
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
