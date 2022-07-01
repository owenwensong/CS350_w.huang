/*!*****************************************************************************
 * @file    Geometry.h
 * @author  Owen Huang Wensong  (w.huang@digipen.edu)
 * @date    27 MAY 2022
 * @brief   Assignment 1: Geometry toolbox
 *
 * @par Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef ASSIGNMENT_GEOMETRY_HEADER_GUARD
#define ASSIGNMENT_GEOMETRY_HEADER_GUARD

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO
//Provide	the	following	functionality	for	operations/tests	on	these	primitives:
//  Basic	Intersection
//    Sphere	Vs	Sphere
//    AABB	Vs	Sphere
//    Sphere	Vs	AABB
//    AABB	Vs	AABB
//  Point-based
//    Point	Vs	Sphere
//    Point	Vs	AABB
//    Point	Vs	Triangle
//    Point	Vs	Plane
//  Ray-based
//    Ray	Vs	Plane
//    Ray	Vs	AABB
//    Ray	Vs	Sphere
//    Ray	Vs	Triangle
//  Plane-based
//    Plane	Vs	AABB
//    Plane	Vs	Sphere

// MT Geometry
namespace MTG
{

  inline float IntrinsicInverseSquare(float number)
  {
    return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ps1(number)));// ONLY FOR MSVC VERSION
  }

  using Vector3D = glm::vec3;

  using Point3D = Vector3D;

  struct Plane
  {
    Plane() = default;
    Plane(glm::vec3 inNormal, glm::vec3 point);

    Plane(Plane const&) = default;
    Plane(Plane&&) = default;

    Plane& operator=(Plane&&) = default;
    Plane& operator=(Plane const&) = default;

    glm::vec3 m_Normal;
    float     m_Dist;
  };
  
  union Triangle
  {
    Triangle() = default;
    Triangle(Point3D const& inV0, Point3D const& inV1, Point3D const& inV2) : 
      v0{ inV0 }, v1{ inV1 }, v2{ inV2 }
    {

    }

    Triangle(Triangle const& RHS) = default;
    Triangle(Triangle&& RHS) = default;

    Triangle& operator=(Triangle&&) = default;
    Triangle& operator=(Triangle const&) = default;

#pragma warning(suppress: 4201)// unnamed struct, exactly what I want.
    struct { Point3D v0, v1, v2; };
    Point3D v[3];
  };

  struct Sphere
  {
    Sphere() = default;
    Sphere(Point3D const& inCenter, float inRadius) : 
      m_Center{ inCenter }, m_Radius{ inRadius }
    {

    }

    Sphere(Sphere const&) = default;
    Sphere(Sphere&&) = default;

    Sphere& operator=(Sphere&&) = default;
    Sphere& operator=(Sphere const&) = default;

    float getVolume() const noexcept;
    float getSurfaceArea() const noexcept;
    //float getOverlapVolume(Sphere const& otherAABB) const noexcept; // not worth the trouble,
    //float getOverlapPercent(Sphere const& otherAABB) const noexcept;// low contribution to heuristic

    Point3D m_Center;
    float   m_Radius;
  };

  union AABB
  {
    AABB() = default;
    AABB(Point3D const& inMin, Point3D inMax) : 
      m_Min{ inMin }, m_Max{ inMax }
    {

    }

    AABB(AABB const&) = default;
    AABB(AABB&&) = default;

    AABB& operator=(AABB&&) = default;
    AABB& operator=(AABB const&) = default;

    float getVolume() const noexcept;
    float getSurfaceArea() const noexcept;
    float getOverlapVolume(AABB const& otherAABB) const noexcept;
    float getOverlapPercent(AABB const& otherAABB) const noexcept;// percent based off smaller volume

    static inline AABB createFromCenterAndHalfExtents(Point3D inCenter, Vector3D inHalfExtents)
    {
      return AABB{ inCenter - inHalfExtents, inCenter + inHalfExtents };
    }

#pragma warning(suppress: 4201)// unnamed struct, exactly what I want.
    struct { Point3D m_Min, m_Max; };
    Point3D m_Bounds[2];// 0 min, 1 max
  };

  struct Ray
  {
    Ray() = default;
    Ray(Point3D const& inPoint, Vector3D inDirection) : 
      m_Point{ inPoint }, m_Direction{ inDirection }
    {

    }

    Ray(Ray const&) = default;
    Ray(Ray&&) = default;

    Ray& operator=(Ray&&) = default;
    Ray& operator=(Ray const&) = default;

    Point3D   m_Point;
    Vector3D  m_Direction;
  };

  bool intersectionSphereSphere(Sphere const&, Sphere const&);
  bool intersectionSphereAABB(Sphere const&, AABB const&);
  bool intersectionAABBAABB(AABB const&, AABB const&);

  bool intersectionPointSphere(Point3D const&, Sphere const&);
  bool intersectionPointAABB(Point3D const&, AABB const&);
  bool intersectionPointTriangle(Point3D const&, Triangle const&, Vector3D& outBarycentric);
  // -1 Outside half plane, 0 Coplanar to plane, 1 Inside half plane
  int cmpPointPlane(Point3D const&, Plane const&, float inEpsilon = FLT_EPSILON);

  bool intersectionRayPlane(Ray const&, Plane const&, float& outTime);
  bool intersectionRayAABB(Ray const&, AABB const&, float& outTime);
  bool intersectionRaySphere(Ray const&, Sphere const&, float& outTime);
  bool intersectionRayTriangle(Ray const&, Triangle const&, float& outTime);

  // -1 Outside half plane, 0 overlaps 1 Outside half plane
  int intersectionPlaneAABB(Plane const&, AABB const&);
  int intersectionPlaneSphere(Plane const&, Sphere const&);
}

#endif//ASSIGNMENT_GEOMETRY_HEADER_GUARD
