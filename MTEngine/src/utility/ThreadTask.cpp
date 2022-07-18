/*!*****************************************************************************
 * @file    ThreadTask.cpp
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    16 JUN 2022
 * @brief   QOL Multi threaded task wrapper
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <cstdio>
#include <cassert>
#include <utility/Timer.h>
#include <utility/ThreadTask.h>

static void taskWrapper(MTU::ThreadTask::TaskData* pTData)
{
  assert(pTData && pTData->m_pFnToDo);
  __try
  {
    pTData->m_pFnToDo(pTData->m_pData);
    pTData->m_bDone = true;
  }
  __except (1)
  {
    printf_s("\n\nMulti Threaded task critical failure!\n\n");
  }
}

MTU::ThreadTask::ThreadTask(void(*toComplete)(void*), void* pData) :
  m_TData{ false, toComplete, pData },
  m_tWork{ taskWrapper, &m_TData }
{
  m_tWork.detach();
}

bool MTU::ThreadTask::isDone() const noexcept
{
  return m_TData.m_bDone;
}

static const char* dotsFromNum(int inDots)
{
  switch (inDots)
  {
  default: return "";
  case 1: return ".";
  case 2: return "..";
  case 3: return "...";
  }
}

void MTU::taskLoading(void* pData)
{
  if (pData == nullptr)
  {
    printf_s("NULLPTR PASSED TO TASK LOADING FUNCTION!\n");
    return;
  }
  int numDots{ 0 };
  volatile bool& bShouldStop{ *reinterpret_cast<bool*>(pData) };
  for (MTU::Timer lazyTimer{ MTU::Timer::getCurrentTP() }; false == bShouldStop; lazyTimer.stop())
  {
    if (lazyTimer.getElapsedCount() / MTU::Timer::clockFrequency)
    {
      printf_s("Loading%s\n", dotsFromNum(numDots));
      numDots = (numDots + 1) % 4;
      lazyTimer.start();// reset the elapsed counter
    }
  }
  printf_s("Loading%s\n", " complete!");
}

void MTU::taskInitializing(void* pData)
{
  if (pData == nullptr)
  {
    printf_s("NULLPTR PASSED TO TASK LOADING FUNCTION!\n");
    return;
  }
  int numDots{ 0 };
  volatile bool& bShouldStop{ *reinterpret_cast<bool*>(pData) };
  for (MTU::Timer lazyTimer{ MTU::Timer::getCurrentTP() }; false == bShouldStop; lazyTimer.stop())
  {
    if (lazyTimer.getElapsedCount() / MTU::Timer::clockFrequency)
    {
      printf_s("Initializing%s\n", dotsFromNum(numDots));
      numDots = (numDots + 1) % 4;
      lazyTimer.start();// reset the elapsed counter
    }
  }
  printf_s("Initialization complete!\n");
}

void MTU::taskWorking(void* pData)
{
  if (pData == nullptr)
  {
    printf_s("NULLPTR PASSED TO TASK LOADING FUNCTION!\n");
    return;
  }
  volatile bool& bShouldStop{ *reinterpret_cast<bool*>(pData) };
  int numDots{ 0 };
  for (MTU::Timer lazyTimer{ MTU::Timer::getCurrentTP() }; false == bShouldStop; lazyTimer.stop())
  {
    static constexpr auto printFrequency{ 3 * MTU::Timer::clockFrequency };
    if (lazyTimer.getElapsedCount() / printFrequency)
    {
      printf_s("Working%s\n", dotsFromNum(numDots));
      numDots = (numDots + 1) % 4;
      lazyTimer.start();// reset the elapsed counter
    }
  }
  printf_s("Work Done!\n");
}
