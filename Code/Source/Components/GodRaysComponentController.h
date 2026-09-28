/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 */
#pragma once
#include <AzCore/Component/TransformBus.h>
#include <GodRays/GodRaysFeatureProcessorInterface.h>

namespace GodRays
{
    class GodRaysComponentController final
        : private AZ::TransformNotificationBus::Handler
    {
    public:
        AZ_RTTI(GodRaysComponentController, "{EDE5B080-0E05-4920-AA24-25A1DB4717B2}");
        AZ_CLASS_ALLOCATOR(GodRaysComponentController, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        GodRaysComponentController() = default;
        explicit GodRaysComponentController(const GodRaysComponentConfig& config);
        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const GodRaysComponentConfig& config);
        const GodRaysComponentConfig& GetConfiguration() const;
    private:
        AZ_DISABLE_COPY(GodRaysComponentController);
        void OnTransformChanged(const AZ::Transform& local, const AZ::Transform& world) override;
        GodRaysHandle m_handle;
        GodRaysFeatureProcessorInterface* m_featureProcessor = nullptr;
        AZ::EntityId m_entityId;
        GodRaysComponentConfig m_configuration;
    };
}
