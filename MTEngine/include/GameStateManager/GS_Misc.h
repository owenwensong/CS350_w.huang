/*!*****************************************************************************
 * @file    GS_Misc.h
 * @author  Owen Huang Wensong  [390008220]  (w.huang@digipen.edu)
 * @date    26 MAY 2022
 * @brief   QOL GameState misc class
 *
 * Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef GAMESTATE_MISC_HEADER_GUARD
#define GAMESTATE_MISC_HEADER_GUARD

#include <GameStateManager/GameStateBase.h>

#include <vulkanHelpers/vulkanModel.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility/matrixTransforms.h>
#include <vulkanHelpers/vulkanModel.h>

namespace MTU
{
  // Quick and dirty GameState class for CS350
  class GS_Misc : public GameState
  {
  public:

    // Constructor will act as Load

    GS_Misc(GameStateManager& rGSM);

    virtual void Init() override;
    virtual void Update(uint64_t dt) override;
    virtual void Draw() override;
    virtual void Free() override;

    // Destructor will act as Unload
    virtual ~GS_Misc();

  private:
    windowsInput& inputs;
  };
}

#endif//GAMESTATE_MISC_HEADER_GUARD
