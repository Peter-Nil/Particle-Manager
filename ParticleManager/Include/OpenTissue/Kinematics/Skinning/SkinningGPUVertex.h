#pragma once
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//

namespace OpenTissue
{
    namespace skinning
    {
        //--- Just a define to ease offset calculation
#define BUFFER_OFFSET(bytes)	((GLubyte*) NULL + (bytes))

        struct gpu_vertex
        {
            float vertex[4];	// Center of rotation is stored in vertex[3], i.e. vertex.w
            float normal[3];
            float weight[4];
            float boneIdx[4];
        };

    } // namespace skinning
} // namespace OpenTissue