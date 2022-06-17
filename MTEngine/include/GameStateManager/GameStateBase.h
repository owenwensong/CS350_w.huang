/*!*****************************************************************************
 * @file    GameStateBase.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   QOL GameState base class
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_BASE_HEADER_GUARD
#define GAMESTATE_BASE_HEADER_GUARD

#include <GameStateManager/GameStateManager.h>

#if defined(DEBUG) || defined(_DEBUG)
#include <cstdio>
#define GS_PRINT_FUNCSIG(...) printf_s("GAMESTATE FUNCTION: %s\n", __FUNCSIG__)
#else
#define GS_PRINT_FUNCSIG(...) 
#endif// DEBUG

namespace MTU
{
  // Quick and dirty GameState class for CS350
  class GameState
  {
  public:
    
    // Constructor will act as Load
    GameState(GameStateManager& rGSM) : GSM{ rGSM } {}

    virtual void Init() = 0;
    virtual void Update(uint64_t dt) = 0;
    virtual void Draw() = 0;
    virtual void Free() = 0;

    // Destructor will act as Unload
    virtual ~GameState() {};

  protected:
    GameStateManager& GSM;
  };
}

#endif//GAMESTATE_BASE_HEADER_GUARD
