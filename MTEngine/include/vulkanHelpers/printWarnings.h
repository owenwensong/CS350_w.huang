/*!*****************************************************************************
 * @file    printWarnings.h
 * @author  Owen Huang Wensong  (w.huang@digipen.edu)
 * @date    14 FEB 2022
 * @brief   This contains declarations for functions to print warnings
 *
 * @par Copyright (C) 2022 DigiPen Institute of Technology. All rights reserved.
*******************************************************************************/

#ifndef VULKAN_HELPERS_PRINT_WARNINGS_HEADER
#define VULKAN_HELPERS_PRINT_WARNINGS_HEADER

#include <vulkan/vulkan.h>
#include <string>           // for std::string_view

#pragma warning (disable: 26812)// seriously shut up about enums

//using std::string_view_literals::operator""sv;// produces warning C4455 due to compiler bug
using namespace std::string_view_literals;	// for literal operator sv

std::string_view VKErrorToString(VkResult errCode) noexcept;
void printVKWarningActual(VkResult errCode, std::string_view msg, const char* cstrFile, long Line, const char* cstrFunc, bool isError = false) noexcept;
void printWarningActual(std::string_view msg, const char* cstrFile, long Line, const char* cstrFunc, bool isError = false) noexcept;

// using hacky stuff cus lab computers no C++20, this was originally <source_location>
#define printVKWarning(A,B,...) printVKWarningActual(A, B, __FILE__, __LINE__, __FUNCSIG__, __VA_ARGS__)
#define printWarning(A, ...) printWarningActual(A, __FILE__, __LINE__, __FUNCSIG__, __VA_ARGS__)

#endif//VULKAN_HELPERS_PRINT_WARNINGS_HEADER
