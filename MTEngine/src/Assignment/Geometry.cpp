/*!*****************************************************************************
 * @file    Geometry.cpp
 * @author  Owen Huang Wensong  (w.huang@digipen.edu)
 * @date    27 MAY 2022
 * @brief   Assignment 1: Geometry toolbox
 *
 * @par Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <Assignment/Geometry.h>
#include <algorithm>// min max

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
    return glm::dot(tmp, tmp);
  }

  static inline float LengthSquared3D(MTG::Vector3D const& v0)
  {
    return glm::dot(v0, v0);
  }

  static inline float Squared(float toSquare) noexcept { return toSquare * toSquare; }

//  static void ClosestPtPointAABB(MTG::Point3D const& p0, MTG::AABB const& a1, MTG::Point3D& outPoint)
//  {
//#define LAZYHELPER(a) outPoint.##a = std::max(std::min(p0.##a, a1.m_Min.##a), a1.m_Max.##a);
//    LAZYHELPER(x);
//    LAZYHELPER(y);
//    LAZYHELPER(z);
//#undef LAZYHELPER
//  }

  float SquareDistancePointAABB(MTG::Point3D const& p0, MTG::AABB const& a1)
  {
    float sqDist{ 0.0f };
    for (unsigned i{ 0 }; i < 3; ++i)
    {
      float v{ p0[i] };
      if (v < a1.m_Min[i])sqDist += Squared(a1.m_Min[i] - v);
      if (v > a1.m_Max[i])sqDist += Squared(v - a1.m_Max[i]);
    }
    return sqDist;
  }

  static inline float TriArea2D(float x1, float y1, float x2, float y2, float x3, float y3) noexcept
  {
    return (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
  }

  // Orange book pg 90/633
  static void Barycentric(MTG::Point3D const& p0, MTG::Triangle const& t1, MTG::Vector3D& outBarycentric)
  {
    // Unnormalized triangle normal
    MTG::Vector3D m{ glm::cross(t1.v1 - t1.v0, t1.v2 - t1.v0) };
    // Nominators and reciprocal of denominator for u and v ratios
    float nu, nv, rd;
    // Absolute components for determining projection plane
    float x{ std::fabs(m.x) }, y{ std::fabs(m.y) }, z{ fabs(m.z) };

    if (x >= y && x >= z)
    {
      // x is the largest, project to the yz plane
      nu = TriArea2D(p0.y, p0.z, t1.v1.y, t1.v1.z, t1.v2.y, t1.v2.z); // Area of PBC in yz plane
      nv = TriArea2D(p0.y, p0.z, t1.v2.y, t1.v2.z, t1.v0.y, t1.v0.z); // Area of PCA in yz plane
      rd = 1.0f / m.x;
    }
    else if (y >= x && y >= z)
    {
      // y is the largest, project to the xz plane
      nu = TriArea2D(p0.x, p0.z, t1.v1.x, t1.v1.z, t1.v2.x, t1.v2.z);
      nv = TriArea2D(p0.x, p0.z, t1.v2.x, t1.v2.z, t1.v0.x, t1.v0.z);
      rd = 1.0f / -m.y;
    }
    else
    {
      // z is the largest, project to the xy plane
      nu = TriArea2D(p0.x, p0.y, t1.v1.x, t1.v1.y, t1.v2.x, t1.v2.y);
      nv = TriArea2D(p0.x, p0.y, t1.v2.x, t1.v2.y, t1.v0.x, t1.v0.y);
      rd = 1.0f / m.z;
    }

    outBarycentric.x = nu * rd;                                   // u
    outBarycentric.y = nv * rd;                                   // v
    outBarycentric.z = 1.0f - outBarycentric.x - outBarycentric.y;// w

  }

}

bool MTG::intersectionSphereSphere(Sphere const& s0, Sphere const& s1)
{
  return localHelper::DistanceSquared3D(s0.m_Center, s1.m_Center) <= localHelper::Squared(s0.m_Radius + s1.m_Radius);
}

bool MTG::intersectionSphereAABB(Sphere const& s0, AABB const& a1)
{
  //if (intersectionPointAABB(s0.m_Center, a1))return true;
  //Point3D closest;
  //localHelper::ClosestPtPointAABB(s0.m_Center, a1, closest);
  //return localHelper::DistanceSquared3D(closest, s0.m_Center) <= localHelper::Squared(s0.m_Radius);
  return localHelper::SquareDistancePointAABB(s0.m_Center, a1) <= localHelper::Squared(s0.m_Radius);
}

bool MTG::intersectionAABBAABB(AABB const& a0, AABB const& a1)
{
#define LAZYHELPER(a) (a0.m_Max.##a < a1.m_Min.##a || a1.m_Max.##a < a0.m_Min.##a)
  return (LAZYHELPER(x) || LAZYHELPER(y) || LAZYHELPER(z)) ? false : true;
#undef LAZYHELPER
}

bool MTG::intersectionPointSphere(Point3D const& p0, Sphere const& s1)
{
  return localHelper::DistanceSquared3D(p0, s1.m_Center) <= localHelper::Squared(s1.m_Radius);
}

bool MTG::intersectionPointAABB(Point3D const& p0, AABB const& a1)
{
#define LAZYHELPER(a) (p0.##a < a1.m_Min.##a || p0.##a > a1.m_Max.##a)
  return (LAZYHELPER(x) || LAZYHELPER(y) || LAZYHELPER(z)) ? false : true;
#undef LAZYHELPER
}

bool MTG::intersectionPointTriangle(Point3D const& p0, Triangle const& t1, Vector3D& outBarycentric)
{
  localHelper::Barycentric(p0, t1, outBarycentric);
  return outBarycentric.y >= 0.0f && outBarycentric.z >= 0.0f && (outBarycentric.y + outBarycentric.z) <= 1.0f;
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
  if (std::fabsf(tmp) < 0.00000625f)
  { // parallel
    return false;
  }
  // control reach here means there is an intersection, but when.
  //outTime = glm::dot(p1.m_Dist * p1.m_Normal - r0.m_Point, p1.m_Normal) / tmp;
  outTime = (p1.m_Dist - glm::dot(r0.m_Point, p1.m_Normal)) / tmp;
  return true;
}

bool MTG::intersectionRayAABB(Ray const& r0, AABB const& a1, float& outTime)
{
  float tMin{ 0.0f }, tMax{ FLT_MAX };
  for (int i{ 0 }; i < 3; ++i)
  {
    if (std::fabsf(r0.m_Direction[i]) < FLT_EPSILON)
    {
      // parallel and outside
      if (r0.m_Point[i] < a1.m_Min[i] || r0.m_Point[i] > a1.m_Max[i])return false;
    }
    else
    {
      float rd{ 1.0f / r0.m_Direction[i] };
      float t1{ (a1.m_Min[i] - r0.m_Point[i]) * rd };
      float t2{ (a1.m_Max[i] - r0.m_Point[i]) * rd };
      if (t1 > t2)std::swap(t1, t2);
      if (t1 > tMin)tMin = t1;
      if (t2 < tMax)tMax = t2;
      if (tMin > tMax)return false; 
    }
  }

  outTime = tMin;
  return true;

  // made a mistake somewhere when trying to speed it up
  //Point3D const& B0{ a1.m_Min };
  //Point3D const& B1{ a1.m_Max };

  //float reciprocal_x{ r0.m_Direction.x ? (1.0f / r0.m_Direction.x) : 0.0f };
  //float reciprocal_y{ r0.m_Direction.y ? (1.0f / r0.m_Direction.y) : 0.0f };
  //float reciprocal_z{ r0.m_Direction.z ? (1.0f / r0.m_Direction.z) : 0.0f };

  //float tMin{ reciprocal_x ? (std::signbit(reciprocal_x) ? B1.x - r0.m_Point.x : B0.x - r0.m_Point.x) * reciprocal_x : 0.0f };
  //float tMax{ reciprocal_x ? (std::signbit(reciprocal_x) ? B0.x - r0.m_Point.x : B1.x - r0.m_Point.x) * reciprocal_x : FLT_MAX };
  //{
  //  float tyMin{ reciprocal_y ? (std::signbit(reciprocal_y) ? B1.y - r0.m_Point.y : B0.y - r0.m_Point.y) * reciprocal_y : tMin };
  //  float tyMax{ reciprocal_y ? (std::signbit(reciprocal_y) ? B0.y - r0.m_Point.y : B1.y - r0.m_Point.y) * reciprocal_y : tMax };
  //  if (tMin > tyMax || tyMin > tMax)return false;
  //  if (tyMin > tMin)tMin = tyMin;
  //  if (tyMax < tMax)tMax = tyMax;
  //}
  //{
  //  float tzMin{ reciprocal_z ? (std::signbit(reciprocal_z) ? B1.z - r0.m_Point.z : B0.z - r0.m_Point.z) * reciprocal_z : tMin };
  //  float tzMax{ reciprocal_z ? (std::signbit(reciprocal_z) ? B0.z - r0.m_Point.z : B1.z - r0.m_Point.z) * reciprocal_z : tMax };
  //  if (tMin > tzMax || tzMin > tMax)return false;
  //  if (tzMin > tMin)tMin = tzMin;
  //  if (tzMax < tMax)tMax = tzMax;
  //}

  //outTime = tMin;
  //return outTime >= 0.0f;
}

bool MTG::intersectionRaySphere(Ray const& r0, Sphere const& s1, float& outTime)
{
  Vector3D m{ r0.m_Point - s1.m_Center };
  float b{ glm::dot(m, r0.m_Direction) };
  float c{ localHelper::LengthSquared3D(m) - localHelper::Squared(s1.m_Radius) };

  if (c > 0.0f && b > 0.0f)return false;

  float discr{ b * b - c };

  if (discr < 0.0f)return false;

  outTime = std::max(0.0f, -b - std::sqrtf(discr));

  return true;

}

bool MTG::intersectionRayTriangle(Ray const& r0, Triangle const& t1, float& outTime)
{
  Vector3D e0{ t1.v1 - t1.v0 }, e1{ t1.v2 - t1.v0 };
  Vector3D h{ glm::cross(r0.m_Direction, e1) };
  float a{ glm::dot(e0, h) };
  if (std::fabsf(a) < FLT_EPSILON)return false;

  float f{ 1.0f / a };
  Vector3D s{ r0.m_Point - t1.v0 };
  float u{ f * glm::dot(s, h) };
  if (u < 0.0f || u > 1.0f)return false;

  Vector3D q{ glm::cross(s, e0) };
  float v{ f * glm::dot(r0.m_Direction, q) };
  if (v < 0.0f || u + v > 1.0f)return false;

  outTime = f * glm::dot(e1, q);
  return outTime > 0.0f;

}

int MTG::intersectionPlaneAABB(Plane const& p0, AABB const& a1)
{
  Point3D c{ 0.5f * (a1.m_Max + a1.m_Min) };
  Vector3D e{ a1.m_Max - c };

  // Projection interval radius of the AABB onto the plane
  float r
  {
    e.x * std::fabs(p0.m_Normal.x) +
    e.y * std::fabs(p0.m_Normal.y) +
    e.z * std::fabs(p0.m_Normal.z)
  };

  // distance of AABB center from plane
  float s{ glm::dot(p0.m_Normal, c) - p0.m_Dist };

  // intersection when distance within -r to r
  return std::fabsf(s) <= r ? 0 : (std::signbit(s) ? -1 : 1);
}

int MTG::intersectionPlaneSphere(Plane const& p0, Sphere const& s1)
{
  float dist{ glm::dot(s1.m_Center, p0.m_Normal) - p0.m_Dist };
  return std::fabsf(dist) <= s1.m_Radius ? 0 : (std::signbit(dist) ? -1 : 1);
}