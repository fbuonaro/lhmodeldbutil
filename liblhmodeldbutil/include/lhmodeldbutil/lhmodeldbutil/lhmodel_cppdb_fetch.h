#ifndef __LHMODEL_CPPDB_FETCH_H__
#define __LHMODEL_CPPDB_FETCH_H__

#include <lhmodel/visitor.h>

#include <lhmodelutil/lhmodel_visitor_helper.h>

#include <lhmodeldbutil/lhmodel_cppdb_fetch_traits.h>

#include <cppdb/frontend.h>

namespace LHModelDbUtilNS
{
    class ResultFetcher : public LHModelUtilNS::FlatPopulator
    {
        public:
            ResultFetcher( cppdb::result& _r )
            : r( _r )
            {
            }

        protected:
            template< typename T >
            void VisitPrimitiveMember( const LHModelNS::MemberMeta& memberMeta, T& primitiveMember )
            {
                PrimitiveResultFetcher< T >::fetchFromResult( r, memberMeta.name, primitiveMember );
            }

        private:
            cppdb::result& r;
    };

    template< typename T >
    void FetchFromResult( T& model, cppdb::result& r )
    {
        LHModelNS::Populator< ResultFetcher > resultFetcher( r );

        model.Accept( resultFetcher );
    }

    template< typename T, typename OutputIterator >
    void FetchFromResult( OutputIterator& outputIt, cppdb::result& r )
    {
        LHModelNS::Populator< ResultFetcher > resultFetcher( r );

        while( r.next() )
        {
            T model;
            model.Accept( resultFetcher );

            outputIt = std::move( model );
        }
    }

    template< typename T, typename MemberInputIterator >
    void FetchMembersFromResult( T& model,
                                 const MemberInputIterator& itMemberStart, const MemberInputIterator& itMemberEnd,
                                 cppdb::result& r )
    {
        LHModelNS::Populator< ResultFetcher > resultFetcher( r );

        for( auto itMember = itMemberStart; itMember != itMemberEnd; ++itMember )
        {
            model.Accept( resultFetcher, *itMember );
        }
    }

    template< typename T, typename OutputIterator, typename MemberInputIterator >
    void FetchMembersFromResult( OutputIterator& outputIt,
                                 const MemberInputIterator& itMemberStart, const MemberInputIterator& itMemberEnd,
                                 cppdb::result& r )
    {
        LHModelNS::Populator< ResultFetcher > resultFetcher( r );

        while( r.next() )
        {
            T model;
            for( auto itMember = itMemberStart; itMember != itMemberEnd; ++itMember )
            {
                model.Accept( resultFetcher, *itMember );
            }

            outputIt = std::move( model );
        }
    }
}

#endif
