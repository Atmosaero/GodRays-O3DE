/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Components/GodRaysComponentController.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace GodRays
{
    inline constexpr AZ::TypeId GodRaysComponentTypeId { "{55E22C3E-D2D9-4A2C-8EAD-E07A39D0E37B}" };

    class GodRaysComponent final
        : public AzFramework::Components::ComponentAdapter<GodRaysComponentController, GodRaysComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<GodRaysComponentController, GodRaysComponentConfig>;
        AZ_COMPONENT(GodRaysComponent, GodRaysComponentTypeId, BaseClass);

        GodRaysComponent() = default;
        GodRaysComponent(const GodRaysComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
}
