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

#ifndef __cpp_consteval

namespace CPPSL
{
  struct source_location {
    _NODISCARD static source_location current(const uint_least32_t _Line_ = __builtin_LINE(),
      const uint_least32_t _Column_ = __builtin_COLUMN(), const char* const _File_ = __builtin_FILE(),
      const char* const _Function_ = __builtin_FUNCTION()) noexcept {
      source_location _Result;
      _Result._Line = _Line_;
      _Result._Column = _Column_;
      _Result._File = _File_;
      _Result._Function = _Function_;
      return _Result;
    }

    _NODISCARD_CTOR constexpr source_location() noexcept = default;

    _NODISCARD constexpr uint_least32_t line() const noexcept {
      return _Line;
    }
    _NODISCARD constexpr uint_least32_t column() const noexcept {
      return _Column;
    }
    _NODISCARD constexpr const char* file_name() const noexcept {
      return _File;
    }
    _NODISCARD constexpr const char* function_name() const noexcept {
      return _Function;
    }

  private:
    uint_least32_t _Line{};
    uint_least32_t _Column{};
    const char* _File = "";
    const char* _Function = "";
  };
}

#else

#include <source_location>  // for error/warning reporting
namespace CPPSL = std;

#endif // !__cpp_consteval


#pragma warning (disable: 26812)// seriously shut up about enums

//using std::string_view_literals::operator""sv;// produces warning C4455 due to compiler bug
using namespace std::string_view_literals;	// for literal operator sv

std::string_view VKErrorToString(VkResult errCode) noexcept;
void printVKWarning(VkResult errCode, std::string_view msg, bool isError = false, CPPSL::source_location const& errLoc = CPPSL::source_location::current()) noexcept;
void printWarning(std::string_view msg, bool isError = false, CPPSL::source_location const& errLoc = CPPSL::source_location::current()) noexcept;

#endif//VULKAN_HELPERS_PRINT_WARNINGS_HEADER
