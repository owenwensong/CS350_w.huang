/*!*****************************************************************************
 * @file    ThreadTask.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    16 JUN 2022
 * @brief   QOL Multi threaded task wrapper
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef THREAD_TASK_HEADER_GUARD
#define THREAD_TASK_HEADER_GUARD

#include <thread>

namespace MTU
{
  class ThreadTask
  {
  public:

    struct TaskData
    {
      bool  m_bDone{ false };
      void(*m_pFnToDo)(void*);
      void* m_pData{ nullptr };
    };

    ThreadTask(void(*toComplete)(void*), void* pData);

    bool isDone() const noexcept;

  private:
    TaskData    m_TData;
    std::thread m_tWork;
  };

  // could have used given struct with string and print frequency but lazy...
  void taskLoading(void* pBoolShouldStop);// to use while loading gamestates
  void taskInitializing(void* pBoolShouldStop);// to use while initializing
  void taskWorking(void* pBoolShouldStop);// to use while doing general stuff

}

#endif//THREAD_TASK_HEADER_GUARD
