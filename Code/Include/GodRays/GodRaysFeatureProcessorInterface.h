/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/base.h>
#include <Atom/RPI.Public/FeatureProcessor.h>
#include <GodRays/GodRaysComponentConfig.h>

namespace GodRays
{
    struct GodRaysData
    {
        GodRaysComponentConfig m_configuration;
        AZ::Vector3 m_direction = AZ::Vector3::CreateAxisY();
    };
    using GodRaysHandle = AZStd::shared_ptr<GodRaysData>;

    // GodRaysFeatureProcessorInterface provides an interface to the feature processor for code outside of Atom
    class GodRaysFeatureProcessorInterface
        : public AZ::RPI::FeatureProcessor
    {
    public:
        AZ_RTTI(GodRaysFeatureProcessorInterface, "{F64F5612-0754-49B3-9A65-467F19BB592E}", AZ::RPI::FeatureProcessor);
        virtual GodRaysHandle AcquireGodRays() = 0;
        virtual void ReleaseGodRays(GodRaysHandle& handle) = 0;
        virtual const GodRaysData* GetActiveGodRays() const = 0;
    };
}
