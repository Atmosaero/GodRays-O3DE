
#include <GodRays/GodRaysTypeIds.h>
#include <GodRaysModuleInterface.h>
#include "GodRaysEditorSystemComponent.h"
#include "Components/EditorGodRaysComponent.h"

namespace GodRays
{
    class GodRaysEditorModule
        : public GodRaysModuleInterface
    {
    public:
        AZ_RTTI(GodRaysEditorModule, GodRaysEditorModuleTypeId, GodRaysModuleInterface);
        AZ_CLASS_ALLOCATOR(GodRaysEditorModule, AZ::SystemAllocator);

        GodRaysEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                GodRaysEditorSystemComponent::CreateDescriptor(),
                GodRaysComponent::CreateDescriptor(),
                EditorGodRaysComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<GodRaysEditorSystemComponent>(),
            };
        }
    };
}// namespace GodRays

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), GodRays::GodRaysEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GodRays_Editor, GodRays::GodRaysEditorModule)
#endif
