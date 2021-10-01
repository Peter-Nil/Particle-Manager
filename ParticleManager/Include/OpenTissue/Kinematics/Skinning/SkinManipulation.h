#pragma once

namespace OpenTissue
{
    namespace skinning
    {
        template <typename skin_type, typename real_type>
        void scale_skin(skin_type& skin, real_type scale)
        {
            for (unsigned int i = 0; i < skin.m_sz; ++i)
            {
                auto vertex = skin.m_skin_parts[i].vertex_begin();
                auto end = skin.m_skin_parts[i].vertex_end();

                for (; vertex != end; ++vertex)
                {
                    vertex->m_coord *= scale;
                    vertex->m_original_coord = vertex->m_coord;
                }
            }
        }
    }
}