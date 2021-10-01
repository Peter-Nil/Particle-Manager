#pragma once
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//
#include <OpenTissue\Kinematics\Skinning\SkinningAnimatedSkin.h>
#include <OpenTissue\Kinematics\Skinning\SkinningLBS.h>
#include <OpenTissue\Kinematics\Skinning\SkinningLBSGPU.h>
#include <OpenTissue\Kinematics\Skinning\SkinningSBS.h>
#include <OpenTissue\Kinematics\Skinning\SkinningSBSGPU.h>
#include <OpenTissue\Kinematics\Skinning\SkinningDBS.h>
#include <OpenTissue\Kinematics\Skinning\SkinRenderer.h>
#include <OpenTissue\Utility\UtilityMaterial.h>

namespace OpenTissue
{
    namespace skinning
    {
        template <typename math_types_, template <typename> class skin_part_type_>
        class Types
        {
        public:
            typedef Types<math_types_, skin_part_type_> skin_types;
            typedef math_types_                         math_types;
            typedef skin_part_type_<math_types>         skin_part_type;
            typedef AnimatedSkin<skin_types>            skin_type;
            typedef typename gl::SkinRenderer           skin_render_type;
            typedef OpenTissue::utility::Material       material_type;
            typedef typename math_types::index_type     key_type;
        };
    }
}
