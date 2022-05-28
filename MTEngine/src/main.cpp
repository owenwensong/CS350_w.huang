/*!*****************************************************************************
 * @file    main.cpp
 * @author  Owen Huang Wensong  (w.huang@digipen.edu)
 * @date    09 FEB 2022
 * @brief   This is the entry point of the program.
 * 
 *          This is where the final project example lives. The vulkan engine
 *          is separate from this file.
 *
 * @par Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#include <memory>
#include <handlers/windowHandler.h>
#include <GameStateManager/GameStateManager.h>


int main()
{
#if defined(DEBUG) || defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//DEBUG || 
  //char* leak{ new char[69] };
  
  windowHandler* pWH
  {
    windowHandler::createInstance
    (
      windowHandler::flagDebugPrint
      //| windowHandler::flagDebugLayer
      //| windowHandler::flagRenderDocLayer
    )
  };
  if (pWH == nullptr || !pWH->OK())
  {
    printWarning("FAILED TO CREATE THE WINDOW HANDLER"sv, true);
    return -3;
  }

  {
    MTU::GameStateManager GSM{ windowSetup{ 1280, 720, false, true, false, 0.0f, 0.0f , 0.0f, 1.0f , L"CS350 Assignment 1 | Owen Huang Wensong"sv } };
    GSM.setNextGameState(MTU::GS::E_MISC);
    GSM.initImgui();
    GSM.Run();
  }
  return 0;

}
