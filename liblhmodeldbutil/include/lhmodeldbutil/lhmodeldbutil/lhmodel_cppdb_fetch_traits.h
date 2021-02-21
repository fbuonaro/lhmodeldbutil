#ifndef __LHMODEL_CPPDB_FETCH_TRAITS_H__
#define __LHMODEL_CPPDB_FETCH_TRAITS_H__

#include <cppdb/frontend.h>

#include <string>

namespace LHModelDbUtilNS
{
    template< typename T >
    void defaultFetch( cppdb::result& r, const std::string& name, T& value )
    {
        r.fetch( name, value );
    }

    template< typename T, typename = void >
    class PrimitiveResultFetcher
    {
        public:
            static void fetchFromResult( cppdb::result& r, const std::string& name, T& value )
            {
                defaultFetch< T >( r, name, value );
            }
    };
}

#endif
