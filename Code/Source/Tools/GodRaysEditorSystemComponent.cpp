/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Serialization/SerializeContext.h>
#include "GodRaysEditorSystemComponent.h"

#include <GodRays/GodRaysTypeIds.h>

namespace GodRays
{
    AZ_COMPONENT_IMPL(GodRaysEditorSystemComponent, "GodRaysEditorSystemComponent",
        GodRaysEditorSystemComponentTypeId, BaseSystemComponent);

    void GodRaysEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GodRaysEditorSystemComponent, GodRaysSystemComponent>()
                ->Version(0);
        }
    }

    GodRaysEditorSystemComponent::GodRaysEditorSystemComponent() = default;

    GodRaysEditorSystemComponent::~GodRaysEditorSystemComponent() = default;

    void GodRaysEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GodRaysSystemEditorService"));
    }

    void GodRaysEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GodRaysSystemEditorService"));
    }

    void GodRaysEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GodRaysEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GodRaysEditorSystemComponent::Activate()
    {
        GodRaysSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void GodRaysEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        GodRaysSystemComponent::Deactivate();
    }

} // namespace GodRays
