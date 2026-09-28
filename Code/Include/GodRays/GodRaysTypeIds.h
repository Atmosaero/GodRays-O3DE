
#pragma once

namespace GodRays
{
    // System Component TypeIds
    inline constexpr const char* GodRaysSystemComponentTypeId = "{2FAF007C-1DB8-49D9-8CE9-C6F2BA9D479D}";
    inline constexpr const char* GodRaysEditorSystemComponentTypeId = "{92F9835A-440E-4850-9608-EA8B1B4F0A48}";

    // Module derived classes TypeIds
    inline constexpr const char* GodRaysModuleInterfaceTypeId = "{A430D76E-7988-48D0-874E-195F1A501D13}";
    inline constexpr const char* GodRaysModuleTypeId = "{C0A5BB99-4ACE-476C-B903-B9B2D6C5BEF8}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GodRaysEditorModuleTypeId = GodRaysModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GodRaysRequestsTypeId = "{54D915D5-F816-445B-A362-63263BB15E5B}";
} // namespace GodRays
