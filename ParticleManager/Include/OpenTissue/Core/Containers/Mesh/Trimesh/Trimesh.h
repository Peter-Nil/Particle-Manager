#pragma once
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//
#include <OpenTissue\Core\Containers\Mesh\Trimesh\TrimeshMesh.h>
#include <OpenTissue\Core\Containers\Mesh\MeshDefaultTraits.h>
#include <OpenTissue\Core\Containers\Mesh\Trimesh\TrimeshArrayKernel.h>
#include <OpenTissue\Core\Math\MathBasicTypes.h>

namespace OpenTissue
{
    namespace trimesh
    {
        /**
        * This is basically just another name for trimesh::detail::mesh
        *
        * The first template argument is supposed to be a math types type binder.
        * OpenTissue provides a simple basic math type-binder in the math sub-library.
        * See OpenTissue::math::BasicMathTypes<real_type, size_type>
        * The next two template arguments are supposed to be vertex traits,
        * and face traits. The last template argument is the trimesh kernel
        * type that is supposed to be used.
        *
        */
        template<
            typename M = math::BasicMathTypes<double, size_t>
            , typename V = mesh::DefaultVertexTraits< M >
            , typename F = mesh::DefaultFaceTraits
            , template <typename, typename> class K = TriMeshArrayKernel
        >
            class TriMesh
            : public detail::TMesh<M, V, F, K>
        {};

    }
}
