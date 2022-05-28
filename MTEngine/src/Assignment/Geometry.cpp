/*!*****************************************************************************
 * @file    Geometry.cpp
 * @author  Owen Huang Wensong  (w.huang@digipen.edu)
 * @date    27 MAY 2022
 * @brief   Assignment 1: Geometry toolbox
 *
 * @par Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <Assignment/Geometry.h>

MTG::Plane::Plane(glm::vec3 inNormal, glm::vec3 point) : 
  m_Normal{ glm::normalize(inNormal) },
  m_Dist{ glm::dot(m_Normal, point) }
{

}

namespace localHelper
{
  static inline float DistanceSquared3D(MTG::Point3D const& p0, MTG::Point3D const& p1)
  { // no fancy stuff to lower likelihood of it not working on lab computers
    MTG::Vector3D tmp{ p1 - p0 };
    return tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
  }

  static inline float LengthSquared3D(MTG::Vector3D const& v0)
  {
    return v0.x * v0.x + v0.y * v0.y + v0.z * v0.z;
  }

  static inline float Squared(float toSquare) noexcept { return toSquare * toSquare; }
}

bool MTG::TestSphereSphere(Sphere const& s0, Sphere const& s1)
{
  return localHelper::DistanceSquared3D(s0.m_Center, s1.m_Center) <= localHelper::Squared(s0.m_Radius + s1.m_Radius);
}

// STUFF GO HERE

bool MTG::intersectionAABBAABB(AABB const& a0, AABB const& a1)
{
  Point3D a0Min{ a0.m_Center - a0.m_HalfExtents }, a0Max{ a0.m_Center + a0.m_HalfExtents };
  Point3D a1Min{ a1.m_Center - a1.m_HalfExtents }, a1Max{ a1.m_Center + a1.m_HalfExtents };
#define LAZYHELPER(a) (a0Max.##a < a1Max.##a || a1Max.x < a0Min.##a)
  return (LAZYHELPER(x) || LAZYHELPER(y) || LAZYHELPER(z)) ? false : true;
#undef LAZYHELPER
}

bool MTG::intersectionPointSphere(Point3D const& p0, Sphere const& s1)
{
  return localHelper::DistanceSquared3D(p0, s1.m_Center) <= localHelper::Squared(s1.m_Radius);
}

bool MTG::intersectionPointAABB(Point3D const& p0, AABB const& a1)
{
  Point3D a1Min{ a1.m_Center - a1.m_HalfExtents }, a1Max{ a1.m_Center + a1.m_HalfExtents };
#define LAZYHELPER(a) (p0.##a < a1Min.##a || p0.##a > a1Max.##a)
  return (LAZYHELPER(x) || LAZYHELPER(y) || LAZYHELPER(z)) ? false : true;
#undef LAZYHELPER
}

//bool MTG::TestPointTriangle(Point3D const& p0, Triangle const& t1)
//{
//  return false;
//}

int MTG::cmpPointPlane(Point3D const& p0, Plane const& p1, float inEpsilon)
{
  float tmp{ glm::dot(p0, p1.m_Normal) - p1.m_Dist };
  if (tmp > inEpsilon)return 1;
  if (tmp < -inEpsilon)return -1;
  return 0;
}

void MTG::TestPointPlane(Point3D const& p0, Plane const& p1, float inEpsilon)
{
  switch (cmpPointPlane(p0, p1, inEpsilon))
  {
  case -1:
    printf_s("  Result:%s\n", "Outside");
    break;
  case 1:
    printf_s("  Result:%s\n", "Outside");
    break;
  case 0:
    printf_s("  Result:%s\n", "Outside");
    break;
  }
}

bool MTG::intersectionRayPlane(Ray const& r0, Plane const& p1, float& outTime)
{
  int PointPlaneResult{ cmpPointPlane(r0.m_Point, p1) };
  if (PointPlaneResult == 0)
  {
    outTime = 0.0f;
    return true;    // ray start point is on the plane
  }
  float tmp{ glm::dot(r0.m_Direction, p1.m_Normal) };
  if (std::signbit(tmp) ? (PointPlaneResult < 0) : (PointPlaneResult > 0))
  { // if (oppositedirection ? outside half plane : inside half plane)
    return false;
  }
  // using FLT_EPSILON was too lenient, mostly parallel stuff still returned true
  if (tmp < 0.00000625f && tmp > -0.00000625f)
  { // parallel
    return false;
  }
  // control reach here means there is an intersection, but when.
  //outTime = glm::dot(p1.m_Dist * p1.m_Normal - r0.m_Point, p1.m_Normal) / tmp;
  outTime = (p1.m_Dist - glm::dot(r0.m_Point, p1.m_Normal)) / tmp;
  return true;
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

bool MTG::intersectionRayAABB(Ray const& r0, AABB const& a1, float& outTime)
{ // I should have just stored it as min max... I'm so tired rn :(
  Point3D B0{ a1.m_Center - a1.m_HalfExtents }, B1{ a1.m_Center + a1.m_HalfExtents };

  float reciprocal_x{ r0.m_Direction.x ? (1.0f / r0.m_Direction.x) : 0.0f };
  float reciprocal_y{ r0.m_Direction.y ? (1.0f / r0.m_Direction.y) : 0.0f };
  float reciprocal_z{ r0.m_Direction.z ? (1.0f / r0.m_Direction.z) : 0.0f };

  float tMin{ reciprocal_x ? (std::signbit(reciprocal_x) ? B1.x - r0.m_Point.x : B0.x - r0.m_Point.x) * reciprocal_x : FLT_MAX };
  float tMax{ reciprocal_x ? (std::signbit(reciprocal_x) ? B0.x - r0.m_Point.x : B1.x - r0.m_Point.x) * reciprocal_x : FLT_MIN };
  {
    float tyMin{ reciprocal_y ? (std::signbit(reciprocal_y) ? B1.y - r0.m_Point.y : B0.y - r0.m_Point.y) * reciprocal_y : tMin };
    float tyMax{ reciprocal_y ? (std::signbit(reciprocal_y) ? B0.y - r0.m_Point.y : B1.y - r0.m_Point.y) * reciprocal_y : tMax };
    if (tMin > tyMax || tyMin > tMax)return false;
    if (tyMin > tMin)tMin = tyMin;
    if (tyMax < tMax)tMax = tyMax;
  }
  {
    float tzMin{ reciprocal_z ? (std::signbit(reciprocal_z) ? B1.z - r0.m_Point.z : B0.z - r0.m_Point.z) * reciprocal_z : tMin };
    float tzMax{ reciprocal_z ? (std::signbit(reciprocal_z) ? B0.z - r0.m_Point.z : B1.z - r0.m_Point.z) * reciprocal_z : tMax };
    if (tMin > tzMax || tzMin > tMax)return false;
    if (tzMin > tMin)tMin = tzMin;
    if (tzMax < tMax)tMax = tzMax;
  }

  outTime = tMin;
  return true;
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

bool MTG::intersectionRaySphere(Ray const& r0, Sphere const& s1, float& outTime0, float& outTime1)
{
  Vector3D RayToSphere{ s1.m_Center - r0.m_Point };
  float tc{ glm::dot(RayToSphere, r0.m_Direction) };
  if (std::signbit(tc))return false;

  float distanceSquare{ tc * tc - localHelper::LengthSquared3D(RayToSphere) };
  float radiusSquare{ s1.m_Radius * s1.m_Radius };
  if (distanceSquare > radiusSquare)return false;

  float internalDistanceTime{ std::sqrtf(radiusSquare - distanceSquare) };
  outTime0 = tc - internalDistanceTime;
  outTime1 = tc + internalDistanceTime;
  return true;
}

void MTG::TestRaySphere(Ray const& r0, Sphere const& s1)
{
  if (float tmp0, tmp1; intersectionRaySphere(r0, s1, tmp0, tmp1))
  {
    printf_s("  Result:%s t:%.2f\n", "true", tmp0);
  }
  else
  {
    printf_s("  Result:%s\n", "false");
  }
}

#include <string>

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

  /** Point Vs Plane - 40 tests **/
  // In front of plane
  void PointPlaneTest1(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest2(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest3(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest4(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest5(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest6(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest7(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest8(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, 1, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest9(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest10(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 2);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest11(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest12(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 0);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest13(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest14(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 1);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest15(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest16(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0, -1, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest17(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest18(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-1, 0, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest19(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 1.25f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest20(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 0, 0.75f);
    Plane plane(Vector3(0, 0, 1), Vector3(0, 0, 1));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest21(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 2.41421f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest22(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.70711f, 1.70711f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest23(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(-0.414214f, 1, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest24(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.292893f, 0.292893f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest25(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.292893f, 1.70711f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest26(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 1, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest27(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.46967f, 1.88388f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest28(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.116116f, 1.53033f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest29(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(2.41421f, 1, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane
  void PointPlaneTest30(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.70711f, 1.70711f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest31(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, -0.414214f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind Plane
  void PointPlaneTest32(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.292893f, 0.292893f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest33(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.70711f, 0.292893f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Directly on Plane
  void PointPlaneTest34(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1, 1, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest35(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.88388f, 0.46967f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest36(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.53033f, 0.116116f, 0);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest37(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.17678f, 1.17678f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // In front of plane within epsilon
  void PointPlaneTest38(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.823223f, 0.823223f, 1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest39(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(1.17678f, 1.17678f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  // Behind of plane within epsilon
  void PointPlaneTest40(const std::string & testName)
  {
    PrintTestHeader(testName);

    float epsilon = 0.5f;
    Vector3 point = Vector3(0.823223f, 0.823223f, -1);
    Plane plane(Vector3(0.707107f, 0.707107f, 0), Vector3(1, 1, 0));

    TestPointPlane(point, plane, epsilon);
  }

  /** Ray Vs Plane : 72 tests **/
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

  using Aabb = AABB;

  /** Ray Vs AABB : 91 tests **/
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

  /** Ray Vs Sphere : 85 tests **/
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

  void test()
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

}