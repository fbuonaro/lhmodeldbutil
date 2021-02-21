#ifndef __LHModelDbUtilTestNS_ExampleModelA_lhmodel_H__
#define __LHModelDbUtilTestNS_ExampleModelA_lhmodel_H__

#include "custom_types.h"
#include <string>

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

#include <array>
#include <unordered_map>

namespace LHModelDbUtilTestNS
{
    class ExampleModelA;
}

namespace LHModelNS
{
    template<>
    class MemberType< LHModelDbUtilTestNS::ExampleModelA >
    {
        public:
            typedef ModelMemberType valueType;
    };

    template<>
    class ModelMembersMeta< LHModelDbUtilTestNS::ExampleModelA >
    {
        public:
            static constexpr std::array< MemberMeta, 4 > membersMeta{ {
                MemberMeta( "pid", "LHModelDbUtilTestNS::Uuid", "uuid" ),
                MemberMeta( "pit", "LHModelDbUtilTestNS::PointInTime", "date-time" ),
                MemberMeta( "token", "std::string", "" ),
                MemberMeta( "enabled", "bool", "" ) } };

            static const std::unordered_map< std::string, size_t > memberIndex;
    };

    template<>
    class ModelModelMeta< LHModelDbUtilTestNS::ExampleModelA >
    {
        public:
            static constexpr ModelMeta modelMeta{ "ExampleModelA" };
    };

}

namespace LHModelDbUtilTestNS
{
    class ExampleModelA
    {
        public:
            template< typename V >
            void Accept( V& visitor ) const
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), pid );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), pit );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 2 >(), token );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 3 >(), enabled );
            }


            template< typename V >
            void Accept( V& visitor )
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), pid );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), pit );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 2 >(), token );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 3 >(), enabled );
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member ) const
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ExampleModelA >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), pid );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), pit );
                        break;
                    }
                    case( 2 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 2 >(), token );
                        break;
                    }
                    case( 3 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 3 >(), enabled );
                        break;
                    }
                }
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member )
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ExampleModelA >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 0 >(), pid );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 1 >(), pit );
                        break;
                    }
                    case( 2 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 2 >(), token );
                        break;
                    }
                    case( 3 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ExampleModelA, 3 >(), enabled );
                        break;
                    }
                }
            }

            LHModelDbUtilTestNS::Uuid pid;
            LHModelDbUtilTestNS::PointInTime pit;
            std::string token;
            bool enabled;
    };
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

#endif
