
#pragma once

#include <GodRays/GodRaysTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/std/string/string.h>

namespace GodRays
{
    class GodRaysRequests
    {
    public:
        AZ_RTTI(GodRaysRequests, GodRaysRequestsTypeId);
        virtual ~GodRaysRequests() = default;
        //! Enable GPU queries explicitly for diagnostics; disabled during normal rendering.
        virtual void SetProfilingEnabled(bool enabled) = 0;
        //! JSON rows with actual pass dimensions and the latest GPU duration (readback is delayed).
        virtual AZStd::string GetPassDiagnostics() const = 0;
    };

    class GodRaysBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GodRaysRequestBus = AZ::EBus<GodRaysRequests, GodRaysBusTraits>;
    using GodRaysInterface = AZ::Interface<GodRaysRequests>;

} // namespace GodRays
