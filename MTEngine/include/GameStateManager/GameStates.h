/*!*****************************************************************************
 * @file    GameStates.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   QOL GameStates enum class header
 *          Quick and dirty for use in CS350
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATES_HEADER_GUARD
#define GAMESTATES_HEADER_GUARD

namespace MTU
{
  // Quick and dirty enumerator, don't forget to add to GSM too.
  enum class GS
  {
    E_FIRST = 0,
    E_ASSIGNMENT_1 = E_FIRST,
    E_ASSIGNMENT_2,
    E_ASSIGNMENT_3,

    E_NUM_GAMESTATES,
    E_RESTART,
    E_QUIT
  };
}

#endif
