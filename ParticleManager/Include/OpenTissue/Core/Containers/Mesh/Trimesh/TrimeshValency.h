#pragma once
#include <OpenTissue\Core\Containers\Mesh\Trimesh\TrimeshVertex.h>
#include <OpenTissue\Core\Containers\Mesh\Trimesh\TrimeshFace.h>

namespace OpenTissue
{
    namespace trimesh
    {

        template<typename mesh_type>
        unsigned int valency(TriMeshVertex<mesh_type> const& v)
        {
            return v.get_face_count();
        }

        template<typename mesh_type>
        unsigned int valency(TriMeshFace<mesh_type> const& /*f*/)
        {
            return 3u;
        }
    }
}
