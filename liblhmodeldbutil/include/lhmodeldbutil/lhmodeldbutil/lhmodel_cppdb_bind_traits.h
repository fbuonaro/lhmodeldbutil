#ifndef __LHMODEL_CPPDB_BIND_TRAITS_H__
#define __LHMODEL_CPPDB_BIND_TRAITS_H__

#include <cppdb/frontend.h>

namespace LHModelDbUtilNS
{
    template< typename T >
    void defaultBind( cppdb::statement& s, int placeholderIndex, const T& value )
    {
        s.bind( placeholderIndex, value );
    }

    template< typename T, typename = void >
    class PrimitiveStatementBinder
    {
        public:
            static void bindToStatement( cppdb::statement& s, int placeholderIndex, const T& value )
            {
                defaultBind< T >( s, placeholderIndex, value );
            }
    };
}

#endif
