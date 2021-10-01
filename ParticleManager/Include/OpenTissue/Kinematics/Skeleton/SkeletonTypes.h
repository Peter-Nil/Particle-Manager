#pragma once
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//
#include <OpenTissue/Kinematics/Skeleton/SkeletonDefaultBoneTraits.h>
#include <OpenTissue/Kinematics/Skeleton/SkeletonBone.h>
#include <OpenTissue/Kinematics/Skeleton/SkeletonSkeleton.h>

#include <OpenTissue/Utility/GL/GLDrawBone.h>
#include <OpenTissue/Utility/UtilityEmptyTraits.h>

namespace OpenTissue
{
    namespace skeleton
    {
        template <
            typename math_types_
            , class bone_traits_        = OpenTissue::skeleton::DefaultBoneTraits<math_types_>
            , class skeleton_traits_    = OpenTissue::utility::EmptyTraits
        >
        class Types
        {
        public:

            typedef Types<math_types_, bone_traits_, skeleton_traits_>	skeleton_types;

            typedef math_types_                   math_types;
            typedef Bone<skeleton_types>          bone_type;
            typedef Skeleton<skeleton_types>      skeleton_type;
            typedef bone_traits_                  bone_traits;
            typedef skeleton_traits_              skeleton_traits;
        };
    }
}