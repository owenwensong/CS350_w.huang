/*!*****************************************************************************
 * @file    GeometryTests.cpp
 * @author  Owen Huang Wensong  (w.huang@digipen.edu)
 * @date    31 MAY 2022
 * @brief   Assignment 1: Geometry toolbox
 *          Helper file containing the tests
 *
 * @par Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <Assignment/GeometryTests.h>
#include <string>

#define TESTEXAMPLE_SPHERE_SPHERE   0 // confirmed good (:
#define TESTEXAMPLE_AABB_SPHERE     0 // confirmed good (: (graphically)
#define TESTEXAMPLE_SPHERE_AABB     0 // confirmed good (: (graphically)
#define TESTEXAMPLE_AABB_AABB       1 // confirmed good (:

#define TESTEXAMPLE_POINT_SPHERE    0 // confirmed good (:
#define TESTEXAMPLE_POINT_AABB      0 // confirmed good (:
#define TESTEXAMPLE_POINT_TRIANGLE  0 // 5,62,63,64 weird. Waiting on teams reply
#define TESTEXAMPLE_POINT_PLANE     0 // confirmed good (:

#define TESTEXAMPLE_RAY_PLANE       0 // confirmed good (:
#define TESTEXAMPLE_RAY_AABB        0 // confirmed good (:
#define TESTEXAMPLE_RAY_SPHERE      0 // confirmed good (:
#define TESTEXAMPLE_RAY_TRIANGLE    0 // confirmed good (:

#define TESTEXAMPLE_PLANE_AABB      0 // confirmed good (:
#define TESTEXAMPLE_PLANE_SPHERE    0 // confirmed good (:

void MTG::TestSphereSphere(Sphere const& s0, Sphere const& s1)
{
  printf_s("  Result:%s\n", intersectionSphereSphere(s0, s1) ? "true" : "false");
}

void MTG::TestAabbAabb(AABB const& a0, AABB const& a1)
{
  printf_s("  Result:%s\n", intersectionAABBAABB(a0, a1) ? "true" : "false");
}

void MTG::TestPointSphere(Point3D const& p0, Sphere const& s1)
{
  printf_s("  Result:%s\n", intersectionPointSphere(p0, s1) ? "true" : "false");
}

void MTG::TestPointAabb(Point3D const& p0, AABB const& a1)
{
  printf_s("  Result:%s\n", intersectionPointAABB(p0, a1) ? "true" : "false");
}

void MTG::TestBarycentricTriangle(Point3D const& tp0, Point3D const& tp1, Point3D const& tp2, float u, float v, float w)
{
  Vector3D BarycentricResult;
  Point3D p0{ u * tp0 + v * tp1 + w * tp2 };
  Triangle t1{ tp0, tp1, tp2 };
  const char* resString{ intersectionPointTriangle(p0, t1, BarycentricResult) ? "true" : "false" };
  printf_s("  Result:%s (u, v, w): (%.2f, %.2f, %.2f)\n", resString, BarycentricResult.x, BarycentricResult.y, BarycentricResult.z);
}

void MTG::TestPointPlane(Point3D const& p0, Plane const& p1, float inEpsilon)
{
  switch (cmpPointPlane(p0, p1, inEpsilon))
  {
  case -1:
    printf_s("  Result:%s\n", "Outside");
    break;
  case 1:
    printf_s("  Result:%s\n", "Inside");
    break;
  case 0:
    printf_s("  Result:%s\n", "Coplanar");
    break;
  }
}

void MTG::TestRayPlane(Ray const& r0, Plane const& p1)
{
  if (float tmp; intersectionRayPlane(r0, p1, tmp))
  {
    printf_s("  Result:%s t:%.2f\n", "true", tmp);
  }
  else
  {
    printf_s("  Result:%s\n", "false");
  }
}

void MTG::TestRayAabb(Ray const& r0, AABB const& a1)
{
  if (float tmp; intersectionRayAABB(r0, a1, tmp))
  {
    printf_s("  Result:%s t:%.2f\n", "true", tmp);
  }
  else
  {
    printf_s("  Result:%s\n", "false");
  }
}

void MTG::TestRaySphere(Ray const& r0, Sphere const& s1)
{
  if (float tmp; intersectionRaySphere(r0, s1, tmp))
  {
    printf_s("  Result:%s t:%.2f\n", "true", tmp);
  }
  else
  {
    printf_s("  Result:%s\n", "false");
  }
}

void MTG::TestRayTriangle(Ray const& r0, Triangle const& t1)
{
  if (float tmp; intersectionRayTriangle(r0, t1, tmp))
  {
    printf_s("  Result:%s t:%.2f\n", "true", tmp);
  }
  else
  {
    printf_s("  Result:%s\n", "false");
  }
}

void MTG::TestPlaneAabb(Plane const& p0, AABB const& a1)
{
  switch (intersectionPlaneAABB(p0, a1))
  {
  case -1:
    printf_s("  Result:%s\n", "Outside");
    break;
  case 1:
    printf_s("  Result:%s\n", "Inside");
    break;
  case 0:
    printf_s("  Result:%s\n", "Overlaps");
    break;
  }
}

void MTG::TestPlaneSphere(Plane const& p0, Sphere const& s1)
{
  switch (intersectionPlaneSphere(p0, s1))
  {
  case -1:
    printf_s("  Result:%s\n", "Outside");
    break;
  case 1:
    printf_s("  Result:%s\n", "Inside");
    break;
  case 0:
    printf_s("  Result:%s\n", "Overlaps");
    break;
  }
}

namespace MTG
{
  using Vector3 = Vector3D;
  void PrintTestHeader(const std::string& testName)
  {
    printf("\n");
    printf("////////////////////////////////////////////////////////////\n");
    printf("%s\n", testName.c_str());
    printf("////////////////////////////////////////////////////////////\n");
  }
  using Aabb = AABB;
}

/** Sphere Vs Sphere -- 15 tests **/
namespace MTG
{

  // Spheres separating
  void SphereSphereTest1(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(-2, 0, 0), 1);
    Sphere sphere1(Vector3(2, 0, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres separating
  void SphereSphereTest2(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(-1.5f, 0, 0), 1);
    Sphere sphere1(Vector3(1.5f, 0, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres just touching
  void SphereSphereTest3(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(-1, 0, 0), 1);
    Sphere sphere1(Vector3(1, 0, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres overlapping a bit
  void SphereSphereTest4(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(-0.5f, 0, 0), 1);
    Sphere sphere1(Vector3(0.5f, 0, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres at the same center
  void SphereSphereTest5(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, 0, 0), 1);
    Sphere sphere1(Vector3(0, 0, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres separating
  void SphereSphereTest6(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, -2, 0), 1);
    Sphere sphere1(Vector3(0, 2, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres separating
  void SphereSphereTest7(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, -1.5f, 0), 1);
    Sphere sphere1(Vector3(0, 1.5f, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres just touching
  void SphereSphereTest8(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, -1, 0), 1);
    Sphere sphere1(Vector3(0, 1, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres overlapping a bit
  void SphereSphereTest9(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, -0.5f, 0), 1);
    Sphere sphere1(Vector3(0, 0.5f, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres at the same center
  void SphereSphereTest10(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, 0, 0), 1);
    Sphere sphere1(Vector3(0, 0, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres separating
  void SphereSphereTest11(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, 0, -2), 1);
    Sphere sphere1(Vector3(0, 0, 2), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres separating
  void SphereSphereTest12(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, 0, -1.5f), 1);
    Sphere sphere1(Vector3(0, 0, 1.5f), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres just touching
  void SphereSphereTest13(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, 0, -1), 1);
    Sphere sphere1(Vector3(0, 0, 1), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres overlapping a bit
  void SphereSphereTest14(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, 0, -0.5f), 1);
    Sphere sphere1(Vector3(0, 0, 0.5f), 1);

    TestSphereSphere(sphere0, sphere1);
  }

  // Spheres at the same center
  void SphereSphereTest15(const std::string& testName)
  {
    PrintTestHeader(testName);

    Sphere sphere0(Vector3(0, 0, 0), 1);
    Sphere sphere1(Vector3(0, 0, 0), 1);

    TestSphereSphere(sphere0, sphere1);
  }

}

/** AABB Vs AABB : 84 tests **/
namespace MTG
{
// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest1(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1.5f, -0.5f, -0.5f), Vector3(-0.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(0.5f, -0.5f, -0.5f), Vector3(1.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest2(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0.5f, -0.5f, -0.5f), Vector3(1.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-1.5f, -0.5f, -0.5f), Vector3(-0.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest3(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1, -0.5f, -0.5f), Vector3(0, 0.5f, 0.5f));
  Aabb aabb1(Vector3(0, -0.5f, -0.5f), Vector3(1, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest4(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0, -0.5f, -0.5f), Vector3(1, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-1, -0.5f, -0.5f), Vector3(0, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest5(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.75f, -0.5f, -0.5f), Vector3(0.25f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.25f, -0.5f, -0.5f), Vector3(0.75f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest6(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.25f, -0.5f, -0.5f), Vector3(0.75f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.75f, -0.5f, -0.5f), Vector3(0.25f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest7(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest8(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -1.5f, -0.5f), Vector3(0.5f, -0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, 0.5f, -0.5f), Vector3(0.5f, 1.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest9(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, 0.5f, -0.5f), Vector3(0.5f, 1.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -1.5f, -0.5f), Vector3(0.5f, -0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest10(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -1, -0.5f), Vector3(0.5f, 0, 0.5f));
  Aabb aabb1(Vector3(-0.5f, 0, -0.5f), Vector3(0.5f, 1, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest11(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, 0, -0.5f), Vector3(0.5f, 1, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -1, -0.5f), Vector3(0.5f, 0, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest12(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.75f, -0.5f), Vector3(0.5f, 0.25f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.25f, -0.5f), Vector3(0.5f, 0.75f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest13(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.25f, -0.5f), Vector3(0.5f, 0.75f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.75f, -0.5f), Vector3(0.5f, 0.25f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest14(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest15(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -1.5f), Vector3(0.5f, 0.5f, -0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.5f, 0.5f, 1.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest16(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.5f, 0.5f, 1.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -1.5f), Vector3(0.5f, 0.5f, -0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest17(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -1), Vector3(0.5f, 0.5f, 0));
  Aabb aabb1(Vector3(-0.5f, -0.5f, 0), Vector3(0.5f, 0.5f, 1));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest18(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, 0), Vector3(0.5f, 0.5f, 1));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -1), Vector3(0.5f, 0.5f, 0));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest19(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.75f), Vector3(0.5f, 0.5f, 0.25f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.25f), Vector3(0.5f, 0.5f, 0.75f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest20(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.25f), Vector3(0.5f, 0.5f, 0.75f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.75f), Vector3(0.5f, 0.5f, 0.25f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest21(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest22(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1.5f, -1.5f, -0.5f), Vector3(-0.5f, -0.5f, 0.5f));
  Aabb aabb1(Vector3(0.5f, 0.5f, -0.5f), Vector3(1.5f, 1.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest23(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0.5f, 0.5f, -0.5f), Vector3(1.5f, 1.5f, 0.5f));
  Aabb aabb1(Vector3(-1.5f, -1.5f, -0.5f), Vector3(-0.5f, -0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest24(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1, -1, -0.5f), Vector3(2.98023e-08f, 2.98023e-08f, 0.5f));
  Aabb aabb1(Vector3(-2.98023e-08f, -2.98023e-08f, -0.5f), Vector3(1, 1, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest25(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-2.98023e-08f, -2.98023e-08f, -0.5f), Vector3(1, 1, 0.5f));
  Aabb aabb1(Vector3(-1, -1, -0.5f), Vector3(2.98023e-08f, 2.98023e-08f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest26(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.75f, -0.75f, -0.5f), Vector3(0.25f, 0.25f, 0.5f));
  Aabb aabb1(Vector3(-0.25f, -0.25f, -0.5f), Vector3(0.75f, 0.75f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest27(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.25f, -0.25f, -0.5f), Vector3(0.75f, 0.75f, 0.5f));
  Aabb aabb1(Vector3(-0.75f, -0.75f, -0.5f), Vector3(0.25f, 0.25f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest28(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest29(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1.5f, -0.5f, -1.5f), Vector3(-0.5f, 0.5f, -0.5f));
  Aabb aabb1(Vector3(0.5f, -0.5f, 0.5f), Vector3(1.5f, 0.5f, 1.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest30(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0.5f, -0.5f, 0.5f), Vector3(1.5f, 0.5f, 1.5f));
  Aabb aabb1(Vector3(-1.5f, -0.5f, -1.5f), Vector3(-0.5f, 0.5f, -0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest31(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1, -0.5f, -1), Vector3(2.98023e-08f, 0.5f, 2.98023e-08f));
  Aabb aabb1(Vector3(-2.98023e-08f, -0.5f, -2.98023e-08f), Vector3(1, 0.5f, 1));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest32(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-2.98023e-08f, -0.5f, -2.98023e-08f), Vector3(1, 0.5f, 1));
  Aabb aabb1(Vector3(-1, -0.5f, -1), Vector3(2.98023e-08f, 0.5f, 2.98023e-08f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest33(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.75f, -0.5f, -0.75f), Vector3(0.25f, 0.5f, 0.25f));
  Aabb aabb1(Vector3(-0.25f, -0.5f, -0.25f), Vector3(0.75f, 0.5f, 0.75f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest34(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.25f, -0.5f, -0.25f), Vector3(0.75f, 0.5f, 0.75f));
  Aabb aabb1(Vector3(-0.75f, -0.5f, -0.75f), Vector3(0.25f, 0.5f, 0.25f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest35(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest36(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -1.5f, -1.5f), Vector3(0.5f, -0.5f, -0.5f));
  Aabb aabb1(Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.5f, 1.5f, 1.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest37(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.5f, 1.5f, 1.5f));
  Aabb aabb1(Vector3(-0.5f, -1.5f, -1.5f), Vector3(0.5f, -0.5f, -0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest38(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -1, -1), Vector3(0.5f, 2.98023e-08f, 2.98023e-08f));
  Aabb aabb1(Vector3(-0.5f, -2.98023e-08f, -2.98023e-08f), Vector3(0.5f, 1, 1));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest39(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -2.98023e-08f, -2.98023e-08f), Vector3(0.5f, 1, 1));
  Aabb aabb1(Vector3(-0.5f, -1, -1), Vector3(0.5f, 2.98023e-08f, 2.98023e-08f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest40(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.75f, -0.75f), Vector3(0.5f, 0.25f, 0.25f));
  Aabb aabb1(Vector3(-0.5f, -0.25f, -0.25f), Vector3(0.5f, 0.75f, 0.75f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest41(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.25f, -0.25f), Vector3(0.5f, 0.75f, 0.75f));
  Aabb aabb1(Vector3(-0.5f, -0.75f, -0.75f), Vector3(0.5f, 0.25f, 0.25f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest42(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest43(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1.5f, -0.5f, -0.1f), Vector3(-0.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(0.5f, -0.5f, -0.1f), Vector3(1.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest44(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0.5f, -0.5f, -0.1f), Vector3(1.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-1.5f, -0.5f, -0.1f), Vector3(-0.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest45(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1, -0.5f, -0.1f), Vector3(0, 0.5f, 0.1f));
  Aabb aabb1(Vector3(0, -0.5f, -0.1f), Vector3(1, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest46(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0, -0.5f, -0.1f), Vector3(1, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-1, -0.5f, -0.1f), Vector3(0, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest47(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.75f, -0.5f, -0.1f), Vector3(0.25f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.25f, -0.5f, -0.1f), Vector3(0.75f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest48(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.25f, -0.5f, -0.1f), Vector3(0.75f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.75f, -0.5f, -0.1f), Vector3(0.25f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest49(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest50(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -1.5f, -0.1f), Vector3(0.5f, -0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, 0.5f, -0.1f), Vector3(0.5f, 1.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest51(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, 0.5f, -0.1f), Vector3(0.5f, 1.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -1.5f, -0.1f), Vector3(0.5f, -0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest52(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -1, -0.1f), Vector3(0.5f, 0, 0.1f));
  Aabb aabb1(Vector3(-0.5f, 0, -0.1f), Vector3(0.5f, 1, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest53(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, 0, -0.1f), Vector3(0.5f, 1, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -1, -0.1f), Vector3(0.5f, 0, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest54(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.75f, -0.1f), Vector3(0.5f, 0.25f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.25f, -0.1f), Vector3(0.5f, 0.75f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest55(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.25f, -0.1f), Vector3(0.5f, 0.75f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.75f, -0.1f), Vector3(0.5f, 0.25f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest56(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest57(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.3f), Vector3(0.5f, 0.5f, -0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, 0.1f), Vector3(0.5f, 0.5f, 0.3f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest58(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, 0.1f), Vector3(0.5f, 0.5f, 0.3f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.3f), Vector3(0.5f, 0.5f, -0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest59(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.2f), Vector3(0.5f, 0.5f, 0));
  Aabb aabb1(Vector3(-0.5f, -0.5f, 0), Vector3(0.5f, 0.5f, 0.2f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest60(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, 0), Vector3(0.5f, 0.5f, 0.2f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.2f), Vector3(0.5f, 0.5f, 0));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest61(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.15f), Vector3(0.5f, 0.5f, 0.05f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.05f), Vector3(0.5f, 0.5f, 0.15f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest62(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.05f), Vector3(0.5f, 0.5f, 0.15f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.15f), Vector3(0.5f, 0.5f, 0.05f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest63(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest64(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1.5f, -1.5f, -0.1f), Vector3(-0.5f, -0.5f, 0.1f));
  Aabb aabb1(Vector3(0.5f, 0.5f, -0.1f), Vector3(1.5f, 1.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest65(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0.5f, 0.5f, -0.1f), Vector3(1.5f, 1.5f, 0.1f));
  Aabb aabb1(Vector3(-1.5f, -1.5f, -0.1f), Vector3(-0.5f, -0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest66(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1, -1, -0.1f), Vector3(2.98023e-08f, 2.98023e-08f, 0.1f));
  Aabb aabb1(Vector3(-2.98023e-08f, -2.98023e-08f, -0.1f), Vector3(1, 1, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest67(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-2.98023e-08f, -2.98023e-08f, -0.1f), Vector3(1, 1, 0.1f));
  Aabb aabb1(Vector3(-1, -1, -0.1f), Vector3(2.98023e-08f, 2.98023e-08f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest68(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.75f, -0.75f, -0.1f), Vector3(0.25f, 0.25f, 0.1f));
  Aabb aabb1(Vector3(-0.25f, -0.25f, -0.1f), Vector3(0.75f, 0.75f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest69(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.25f, -0.25f, -0.1f), Vector3(0.75f, 0.75f, 0.1f));
  Aabb aabb1(Vector3(-0.75f, -0.75f, -0.1f), Vector3(0.25f, 0.25f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest70(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest71(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-1.1f, -0.5f, -0.7f), Vector3(-0.1f, 0.5f, -0.5f));
  Aabb aabb1(Vector3(0.1f, -0.5f, 0.5f), Vector3(1.1f, 0.5f, 0.7f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest72(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(0.1f, -0.5f, 0.5f), Vector3(1.1f, 0.5f, 0.7f));
  Aabb aabb1(Vector3(-1.1f, -0.5f, -0.7f), Vector3(-0.1f, 0.5f, -0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest73(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.8f, -0.5f, -0.4f), Vector3(0.2f, 0.5f, -0.2f));
  Aabb aabb1(Vector3(-0.2f, -0.5f, 0.2f), Vector3(0.8f, 0.5f, 0.4f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest74(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.2f, -0.5f, 0.2f), Vector3(0.8f, 0.5f, 0.4f));
  Aabb aabb1(Vector3(-0.8f, -0.5f, -0.4f), Vector3(0.2f, 0.5f, -0.2f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest75(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.65f, -0.5f, -0.25f), Vector3(0.35f, 0.5f, -0.05f));
  Aabb aabb1(Vector3(-0.35f, -0.5f, 0.05f), Vector3(0.65f, 0.5f, 0.25f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest76(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.35f, -0.5f, 0.05f), Vector3(0.65f, 0.5f, 0.25f));
  Aabb aabb1(Vector3(-0.65f, -0.5f, -0.25f), Vector3(0.35f, 0.5f, -0.05f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest77(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb0 behind aabb1)
void AabbAabbTest78(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -1.1f, -0.7f), Vector3(0.5f, -0.1f, -0.5f));
  Aabb aabb1(Vector3(-0.5f, 0.1f, 0.5f), Vector3(0.5f, 1.1f, 0.7f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs separating (with aabb1 behind aabb0)
void AabbAabbTest79(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, 0.1f, 0.5f), Vector3(0.5f, 1.1f, 0.7f));
  Aabb aabb1(Vector3(-0.5f, -1.1f, -0.7f), Vector3(0.5f, -0.1f, -0.5f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb0 behind aabb1)
void AabbAabbTest80(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.8f, -0.4f), Vector3(0.5f, 0.2f, -0.2f));
  Aabb aabb1(Vector3(-0.5f, -0.2f, 0.2f), Vector3(0.5f, 0.8f, 0.4f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs just touching (with aabb1 behind aabb0)
void AabbAabbTest81(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.2f, 0.2f), Vector3(0.5f, 0.8f, 0.4f));
  Aabb aabb1(Vector3(-0.5f, -0.8f, -0.4f), Vector3(0.5f, 0.2f, -0.2f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb0 behind aabb1)
void AabbAabbTest82(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.65f, -0.25f), Vector3(0.5f, 0.35f, -0.05f));
  Aabb aabb1(Vector3(-0.5f, -0.35f, 0.05f), Vector3(0.5f, 0.65f, 0.25f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs overlapping (with aabb1 behind aabb0)
void AabbAabbTest83(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.35f, 0.05f), Vector3(0.5f, 0.65f, 0.25f));
  Aabb aabb1(Vector3(-0.5f, -0.65f, -0.25f), Vector3(0.5f, 0.35f, -0.05f));

  TestAabbAabb(aabb0, aabb1);
}

// Aabbs at the same center
void AabbAabbTest84(const std::string& testName)
{
  PrintTestHeader(testName);

  Aabb aabb0(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));
  Aabb aabb1(Vector3(-0.5f, -0.5f, -0.1f), Vector3(0.5f, 0.5f, 0.1f));

  TestAabbAabb(aabb0, aabb1);
}

}

/** Point Vs Sphere : 26 tests **/
namespace MTG
{

void PointSphereTest1(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(4, 2, 3);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest2(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(2.5f, 2, 3);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest3(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.75f, 2, 3);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest4(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 5, 3);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest5(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 3.5f, 3);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest6(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2.75f, 3);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest7(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 6);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest8(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 4.5f);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest9(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 3.75f);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest10(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(2.73205f, 3.73205f, 4.73205f);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest11(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.86603f, 2.86603f, 3.86603f);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest12(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.43301f, 2.43301f, 3.43301f);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest13(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 3);
  Sphere sphere(Vector3(1, 2, 3), 1.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest14(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 0, 0);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest15(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.5f, 0, 0);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest16(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.25f, 0, 0);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest17(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 1, 0);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest18(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0.5f, 0);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest19(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0.25f, 0);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest20(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, 1);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest21(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, 0.5f);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest22(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, 0.25f);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest23(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.57735f, 0.57735f, 0.57735f);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest24(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.288675f, 0.288675f, 0.288675f);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest25(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.144338f, 0.144338f, 0.144338f);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}

void PointSphereTest26(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, 0);
  Sphere sphere(Vector3(0, 0, 0), 0.5f);

  TestPointSphere(point, sphere);
}
}

/** Point Vs AABB : 84 tests  **/
namespace MTG
{

void PointAabbTest1(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 0, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest2(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.5f, 0, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest3(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.25f, 0, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest4(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.25f, 0, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest5(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.5f, 0, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest6(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-1, 0, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest7(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 1, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest8(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0.5f, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest9(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0.25f, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest10(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, -0.25f, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest11(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, -0.5f, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest12(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, -1, 0);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest13(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, 1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest14(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, 0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest15(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, 0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest16(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, -0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest17(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, -0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest18(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0, 0, -1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest19(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 1, 1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest20(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.5f, 0.5f, 0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest21(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.25f, 0.25f, 0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest22(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.25f, -0.25f, -0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest23(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.5f, -0.5f, -0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest24(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-1, -1, -1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest25(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 1, -1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest26(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.5f, 0.5f, -0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest27(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.25f, 0.25f, -0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest28(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.25f, -0.25f, 0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest29(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.5f, -0.5f, 0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest30(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-1, -1, 1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest31(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, -1, -1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest32(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.5f, -0.5f, -0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest33(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.25f, -0.25f, -0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest34(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.25f, 0.25f, 0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest35(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.5f, 0.5f, 0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest36(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-1, 1, 1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest37(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, -1, 1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest38(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.5f, -0.5f, 0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest39(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.25f, -0.25f, 0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest40(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.25f, 0.25f, -0.25f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest41(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-0.5f, 0.5f, -0.5f);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest42(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(-1, 1, -1);
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPointAabb(point, aabb);
}

void PointAabbTest43(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.2f, 2, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest44(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.1f, 2, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest45(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.05f, 2, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest46(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.95f, 2, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest47(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.9f, 2, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest48(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.8f, 2, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest49(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 6, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest50(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 4, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest51(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 3, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest52(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 1, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest53(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 0, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest54(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, -2, 3);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest55(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 4.4f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest56(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 3.7f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest57(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 3.35f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest58(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 2.65f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest59(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 2.3f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest60(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1, 2, 1.6f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest61(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.2f, 6, 4.4f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest62(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.1f, 4, 3.7f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest63(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.05f, 3, 3.35f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest64(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.95f, 1, 2.65f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest65(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.9f, 0, 2.3f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest66(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.8f, -2, 1.6f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest67(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.2f, 6, 1.6f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest68(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.1f, 4, 2.3f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest69(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.05f, 3, 2.65f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest70(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.95f, 1, 3.35f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest71(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.9f, 0, 3.7f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest72(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.8f, -2, 4.4f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest73(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.2f, -2, 1.6f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest74(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.1f, 0, 2.3f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest75(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.05f, 1, 2.65f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest76(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.95f, 3, 3.35f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest77(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.9f, 4, 3.7f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest78(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.8f, 6, 4.4f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest79(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.2f, -2, 4.4f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest80(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.1f, 0, 3.7f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest81(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(1.05f, 1, 3.35f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest82(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.95f, 3, 2.65f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest83(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.9f, 4, 2.3f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

void PointAabbTest84(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 point = Vector3(0.8f, 6, 1.6f);
  Aabb aabb(Vector3(0.9f, 0, 2.3f), Vector3(1.1f, 4, 3.7f));

  TestPointAabb(point, aabb);
}

}

/** Point Vs Triangle : 64 tests **/ // Why the hell are these so weird?
namespace MTG
{
void BarycentricTriangleTest0(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.33f;
  float v = 0.33f;
  float w = 0.33f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Halfway in-between p0 and p1
void BarycentricTriangleTest1(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.5f;
  float v = 0.5f;
  float w = 0;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Halfway in-between p0 and p2
void BarycentricTriangleTest2(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.5f;
  float v = 0;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Halfway in-between p1 and p2
void BarycentricTriangleTest3(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0;
  float v = 0.5f;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Coordinate at p0
void BarycentricTriangleTest4(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 1;
  float v = 0;
  float w = 0;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Coordinate at p1
void BarycentricTriangleTest5(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0;
  float v = 1;
  float w = 0;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Coordinate at p2
void BarycentricTriangleTest6(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0;
  float v = 0;
  float w = 1;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest7(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.25f;
  float v = 0.5f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest8(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.5f;
  float v = 0.25f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest9(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.25f;
  float v = 0.25f;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest10(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.25f;
  float v = 0.25f;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest11(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.5f;
  float v = 0.25f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest12(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.25f;
  float v = 0.5f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest13(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.1f;
  float v = 0.3f;
  float w = 0.6f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest14(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.3f;
  float v = 0.6f;
  float w = 0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest15(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.6f;
  float v = 0.1f;
  float w = 0.3f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest16(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.1f;
  float v = 0.6f;
  float w = 0.3f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest17(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.3f;
  float v = 0.1f;
  float w = 0.6f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest18(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.6f;
  float v = 0.3f;
  float w = 0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest19(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = -0.1f;
  float v = 0.45f;
  float w = 0.65f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest20(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.45f;
  float v = 0.65f;
  float w = -0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest21(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.65f;
  float v = -0.1f;
  float w = 0.45f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest22(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = -0.1f;
  float v = 0.65f;
  float w = 0.45f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest23(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.45f;
  float v = -0.1f;
  float w = 0.65f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest24(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 0.65f;
  float v = 0.45f;
  float w = -0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest25(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = -0.15f;
  float v = -0.05f;
  float w = 1.2f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest26(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = -0.05f;
  float v = 1.2f;
  float w = -0.15f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest27(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 1.2f;
  float v = -0.15f;
  float w = -0.05f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest28(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = -0.15f;
  float v = 1.2f;
  float w = -0.05f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest29(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = -0.05f;
  float v = -0.15f;
  float w = 1.2f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest30(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(0, 0, 0);
  Vector3 p1 = Vector3(1, 0, 0);
  Vector3 p2 = Vector3(0, 1, 0);
  float u = 1.2f;
  float v = -0.05f;
  float w = -0.15f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest31(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.33f;
  float v = 0.33f;
  float w = 0.33f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Halfway in-between p0 and p1
void BarycentricTriangleTest32(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.5f;
  float v = 0.5f;
  float w = 0;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Halfway in-between p0 and p2
void BarycentricTriangleTest33(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.5f;
  float v = 0;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Halfway in-between p1 and p2
void BarycentricTriangleTest34(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0;
  float v = 0.5f;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Coordinate at p0
void BarycentricTriangleTest35(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 1;
  float v = 0;
  float w = 0;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Coordinate at p1
void BarycentricTriangleTest36(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0;
  float v = 1;
  float w = 0;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Coordinate at p2
void BarycentricTriangleTest37(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0;
  float v = 0;
  float w = 1;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest38(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.25f;
  float v = 0.5f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest39(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.5f;
  float v = 0.25f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest40(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.25f;
  float v = 0.25f;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest41(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.25f;
  float v = 0.25f;
  float w = 0.5f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest42(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.5f;
  float v = 0.25f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest43(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.25f;
  float v = 0.5f;
  float w = 0.25f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest44(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.1f;
  float v = 0.3f;
  float w = 0.6f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest45(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.3f;
  float v = 0.6f;
  float w = 0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest46(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.6f;
  float v = 0.1f;
  float w = 0.3f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest47(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.1f;
  float v = 0.6f;
  float w = 0.3f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest48(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.3f;
  float v = 0.1f;
  float w = 0.6f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest49(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.6f;
  float v = 0.3f;
  float w = 0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest50(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = -0.1f;
  float v = 0.45f;
  float w = 0.65f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest51(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.45f;
  float v = 0.65f;
  float w = -0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest52(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.65f;
  float v = -0.1f;
  float w = 0.45f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest53(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = -0.1f;
  float v = 0.65f;
  float w = 0.45f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest54(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.45f;
  float v = -0.1f;
  float w = 0.65f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest55(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.65f;
  float v = 0.45f;
  float w = -0.1f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest56(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = -0.15f;
  float v = -0.05f;
  float w = 1.2f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest57(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = -0.05f;
  float v = 1.2f;
  float w = -0.15f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest58(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 1.2f;
  float v = -0.15f;
  float w = -0.05f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest59(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = -0.15f;
  float v = 1.2f;
  float w = -0.05f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest60(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = -0.05f;
  float v = -0.15f;
  float w = 1.2f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

void BarycentricTriangleTest61(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 1.2f;
  float v = -0.05f;
  float w = -0.15f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: false. Testing a degenerate triangle
void BarycentricTriangleTest62(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(-1, -2, -3);
  Vector3 p1 = Vector3(-1, -2, -3);
  Vector3 p2 = Vector3(0, 0, 0);
  float u = 0;
  float v = 1;
  float w = 0;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Testing the use of epsilon
void BarycentricTriangleTest63(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = -0.025f;
  float v = -0.025f;
  float w = 1.05f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

// Expected Result: true. Testing the use of epsilon
void BarycentricTriangleTest64(const std::string& testName)
{
  PrintTestHeader(testName);

  Vector3 p0 = Vector3(1, 0, -1);
  Vector3 p1 = Vector3(1, 0, 1);
  Vector3 p2 = Vector3(-1, 0, 0);
  float u = 0.52f;
  float v = 0.52f;
  float w = -0.04f;

  TestBarycentricTriangle(p0, p1, p2, u, v, w);
}

}

  /** Point Vs Plane - 40 tests **/
namespace MTG
{
  // In front of plane
  void PointPlaneTest1(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest2(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest3(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest4(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest5(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest6(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest7(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest8(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest9(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest10(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest11(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest12(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest13(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest14(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest15(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest16(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest17(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest18(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest19(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest20(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest21(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 2.41421f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest22(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.70711f, 1.70711f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest23(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-0.414214f, 1, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest24(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.292893f, 0.292893f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest25(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.292893f, 1.70711f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest26(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 1, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest27(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.46967f, 1.88388f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest28(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.116116f, 1.53033f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest29(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(2.41421f, 1, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest30(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.70711f, 1.70711f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest31(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, -0.414214f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest32(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.292893f, 0.292893f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest33(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.70711f, 0.292893f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest34(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 1, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest35(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.88388f, 0.46967f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest36(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.53033f, 0.116116f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest37(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.17678f, 1.17678f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest38(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.823223f, 0.823223f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest39(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.17678f, 1.17678f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest40(const std::string& testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.823223f, 0.823223f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }
}

  /** Ray Vs Plane : 72 tests **/
namespace MTG
{
  // Ray in front of plane pointing forward
  void RayPlaneTest1(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 1, 0), Vector3(1, 0, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest2(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-1, 1, 0), Vector3(1, 0, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest3(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 1, 0), Vector3(-1, -0, -0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest4(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-1, 1, 0), Vector3(-1, -0, -0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest5(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 1, 0), Vector3(-0, 1, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest6(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-1, 1, 0), Vector3(-0, 1, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest7(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 1, 0), Vector3(0, -0, 1));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest8(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-1, 1, 0), Vector3(0, -0, 1));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest9(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(0, 1, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest10(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -1, -1), Vector3(0, 1, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest11(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(-0, -1, -0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest12(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -1, -1), Vector3(-0, -1, -0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest13(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(0, 0, -1));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest14(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -1, -1), Vector3(0, 0, -1));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest15(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest16(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -1, -1), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest17(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 1), Vector3(0, 0, 1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest18(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(0, 0, 1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest19(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 1), Vector3(-0, -0, -1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest20(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(-0, -0, -1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest21(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 1), Vector3(0, 1, -0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest22(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(0, 1, -0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest23(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 1), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest24(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -1), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest25(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3, 1, 0), Vector3(1, 0, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest26(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3, 1, 0), Vector3(1, 0, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest27(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3, 1, 0), Vector3(-1, -0, -0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest28(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3, 1, 0), Vector3(-1, -0, -0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest29(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3, 1, 0), Vector3(-0, 1, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest30(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3, 1, 0), Vector3(-0, 1, 0));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest31(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3, 1, 0), Vector3(0, -0, 1));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest32(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3, 1, 0), Vector3(0, -0, 1));
    Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest33(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 3, -1), Vector3(0, 1, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest34(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -3, -1), Vector3(0, 1, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest35(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 3, -1), Vector3(-0, -1, -0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest36(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -3, -1), Vector3(-0, -1, -0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest37(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 3, -1), Vector3(0, 0, -1));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest38(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -3, -1), Vector3(0, 0, -1));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest39(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 3, -1), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest40(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -3, -1), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest41(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 3), Vector3(0, 0, 1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest42(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -3), Vector3(0, 0, 1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest43(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 3), Vector3(-0, -0, -1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest44(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -3), Vector3(-0, -0, -1));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest45(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 3), Vector3(0, 1, -0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest46(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -3), Vector3(0, 1, -0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest47(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, 3), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest48(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 1, -3), Vector3(-1, 0, 0));
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest49(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.29289f, 3.70711f, 3), Vector3(1, 1, 1));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest50(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3.70711f, -2.29289f, -3), Vector3(1, 1, 1));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest51(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.29289f, 3.70711f, 3), Vector3(-1, -1, -1));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest52(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3.70711f, -2.29289f, -3), Vector3(-1, -1, -1));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest53(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.29289f, 3.70711f, 3), Vector3(-0.707107f, 0.707107f, 0));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest54(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3.70711f, -2.29289f, -3), Vector3(-0.707107f, 0.707107f, 0));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest55(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.29289f, 3.70711f, 3), Vector3(-0.408248f, -0.408248f, 0.816497f));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest56(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3.70711f, -2.29289f, -3), Vector3(-0.408248f, -0.408248f, 0.816497f));
    Plane plane(Vector3(1, 1, 1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest57(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -3.70711f, -3), Vector3(-1, -1, -1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest58(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 2.29289f, 3), Vector3(-1, -1, -1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest59(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -3.70711f, -3), Vector3(1, 1, 1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest60(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 2.29289f, 3), Vector3(1, 1, 1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest61(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -3.70711f, -3), Vector3(0.707107f, -0.707107f, 0));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest62(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 2.29289f, 3), Vector3(0.707107f, -0.707107f, 0));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest63(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -3.70711f, -3), Vector3(-0.408248f, -0.408248f, 0.816497f));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest64(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 2.29289f, 3), Vector3(-0.408248f, -0.408248f, 0.816497f));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 0, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing forward
  void RayPlaneTest65(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -2.70711f, -3), Vector3(-1, -1, -1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing forward
  void RayPlaneTest66(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 3.29289f, 3), Vector3(-1, -1, -1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }

  // Ray in front of plane pointing backwards
  void RayPlaneTest67(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -2.70711f, -3), Vector3(1, 1, 1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }

  // Ray behind plane pointing backwards
  void RayPlaneTest68(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 3.29289f, 3), Vector3(1, 1, 1));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest69(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -2.70711f, -3), Vector3(0.707107f, -0.707107f, 0));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest70(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 3.29289f, 3), Vector3(0.707107f, -0.707107f, 0));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest71(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2.29289f, -2.70711f, -3), Vector3(-0.408248f, -0.408248f, 0.816497f));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }

  // Ray parallel to plane
  void RayPlaneTest72(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.70711f, 3.29289f, 3), Vector3(-0.408248f, -0.408248f, 0.816497f));
    Plane plane(Vector3(-1, -1, -1), Vector3(0, 1, 0));

    TestRayPlane(ray, plane);
  }
}

  /** Ray Vs AABB : 91 tests **/
namespace MTG
{
  // Ray behind aabb pointing backwards
  void RayAabbTest1(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 0, 0), Vector3(-1, -0, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest2(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 0, 0), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray at aabb center
  void RayAabbTest3(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 0), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest4(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 0, 0), Vector3(-1, -0, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest5(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 0, 0), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest6(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 0, 0), Vector3(-1, -0, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest7(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 0, 0), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest8(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, 0, 0), Vector3(-1, -0, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest9(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, 0, 0), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest10(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 2, 0), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest11(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, 0), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest12(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 0, 2), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest13(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 0, -2), Vector3(1, 0, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest14(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -2, 0), Vector3(-0, -1, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest15(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -2, 0), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray at aabb center
  void RayAabbTest16(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 0), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest17(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 2, 0), Vector3(-0, -1, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest18(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 2, 0), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest19(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 4, 0), Vector3(-0, -1, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest20(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 4, 0), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest21(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -4, 0), Vector3(-0, -1, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest22(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -4, 0), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest23(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -2, -2), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest24(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -2, 2), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest25(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, 0), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest26(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, -2, 0), Vector3(0, 1, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest27(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, -2), Vector3(-0, -0, -1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest28(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, -2), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray at aabb center
  void RayAabbTest29(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 0), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest30(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 2), Vector3(-0, -0, -1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest31(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 2), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest32(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 4), Vector3(-0, -0, -1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest33(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 4), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest34(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, -4), Vector3(-0, -0, -1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest35(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, -4), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest36(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 2, -2), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest37(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -2, -2), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest38(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 0, -2), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest39(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 0, -2), Vector3(0, 0, 1));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest40(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, 0), Vector3(-0.707107f, -0.707107f, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest41(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, 0), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray at aabb center
  void RayAabbTest42(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 0), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest43(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 2, 0), Vector3(-0.707107f, -0.707107f, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest44(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 2, 0), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest45(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 4, 0), Vector3(-0.707107f, -0.707107f, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest46(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 4, 0), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest47(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, -4, 0), Vector3(-0.707107f, -0.707107f, -0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest48(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, -4, 0), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest49(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, 3.57628e-07f, 0), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest50(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.57628e-07f, -4, 0), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest51(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, 2), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest52(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, -2), Vector3(0.707107f, 0.707107f, 0));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest53(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 0, -2), Vector3(-0.707107f, -0, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest54(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 0, -2), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray at aabb center
  void RayAabbTest55(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 0), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest56(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 0, 2), Vector3(-0.707107f, -0, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest57(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 0, 2), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest58(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 0, 4), Vector3(-0.707107f, -0, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest59(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 0, 4), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest60(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, 0, -4), Vector3(-0.707107f, -0, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest61(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, 0, -4), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest62(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, 2, -2), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest63(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, -2), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest64(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, 0, 3.57628e-07f), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest65(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(3.57628e-07f, 0, -4), Vector3(0.707107f, 0, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest66(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -2, -2), Vector3(-0, -0.707107f, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest67(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -2, -2), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray at aabb center
  void RayAabbTest68(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 0), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest69(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 2, 2), Vector3(-0, -0.707107f, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest70(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 2, 2), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest71(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 4, 4), Vector3(-0, -0.707107f, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest72(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 4, 4), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest73(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -4, -4), Vector3(-0, -0.707107f, -0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest74(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -4, -4), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest75(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 3.57628e-07f, -4), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest76(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -4, 3.57628e-07f), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest77(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, -2), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest78(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, -2, -2), Vector3(0, 0.707107f, 0.707107f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest79(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, -2), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest80(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-2, -2, -2), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray at aabb center
  void RayAabbTest81(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, 0, 0), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest82(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 2, 2), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest83(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2, 2, 2), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing backwards
  void RayAabbTest84(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 4, 4), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray in front of aabb pointing forwards
  void RayAabbTest85(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(4, 4, 4), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing backwards
  void RayAabbTest86(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, -4, -4), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  // Ray behind aabb pointing forwards
  void RayAabbTest87(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, -4, -4), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest88(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4, 0, -2), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest89(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(0, -4, -2), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest90(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3.33333f, -3.33333f, 0.666667f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }

  void RayAabbTest91(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-0.666667f, -0.666667f, -4.66667f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Aabb aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

    TestRayAabb(ray, aabb);
  }
}

  /** Ray Vs Sphere : 85 tests **/
namespace MTG
{

  // Ray behind sphere pointing backwards
  void RaySphereTest1(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, -1, 0), Vector3(-1, -0, -0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing forwards
  void RaySphereTest2(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, -1, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing but not going through the sphere center
  void RaySphereTest3(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, -0.333333f, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray at sphere center
  void RaySphereTest4(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing backwards
  void RaySphereTest5(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(11, -1, 0), Vector3(-1, -0, -0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing forwards
  void RaySphereTest6(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(11, -1, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far behind sphere pointing forwards
  void RaySphereTest7(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, -1, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far in front of sphere pointing backwards
  void RaySphereTest8(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(11, -1, 0), Vector3(-1, -0, -0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest9(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, 0.98f, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest10(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, -2.98f, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest11(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, -1, 1.98f), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest12(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-9, -1, -1.98f), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest13(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-29, -1, 1.98f), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest14(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(31, -1, 1.98f), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest15(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-29, -1, -1.98f), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest16(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(31, -1, -1.98f), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray misses sphere
  void RaySphereTest17(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, -4), Vector3(1, 0, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing backwards
  void RaySphereTest18(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -11, 0), Vector3(-0, -1, -0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing forwards
  void RaySphereTest19(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -11, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing but not going through the sphere center
  void RaySphereTest20(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -11, -0.666667f), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray at sphere center
  void RaySphereTest21(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing backwards
  void RaySphereTest22(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 9, 0), Vector3(-0, -1, -0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing forwards
  void RaySphereTest23(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 9, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far behind sphere pointing forwards
  void RaySphereTest24(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -11, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far in front of sphere pointing backwards
  void RaySphereTest25(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 9, 0), Vector3(-0, -1, -0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest26(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -11, -1.98f), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest27(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -11, 1.98f), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest28(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-0.98f, -11, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest29(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.98f, -11, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest30(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-0.98f, -31, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest31(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-0.98f, 29, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest32(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.98f, -31, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest33(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.98f, 29, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray misses sphere
  void RaySphereTest34(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(5, -1, 0), Vector3(0, 1, 0));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing backwards
  void RaySphereTest35(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, -10), Vector3(-0, -0, -1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing forwards
  void RaySphereTest36(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, -10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing but not going through the sphere center
  void RaySphereTest37(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -0.333333f, -10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray at sphere center
  void RaySphereTest38(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 0), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing backwards
  void RaySphereTest39(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 10), Vector3(-0, -0, -1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing forwards
  void RaySphereTest40(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far behind sphere pointing forwards
  void RaySphereTest41(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, -10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far in front of sphere pointing backwards
  void RaySphereTest42(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 10), Vector3(-0, -0, -1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest43(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, 0.98f, -10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest44(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -2.98f, -10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest45(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-0.98f, -1, -10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest46(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.98f, -1, -10), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest47(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-0.98f, -1, -30), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest48(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-0.98f, -1, 30), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest49(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.98f, -1, -30), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest50(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.98f, -1, 30), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray misses sphere
  void RaySphereTest51(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(5, -1, 0), Vector3(0, 0, 1));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing backwards
  void RaySphereTest52(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4.7735f, -6.7735f, -5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing forwards
  void RaySphereTest53(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4.7735f, -6.7735f, -5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing but not going through the sphere center
  void RaySphereTest54(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-5.24491f, -6.3021f, -5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray at sphere center
  void RaySphereTest55(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 0), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing backwards
  void RaySphereTest56(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(6.7735f, 4.7735f, 5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing forwards
  void RaySphereTest57(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(6.7735f, 4.7735f, 5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far behind sphere pointing forwards
  void RaySphereTest58(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4.7735f, -6.7735f, -5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far in front of sphere pointing backwards
  void RaySphereTest59(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(6.7735f, 4.7735f, 5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest60(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-6.17357f, -5.37343f, -5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest61(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3.37343f, -8.17357f, -5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest62(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-5.58183f, -7.58183f, -4.15684f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest63(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-3.96517f, -5.96517f, -7.39017f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest64(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-17.1288f, -19.1288f, -15.7038f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest65(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(17.5122f, 15.5122f, 18.9372f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest66(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-15.5122f, -17.5122f, -18.9372f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest67(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(19.1288f, 17.1288f, 15.7038f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray misses sphere
  void RaySphereTest68(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.63299f, 0.632993f, -3.26599f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing backwards
  void RaySphereTest69(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(6.7735f, 4.7735f, 5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing forwards
  void RaySphereTest70(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(6.7735f, 4.7735f, 5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray behind sphere pointing but not going through the sphere center
  void RaySphereTest71(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(7.24491f, 4.3021f, 5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray at sphere center
  void RaySphereTest72(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(1, -1, 0), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing backwards
  void RaySphereTest73(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4.7735f, -6.7735f, -5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray in front of sphere pointing forwards
  void RaySphereTest74(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4.7735f, -6.7735f, -5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far behind sphere pointing forwards
  void RaySphereTest75(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(6.7735f, 4.7735f, 5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray far in front of sphere pointing backwards
  void RaySphereTest76(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-4.7735f, -6.7735f, -5.7735f), Vector3(0.57735f, 0.57735f, 0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest77(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(8.17357f, 3.37343f, 5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest78(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(5.37343f, 6.17357f, 5.7735f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest79(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(5.96517f, 3.96517f, 7.39017f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest80(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(7.58183f, 5.58183f, 4.15684f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest81(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(17.5122f, 15.5122f, 18.9372f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest82(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-17.1288f, -19.1288f, -15.7038f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest83(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(19.1288f, 17.1288f, 15.7038f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray Tangent to sphere
  void RaySphereTest84(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(-15.5122f, -17.5122f, -18.9372f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }

  // Ray misses sphere
  void RaySphereTest85(const std::string& testName)
  {
    PrintTestHeader(testName);

    Ray ray(Vector3(2.63299f, 0.632993f, -3.26599f), Vector3(-0.57735f, -0.57735f, -0.57735f));
    Sphere sphere(Vector3(1, -1, 0), 2);

    TestRaySphere(ray, sphere);
  }
}

/** Ray Vs Triangle : 30 tests **/
namespace MTG
{
// Ray behind triangle pointing forwards
void RayTriangleTest1(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, -1, 0), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind triangle pointing backwards
void RayTriangleTest2(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, -1, 0), Vector3(-0, -1, -0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray in front of triangle pointing backwards
void RayTriangleTest3(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, 1, 0), Vector3(-0, -1, -0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray in front of triangle pointing forwards
void RayTriangleTest4(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, 1, 0), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind point 1
void RayTriangleTest5(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1.1f, -1, -1.1f), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind point 2
void RayTriangleTest6(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1.1f, -1, 1.1f), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind point 3
void RayTriangleTest7(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1.1f, -1, 0), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray outside edge12
void RayTriangleTest8(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1.1f, -1, 0), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray outside edge23
void RayTriangleTest9(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-0.22f, -1, 0.44f), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray outside edge31
void RayTriangleTest10(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-0.22f, -1, -0.44f), Vector3(0, 1, 0));
  Triangle tri(Vector3(1, 0, -1), Vector3(1, 0, 1), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind triangle pointing forwards
void RayTriangleTest11(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, 0, 0), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray behind triangle pointing backwards
void RayTriangleTest12(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, 0, 0), Vector3(-1, -0, -0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray in front of triangle pointing backwards
void RayTriangleTest13(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1, 0, 0), Vector3(-1, -0, -0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray in front of triangle pointing forwards
void RayTriangleTest14(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1, 0, 0), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray behind point 1
void RayTriangleTest15(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, 1.1f, -1.1f), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray behind point 2
void RayTriangleTest16(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, -1.1f, -1.1f), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray behind point 3
void RayTriangleTest17(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, 0, 1.1f), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray outside edge12
void RayTriangleTest18(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, 0, -1.1f), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray outside edge23
void RayTriangleTest19(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, -0.44f, 0.22f), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray outside edge31
void RayTriangleTest20(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1, 0.44f, 0.22f), Vector3(1, 0, 0));
  Triangle tri(Vector3(0, 1, -1), Vector3(0, -1, -1), Vector3(0, 0, 1));

  TestRayTriangle(ray, tri);
}

// Ray behind triangle pointing forwards
void RayTriangleTest21(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, 0, -1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind triangle pointing backwards
void RayTriangleTest22(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, 0, -1), Vector3(-0, -0, -1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray in front of triangle pointing backwards
void RayTriangleTest23(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, 0, 1), Vector3(-0, -0, -1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray in front of triangle pointing forwards
void RayTriangleTest24(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(0, 0, 1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind point 1
void RayTriangleTest25(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1.1f, 1.1f, -1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind point 2
void RayTriangleTest26(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1.1f, -1.1f, -1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray behind point 3
void RayTriangleTest27(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-1.1f, 0, -1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray outside edge12
void RayTriangleTest28(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(1.1f, 0, -1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray outside edge23
void RayTriangleTest29(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-0.22f, -0.44f, -1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}

// Ray outside edge31
void RayTriangleTest30(const std::string& testName)
{
  PrintTestHeader(testName);

  Ray ray(Vector3(-0.22f, 0.44f, -1), Vector3(0, 0, 1));
  Triangle tri(Vector3(1, 1, 0), Vector3(1, -1, 0), Vector3(-1, 0, 0));

  TestRayTriangle(ray, tri);
}
}

/** Plane Vs AABB : 50 tests **/
namespace MTG
{
// Aabb in front of plane
void PlaneAabbTest1(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(0.5f, -0.5f, -0.5f), Vector3(1.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely in front of plane
void PlaneAabbTest2(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(0.05f, -0.5f, -0.5f), Vector3(1.05f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb on front)
void PlaneAabbTest3(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.05f, -0.5f, -0.5f), Vector3(0.95f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb slightly touching plane (aabb on front)
void PlaneAabbTest4(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.125f, -0.5f, -0.5f), Vector3(0.875f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb centered on plane
void PlaneAabbTest5(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb signtly touching plane (aabb in back)
void PlaneAabbTest6(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.875f, -0.5f, -0.5f), Vector3(0.125f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb in back)
void PlaneAabbTest7(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.95f, -0.5f, -0.5f), Vector3(0.05f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely behind plane
void PlaneAabbTest8(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-1.05f, -0.5f, -0.5f), Vector3(-0.05f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb behind plane
void PlaneAabbTest9(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-1.5f, -0.5f, -0.5f), Vector3(-0.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb in front of plane
void PlaneAabbTest10(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, 0.5f, -0.5f), Vector3(0.5f, 1.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely in front of plane
void PlaneAabbTest11(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, 0.05f, -0.5f), Vector3(0.5f, 1.05f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb on front)
void PlaneAabbTest12(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.05f, -0.5f), Vector3(0.5f, 0.95f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb slightly touching plane (aabb on front)
void PlaneAabbTest13(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.125f, -0.5f), Vector3(0.5f, 0.875f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb centered on plane
void PlaneAabbTest14(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb signtly touching plane (aabb in back)
void PlaneAabbTest15(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.875f, -0.5f), Vector3(0.5f, 0.125f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb in back)
void PlaneAabbTest16(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.95f, -0.5f), Vector3(0.5f, 0.05f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely behind plane
void PlaneAabbTest17(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -1.05f, -0.5f), Vector3(0.5f, -0.05f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb behind plane
void PlaneAabbTest18(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -1.5f, -0.5f), Vector3(0.5f, -0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb in front of plane
void PlaneAabbTest19(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.5f, 0.5f, 1.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely in front of plane
void PlaneAabbTest20(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, 0.05f), Vector3(0.5f, 0.5f, 1.05f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb on front)
void PlaneAabbTest21(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.05f), Vector3(0.5f, 0.5f, 0.95f));

  TestPlaneAabb(plane, aabb);
}

// Aabb slightly touching plane (aabb on front)
void PlaneAabbTest22(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.125f), Vector3(0.5f, 0.5f, 0.875f));

  TestPlaneAabb(plane, aabb);
}

// Aabb centered on plane
void PlaneAabbTest23(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb signtly touching plane (aabb in back)
void PlaneAabbTest24(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.875f), Vector3(0.5f, 0.5f, 0.125f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb in back)
void PlaneAabbTest25(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.95f), Vector3(0.5f, 0.5f, 0.05f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely behind plane
void PlaneAabbTest26(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -1.05f), Vector3(0.5f, 0.5f, -0.05f));

  TestPlaneAabb(plane, aabb);
}

// Aabb behind plane
void PlaneAabbTest27(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -1.5f), Vector3(0.5f, 0.5f, -0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb in front of plane
void PlaneAabbTest28(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(0.5f, 0.5f, -0.5f), Vector3(1.5f, 1.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely in front of plane
void PlaneAabbTest29(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(0.05f, 0.05f, -0.5f), Vector3(1.05f, 1.05f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb on front)
void PlaneAabbTest30(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.05f, -0.05f, -0.5f), Vector3(0.95f, 0.95f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb slightly touching plane (aabb on front)
void PlaneAabbTest31(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.125f, -0.125f, -0.5f), Vector3(0.875f, 0.875f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb centered on plane
void PlaneAabbTest32(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb signtly touching plane (aabb in back)
void PlaneAabbTest33(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.875f, -0.875f, -0.5f), Vector3(0.125f, 0.125f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb in back)
void PlaneAabbTest34(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.95f, -0.95f, -0.5f), Vector3(0.05f, 0.05f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely behind plane
void PlaneAabbTest35(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-1.05f, -1.05f, -0.5f), Vector3(-0.05f, -0.05f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb behind plane
void PlaneAabbTest36(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-1.5f, -1.5f, -0.5f), Vector3(-0.5f, -0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb in front of plane
void PlaneAabbTest37(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(0.5f, -0.5f, 0.5f), Vector3(1.5f, 0.5f, 1.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely in front of plane
void PlaneAabbTest38(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(0.05f, -0.5f, 0.05f), Vector3(1.05f, 0.5f, 1.05f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb on front)
void PlaneAabbTest39(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.05f, -0.5f, -0.05f), Vector3(0.95f, 0.5f, 0.95f));

  TestPlaneAabb(plane, aabb);
}

// Aabb slightly touching plane (aabb on front)
void PlaneAabbTest40(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.125f, -0.5f, -0.125f), Vector3(0.875f, 0.5f, 0.875f));

  TestPlaneAabb(plane, aabb);
}

// Aabb centered on plane
void PlaneAabbTest41(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb signtly touching plane (aabb in back)
void PlaneAabbTest42(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.875f, -0.5f, -0.875f), Vector3(0.125f, 0.5f, 0.125f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb in back)
void PlaneAabbTest43(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.95f, -0.5f, -0.95f), Vector3(0.05f, 0.5f, 0.05f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely behind plane
void PlaneAabbTest44(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-1.05f, -0.5f, -1.05f), Vector3(-0.05f, 0.5f, -0.05f));

  TestPlaneAabb(plane, aabb);
}

// Aabb behind plane
void PlaneAabbTest45(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.707107f, 0, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-1.5f, -0.5f, -1.5f), Vector3(-0.5f, 0.5f, -0.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb in front of plane
void PlaneAabbTest46(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0.707107f, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.5f, 1.5f, 1.5f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely in front of plane
void PlaneAabbTest47(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0.707107f, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, 0.05f, 0.05f), Vector3(0.5f, 1.05f, 1.05f));

  TestPlaneAabb(plane, aabb);
}

// Aabb just barely touching plane (aabb on front)
void PlaneAabbTest48(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0.707107f, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.05f, -0.05f), Vector3(0.5f, 0.95f, 0.95f));

  TestPlaneAabb(plane, aabb);
}

// Aabb slightly touching plane (aabb on front)
void PlaneAabbTest49(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0.707107f, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.125f, -0.125f), Vector3(0.5f, 0.875f, 0.875f));

  TestPlaneAabb(plane, aabb);
}

// Aabb centered on plane
void PlaneAabbTest50(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0.707107f, 0.707107f), Vector3(0, 0, 0));
  Aabb aabb(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));

  TestPlaneAabb(plane, aabb);
}
}

/** Plane Vs Sphere : 20 tests **/
namespace MTG
{
// Sphere in front of plane
void PlaneSphereTest1(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(2, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the front)
void PlaneSphereTest2(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(1, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere centered at plane
void PlaneSphereTest3(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the back)
void PlaneSphereTest4(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(-1, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere behind plane
void PlaneSphereTest5(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(1, 0, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(-2, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere in front of plane
void PlaneSphereTest6(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 2, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the front)
void PlaneSphereTest7(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 1, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere centered at plane
void PlaneSphereTest8(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the back)
void PlaneSphereTest9(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, -1, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere behind plane
void PlaneSphereTest10(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, -2, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere in front of plane
void PlaneSphereTest11(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, 2), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the front)
void PlaneSphereTest12(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, 1), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere centered at plane
void PlaneSphereTest13(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the back)
void PlaneSphereTest14(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, -1), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere behind plane
void PlaneSphereTest15(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, -2), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere in front of plane
void PlaneSphereTest16(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.57735f, 0.57735f, 0.57735f), Vector3(0, 0, 0));
  Sphere sphere(Vector3(1.1547f, 1.1547f, 1.1547f), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the front)
void PlaneSphereTest17(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.57735f, 0.57735f, 0.57735f), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0.57735f, 0.57735f, 0.57735f), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere centered at plane
void PlaneSphereTest18(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.57735f, 0.57735f, 0.57735f), Vector3(0, 0, 0));
  Sphere sphere(Vector3(0, 0, 0), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere just touching plane (on the back)
void PlaneSphereTest19(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.57735f, 0.57735f, 0.57735f), Vector3(0, 0, 0));
  Sphere sphere(Vector3(-0.57735f, -0.57735f, -0.57735f), 1);

  TestPlaneSphere(plane, sphere);
}

// Sphere behind plane
void PlaneSphereTest20(const std::string& testName)
{
  PrintTestHeader(testName);

  Plane plane(Vector3(0.57735f, 0.57735f, 0.57735f), Vector3(0, 0, 0));
  Sphere sphere(Vector3(-1.1547f, -1.1547f, -1.1547f), 1);

  TestPlaneSphere(plane, sphere);
}
}

// test functions which calls the rest
namespace MTG
{
#if TESTEXAMPLE_SPHERE_SPHERE
  static void TestExampleSphereSphere()
  {
#define TESTYMACRO(a) SphereSphereTest ##a ("SphereSphereTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_AABB_SPHERE

#endif
#if TESTEXAMPLE_SPHERE_AABB

#endif
#if TESTEXAMPLE_AABB_AABB
  static void TestExampleAABBAABB()
  {
#define TESTYMACRO(a) AabbAabbTest ##a ("AabbAabbTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
    TESTYMACRO(41);
    TESTYMACRO(42);
    TESTYMACRO(43);
    TESTYMACRO(44);
    TESTYMACRO(45);
    TESTYMACRO(46);
    TESTYMACRO(47);
    TESTYMACRO(48);
    TESTYMACRO(49);
    TESTYMACRO(50);
    TESTYMACRO(51);
    TESTYMACRO(52);
    TESTYMACRO(53);
    TESTYMACRO(54);
    TESTYMACRO(55);
    TESTYMACRO(56);
    TESTYMACRO(57);
    TESTYMACRO(58);
    TESTYMACRO(59);
    TESTYMACRO(60);
    TESTYMACRO(61);
    TESTYMACRO(62);
    TESTYMACRO(63);
    TESTYMACRO(64);
    TESTYMACRO(65);
    TESTYMACRO(66);
    TESTYMACRO(67);
    TESTYMACRO(68);
    TESTYMACRO(69);
    TESTYMACRO(70);
    TESTYMACRO(71);
    TESTYMACRO(72);
    TESTYMACRO(73);
    TESTYMACRO(74);
    TESTYMACRO(75);
    TESTYMACRO(76);
    TESTYMACRO(77);
    TESTYMACRO(78);
    TESTYMACRO(79);
    TESTYMACRO(80);
    TESTYMACRO(81);
    TESTYMACRO(82);
    TESTYMACRO(83);
    TESTYMACRO(84);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_POINT_SPHERE
  static void TestExamplePointSphere()
  {
#define TESTYMACRO(a) PointSphereTest ##a ("PointSphereTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_POINT_AABB
  static void TestExamplePointAABB()
  {
#define TESTYMACRO(a) PointAabbTest ##a ("PointAabbTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
    TESTYMACRO(41);
    TESTYMACRO(42);
    TESTYMACRO(43);
    TESTYMACRO(44);
    TESTYMACRO(45);
    TESTYMACRO(46);
    TESTYMACRO(47);
    TESTYMACRO(48);
    TESTYMACRO(49);
    TESTYMACRO(50);
    TESTYMACRO(51);
    TESTYMACRO(52);
    TESTYMACRO(53);
    TESTYMACRO(54);
    TESTYMACRO(55);
    TESTYMACRO(56);
    TESTYMACRO(57);
    TESTYMACRO(58);
    TESTYMACRO(59);
    TESTYMACRO(60);
    TESTYMACRO(61);
    TESTYMACRO(62);
    TESTYMACRO(63);
    TESTYMACRO(64);
    TESTYMACRO(65);
    TESTYMACRO(66);
    TESTYMACRO(67);
    TESTYMACRO(68);
    TESTYMACRO(69);
    TESTYMACRO(70);
    TESTYMACRO(71);
    TESTYMACRO(72);
    TESTYMACRO(73);
    TESTYMACRO(74);
    TESTYMACRO(75);
    TESTYMACRO(76);
    TESTYMACRO(77);
    TESTYMACRO(78);
    TESTYMACRO(79);
    TESTYMACRO(80);
    TESTYMACRO(81);
    TESTYMACRO(82);
    TESTYMACRO(83);
    TESTYMACRO(84);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_POINT_TRIANGLE
  static void TestExamplePointTriangle()
  {
#define TESTYMACRO(a) BarycentricTriangleTest ##a ("BarycentricTriangleTest"#a)
    TESTYMACRO(0);
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
    TESTYMACRO(41);
    TESTYMACRO(42);
    TESTYMACRO(43);
    TESTYMACRO(44);
    TESTYMACRO(45);
    TESTYMACRO(46);
    TESTYMACRO(47);
    TESTYMACRO(48);
    TESTYMACRO(49);
    TESTYMACRO(50);
    TESTYMACRO(51);
    TESTYMACRO(52);
    TESTYMACRO(53);
    TESTYMACRO(54);
    TESTYMACRO(55);
    TESTYMACRO(56);
    TESTYMACRO(57);
    TESTYMACRO(58);
    TESTYMACRO(59);
    TESTYMACRO(60);
    TESTYMACRO(61);
    TESTYMACRO(62);
    TESTYMACRO(63);
    TESTYMACRO(64);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_POINT_PLANE
  static void TestExamplePointPlane()
  {
#define TESTYMACRO(a) PointPlaneTest ##a ("PointPlaneTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_RAY_PLANE
  static void TestExampleRayPlane()
  {
#define TESTYMACRO(a) RayPlaneTest ##a ("RayPlaneTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
    TESTYMACRO(41);
    TESTYMACRO(42);
    TESTYMACRO(43);
    TESTYMACRO(44);
    TESTYMACRO(45);
    TESTYMACRO(46);
    TESTYMACRO(47);
    TESTYMACRO(48);
    TESTYMACRO(49);
    TESTYMACRO(50);
    TESTYMACRO(51);
    TESTYMACRO(52);
    TESTYMACRO(53);
    TESTYMACRO(54);
    TESTYMACRO(55);
    TESTYMACRO(56);
    TESTYMACRO(57);
    TESTYMACRO(58);
    TESTYMACRO(59);
    TESTYMACRO(60);
    TESTYMACRO(61);
    TESTYMACRO(62);
    TESTYMACRO(63);
    TESTYMACRO(64);
    TESTYMACRO(65);
    TESTYMACRO(66);
    TESTYMACRO(67);
    TESTYMACRO(68);
    TESTYMACRO(69);
    TESTYMACRO(70);
    TESTYMACRO(71);
    TESTYMACRO(72);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_RAY_AABB
  static void TestExampleRayAABB()
  {
#define TESTYMACRO(a) RayAabbTest ##a ("RayAabbTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
    TESTYMACRO(41);
    TESTYMACRO(42);
    TESTYMACRO(43);
    TESTYMACRO(44);
    TESTYMACRO(45);
    TESTYMACRO(46);
    TESTYMACRO(47);
    TESTYMACRO(48);
    TESTYMACRO(49);
    TESTYMACRO(50);
    TESTYMACRO(51);
    TESTYMACRO(52);
    TESTYMACRO(53);
    TESTYMACRO(54);
    TESTYMACRO(55);
    TESTYMACRO(56);
    TESTYMACRO(57);
    TESTYMACRO(58);
    TESTYMACRO(59);
    TESTYMACRO(60);
    TESTYMACRO(61);
    TESTYMACRO(62);
    TESTYMACRO(63);
    TESTYMACRO(64);
    TESTYMACRO(65);
    TESTYMACRO(66);
    TESTYMACRO(67);
    TESTYMACRO(68);
    TESTYMACRO(69);
    TESTYMACRO(70);
    TESTYMACRO(71);
    TESTYMACRO(72);
    TESTYMACRO(73);
    TESTYMACRO(74);
    TESTYMACRO(75);
    TESTYMACRO(76);
    TESTYMACRO(77);
    TESTYMACRO(78);
    TESTYMACRO(79);
    TESTYMACRO(80);
    TESTYMACRO(81);
    TESTYMACRO(82);
    TESTYMACRO(83);
    TESTYMACRO(84);
    TESTYMACRO(85);
    TESTYMACRO(86);
    TESTYMACRO(87);
    TESTYMACRO(88);
    TESTYMACRO(89);
    TESTYMACRO(90);
    TESTYMACRO(91);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_RAY_SPHERE
  static void TestExampleRaySphere()
  {
#define TESTYMACRO(a) RaySphereTest ##a ("RaySphereTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
    TESTYMACRO(41);
    TESTYMACRO(42);
    TESTYMACRO(43);
    TESTYMACRO(44);
    TESTYMACRO(45);
    TESTYMACRO(46);
    TESTYMACRO(47);
    TESTYMACRO(48);
    TESTYMACRO(49);
    TESTYMACRO(50);
    TESTYMACRO(51);
    TESTYMACRO(52);
    TESTYMACRO(53);
    TESTYMACRO(54);
    TESTYMACRO(55);
    TESTYMACRO(56);
    TESTYMACRO(57);
    TESTYMACRO(58);
    TESTYMACRO(59);
    TESTYMACRO(60);
    TESTYMACRO(61);
    TESTYMACRO(62);
    TESTYMACRO(63);
    TESTYMACRO(64);
    TESTYMACRO(65);
    TESTYMACRO(66);
    TESTYMACRO(67);
    TESTYMACRO(68);
    TESTYMACRO(69);
    TESTYMACRO(70);
    TESTYMACRO(71);
    TESTYMACRO(72);
    TESTYMACRO(73);
    TESTYMACRO(74);
    TESTYMACRO(75);
    TESTYMACRO(76);
    TESTYMACRO(77);
    TESTYMACRO(78);
    TESTYMACRO(79);
    TESTYMACRO(80);
    TESTYMACRO(81);
    TESTYMACRO(82);
    TESTYMACRO(83);
    TESTYMACRO(84);
    TESTYMACRO(85);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_RAY_TRIANGLE
  static void TestExampleRayTriangle()
  {
#define TESTYMACRO(a) RayTriangleTest ##a ("RayTriangleTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_PLANE_AABB
  static void TestExamplePlaneAABB()
  {
#define TESTYMACRO(a) PlaneAabbTest ##a ("PlaneAabbTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
    TESTYMACRO(21);
    TESTYMACRO(22);
    TESTYMACRO(23);
    TESTYMACRO(24);
    TESTYMACRO(25);
    TESTYMACRO(26);
    TESTYMACRO(27);
    TESTYMACRO(28);
    TESTYMACRO(29);
    TESTYMACRO(30);
    TESTYMACRO(31);
    TESTYMACRO(32);
    TESTYMACRO(33);
    TESTYMACRO(34);
    TESTYMACRO(35);
    TESTYMACRO(36);
    TESTYMACRO(37);
    TESTYMACRO(38);
    TESTYMACRO(39);
    TESTYMACRO(40);
    TESTYMACRO(41);
    TESTYMACRO(42);
    TESTYMACRO(43);
    TESTYMACRO(44);
    TESTYMACRO(45);
    TESTYMACRO(46);
    TESTYMACRO(47);
    TESTYMACRO(48);
    TESTYMACRO(49);
    TESTYMACRO(50);
#undef TESTYMACRO
  }
#endif
#if TESTEXAMPLE_PLANE_SPHERE
  static void TestExamplePlaneSphere()
  {
#define TESTYMACRO(a) PlaneSphereTest ##a ("PlaneSphereTest"#a)
    TESTYMACRO(1);
    TESTYMACRO(2);
    TESTYMACRO(3);
    TESTYMACRO(4);
    TESTYMACRO(5);
    TESTYMACRO(6);
    TESTYMACRO(7);
    TESTYMACRO(8);
    TESTYMACRO(9);
    TESTYMACRO(10);
    TESTYMACRO(11);
    TESTYMACRO(12);
    TESTYMACRO(13);
    TESTYMACRO(14);
    TESTYMACRO(15);
    TESTYMACRO(16);
    TESTYMACRO(17);
    TESTYMACRO(18);
    TESTYMACRO(19);
    TESTYMACRO(20);
#undef TESTYMACRO
  }
#endif

  void test()
  {
#if TESTEXAMPLE_SPHERE_SPHERE
    TestExampleSphereSphere();
#endif
#if TESTEXAMPLE_AABB_SPHERE

#endif
#if TESTEXAMPLE_SPHERE_AABB

#endif
#if TESTEXAMPLE_AABB_AABB
    TestExampleAABBAABB();
#endif
#if TESTEXAMPLE_POINT_SPHERE
    TestExamplePointSphere();
#endif
#if TESTEXAMPLE_POINT_AABB
    TestExamplePointAABB();
#endif
#if TESTEXAMPLE_POINT_TRIANGLE
    TestExamplePointTriangle();
#endif
#if TESTEXAMPLE_POINT_PLANE
    TestExamplePointPlane();
#endif
#if TESTEXAMPLE_RAY_PLANE
    TestExampleRayPlane();
#endif
#if TESTEXAMPLE_RAY_AABB
    TestExampleRayAABB();
#endif
#if TESTEXAMPLE_RAY_SPHERE
    TestExampleRaySphere();
#endif
#if TESTEXAMPLE_RAY_TRIANGLE
    TestExampleRayTriangle();
#endif
#if TESTEXAMPLE_PLANE_AABB
    TestExamplePlaneAABB();
#endif
#if TESTEXAMPLE_PLANE_SPHERE
    TestExamplePlaneSphere();
#endif
  }

}
