#pragma once
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//

#include <OpenTissue\Core\Containers\Mesh\Trimesh\TrimeshFace.h>

namespace OpenTissue
{
    namespace trimesh
    {

        template<typename mesh_type>
        bool face_flip(TriMeshFace<mesh_type> const& f)
        {
            typedef typename TriMeshFace<mesh_type>::vertex_handle vertex_handle;
            vertex_handle h0 = f.get_vertex0_handle();
            vertex_handle h1 = f.get_vertex1_handle();
            //vertex_handle h2 = f.get_vertex2_handle();
            trimesh_core_access::set_vertex0_handle(&f, h1);
            trimesh_core_access::set_vertex1_handle(&f, h0);
            //trimesh_core_access::set_vertex2_handle( &f, h2 );
            return true;
        }
    }
}
