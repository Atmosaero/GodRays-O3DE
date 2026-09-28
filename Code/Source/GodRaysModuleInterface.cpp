
#include "GodRaysModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <GodRays/GodRaysTypeIds.h>

#include <Clients/GodRaysSystemComponent.h>

namespace GodRays
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GodRaysModuleInterface,
        "GodRaysModuleInterface", GodRaysModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GodRaysModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GodRaysModuleInterface, AZ::SystemAllocator);

    GodRaysModuleInterface::GodRaysModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            GodRaysSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList GodRaysModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GodRaysSystemComponent>(),
        };
    }
} // namespace GodRays
