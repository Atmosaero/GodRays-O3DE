
#include <GodRays/GodRaysTypeIds.h>
#include <GodRaysModuleInterface.h>
#include "GodRaysSystemComponent.h"

#include <AzCore/RTTI/RTTI.h>
#include <Components/GodRaysComponent.h>


namespace GodRays
{
    class GodRaysModule
        : public GodRaysModuleInterface
    {
    public:
        AZ_RTTI(GodRaysModule, GodRaysModuleTypeId, GodRaysModuleInterface);
        AZ_CLASS_ALLOCATOR(GodRaysModule, AZ::SystemAllocator);

        GodRaysModule()
        {
            m_descriptors.push_back(GodRaysComponent::CreateDescriptor());
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const
        {
            return AZ::ComponentTypeList{ azrtti_typeid<GodRaysSystemComponent>() };
        }
    };
}// namespace GodRays

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GodRays::GodRaysModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GodRays, GodRays::GodRaysModule)
#endif
