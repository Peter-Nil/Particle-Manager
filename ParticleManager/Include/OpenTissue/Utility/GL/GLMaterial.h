#pragma once
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//
#include <OpenTissue\Utility\GL\GL.h>
#include <OpenTissue\Utility\UtilityMaterial.h>

namespace OpenTissue
{
    namespace gl
    {
        /**
        * A Material.
        * This class encapsulates an openGL material into an easy to use interface.
        */
        class Material : public OpenTissue::utility::Material
        {
        public:

            void use()
            {
                if (glIsEnabled(GL_COLOR_MATERIAL) || !glIsEnabled(GL_LIGHTING))
                {
                    glColor4f(m_diffuse[0], m_diffuse[1], m_diffuse[2], m_diffuse[3]);
                }
                else
                {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_shininess);
                }
            }
        };
    }
}