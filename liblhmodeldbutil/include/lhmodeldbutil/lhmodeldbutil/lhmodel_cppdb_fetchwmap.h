#ifndef __LHMODEL_CPPDB_FETCHWMAP_H__
#define __LHMODEL_CPPDB_FETCHWMAP_H__

#include <lhmodel/visitor.h>

#include <lhmodelutil/lhmodel_visitor_helper.h>

#include <lhmodeldbutil/lhmodel_cppdb_fetch_traits.h>

#include <cppdb/frontend.h>

namespace LHModelDbUtilNS
{
    // For cases where the columns have been aliased
    // such as in joins with otherwise overlapping column names
    // would be used as follows:
    // struct{ int a, int c } t1; struct { int a, int d } t2;
    // r =  select t1.a as a, t1.c as c, t2.a as b from t1, t2
    // someMap{ b -> a }
    // ResultFetcher fetcher( r )
    // ResultFetcherWithMap fetcherMapped( r, someMap )
    // vector< pair< t1, t2 > > fetched;
    // while( r.next() )
    // { t1 a, t2 b;
    //   a.Accept( fetcher ); b.Accept( fetcherMapped, "a" );
    //   fetched.emplace_back( a, b );
    // }

    template< typename U >
    class ResultFetcherWithMap : public LHModelUtilNS::FlatPopulator
    {
        public:
            typedef U MapType;

            ResultFetcherWithMap( cppdb::result& _r, const MapType& _memberToFieldMap )
            : r( _r )
            , memberToFieldMap( _memberToFieldMap )
            {
            }

        protected:
            template< typename T >
            void VisitPrimitiveMember( const LHModelNS::MemberMeta& memberMeta, T& primitiveMember )
            {
                typename MapType::Key mapKey( memberMeta.name );
                auto it = memberToFieldMap.find( mapKey );

                PrimitiveResultFetcher< T >::fetchFromResult( r,
                                                              it != memberToFieldMap.cend()
                                                              ? it->second : mapKey,
                                                              primitiveMember );
            }

        private:
            cppdb::result& r;
            const MapType& memberToFieldMap;
    };
}

#endif
