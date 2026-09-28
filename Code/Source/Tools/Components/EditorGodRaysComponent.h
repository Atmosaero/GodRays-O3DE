/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 */
#pragma once
#include <Atom/Feature/Utils/EditorRenderComponentAdapter.h>
#include <AzFramework/Entity/EntityDebugDisplayBus.h>
#include <Components/GodRaysComponent.h>

namespace GodRays
{
    inline constexpr AZ::TypeId EditorComponentTypeId { "{76540BFB-4E68-4154-9B75-C92FDA9327F5}" };
    class EditorGodRaysComponent final
        : public AZ::Render::EditorRenderComponentAdapter<GodRaysComponentController, GodRaysComponent, GodRaysComponentConfig>
        , private AzFramework::EntityDebugDisplayEventBus::Handler
    {
    public:
        using BaseClass = AZ::Render::EditorRenderComponentAdapter<GodRaysComponentController, GodRaysComponent, GodRaysComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorGodRaysComponent, EditorComponentTypeId, BaseClass);
        static void Reflect(AZ::ReflectContext* context);
        EditorGodRaysComponent() = default;
        explicit EditorGodRaysComponent(const GodRaysComponentConfig& config) : BaseClass(config) {}
        void Activate() override;
        void Deactivate() override;
    private:
        void DisplayEntityViewport(const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& display) override;
    };
}