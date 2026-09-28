/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 */
#include <Tools/Components/EditorGodRaysComponent.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace GodRays
{
    void EditorGodRaysComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<EditorGodRaysComponent, BaseClass>()->Version(1);
            if (auto* ec = sc->GetEditContext())
            {
                ec->Class<EditorGodRaysComponent>("GodRays", "Godot-style screen-space light shafts. Add beside a Directional Light; no shadow maps required.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Graphics/Lighting")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
        if (auto* bc = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            bc->ConstantProperty("EditorGodRaysComponentTypeId", BehaviorConstant(AZ::Uuid(EditorComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "render")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation);
        }
    }
    void EditorGodRaysComponent::Activate()
    {
        BaseClass::Activate();
        AzFramework::EntityDebugDisplayEventBus::Handler::BusConnect(GetEntityId());
    }
    void EditorGodRaysComponent::Deactivate()
    {
        AzFramework::EntityDebugDisplayEventBus::Handler::BusDisconnect();
        BaseClass::Deactivate();
    }
    void EditorGodRaysComponent::DisplayEntityViewport(const AzFramework::ViewportInfo&, AzFramework::DebugDisplayRequests& display)
    {
        if (!IsVisible())
        {
            return;
        }
        AZ::Transform world = AZ::Transform::CreateIdentity();
        AZ::TransformBus::EventResult(world, GetEntityId(), &AZ::TransformBus::Events::GetWorldTM);
        world.ExtractUniformScale();
        const AZ::Vector3 origin = world.GetTranslation();
        const AZ::Vector3 direction = world.GetBasisY();
        const AZ::Vector3 right = world.GetBasisX();
        const AZ::Vector3 up = world.GetBasisZ();
        // Same +Y convention and disk/parallel-arrow design as Directional Light.
        const auto state = display.GetState();
        display.SetColor(m_controller.GetConfiguration().m_color);
        display.DrawWireDisk(origin, direction, 0.55f);
        for (const auto& offset : {right * 0.45f, right * -0.45f, up * 0.45f, up * -0.45f})
        {
            display.DrawArrow(origin + offset - direction * 2.8f, origin + offset - direction * 0.6f, 0.4f);
        }
        display.SetState(state);
    }
}
