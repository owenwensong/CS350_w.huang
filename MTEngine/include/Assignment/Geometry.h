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
  using Vector3D = glm::vec3;

  using Point3D = Vector3D;

  struct Plane
  {
    Plane() = default;
    Plane(glm::vec3 inNormal, glm::vec3 point);

    Plane(Plane const&) = default;
    Plane(Plane&&) = default;


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


    Point3D m_Center;
    float   m_Radius;
  };

  struct AABB
  {
    AABB() = default;
    AABB(Point3D const& inCenter, Vector3D inHalfExtents) : 
      m_Center{ inCenter }, m_HalfExtents{ inHalfExtents }
    {

    }

    AABB(AABB const&) = default;
    AABB(AABB&&) = default;

    AABB createFromMinMax(Point3D inMin, Point3D inMax)
    {
      AABB retval{ inMin, 0.5f * (inMax - inMin) };
      retval.m_Center += retval.m_HalfExtents;
      return retval; // hoping for NRVO
    }

    Point3D   m_Center;
    Vector3D  m_HalfExtents;
  };

  struct Ray
  {
    Ray() = default;
    Ray(Point3D const& inPoint, Vector3D inDirection) : 
      m_Point{ inPoint }, m_Direction{ glm::normalize(inDirection) }
    {

    }

    Ray(Ray const&) = default;
    Ray(Ray&&) = default;

    Point3D   m_Point;
    Vector3D  m_Direction;
  };

  bool TestSphereSphere(Sphere const&, Sphere const&);

  //bool TestAABBSphere
  //bool TestSphereAABB

  bool intersectionAABBAABB(AABB const&, AABB const&);

  bool intersectionPointSphere(Point3D const&, Sphere const&);

  bool intersectionPointAABB(Point3D const&, AABB const&);

  //bool TestPointTriangle(Point3D const&, Triangle const&);

  // -1 Outside half plane, 0 Coplanar to plane, 1 Inside half plane
  int cmpPointPlane(Point3D const&, Plane const&, float inEpsilon = FLT_EPSILON);
  void TestPointPlane(Point3D const&, Plane const&, float inEpsilon = FLT_EPSILON);

  bool intersectionRayPlane(Ray const&, Plane const&, float& outTime);
  void TestRayPlane(Ray const&, Plane const&);

  // doesn't work
  bool intersectionRayAABB(Ray const&, AABB const&, float& outTime);
  void TestRayAabb(Ray const&, AABB const&);// why is it only one capital

  // doesn't work
  bool intersectionRaySphere(Ray const&, Sphere const&, float& outTime0, float& outTime1);
  void TestRaySphere(Ray const&, Sphere const&);

  // Too damn tired to get anything to work...

  void test();

}

#endif//ASSIGNMENT_GEOMETRY_HEADER_GUARD
