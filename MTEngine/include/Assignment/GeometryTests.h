/*!*****************************************************************************
 * @file    GeometryTests.h
 * @author  Owen Huang Wensong  (w.huang@digipen.edu)
 * @date    31 MAY 2022
 * @brief   Assignment 1: Geometry toolbox
 *
 * @par Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GEOMETRY_TESTS_HEADER_GUARD
#define GEOMETRY_TESTS_HEADER_GUARD

#include <Assignment/Geometry.h>

namespace MTG
{
  void TestSphereSphere(Sphere const&, Sphere const&);
  void TestAabbAabb(AABB const&, AABB const&);

  void TestPointSphere(Point3D const&, Sphere const&);
  void TestPointAabb(Point3D const&, AABB const&);
  void TestBarycentricTriangle(Point3D const&, Point3D const&, Point3D const&, float, float, float);
  void TestPointPlane(Point3D const&, Plane const&, float inEpsilon = FLT_EPSILON);

  void TestRayPlane(Ray const&, Plane const&);
  void TestRayAabb(Ray const&, AABB const&);// why is it only one capital
  void TestRaySphere(Ray const&, Sphere const&);
  void TestRayTriangle(Ray const&, Triangle const&);

  void TestPlaneAabb(Plane const&, AABB const&);
  void TestPlaneSphere(Plane const&, Sphere const&);

  void test();
}

#endif//GEOMETRY_TESTS_HEADER_GUARD
