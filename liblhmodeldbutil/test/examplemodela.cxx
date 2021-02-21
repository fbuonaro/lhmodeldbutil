
#include "examplemodela.h"

#include <lhmodel/membermeta.h>

#include <array>
#include <unordered_map>

namespace LHModelNS
{
    constexpr std::array< MemberMeta, 4 > ModelMembersMeta< LHModelDbUtilTestNS::ExampleModelA >::membersMeta;

    const std::unordered_map< std::string, size_t > ModelMembersMeta< LHModelDbUtilTestNS::ExampleModelA >::memberIndex{ {
    { "pid", 0 },
    { "pit", 1 },
    { "token", 2 },
    { "enabled", 3 } } };

    constexpr ModelMeta ModelModelMeta< LHModelDbUtilTestNS::ExampleModelA >::modelMeta;
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

