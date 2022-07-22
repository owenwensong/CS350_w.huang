/*!*****************************************************************************
 * @file    GS_Assignment_3.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    14 JUL 2022
 * @brief   Gamestate for assignment 3
 * 
 * 
 *          TODO:
 *            Freeing the octtree
 *            Polygon splitting on the octtree
 *            model load from polygon
 *            faces unused in buildocttree
 * 
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <random>
#include <imgui.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <utility/ThreadTask.h>
#include <Assignment/Geometry.h>
#include <GameStateManager/GS_Assignment_3.h>
#include <glm/gtc/quaternion.hpp>

#include <fstream>

namespace A3H
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

  static bool loadModels(MVA& ModelVerticesArray, MFA& ModelFacesArray, MA& ModelArray)
  {
    bool retval{ true };
#define A2H_LOAD_MODEL_HELPER(enumA, strB) if (false == ModelArray[enumA].load3DNmlModelGetFaces("../Assets/Meshes/" strB, ModelVerticesArray[enumA], ModelFacesArray[enumA], true)) { printWarning("Failed to load model: " strB); retval = false; }
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
      pipelineSetup.m_PathShaderVert = "../Assets/Shaders/vert_Wireframe_Ass3.spv";
      pipelineSetup.m_PathShaderFrag = "../Assets/Shaders/frag_Wireframe_Ass3.spv";
      //pipelineSetup.m_UniformsVert
      //pipelineSetup.m_UniformsFrag = vulkanPipeline::createUniformInfo<glm::vec3>();
      pipelineSetup.m_PushConstantRangeVert = vulkanPipeline::createPushConstantInfo<glm::mat4>(VK_SHADER_STAGE_VERTEX_BIT);
      pipelineSetup.m_PushConstantRangeFrag = vulkanPipeline::createPushConstantInfo<glm::vec3>(VK_SHADER_STAGE_FRAGMENT_BIT);
      pipelineSetup.m_PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
      pipelineSetup.m_CullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;

      if (false == vkWin->createPipelineInfo(PipelineArray[A3H::E_PIPELINE_WIREFRAME], pipelineSetup))
      {
        retval = false;
      }
    }

    // SOLID COLOR, COLORED CAMERA LIGHT
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

      if (false == vkWin->createPipelineInfo(PipelineArray[A3H::E_PIPELINE_BASICLIGHT], pipelineSetup))
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

  // ***************************************************************************
  // ***************************************************** BOUNDING VOLUMES ****

  /// @brief  Get opposite point from a sphere and a point.
  ///         This function assumes the sphere has a squared radius.
  ///         This function also ignores the possibility of p1 being s0.m_Center
  ///         because it should only be called if dist is greater than radius.
  /// @param s0 sphere input with square radius to save some time
  /// @param p1 new extreme point of the sphere
  /// @return a point on s0's circumference in the direction of s0 center from p1
  //static glm::vec3 getOppositePoint(MTG::Sphere const& s0, glm::vec3 p1) noexcept
  //{
  //  p1 = s0.m_Center - p1;  // p1 is now the vector from the point to the center
  //  return s0.m_Center + std::sqrtf(s0.m_Radius / sqrLen(p1)) * p1; // own optimization, ask me for more info.
  //}

  /// @brief  create a new sphere with radius data member being squared as this 
  ///         is not the final sphere. It will be used for further computation.
  ///         The arguments are taken by copy to be used as center and radius. 
  /// @param inA one of 2 opposing sides of a sphere (the opposite of inB)
  /// @param inB one of 2 opposing sides of a sphere (the opposite of inA)
  /// @return new bounding sphere with a squared radius component
  //static MTG::Sphere createNewSquaredSphere(glm::vec3 inA, glm::vec3 inB) noexcept
  //{
  //  inA = 0.5f * (inA + inB); // inA now holds the center
  //  inB -= inA;               // inB now holds the radius vector
  //  return MTG::Sphere{ inA, sqrLen(inB) };
  //}

  // ***************************************************************************
  // *************************************************** SPLITTING POLYGONS ****

  void splitPoly(VV& inPoly, MTG::Plane const& splitPlane, VV& frontPoly, VV& backPoly)
  {
    inPoly.emplace_back(inPoly.front());
    for (auto j{ inPoly.begin() }, i{ j++ }, k{ inPoly.end() }; j < k; i = j++)
    {
      MTG::Ray edge{ *i, *j - *i };
      float tIntersect;
      switch (MTG::intersectionEdgePlaneSpecial(edge, splitPlane, tIntersect))
      {
      case MTG::EPSResult::E_COPLANAR:
      case MTG::EPSResult::E_INSIDE:
        backPoly.emplace_back(*j);
        break;
      case MTG::EPSResult::E_OUTSIDE:
        frontPoly.emplace_back(*j);
        break;
      case MTG::EPSResult::E_INSIDE_TO_OUTSIDE:
      {
        glm::vec3 planeIntersect{ edge.m_Point + tIntersect * edge.m_Direction };
        frontPoly.emplace_back(planeIntersect);
        backPoly.emplace_back(planeIntersect);
      }
        frontPoly.emplace_back(*j);
        break;
      case MTG::EPSResult::E_OUTSIDE_TO_INSIDE:
        backPoly.emplace_back(*j);// maintain winding
      {
        glm::vec3 planeIntersect{ edge.m_Point + tIntersect * edge.m_Direction };
        frontPoly.emplace_back(planeIntersect);
        backPoly.emplace_back(planeIntersect);
      }
        break;
      }
    }
    inPoly.pop_back();

    // help with some polygons being wonky
    if (frontPoly.size() < 3)
    {
      frontPoly.resize(0);
      backPoly = inPoly;
    }
    else if (backPoly.size() < 3)
    {
      backPoly.resize(0);
      frontPoly = inPoly;
    }
  }

  size_t getNumTriangles(FV const& inPolys)
  {
    size_t retval{ 0 };
    for (auto const& x : inPolys)
    {
      assert(x.size() >= 3);  // minimum polygon is a triangle
      retval += x.size() - 2; // assume convex polygon (it should be anyway)
    }
    return retval;
  }

  struct vec3Hasher
  {
    size_t operator() (glm::vec3 const& inA) const noexcept
    {
      std::hash<float> hasher{};
      return hasher(inA.x) ^ hasher(inA.y) ^ hasher(inA.z);
    }
  };

  void triangulatePolygons(FV const& inPolys, VV& outPositions, IV& outIndices)
  {
    if (0 == inPolys.size())return;
    // reserve space to reduce number of allocations
    {
      size_t vSpace{ 0 }, iSpace{ 0 };
      for (auto const& x : inPolys)
      {
        vSpace += x.size();           // number of vertices
        iSpace += 3 * (x.size() - 2); // number of triangles * 3
      }
      //outPositions.clear();
      //outIndices.clear();
      outPositions.reserve(outPositions.size() + vSpace);
      outIndices.reserve(outIndices.size() + iSpace);
      //std::cout << "vSpace: " << vSpace << std::endl;
      //std::cout << "iSpace: " << iSpace << std::endl;
    }

    // TODO FIX TRIANGULATION
    std::unordered_map<glm::vec3, uint32_t, vec3Hasher> uniqueVerts;

    uint32_t nextOffset{ static_cast<uint32_t>(outPositions.size()) };
    std::vector<uint32_t> polyIndices;
    for (auto const& x : inPolys)
    {
      polyIndices.clear();
      for (auto const& y : x)
      {
        if (auto found{ uniqueVerts.find(y) }; found == uniqueVerts.end())
        { // Is a new unique vertex
          polyIndices.emplace_back(nextOffset);
          uniqueVerts.emplace(y, nextOffset++);
          outPositions.emplace_back(y);
        }
        else
        {
          polyIndices.emplace_back(found->second);
        }
      }

      // Triangulation
      for (size_t i{ 0 }, t{ polyIndices.size() - 2 }; i < t; ++i)
      {
        outIndices.emplace_back(polyIndices[i]);
        outIndices.emplace_back(polyIndices[i + 1]);
        outIndices.emplace_back(polyIndices[i + 2]);
      }

    }

    //std::cout << "vFinal: " << outPositions.size() << std::endl;
    //std::cout << "iFinal: " << outIndices.size() << std::endl;
  }

  // ***************************************************************************
  // ************************************************************** OCTTREE ****

  bool isStraddling(OctTreeNode* pNode, Object const& inObj, int& outIndex)
  {
    outIndex = 0;
    glm::vec3 objExtent{ 0.5f * (inObj.m_AABB.m_Max - inObj.m_AABB.m_Min) };
    glm::vec3 objCenter{ inObj.m_AABB.m_Min + objExtent };
    for (glm::vec3::length_type i{ 0 }; i < 3; ++i)
    {
      float d{ objCenter[i] - pNode->m_Center[i] };
      if (std::fabsf(d) <= objExtent[i])return true;
      if (false == std::signbit(d))outIndex |= (1 << i);
    }
    return false;
  }

  OctTreeNode* createChildNode(OctTreeNode const* pParent, int index)
  {
    OctTreeNode* retval{ new OctTreeNode };
    retval->m_HalfWidth = 0.5f * pParent->m_HalfWidth;
    retval->m_Center.x = pParent->m_Center.x + (index & 0b001 ? retval->m_HalfWidth : -retval->m_HalfWidth);
    retval->m_Center.y = pParent->m_Center.y + (index & 0b010 ? retval->m_HalfWidth : -retval->m_HalfWidth);
    retval->m_Center.z = pParent->m_Center.z + (index & 0b100 ? retval->m_HalfWidth : -retval->m_HalfWidth);
    return retval;
  }

  // to be only called by buildOctTree, fragile design.
  void insertIntoOctTree(OctTreeNode* pNode, Object const& inObj, Object::Proxy inObjProxy)
  {
    int index;
    if (true == isStraddling(pNode, inObj, index))
    {
      pNode->m_Objects.emplace_back(inObjProxy);
      return;
    }

    OctTreeNode*& newNode{ std::get<0>(pNode->m_Data).m_pChild[index] };
    if (nullptr == newNode)newNode = createChildNode(pNode, index);
    insertIntoOctTree(newNode, inObj, inObjProxy);
  }

  void generateOctantFaces(FV& inPolys, std::array<FV, 8>& outFVs, glm::vec3 const& splitPoint)
  {
    // 00: 000 & 001 (back), 01: 010 & 011 (front)
    // 10: 100 & 101 (back), 11: 110 & 111 (front)
    std::array<FV, 4> yzSplit;
    {
      std::array<FV, 2> zSplit;// 0: 000 to 011 (back), 1: 100 to 111 (front)

      // populate front and back of zSplit temporarily
      {
        MTG::Plane zSplitPlane{ glm::vec3{ 0.0f, 0.0f, 1.0f }, splitPoint, 69 };
        for (auto& x : inPolys)
        {
          VV frontPoly, backPoly;
          splitPoly(x, zSplitPlane, frontPoly, backPoly);
          if (0 != frontPoly.size())zSplit[0].emplace_back(std::move(frontPoly));
          if (0 != backPoly.size())zSplit[1].emplace_back(std::move(backPoly));
        }
      }


      // populate front and back of yzSplit temporarily
      {
        MTG::Plane ySplitPlane{ glm::vec3{ 0.0f, 1.0f, 0.0f }, splitPoint, 69 };
        for (size_t i{ 0 }; i < 2; ++i)
        {
          size_t baseIndex{ i << 1 };
          for (auto& x : zSplit[i])
          {
            VV frontPoly, backPoly;
            splitPoly(x, ySplitPlane, frontPoly, backPoly);
            //if (0 != frontPoly.size())yzSplit[i ? 0b11 : 0b01].emplace_back(std::move(frontPoly));
            //if (0 != backPoly.size())yzSplit[i ? 0b10 : 0b00].emplace_back(std::move(backPoly));
            if (0 != frontPoly.size())yzSplit[baseIndex].emplace_back(std::move(frontPoly));
            if (0 != backPoly.size())yzSplit[baseIndex | 1].emplace_back(std::move(backPoly));
          }
        }
      }
    }

    // finally populate the octant faces
    MTG::Plane xSplitPlane{ glm::vec3{ 1.0f, 0.0f, 0.0f }, splitPoint, 69 };
    for (size_t i{ 0 }; i < 4; ++i)
    {
      size_t baseIndex{ i << 1 };
      for (auto& x : yzSplit[i])
      {
        VV frontPoly, backPoly;
        splitPoly(x, xSplitPlane, frontPoly, backPoly);
        if (0 != frontPoly.size())outFVs[baseIndex].emplace_back(std::move(frontPoly));
        if (0 != backPoly.size())outFVs[baseIndex | 1].emplace_back(std::move(backPoly));
      }
    }

  }

  void subDivideOctTreeGeometry(OctTreeNode* pNode, OV const& objects, MFA const& faces, int maxTriPerCell, FV&& inPolys)
  {
    // Add any children into the polygon list
    for (auto x : pNode->m_Objects)
    {
      size_t faceIndex{ static_cast<size_t>(objects[x].m_Model) };
      glm::mat3 linearxform{ objects[x].m_M2W };
      glm::vec3 translate{ objects[x].m_M2W[3] };
      inPolys.reserve(faces[faceIndex].size() + inPolys.size());
      std::transform
      (
        faces[faceIndex].begin(),
        faces[faceIndex].end(),
        std::back_inserter(inPolys),
        [&](VV const& inFace)
        {
          VV retval{ inFace };
          assert(retval.size() >= 3);
          for (glm::vec3& vertex : retval)
          {
            vertex = linearxform * vertex + translate;
          }
          return retval;
        }
      );
    }

    // Check condition (if it's not a leaf alr the children might have more geometry)
    {
      bool leafCheck{ std::all_of(std::get<0>(pNode->m_Data).m_pChild.begin(), std::get<0>(pNode->m_Data).m_pChild.end(), +[](OctTreeNode const* inPtr) { return nullptr == inPtr;  }) };
      if (leafCheck && getNumTriangles(inPolys) <= static_cast<size_t>(maxTriPerCell))
      {
        // node type converted to leaf and contains geometry
        pNode->m_Data = OctTreeNode::leafType{ std::move(inPolys) };

        return;
      }
    }

    // Divide
    std::array<FV, 8> tempFVs;
    generateOctantFaces(inPolys, tempFVs, pNode->m_Center);
    inPolys.resize(0);// inPolys is a subset of the union of all tempFVs
    for (int i{ 0 }; i < 8; ++i)
    {
      if (tempFVs[i].empty())
      {
        if (nullptr != std::get<0>(pNode->m_Data).m_pChild[i])
        {
          subDivideOctTreeGeometry(std::get<0>(pNode->m_Data).m_pChild[i], objects, faces, maxTriPerCell, {});
        }
      }
      else
      {
        OctTreeNode*& pChild{ std::get<0>(pNode->m_Data).m_pChild[i] };
        if (nullptr == pChild)pChild = createChildNode(pNode, i);
        subDivideOctTreeGeometry(pChild, objects, faces, maxTriPerCell, std::move(tempFVs[i]));
      }
    }

  }

  OctTreeNode* buildOctTree(OV const& objects, MFA const& faces, int maxTriPerCell)
  {
    if (0 == objects.size())return nullptr;

    OctTreeNode* retval{ new OctTreeNode };
    {
      // get AABB of all objects
      MTG::AABB extents{ glm::vec3{ std::numeric_limits<float>::max() }, glm::vec3{ std::numeric_limits<float>::lowest() } };
      for (auto const& x : objects)
      {
#define EXTENTSHELPER(memA, memB, cmpC) if (x.m_AABB.memA.memB cmpC extents.memA.memB)extents.memA.memB = x.m_AABB.memA.memB
        EXTENTSHELPER(m_Min, x, <);
        EXTENTSHELPER(m_Min, y, <);
        EXTENTSHELPER(m_Min, z, <);
        EXTENTSHELPER(m_Max, x, >);
        EXTENTSHELPER(m_Max, y, >);
        EXTENTSHELPER(m_Max, z, >);
#undef EXTENTSHELPER
      }

      // get top level octtree size
      glm::vec3 halfWidths{ 0.5f * (extents.m_Max - extents.m_Min) };
      retval->m_Center = extents.m_Min + halfWidths;
      retval->m_HalfWidth = std::max(std::max(halfWidths.x, halfWidths.y), halfWidths.z);
    }

    // insert objects into octree
    for (Object::Proxy i{ 0 }, t{ objects.size() }; i < t; ++i)
    {
      insertIntoOctTree(retval, objects[i], i);
    }

    // Break up geometry & load final meshes, expands octtree based on maxTriPerCell
    subDivideOctTreeGeometry(retval, objects, faces, maxTriPerCell, {});
    
    return retval;
  }

  void destroyOctTree(OctTreeNode* pNode)
  {
    if (nullptr == pNode)return;
    //if (true == pNode->isLeaf())
    //{
    //  pNode->m_Data.asLeaf.m_Mesh.destroyModel();
    //}
    //else for (OctTreeNode* pChild : pNode->m_Data.asInternal.m_pChild)
    //{
    //  destroyOctTree(pChild);
    //}
    if (false == pNode->isLeaf())
    {
      for (OctTreeNode* pChild : std::get<0>(pNode->m_Data).m_pChild)
      {
        destroyOctTree(pChild);
      }
    }
    delete pNode;
  }

  template <typename funcType>
  void depthFirst(OctTreeNode* pTree, funcType pFn, size_t currDepth = 0)
  {
    if (nullptr == pTree)return;

    pFn(pTree, currDepth);

    if (true == pTree->isLeaf())return;

    for (auto& x : std::get<0>(pTree->m_Data).m_pChild)
    {
      if (nullptr == x)continue;
      depthFirst(x, pFn, currDepth + 1);
    }
  }

  vulkanModel loadFromOctTree(OctTreeNode* pNode, std::vector<uint32_t>& outObjIndices)
  {
    if (nullptr == pNode)return vulkanModel{  };
    windowHandler* pWH{ windowHandler::getPInstance() };
    assert(pWH != nullptr);// debug only, flow should be pretty standard.

    vulkanModel retval;
    VV positions;
    IV indices;
    outObjIndices.clear();
    depthFirst
    (
      pNode,
      [&](OctTreeNode* pLambdaNode, size_t /*currDepth*/)
      {
        if (false == pLambdaNode->isLeaf())return;
        size_t before{ indices.size() };
        triangulatePolygons(std::get<1>(pLambdaNode->m_Data).pGeometry, positions, indices);
        if (size_t after{ indices.size() }; after > before)
        {
          outObjIndices.emplace_back(static_cast<uint32_t>(after - before));
        }
      }
    );

    retval.m_VertexCount = static_cast<uint32_t>(positions.size());
    retval.m_IndexCount = static_cast<uint32_t>(indices.size());
    retval.m_IndexType = VK_INDEX_TYPE_UINT32;

    // set up vertex buffer
    if (false == pWH->createBuffer(retval.m_Buffer_Vertex, vulkanBuffer::Setup{ vulkanBuffer::s_BufferUsage_Vertex, vulkanBuffer::s_MemPropFlag_Vertex, retval.m_VertexCount, sizeof(decltype(positions)::value_type) }))
    {
      printWarning("failed to create mesh vertex buffer");
      retval.destroyModel();
      return vulkanModel{ /* return default struct if fail */ };
    }

    // write to vertex buffer
    pWH->writeToBuffer(retval.m_Buffer_Vertex, { positions.data() }, { static_cast<VkDeviceSize>(positions.size() * sizeof(decltype(positions)::value_type)) });

    // set up index buffer
    if (false == pWH->createBuffer(retval.m_Buffer_Index, vulkanBuffer::Setup{ vulkanBuffer::s_BufferUsage_Index, vulkanBuffer::s_MemPropFlag_Index, retval.m_IndexCount, sizeof(decltype(indices)::value_type) }))
    {
      printWarning("failed to create mesh index buffer");
      retval.destroyModel();
      return vulkanModel{ /* return default struct if fail */ };
    }

    // write to index buffer
    pWH->writeToBuffer(retval.m_Buffer_Index, { indices.data() }, { static_cast<VkDeviceSize>(indices.size() * sizeof(decltype(indices)::value_type)) });

    return retval;
  }

  // ***************************************************************************
  // ****************************************************************** BSP ****

  void countPolygons(FV const& inPolys, MTG::Plane const& inPlane, size_t& outFront, size_t& outBack)
  {
    outFront = 0;
    outBack = 0;
    for (auto const& x : inPolys)
    {
      switch (MTG::ClassifyPolygonToPlane(inPlane, x.data(), x.size()))
      {
      case MTG::PolyPlaneResult::POLYGON_STRADDLING:
        ++outFront;
        ++outBack;
        break;
      default:
      case MTG::PolyPlaneResult::POLYGON_COPLANAR:
      case MTG::PolyPlaneResult::POLYGON_IN_FRONT:
        ++outFront;
        break;
      case MTG::PolyPlaneResult::POLYGON_IN_BACK:
        ++outBack;
        break;
      }
    }
  }

  bool chooseSplitPlaneAuto(MTG::Plane& outPlane, FV const& inPolys)
  {
    const size_t maxPolyDiff{ std::max<size_t>(inPolys.size() / 10, 1) };
    const size_t maxPolySide{ inPolys.size() + maxPolyDiff };
    for (auto const& x : inPolys)
    {
      MTG::Plane splitPlane{ glm::cross(x[1] - x[0], x[2] - x[0]), x[0]};
      if (isnan(splitPlane.m_Normal.x))continue;// only check one cus it's normalization that does it
      size_t frontPolyEstimate, backPolyEstimate;
      countPolygons(inPolys, splitPlane, frontPolyEstimate, backPolyEstimate);
      size_t polyDiff{ frontPolyEstimate > backPolyEstimate ? frontPolyEstimate - backPolyEstimate : backPolyEstimate - frontPolyEstimate };
      if (polyDiff <= maxPolyDiff)
      {
        if (frontPolyEstimate + backPolyEstimate >= maxPolySide)continue;
        outPlane = splitPlane;
        return true;
      }
    }
    return false;// failed to find split plane
  }

  void partitionBSP(BSPNode* pNode, FV&& inPolys, FV& frontPolys, FV& backPolys)
  {
    MTG::Plane const& splitPlane{ std::get<0>(pNode->m_Data).m_SplitPlane };
    for (auto& x : inPolys)
    {
      VV frontPoly, backPoly;
      splitPoly(x, splitPlane, frontPoly, backPoly);
      if (0 != frontPoly.size())frontPolys.emplace_back(std::move(frontPoly));
      if (0 != backPoly.size())backPolys.emplace_back(std::move(backPoly));
    }
  }

  template <typename ChoosePlaneFnType>
  void subDivideBSPGeometry(BSPNode* pNode, FV&& inPolys, size_t maxTriPerPart, ChoosePlaneFnType choosePlaneFn)
  {
    if (getNumTriangles(inPolys) <= maxTriPerPart || false == choosePlaneFn(std::get<0>(pNode->m_Data).m_SplitPlane, inPolys))
    {
      pNode->m_Data = BSPNode::leafType{ std::move(inPolys) };
      return;
    }

    std::array<FV, 2> splitGeometry;
    partitionBSP(pNode, std::move(inPolys), splitGeometry[0], splitGeometry[1]);
    for (size_t i{ 0 }; i < 2; ++i)
    {
      if (0 != splitGeometry[i].size())
      {
        std::get<0>(pNode->m_Data).m_pChild[i] = new BSPNode;
        subDivideBSPGeometry(std::get<0>(pNode->m_Data).m_pChild[i], std::move(splitGeometry[i]), maxTriPerPart, choosePlaneFn);
      }
    }

  }

  template <typename ChoosePlaneFnType>
  BSPNode* buildBSP(OV const& objects, MFA const& faces, int maxTriPerPart, ChoosePlaneFnType choosePlaneFn)
  {
    BSPNode* pRoot{ new BSPNode };

    FV allFaces;
    for (auto x : objects)
    {
      size_t faceIndex{ static_cast<size_t>(x.m_Model) };
      glm::mat3 linearxform{ x.m_M2W };
      glm::vec3 translate{ x.m_M2W[3] };
      allFaces.reserve(allFaces.size() + faces[faceIndex].size());
      std::transform
      (
        faces[faceIndex].begin(),
        faces[faceIndex].end(),
        std::back_inserter(allFaces),
        [&](VV const& inFace)
        {
          VV retval{ inFace };
          assert(retval.size() >= 3);
          for (glm::vec3& vertex : retval)
          {
            vertex = linearxform * vertex + translate;
          }
          return retval;
        }
      );
    }
    subDivideBSPGeometry(pRoot, std::move(allFaces), static_cast<size_t>(maxTriPerPart), choosePlaneFn);

    return pRoot;
  }

  void destroyBSPTree(BSPNode*pNode)
  {
    if (nullptr == pNode)return;
    if (false == pNode->isLeaf())
    {
      for (BSPNode* pChild : std::get<0>(pNode->m_Data).m_pChild)
      {
        destroyBSPTree(pChild);
      }
    }
    delete pNode;
  }

  template <typename funcType>
  void depthFirst(BSPNode* pTree, funcType pFn, size_t currDepth = 0)
  {
    if (nullptr == pTree)return;

    pFn(pTree, currDepth);

    if (true == pTree->isLeaf())return;

    for (auto& x : std::get<0>(pTree->m_Data).m_pChild)
    {
      if (nullptr == x)continue;
      depthFirst(x, pFn, currDepth + 1);
    }
  }

  vulkanModel loadFromBSPTree(BSPNode* pNode, std::vector<uint32_t>& outObjIndices)
  {
    if (nullptr == pNode)return vulkanModel{  };
    windowHandler* pWH{ windowHandler::getPInstance() };
    assert(pWH != nullptr);// debug only, flow should be pretty standard.

    vulkanModel retval;
    VV positions;
    IV indices;
    outObjIndices.clear();
    depthFirst
    (
      pNode,
      [&](BSPNode* pLambdaNode, size_t /*currDepth*/)
      {
        if (false == pLambdaNode->isLeaf())return;
        size_t before{ indices.size() };
        triangulatePolygons(std::get<1>(pLambdaNode->m_Data).pGeometry, positions, indices);
        if (size_t after{ indices.size() }; after > before)
        {
          outObjIndices.emplace_back(static_cast<uint32_t>(after - before));
        }
      }
    );

    retval.m_VertexCount = static_cast<uint32_t>(positions.size());
    retval.m_IndexCount = static_cast<uint32_t>(indices.size());
    retval.m_IndexType = VK_INDEX_TYPE_UINT32;

    // set up vertex buffer
    if (false == pWH->createBuffer(retval.m_Buffer_Vertex, vulkanBuffer::Setup{ vulkanBuffer::s_BufferUsage_Vertex, vulkanBuffer::s_MemPropFlag_Vertex, retval.m_VertexCount, sizeof(decltype(positions)::value_type) }))
    {
      printWarning("failed to create mesh vertex buffer");
      retval.destroyModel();
      return vulkanModel{ /* return default struct if fail */ };
    }

    // write to vertex buffer
    pWH->writeToBuffer(retval.m_Buffer_Vertex, { positions.data() }, { static_cast<VkDeviceSize>(positions.size() * sizeof(decltype(positions)::value_type)) });

    // set up index buffer
    if (false == pWH->createBuffer(retval.m_Buffer_Index, vulkanBuffer::Setup{ vulkanBuffer::s_BufferUsage_Index, vulkanBuffer::s_MemPropFlag_Index, retval.m_IndexCount, sizeof(decltype(indices)::value_type) }))
    {
      printWarning("failed to create mesh index buffer");
      retval.destroyModel();
      return vulkanModel{ /* return default struct if fail */ };
    }

    // write to index buffer
    pWH->writeToBuffer(retval.m_Buffer_Index, { indices.data() }, { static_cast<VkDeviceSize>(indices.size() * sizeof(decltype(indices)::value_type)) });

    return retval;
  }

  // ***************************************************************************
  // ******************************************************** Serialization ****

  // basic validation
  static const uint32_t proprietaryHeader[]
  {
    69,
    420,
    6969,
    69420
  };

  template <typename TreeType>
  void getTreeModelData(TreeType* pNode, VV& outPositions, IV& outIndices, std::vector<uint32_t>& outObjIndices)
  {
    outPositions.clear();
    outIndices.clear();
    outObjIndices.clear();
    depthFirst
    (
      pNode,
      [&](TreeType* pLambdaNode, size_t /*currDepth*/)
      {
        if (false == pLambdaNode->isLeaf())return;
        size_t before{ outIndices.size() };
        triangulatePolygons(std::get<1>(pLambdaNode->m_Data).pGeometry, outPositions, outIndices);
        if (size_t after{ outIndices.size() }; after > before)
        {
          outObjIndices.emplace_back(static_cast<uint32_t>(after - before));
        }
      }
    );
  }

  // for convenience and speed in final run, only model and indices are saved
  template <typename TreeType>
  bool saveTreeModelToFile(TreeType* pNode, std::filesystem::path outPath, int maxTriSetting)
  {
    std::ofstream ofs{ outPath, std::ios_base::binary };
    if (false == ofs.is_open())return false;

    VV positions;
    IV indices;
    std::vector<uint32_t> objIndices;
    getTreeModelData<TreeType>(pNode, positions, indices, objIndices);

    uint32_t posSize{ static_cast<uint32_t>(positions.size()) };
    uint32_t iSize{ static_cast<uint32_t>(indices.size()) };
    uint32_t oSize{ static_cast<uint32_t>(objIndices.size()) };

    if (0 == posSize || 0 == iSize || 0 == oSize)return false;

    // write first header, number of positions, array of vectors
    ofs.write(reinterpret_cast<const char*>(proprietaryHeader + 0), sizeof(uint32_t));
    ofs.write(reinterpret_cast<const char*>(&posSize), sizeof(uint32_t));
    ofs.write(reinterpret_cast<const char*>(positions.data()), posSize * sizeof(decltype(positions)::value_type));

    // write second header, number of indices, array of indices
    ofs.write(reinterpret_cast<const char*>(proprietaryHeader + 1), sizeof(uint32_t));
    ofs.write(reinterpret_cast<const char*>(&iSize), sizeof(uint32_t));
    ofs.write(reinterpret_cast<const char*>(indices.data()), iSize * sizeof(decltype(indices)::value_type));

    // write third header, number of obj indices, array of obj indices
    ofs.write(reinterpret_cast<const char*>(proprietaryHeader + 2), sizeof(uint32_t));
    ofs.write(reinterpret_cast<const char*>(&oSize), sizeof(uint32_t));
    ofs.write(reinterpret_cast<const char*>(objIndices.data()), oSize * sizeof(decltype(objIndices)::value_type));

    // write fourth header, tri per cell/part
    ofs.write(reinterpret_cast<const char*>(proprietaryHeader + 3), sizeof(uint32_t));
    ofs.write(reinterpret_cast<const char*>(&maxTriSetting), sizeof(maxTriSetting));

    return true;
  }

  bool loadTreeModelFromFile(vulkanModel& outModel, std::vector<uint32_t>& outObjIndices, std::filesystem::path inPath, int& outMaxTriSetting)
  {
    windowHandler* pWH{ windowHandler::getPInstance() };
    assert(pWH != nullptr);// debug only, flow should be pretty standard.

    std::ifstream ifs{ inPath, std::ios_base::binary };
    if (false == ifs.is_open())return false;

    VV positions;
    IV indices;
    std::vector<uint32_t> objIndices;

    uint32_t headerCheck{ 0 }, pSize{ 0 }, iSize{ 0 }, oSize{ 0 };

    // read positions
    ifs.read(reinterpret_cast<char*>(&headerCheck), sizeof(uint32_t));
    if (headerCheck != proprietaryHeader[0])return false;
    ifs.read(reinterpret_cast<char*>(&pSize), sizeof(uint32_t));
    positions.resize(pSize);
    ifs.read(reinterpret_cast<char*>(positions.data()), pSize * sizeof(decltype(positions)::value_type));

    // read indices
    ifs.read(reinterpret_cast<char*>(&headerCheck), sizeof(uint32_t));
    if (headerCheck != proprietaryHeader[1])return false;
    ifs.read(reinterpret_cast<char*>(&iSize), sizeof(uint32_t));
    indices.resize(iSize);
    ifs.read(reinterpret_cast<char*>(indices.data()), iSize * sizeof(decltype(indices)::value_type));

    // read objIndices
    ifs.read(reinterpret_cast<char*>(&headerCheck), sizeof(uint32_t));
    if (headerCheck != proprietaryHeader[2])return false;
    ifs.read(reinterpret_cast<char*>(&oSize), sizeof(uint32_t));
    objIndices.resize(oSize);
    ifs.read(reinterpret_cast<char*>(objIndices.data()), oSize * sizeof(decltype(objIndices)::value_type));

    // read maxTriSetting
    ifs.read(reinterpret_cast<char*>(&headerCheck), sizeof(uint32_t));
    if (headerCheck != proprietaryHeader[3])return false;
    ifs.read(reinterpret_cast<char*>(&outMaxTriSetting), sizeof(outMaxTriSetting));

    // Reading complete, start loading model
    ifs.close();

    outModel.m_VertexCount = static_cast<uint32_t>(positions.size());
    outModel.m_IndexCount = static_cast<uint32_t>(indices.size());
    outModel.m_IndexType = VK_INDEX_TYPE_UINT32;

    // set up vertex buffer
    if (false == pWH->createBuffer(outModel.m_Buffer_Vertex, vulkanBuffer::Setup{ vulkanBuffer::s_BufferUsage_Vertex, vulkanBuffer::s_MemPropFlag_Vertex, outModel.m_VertexCount, sizeof(decltype(positions)::value_type) }))
    {
      printWarning("failed to create mesh vertex buffer");
      outModel.destroyModel();
      false;
    }

    // write to vertex buffer
    pWH->writeToBuffer(outModel.m_Buffer_Vertex, { positions.data() }, { static_cast<VkDeviceSize>(positions.size() * sizeof(decltype(positions)::value_type)) });

    // set up index buffer
    if (false == pWH->createBuffer(outModel.m_Buffer_Index, vulkanBuffer::Setup{ vulkanBuffer::s_BufferUsage_Index, vulkanBuffer::s_MemPropFlag_Index, outModel.m_IndexCount, sizeof(decltype(indices)::value_type) }))
    {
      printWarning("failed to create mesh index buffer");
      outModel.destroyModel();
      return false;
    }

    // write to index buffer
    pWH->writeToBuffer(outModel.m_Buffer_Index, { indices.data() }, { static_cast<VkDeviceSize>(indices.size() * sizeof(decltype(indices)::value_type)) });

    // all good, move objIndices
    outObjIndices = std::move(objIndices);

    return true;

  }

  // ***************************************************************************

  static const glm::vec3 AABB_WireColor{ 0.0625f, 1.0f, 0.0625f };
  //static const glm::vec3 BS_Pearson_WireColor{ 0.0625f, 0.0625f, 1.0f };
  static const std::array<glm::vec3, 13> WireColors
  {
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 0.75f, 0.5f },
    glm::vec3{ 1.0f, 0.25f, 0.25f },
    glm::vec3{ 1.0f, 0.75f, 0.125f },
    glm::vec3{ 0.75f, 0.75f, 0.0f },
    glm::vec3{ 0.5f, 0.75f, 0.25f },
    glm::vec3{ 1.0f, 1.0f, 1.0f },
    glm::vec3{ 0.75f, 0.25f, 0.75f },
    glm::vec3{ 0.25f, 0.25f, 0.75f },
    glm::vec3{ 0.5f, 0.75f, 1.0f },
    glm::vec3{ 0.5f, 0.5f, 0.5f },
  };

}

MTU::GS_Assignment_3::GS_Assignment_3(GameStateManager& rGSM) :
  GameState{ rGSM },
  inputs{ rGSM.getInput() },
  m_Cam{  },
  m_LightColor{  },
  m_CamMoveSpeed{  },
  m_CamFastModifier{  },
  m_Pipelines{  },
  m_DebugModels{  },
  m_Vertices{  },
  m_Faces{  },
  m_Models{  },
  m_Objects{  },
  m_OctTree{ nullptr },
  m_BSPTree{ nullptr },
  m_OctTreeModel{  },
  m_OctTreeObjectIndexCounts{  },
  m_BSPTreeModel{  },
  m_BSPTreeObjectIndexCounts{  },
  m_Octree_TriPerCell{  },
  m_BSPTree_TriPerPart{  },
  m_bEditMode{  },
  m_bDrawObj{  },
  m_bDrawAABB{  },
  //m_bDrawBS_Pearson{  },
  m_bDrawOctTreeBounds{  },
  m_bDrawOctTreeTris{  },
  m_bKeepOctTreeModel{  },
  m_bDrawBSPTreeTris{  },
  m_bKeepBSPTreeModel{  }
{
  GS_PRINT_FUNCSIG();

  bool shouldStopTaskLoading{ false };
  MTU::ThreadTask isLoadingTask{ MTU::taskLoading, &shouldStopTaskLoading };

  // ***************************************************************************
  // ************************************************************** LOADING ****

  // Load debug models
  if (false == A3H::loadDebugModels(m_DebugModels))
  {
    printWarning("Failed to load one or more models, please check file structure!");
  }

  // Load models
  if (false == A3H::loadModels(m_Vertices, m_Faces, m_Models))
  {
    printWarning("Failed to load one or more models, please check file structure!");
  }
  
  // Load pipelines
  if (false == A3H::loadPipelines(m_Pipelines, GSM.getVKWin()))
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

void MTU::GS_Assignment_3::Init()
{
  GS_PRINT_FUNCSIG();

  bool shouldStopTaskInit{ false };
  MTU::ThreadTask isInitializingTask{ MTU::taskInitializing, &shouldStopTaskInit };

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
  m_Objects.reserve(15);// no time to support extra objects this time

  for (int i{ 0 }; i < 3; ++i)
  {
    float zPos{ i * -2.0f };
    // Left side display stands
    {
      A3H::Object& obj{ m_Objects.emplace_back() };
      obj.m_Pos = glm::vec3{ -1.5f, -1.0f, zPos };
      obj.m_Rot = glm::vec3{ 0.0f, glm::half_pi<float>() , 0.0f };
      obj.m_Scale = glm::vec3{ 3.0f, 3.0f, 3.0f };
      obj.m_Model = A3H::E_MODEL_DISPLAY_CASE;
    }
    // Right side display stands
    {
      A3H::Object& obj{ m_Objects.emplace_back() };
      obj.m_Pos = glm::vec3{ 1.5f, -1.0f, zPos };
      obj.m_Rot = glm::vec3{ 0.0f, -glm::half_pi<float>() , 0.0f };
      obj.m_Scale = glm::vec3{ 3.0f, 3.0f, 3.0f };
      obj.m_Model = A3H::E_MODEL_DISPLAY_CASE;
    }
  }

  // adjust R3 display stand to be tipped over
  {
    A3H::Object& obj{ m_Objects.back() };
    obj.m_Pos = glm::vec3{ 3.0f, -2.25f, -3.00f };
    obj.m_Rot = glm::vec3{ 0.5f, 0.0f , -glm::half_pi<float>() };
  }

  // L1 Armadillo
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -1.5f, 0.25f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, -glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A3H::E_MODEL_ARMADILLO;
  }

  // R1 Bunny
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 1.5f, 0.25f, 0.0f };
    obj.m_Rot = glm::vec3{ 0.0f, -glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A3H::E_MODEL_BUNNY;
  }

  // L2 4Sphere
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -1.5f, 0.25f, -2.0f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A3H::E_MODEL_4SPHERE;
  }

  // R2 Star Destroyer
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 1.475f, 0.125f, -2.0f };
    obj.m_Rot = glm::vec3{ 0.325f, -glm::quarter_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A3H::E_MODEL_STAR_DESTROYER;
  }

  // L3 Blender Monkey
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -1.5f, 0.25f, -4.0f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::half_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A3H::E_MODEL_BLENDER_MONKEY;
  }

  // R3 Tipped over Question Mark
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 4.0f, -2.2625f, -2.375f };
    obj.m_Rot = glm::vec3{ 0.0f, -0.55f, -0.4f };
    obj.m_Scale = glm::vec3{ 0.5f, 0.5f, 0.5f };
    obj.m_Model = A3H::E_MODEL_QUESTION_MARK;
  }

  // C4 Lucy Princeton
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 0.0f, 0.0f, -6.0f };
    obj.m_Rot = glm::vec3{ 0.0f, 0.0f , 0.0f };
    obj.m_Scale = glm::vec3{ 5.0f, 5.0f, 5.0f };
    obj.m_Model = A3H::E_MODEL_LUCY_PRINCETON;
  }

  // L4 Dragon (not on a stand to be respectful)
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ -6.375f, -0.5f, -4.125f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::quarter_pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 5.0f, 5.0f, 5.0f };
    obj.m_Model = A3H::E_MODEL_DRAGON;
  }

  // Misc Trash bin
  {
    A3H::Object& obj{ m_Objects.emplace_back() };
    obj.m_Pos = glm::vec3{ 2.375f, -2.0f, 0.125f };
    obj.m_Rot = glm::vec3{ 0.0f, glm::pi<float>(), 0.0f };
    obj.m_Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
    obj.m_Model = A3H::E_MODEL_TRASH_BIN;
  }

  for (auto& x : m_Objects)
  {
    x.updateMatrices();                   // first matrix update
    x.computeBoundingVolumes(m_Vertices); // first bounding volume calc
  }

  // ***************************************************************************
  // *************************************************** SET BOOLS AND MORE ****

#if defined(DEBUG) || defined(_DEBUG)
  m_Octree_TriPerCell = s_OctTreeMaxTriPerCell;
  // CreateOctTree();
  m_BSPTree_TriPerPart = s_OctTreeMaxTriPerCell;
  //CreateBSPTree();
#else

  //if (std::filesystem::directory_entry dir{ "../Assets/Precomputed/Assignment3/OctTrees/256.OctTree" }; dir.exists() && dir.is_regular_file())
  //{
  //  loadOctTreeModel(dir.path());
  //}
  //else
  {
    //std::cout << "Failed to find precomputed OctTree, recomputing!" << std::endl;
    m_Octree_TriPerCell = s_OctTreeDefTriPerCell;
    CreateOctTree();
  }

  if (std::filesystem::directory_entry dir{ "../Assets/Precomputed/Assignment3/BSPTrees/256.BSPTree" }; dir.exists() && dir.is_regular_file())
  {
    loadBSPTreeModel(dir.path());
  }
  else
  {
    std::cout << "Failed to find precomputed BSPTree, recomputing!" << std::endl;
    m_BSPTree_TriPerPart = s_BSPTreeDefTriPerPart;
    CreateBSPTree();
  }

#endif

  m_bEditMode = false;
  m_bDrawObj = false;
  m_bDrawAABB = false;
  //m_bDrawBS_Pearson = false;

  m_bDrawOctTreeBounds = false;
  m_bDrawOctTreeTris = true;
  m_bKeepOctTreeModel = true;

  m_bDrawBSPTreeTris = false;
  m_bKeepBSPTreeModel = true;

  // ***************************************************************************

  shouldStopTaskInit = true;
  for (MTU::Timer lazyTimer{ MTU::Timer::getCurrentTP() }; false == isInitializingTask.isDone(); lazyTimer.stop())
  { // Error if 1s goes by after the loading finishes but thread doesn't end
    if (lazyTimer.getElapsedCount() / MTU::Timer::clockFrequency)
    {
      printf_s("Something has gone wrong! Attempting to continue...\n");
      break;
    }
  }

}

void MTU::GS_Assignment_3::Update(uint64_t dt)
{
  if (inputs.isTriggered(VK_F1))GSM.setNextGameState(GS::E_ASSIGNMENT_1);
  else if (inputs.isTriggered(VK_F2))GSM.setNextGameState(GS::E_ASSIGNMENT_2);
  else if (inputs.isTriggered(VK_F3))GSM.setNextGameState(GS::E_RESTART);

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
    //IMGUI_SAMELINE_TOOLTIPV_HELPER("These (?) tooltips contain more information to use the program as intended\n\nExtra info:\n\nVertices: %d (total number of vertices in the scene)\nObjects RAM: %d B (Memory used to store per object pos, rot, scale, etc)\nModels RAM:  %d B (Memory used to store positions to calculate BVs)\nModels VRAM: %d B (GPU Memory used to store model data)", static_cast<int>(getNumSceneVertices()), static_cast<int>(getObjectsRam()), static_cast<int>(getModelsRAM()), static_cast<int>(getModelsVRAM()));
    IMGUI_SAMELINE_TOOLTIPV_HELPER("These (?) tooltips contain more information to use the program as intended\n\nExtra info:\n\nVertices: %d (total number of vertices in the scene being drawn)\nVRAM usage: %d B", static_cast<int>(getNumSceneVertices()), static_cast<int>(getModelsVRAM()));
    ImGui::TextUnformatted("Window controls");
    IMGUI_SAMELINE_TOOLTIPV_HELPER("F11: Fullscreen\n\n%s", "F1: Go to Assignment 1 state\nF2: Go to Assignment 2 state\nF3: Restart Assignment 3 state");
    ImGui::TextUnformatted("Camera controls");
    IMGUI_SAMELINE_TOOLTIP_HELPER("Right Mouse Button (hold): look around\nW: Move Forward\nA: Move Left\nS: Move Back\nD: Move Right\nSPACE: Move Upwards\nCONTROL: Move Downwards\nSHIFT (hold): Use speed multiplier");

    if (ImGui::TreeNode("Camera Settings"))
    {
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

      ImGui::TreePop();
    }

    ImGui::Separator();

    // draw checkbox
    ImGui::Separator();
    ImGui::TextUnformatted("Bounding Volumes");
    IMGUI_COLOR_CHECKBOX_HELPER("draw solid objects", m_bDrawObj, m_LightColor);
    IMGUI_COLOR_CHECKBOX_HELPER("draw AABB", m_bDrawAABB, A3H::AABB_WireColor);
    //IMGUI_COLOR_CHECKBOX_HELPER("draw BS Pearson (PCA)", m_bDrawBS_Pearson, A3H::BS_Pearson_WireColor);

    ImGui::Separator();
    ImGui::TextUnformatted("Spatial Partitioning");
    IMGUI_SAMELINE_TOOLTIP_HELPER("The default triangles per cell/partition is increased.\n\nThe current minimum of 256 has been tested to work on an AMD Ryzen 7 5800HS");
    
    ImGui::SliderInt("Tri per cell (OctTree)", &m_Octree_TriPerCell, s_OctTreeMinTriPerCell, s_OctTreeMaxTriPerCell);
    ImGui::SliderInt("Tri per part (BSPTree)", &m_BSPTree_TriPerPart, s_BSPTreeMinTriPerPart, s_BSPTreeMaxTriPerCell);
    
    if (ImGui::Button("Recreate OctTree"))CreateOctTree();
    ImGui::SameLine();
    if (ImGui::Button("Destroy OctTree"))DestroyOctTree(m_bKeepOctTreeModel);
    ImGui::SameLine();
    ImGui::Checkbox("Keep model (OctTree)", &m_bKeepOctTreeModel);
    if (ImGui::Button("Recreate BSPTree"))CreateBSPTree();
    ImGui::SameLine();
    if (ImGui::Button("Destroy BSPTree"))DestroyBSPTree(m_bKeepBSPTreeModel);
    ImGui::SameLine();
    ImGui::Checkbox("Keep model (BSPTree)", &m_bKeepBSPTreeModel);

    ImGui::Checkbox("draw OctTree Bounds", &m_bDrawOctTreeBounds);
    IMGUI_SAMELINE_TOOLTIP_HELPER("Unavailable on load, recompute OctTree to view this");
    ImGui::Checkbox("draw OctTree Triangles", &m_bDrawOctTreeTris);
    ImGui::Checkbox("draw BSPTree Triangles", &m_bDrawBSPTreeTris);

    ImGui::Separator();
    ImGui::TextUnformatted("Precomputed Trees");
    IMGUI_SAMELINE_TOOLTIP_HELPER("Click an entry on the combo to load that precomputed tree.\n\nNote: OctTree bounds are not saved, recompute to see those.");
    
    if (ImGui::BeginCombo("Load (OctTree)", "Load Precomputed OctTree"))
    {
      if (std::filesystem::directory_entry dir{ "../Assets/Precomputed/Assignment3/OctTrees" }; dir.exists() && dir.is_directory())
      {
        for (auto const& file : std::filesystem::recursive_directory_iterator(dir))
        {
          if (file.is_directory())continue;
          std::string fileName{ file.path().generic_string() };
          fileName = fileName.substr(fileName.find_last_of('/'));
          if (ImGui::Selectable(fileName.c_str()))
          {
            loadOctTreeModel(file.path());
          }
        }
      }

      ImGui::EndCombo();// OctTree combo
    }

    if (ImGui::BeginCombo("Load (BSPTree)", "Load Precomputed BSPTree"))
    {
      if (std::filesystem::directory_entry dir{ "../Assets/Precomputed/Assignment3/BSPTrees" }; dir.exists() && dir.is_directory())
      {
        for (auto const& file : std::filesystem::recursive_directory_iterator(dir))
        {
          if (file.is_directory())continue;
          std::string fileName{ file.path().generic_string() };
          fileName = fileName.substr(fileName.find_last_of('/'));
          if (ImGui::Selectable(fileName.c_str()))
          {
            loadBSPTreeModel(file.path());
          }
        }
      }

      ImGui::EndCombo();// OctTree combo
    }

    // temp button to save
    //if (ImGui::Button("SAVE BOTH TREES"))
    //{
    //  A3H::saveTreeModelToFile(m_OctTree, "../Assets/Precomputed/Assignment3/OctTrees/LatestSave.OctTree", m_Octree_TriPerCell);
    //  A3H::saveTreeModelToFile(m_BSPTree, "../Assets/Precomputed/Assignment3/BSPTrees/LatestSave.BSPTree", m_BSPTree_TriPerPart);
    //}

    // SCENE OBJECTS EDITOR (DISABLED FOR ASSIGNMENT 3)
    ImGui::Separator();
    ImGui::Text("Scene editor [%d objects]", static_cast<int>(m_Objects.size()));
    //IMGUI_SAMELINE_TOOLTIP_HELPER("You can edit the scene here.\nEdits are on a per object level.\nPlease recompute BV and BVH after making changes.");
    ImGui::SameLine();
    ImGui::Checkbox("Override Edit Lock", &m_bEditMode);
    IMGUI_SAMELINE_TOOLTIP_HELPER("changes reflected on solid object.\nRecreate OctTree to see new OctTree.\nRecreate BSPTree to see new BSPTree\n\nPrecomputed Trees use the default scene.\nAny changes made by overriding this will\nNOT be reflected upon loading.");
    if (false == m_bEditMode)ImGui::BeginDisabled();
    if (ImGui::BeginChild("Objects", ImVec2{ 0.0f, 7.5f * ImGui::GetFrameHeightWithSpacing() }, true))
    {

      for (size_t i{ 0 }, t{ m_Objects.size() }; i < t; ++i)
      {

        bool shouldUpdateObj{ false };

        // quick and dirty ptr_id, idk if it will cause any leaks with
        // imgui when the vector is resized. Hopefully not, but this
        // isn't meant to be a long term framework, just for assignment 2.
        A3H::Object* pObject{ m_Objects.data() + i };

        if (ImGui::TreeNode(pObject, "Object %d", static_cast<int>(i)))
        {
          // Object Position
          if (ImGui::DragFloat3("Pos", &pObject->m_Pos.x, 0.125f, -1000.0f, 1000.0f))
          {
            shouldUpdateObj = true;
          }

          // Object Rotation
          if (ImGui::DragFloat3("Rot", &pObject->m_Rot.x, 0.0125f, -glm::two_pi<float>(), glm::two_pi<float>()))
          {
            shouldUpdateObj = true;
          }
          IMGUI_SAMELINE_TOOLTIP_HELPER("Rotation display: Pitch->Yaw->Roll\nEvaluation order: Roll->Yaw->Pitch");

          // Object Scale
          if (ImGui::DragFloat3("Scale", &pObject->m_Scale.x, 0.0125f, 0.125f, 1000.0f))
          {
            shouldUpdateObj = true;
          }

          // Object Model
          if (ImGui::BeginCombo("Model", A3H::namesAss3Models[pObject->m_Model]))
          {

            for (size_t j{ 0 }; j < A3H::E_NUM_MODELS; ++j)
            {
              if (ImGui::Selectable(A3H::namesAss3Models[j], j == pObject->m_Model))
              {
                pObject->m_Model = static_cast<A3H::enumAss3Models>(j);
                break;
              }
            }

            ImGui::EndCombo();// objModel combo
          }

          ImGui::TreePop();// object
        }

        if (shouldUpdateObj)
        {
          pObject->updateMatrices();
          pObject->computeBoundingVolumes(m_Vertices);
        }

      }

    }
    ImGui::EndChild();// Objects child
    if (false == m_bEditMode)ImGui::EndDisabled();

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

}

void MTU::GS_Assignment_3::Draw()
{
  VkCommandBuffer FCB{ GSM.getFCB() };
  if (FCB == VK_NULL_HANDLE)return;

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[A3H::E_PIPELINE_BASICLIGHT]);
  if (true == m_bDrawObj)
  {
    m_Pipelines[A3H::E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 16, sizeof(glm::vec3), &m_LightColor);
    for (auto& x : m_Objects)
    {
      glm::mat4 xform{ m_Cam.m_W2V * x.m_M2W };
      glm::vec3 lightpos{ x.m_W2M * glm::vec4{ m_Cam.m_Pos, 1.0f } };

      m_Pipelines[A3H::E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &lightpos);
      m_Pipelines[A3H::E_PIPELINE_BASICLIGHT].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
      m_Models[x.m_Model].draw(FCB);
    }
  }

  GSM.getVKWin()->createAndSetPipeline(m_Pipelines[A3H::E_PIPELINE_WIREFRAME]);

  if (true == m_bDrawAABB)
  {
    m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A3H::AABB_WireColor);
    for (auto& x : m_Objects)
    {
      glm::mat4 xform{ m_Cam.m_W2V * A3H::getAABBMat(x.m_AABB) };

      m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);

      m_DebugModels[A3H::E_DEBUGMODEL_CUBE].draw(FCB);
    }
  }

  //if (true == m_bDrawBS_Pearson)
  //{
  //  m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A3H::BS_Pearson_WireColor);
  //  for (auto& x : m_Objects)
  //  {
  //    glm::mat4 xform{ m_Cam.m_W2V * A3H::getBSMat(x.m_BS_Pearson) };

  //    m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);

  //    m_DebugModels[A3H::E_DEBUGMODEL_SPHERE].draw(FCB);
  //  }
  //}

  if (true == m_bDrawOctTreeTris)
  {
    size_t colIdx{ 0 };
    uint32_t iBegin{ 0 };
    for (uint32_t numIndices : m_OctTreeObjectIndexCounts)
    {
      m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A3H::WireColors[colIdx]);
      m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &m_Cam.m_W2V);

      m_OctTreeModel.drawIndexedManual(FCB, iBegin, numIndices);
      iBegin += numIndices;

      colIdx = (colIdx + 1) % A3H::WireColors.size();
    }
  }

  if (true == m_bDrawOctTreeBounds && nullptr != m_OctTree)
  {
    A3H::depthFirst
    (
      m_OctTree,
      [&](A3H::OctTreeNode* pTree, size_t currDepth)
      {
        glm::mat4 xform{ m_Cam.m_W2V * A3H::getOctTreeAABBMat(pTree) };
        m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A3H::WireColors[currDepth % A3H::WireColors.size()]);
        m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &xform);
        m_DebugModels[A3H::E_DEBUGMODEL_CUBE].draw(FCB);
      }
    );
  }

  if (true == m_bDrawBSPTreeTris)
  {
    size_t colIdx{ 0 };
    uint32_t iBegin{ 0 };
    for (uint32_t numIndices : m_BSPTreeObjectIndexCounts)
    {
      m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec3), &A3H::WireColors[colIdx]);
      m_Pipelines[A3H::E_PIPELINE_WIREFRAME].pushConstant(FCB, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &m_Cam.m_W2V);

      m_BSPTreeModel.drawIndexedManual(FCB, iBegin, numIndices);
      iBegin += numIndices;

      colIdx = (colIdx + 1) % A3H::WireColors.size();
    }
  }

}

void MTU::GS_Assignment_3::Free()
{
  GS_PRINT_FUNCSIG();

  m_Objects.clear();// double call, consider removing one?
  
  DestroyOctTree(false);
  DestroyBSPTree(false);

}

MTU::GS_Assignment_3::~GS_Assignment_3()
{
  GS_PRINT_FUNCSIG();

  for (auto& x : m_Pipelines)GSM.getVKWin()->destroyPipelineInfo(x);
  for (auto& x : m_DebugModels)x.destroyModel();
  for (auto& x : m_Models)x.destroyModel();
}

// *****************************************************************************
// ****************************************** HELPER CLASS/STRUCT FUNCTIONS ****

void A3H::Object::updateMatrices()
{
  glm::mat3 rotMat{ glm::mat3_cast(glm::quat{ m_Rot }) };

  m_M2W = glm::mat4{ rotMat * glm::mat3{ m_Scale.x, 0.0f, 0.0f, 0.0f, m_Scale.y, 0.0f, 0.0f, 0.0f, m_Scale.z } };
  m_M2W[3].x = m_Pos.x;
  m_M2W[3].y = m_Pos.y;
  m_M2W[3].z = m_Pos.z;
  m_W2M = glm::translate(glm::mat4{ glm::mat3{(m_Scale.x ? 1.0f / m_Scale.x : 0.0f), 0.0f, 0.0f, 0.0f, (m_Scale.y ? 1.0f / m_Scale.y : 0.0f), 0.0f, 0.0f, 0.0f, (m_Scale.z ? 1.0f / m_Scale.z : 0.0f) } *glm::transpose(rotMat) }, -m_Pos);
}

void A3H::Object::computeBoundingVolumes(MVA const& inModelVertexArray)
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

  // Principal Component Analysis
  //m_BS_Pearson = MTG::createEigenSquaredRadiusSphere(vertices.data(), vertices.size());
  //for (auto const& x : vertices)
  //{
  //  if (sqrDist(x, m_BS_Pearson.m_Center) > m_BS_Pearson.m_Radius)
  //  {
  //    m_BS_Pearson = createNewSquaredSphere(getOppositePoint(m_BS_Pearson, x), x);
  //  }
  //}
  //m_BS_Pearson.m_Radius = std::sqrtf(m_BS_Pearson.m_Radius);
}

size_t MTU::GS_Assignment_3::getNumSceneVertices() const noexcept
{
  size_t retval{ 0 };
  if (m_bDrawObj)for (auto const& x : m_Objects)retval += m_Vertices[x.m_Model].size();
  if (m_bDrawAABB)retval += m_Objects.size() * m_DebugModels[A3H::E_DEBUGMODEL_CUBE].m_VertexCount;
  if (m_bDrawOctTreeTris)retval += m_OctTreeModel.m_VertexCount;
  if (m_bDrawBSPTreeTris)retval += m_BSPTreeModel.m_VertexCount;
  if (m_bDrawOctTreeBounds)retval += m_OctTreeObjectIndexCounts.size() * m_DebugModels[A3H::E_DEBUGMODEL_CUBE].m_VertexCount;
  return retval;
}

//size_t MTU::GS_Assignment_3::getModelsRAM() const noexcept
//{
//  size_t retval{ 0 };
//  for (auto const& x : m_Vertices)retval += x.size() * sizeof(decltype(m_Vertices)::value_type::value_type);
//  return retval;
//}

size_t MTU::GS_Assignment_3::getModelsVRAM() const noexcept
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
  retval += static_cast<size_t>(m_OctTreeModel.m_Buffer_Vertex.m_Settings.m_Count) * static_cast<size_t>(m_OctTreeModel.m_Buffer_Vertex.m_Settings.m_ElemSize) + static_cast<size_t>(m_OctTreeModel.m_Buffer_Index.m_Settings.m_Count) * static_cast<size_t>(m_OctTreeModel.m_Buffer_Index.m_Settings.m_ElemSize);
  retval += static_cast<size_t>(m_BSPTreeModel.m_Buffer_Vertex.m_Settings.m_Count) * static_cast<size_t>(m_BSPTreeModel.m_Buffer_Vertex.m_Settings.m_ElemSize) + static_cast<size_t>(m_BSPTreeModel.m_Buffer_Index.m_Settings.m_Count) * static_cast<size_t>(m_BSPTreeModel.m_Buffer_Index.m_Settings.m_ElemSize);

  return retval;
}

void MTU::GS_Assignment_3::loadOctTreeModel(std::filesystem::path inPath)
{
  DestroyOctTree(true);// keep model in case loading fails
  if (false == A3H::loadTreeModelFromFile(m_OctTreeModel, m_OctTreeObjectIndexCounts, inPath, m_Octree_TriPerCell))
  {
    printWarning("Failed to load from selected file");
  }
}

void MTU::GS_Assignment_3::CreateOctTree()
{
  DestroyOctTree(false);

  std::cout << "Starting work: Create OctTree" << std::endl;

  bool shouldStopTaskWorking{ false };
  MTU::ThreadTask isLoadingTask{ MTU::taskWorking, &shouldStopTaskWorking };

  // Split geometry
  m_OctTree = A3H::buildOctTree(m_Objects, m_Faces, m_Octree_TriPerCell);

  // Join geometry (retriangulate with different colors)
  m_OctTreeModel = A3H::loadFromOctTree(m_OctTree, m_OctTreeObjectIndexCounts);

  shouldStopTaskWorking = true;
  for (MTU::Timer lazyTimer{ MTU::Timer::getCurrentTP() }; false == isLoadingTask.isDone(); lazyTimer.stop())
  { // Error if 1s goes by after the loading finishes but thread doesn't end
    if (lazyTimer.getElapsedCount() / MTU::Timer::clockFrequency)
    {
      printf_s("Something has gone wrong! Attempting to continue...\n");
      break;
    }
  }

}

void MTU::GS_Assignment_3::DestroyOctTree(bool keepModel)
{
  A3H::destroyOctTree(m_OctTree);
  m_OctTree = nullptr;
  if (true == keepModel)return;
  m_OctTreeModel.destroyModel();
  m_OctTreeModel = vulkanModel{  };
  m_OctTreeObjectIndexCounts.clear();
}

void MTU::GS_Assignment_3::loadBSPTreeModel(std::filesystem::path inPath)
{
  DestroyBSPTree(true);// keep model in case loading fails
  if (false == A3H::loadTreeModelFromFile(m_BSPTreeModel, m_BSPTreeObjectIndexCounts, inPath, m_BSPTree_TriPerPart))
  {
    printWarning("Failed to load from selected file");
  }
}

void MTU::GS_Assignment_3::CreateBSPTree()
{
  DestroyBSPTree(false);

  std::cout << "Starting work: Create BSPTree" << std::endl;

  bool shouldStopTaskWorking{ false };
  MTU::ThreadTask isLoadingTask{ MTU::taskWorking, &shouldStopTaskWorking };

  // Split geometry
  m_BSPTree = A3H::buildBSP(m_Objects, m_Faces, m_BSPTree_TriPerPart, A3H::chooseSplitPlaneAuto);

  // Join geometry (retriangulate with different colors)
  m_BSPTreeModel = A3H::loadFromBSPTree(m_BSPTree, m_BSPTreeObjectIndexCounts);

  shouldStopTaskWorking = true;
  for (MTU::Timer lazyTimer{ MTU::Timer::getCurrentTP() }; false == isLoadingTask.isDone(); lazyTimer.stop())
  { // Error if 1s goes by after the loading finishes but thread doesn't end
    if (lazyTimer.getElapsedCount() / MTU::Timer::clockFrequency)
    {
      printf_s("Something has gone wrong! Attempting to continue...\n");
      break;
    }
  }
}

void MTU::GS_Assignment_3::DestroyBSPTree(bool keepModel)
{
  A3H::destroyBSPTree(m_BSPTree);
  m_BSPTree = nullptr;
  if (true == keepModel)return;
  m_BSPTreeModel.destroyModel();
  m_BSPTreeModel = vulkanModel{  };
  m_BSPTreeObjectIndexCounts.clear();
}

// *****************************************************************************
// ******************************************************** OTHER FUNCTIONS ****

glm::mat4 A3H::getAABBMat(MTG::AABB const& inAABB) noexcept
{ // extent is the scale, debug cube is normalized [-0.5f, 0.5f], thus full extent for scale.
  glm::vec3 extent{ inAABB.m_Max - inAABB.m_Min }, center{ inAABB.m_Min + 0.5f * extent };
  return glm::mat4{ extent.x, 0.0f, 0.0f, 0.0f, 0.0f, extent.y, 0.0f, 0.0f, 0.0f, 0.0f, extent.z, 0.0f, center.x, center.y, center.z, 1.0f };
}

glm::mat4 A3H::getBSMat(MTG::Sphere const& inBS) noexcept
{ // normalized sphere model, diameter is 1 so take 2 * radius for matrix scale
  float uniformScale{ 2.0f * inBS.m_Radius };
  return glm::mat4{ uniformScale, 0.0f, 0.0f, 0.0f, 0.0f, uniformScale, 0.0f, 0.0f, 0.0f, 0.0f, uniformScale, 0.0f, inBS.m_Center.x, inBS.m_Center.y, inBS.m_Center.z, 1.0f };
}

glm::mat4 A3H::getOctTreeAABBMat(OctTreeNode const* inNode) noexcept
{
  if (nullptr == inNode)return glm::mat4{ 0.0f };
  float uniformScale{ 2.0f * inNode->m_HalfWidth };
  return glm::mat4{ uniformScale, 0.0f, 0.0f, 0.0f, 0.0f, uniformScale, 0.0f, 0.0f, 0.0f, 0.0f, uniformScale, 0.0f, inNode->m_Center.x, inNode->m_Center.y, inNode->m_Center.z, 1.0f };
}

// *****************************************************************************
