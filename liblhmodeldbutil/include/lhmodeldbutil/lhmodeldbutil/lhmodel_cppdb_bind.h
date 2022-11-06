#ifndef __LHMODEL_CPPDB_BIND_H__
#define __LHMODEL_CPPDB_BIND_H__

#include <lhmodel/visitor.h>

#include <lhmodelutil/lhmodel_visitor_helper.h>

#include <lhmodeldbutil/lhmodel_cppdb_bind_traits.h>

#include <cppdb/frontend.h>

#include <sstream>
#include <stdexcept>

namespace LHModelDbUtilNS
{
    class StatementBinder : public LHModelUtilNS::FlatInspector
    {
    public:
        StatementBinder( cppdb::statement& _s, int _placeholderIndex = 1 )
            : s( _s )
            , placeholderIndex( _placeholderIndex )
        {
        }

        void AdvancePlaceholderIndexTo( int newPlaceholderIndex )
        {
            if ( newPlaceholderIndex < placeholderIndex )
            {
                std::ostringstream oss;
                oss << "new placeholder index " << newPlaceholderIndex
                    << " must be after current placeholder index " << placeholderIndex;
                throw std::runtime_error( oss.str() );
            }

            placeholderIndex = newPlaceholderIndex;
        }

        void AdvancePlaceholderIndexBy( int advanceBy )
        {
            AdvancePlaceholderIndexTo( placeholderIndex + advanceBy );
        }

    protected:
        template< typename T >
        void VisitPrimitiveMember( const LHModelNS::MemberMeta& memberMeta, const T& primitiveMember )
        {
            (void)memberMeta;
            PrimitiveStatementBinder< T >::bindToStatement( s, placeholderIndex, primitiveMember );
            ++placeholderIndex;
        }

    private:
        cppdb::statement& s;
        int placeholderIndex;

    };

    template< typename T >
    void BindToStatement( const T& model, cppdb::statement& s )
    {
        LHModelNS::Inspector< StatementBinder > statementBinder( s );
        model.Accept( statementBinder );
    }

    template< typename InputIterator >
    void BindToStatement( const InputIterator& itModelStart, const InputIterator& itModelEnd,
        cppdb::statement& s )
    {
        LHModelNS::Inspector< StatementBinder > statementBinder( s );
        for ( auto it = itModelStart; it != itModelEnd; ++it )
        {
            ( *it ).Accept( statementBinder );
        }
    }

    template< typename T, typename MemberInputIterator >
    void BindMembersToStatement( const T& model,
        const MemberInputIterator& itMemberStart, const MemberInputIterator& itMemberEnd,
        cppdb::statement& s )
    {
        LHModelNS::Inspector< StatementBinder > statementBinder( s );
        for ( auto itMember = itMemberStart; itMember != itMemberEnd; ++itMember )
        {
            model.Accept( statementBinder, *itMember );
        }
    }

    template< typename InputIterator, typename MemberInputIterator >
    void BindMembersToStatement( const InputIterator& itModelStart, const InputIterator& itModelEnd,
        const MemberInputIterator& itMemberStart, const MemberInputIterator& itMemberEnd,
        cppdb::statement& s )
    {
        LHModelNS::Inspector< StatementBinder > statementBinder( s );
        for ( auto itModel = itModelStart; itModel != itModelEnd; ++itModel )
        {
            for ( auto itMember = itMemberStart; itMember != itMemberEnd; ++itMember )
            {
                ( *itModel ).Accept( statementBinder, *itMember );
            }
        }
    }
}

#endif
