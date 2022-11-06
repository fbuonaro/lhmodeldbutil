#include <gtest/gtest.h>

#include <lhmodeldbutil/lhmodel_cppdb.h>

#include <lhmodeldbtest/mock_cppdb_backend.h>

#include "examplemodela.h"

#include <iterator>

namespace LHModelDbUtilNS
{
    template<>
    void defaultBind( cppdb::statement& s, int placeholderIndex, const LHModelDbUtilTestNS::Uuid& uuid )
    {
        s.bind( placeholderIndex, uuid );
    }

    template<>
    void defaultBind( cppdb::statement& s, int placeholderIndex, const LHModelDbUtilTestNS::PointInTime& pit )
    {
        (void)pit;
        s.bind( placeholderIndex, 1234 );
    }

    // template<>
    // void defaultFetch( cppdb::result& r, const std::string& name, LHModelDbUtilTestNS::Uuid& uuid )
    // {
    //     r.fetch( name, uuid );
    // }

    template<>
    void defaultFetch( cppdb::result& r, const std::string& name, LHModelDbUtilTestNS::PointInTime& pit )
    {
        (void)pit;
        std::string pitString;
        r.fetch( name, pitString );
    }

    template<>
    void defaultFetch( cppdb::result& r, const std::string& name, bool& enabled )
    {
        int enabledInt;
        r.fetch( name, enabledInt );
        enabled = enabledInt != 0;
    }

}

namespace LHModelDbUtilTestNS
{
    using namespace MockCppdbBackendNS;

    TEST( TestLHModelCppdb, TestBindToStatementSingle )
    {
        ExampleModelA exModelA1;
        exModelA1.pid = 9999999;
        exModelA1.token = "abc";
        exModelA1.enabled = false;

        ExampleModelA exModelA2;
        exModelA2.pid = 88;
        exModelA2.token = "ggg";
        exModelA2.enabled = true;

        std::unique_ptr< MockDriver > mockDriver( new MockDriver() );
        EXPECT_CALL( *mockDriver, open( ::testing::_ ) )
            .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const cppdb::connection_info& ci )
        {
            std::unique_ptr< MockConnection > mockConnection( new MockConnection( ci ) );
            EXPECT_CALL( *mockConnection, create_statement( ::testing::_ ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, bind( 1, ::testing::TypedEq< decltype( exModelA1.pid ) >( exModelA1.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 2, ::testing::TypedEq< int >( 1234 ) ) );
                EXPECT_CALL( *mockStatement, bind( 3, ::testing::TypedEq< const std::string& >( exModelA1.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 4, ::testing::TypedEq< int >( exModelA1.enabled ) ) );
                return mockStatement.release();
            } ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, bind( 1, ::testing::TypedEq< decltype( exModelA1.pid ) >( exModelA1.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 2, ::testing::TypedEq< int >( 1234 ) ) );
                EXPECT_CALL( *mockStatement, bind( 3, ::testing::TypedEq< const std::string& >( exModelA1.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 4, ::testing::TypedEq< int >( exModelA1.enabled ) ) );
                EXPECT_CALL( *mockStatement, bind( 5, ::testing::TypedEq< decltype( exModelA2.pid ) >( exModelA2.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 6, ::testing::TypedEq< int >( 1234 ) ) );
                EXPECT_CALL( *mockStatement, bind( 7, ::testing::TypedEq< const std::string& >( exModelA2.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 8, ::testing::TypedEq< int >( exModelA2.enabled ) ) );
                return mockStatement.release();
            } ) );

            return mockConnection.release();
        } ) );

        InstallMockDriverGuard mockDriverGuard( "mocksql", mockDriver );
        cppdb::session cppdbSession( "mocksql:" );
        cppdb::statement cppdbStatement1( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 1" ) );
        cppdb::statement cppdbStatement2( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 2" ) );

        LHModelDbUtilNS::BindToStatement( exModelA1, cppdbStatement1 );

        LHModelNS::Inspector< LHModelDbUtilNS::StatementBinder > statementBinder( cppdbStatement2, 1 );
        exModelA1.Accept( statementBinder );
        exModelA2.Accept( statementBinder );
    }

    TEST( TestLHModelCppdb, TestBindToStatementRange )
    {
        std::vector< ExampleModelA > exModelAs( 2 );

        ExampleModelA& exModelA1( exModelAs.at( 0 ) );
        exModelA1.pid = 9999999;
        exModelA1.token = "abc";
        exModelA1.enabled = false;

        ExampleModelA& exModelA2( exModelAs.at( 1 ) );
        exModelA2.pid = 88;
        exModelA2.token = "ggg";
        exModelA2.enabled = true;

        std::unique_ptr< MockDriver > mockDriver( new MockDriver() );
        EXPECT_CALL( *mockDriver, open( ::testing::_ ) )
            .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const cppdb::connection_info& ci )
        {
            std::unique_ptr< MockConnection > mockConnection( new MockConnection( ci ) );
            EXPECT_CALL( *mockConnection, create_statement( ::testing::_ ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, bind( 1, ::testing::TypedEq< decltype( exModelA1.pid ) >( exModelA1.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 2, ::testing::TypedEq< int >( 1234 ) ) );
                EXPECT_CALL( *mockStatement, bind( 3, ::testing::TypedEq< const std::string& >( exModelA1.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 4, ::testing::TypedEq< int >( exModelA1.enabled ) ) );
                return mockStatement.release();
            } ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, bind( 1, ::testing::TypedEq< decltype( exModelA1.pid ) >( exModelA1.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 2, ::testing::TypedEq< int >( 1234 ) ) );
                EXPECT_CALL( *mockStatement, bind( 3, ::testing::TypedEq< const std::string& >( exModelA1.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 4, ::testing::TypedEq< int >( exModelA1.enabled ) ) );
                EXPECT_CALL( *mockStatement, bind( 5, ::testing::TypedEq< decltype( exModelA2.pid ) >( exModelA2.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 6, ::testing::TypedEq< int >( 1234 ) ) );
                EXPECT_CALL( *mockStatement, bind( 7, ::testing::TypedEq< const std::string& >( exModelA2.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 8, ::testing::TypedEq< int >( exModelA2.enabled ) ) );
                return mockStatement.release();
            } ) );

            return mockConnection.release();
        } ) );

        InstallMockDriverGuard mockDriverGuard( "mocksql", mockDriver );
        cppdb::session cppdbSession( "mocksql:" );
        cppdb::statement cppdbStatement1( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 1" ) );
        cppdb::statement cppdbStatement2( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 2" ) );

        LHModelDbUtilNS::BindToStatement( exModelAs.begin(), exModelAs.begin() + 1, cppdbStatement1 );
        LHModelDbUtilNS::BindToStatement( exModelAs.begin(), exModelAs.end(), cppdbStatement2 );
    }

    TEST( TestLHModelCppdb, TestBindMembersToStatementSingle )
    {
        ExampleModelA exModelA1;
        exModelA1.pid = 9999999;
        exModelA1.token = "abc";
        exModelA1.enabled = false;

        ExampleModelA exModelA2;
        exModelA2.pid = 88;
        exModelA2.token = "ggg";
        exModelA2.enabled = true;

        std::vector< std::string > noMembers;
        std::vector< std::string > allMembers;
        std::vector< std::string > someMembers{ "pid", "token" };

        for ( auto it = LHModelNS::ModelMembersMeta< ExampleModelA >::membersMeta.cbegin();
            it != LHModelNS::ModelMembersMeta< ExampleModelA >::membersMeta.cend();
            ++it )
        {
            allMembers.push_back( it->name );
        }

        std::unique_ptr< MockDriver > mockDriver( new MockDriver() );
        EXPECT_CALL( *mockDriver, open( ::testing::_ ) )
            .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const cppdb::connection_info& ci )
        {
            std::unique_ptr< MockConnection > mockConnection( new MockConnection( ci ) );
            EXPECT_CALL( *mockConnection, create_statement( ::testing::_ ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, bind( 1, ::testing::TypedEq< decltype( exModelA1.pid ) >( exModelA1.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 2, ::testing::TypedEq< int >( 1234 ) ) );
                EXPECT_CALL( *mockStatement, bind( 3, ::testing::TypedEq< const std::string& >( exModelA1.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 4, ::testing::TypedEq< int >( exModelA1.enabled ) ) );
                return mockStatement.release();
            } ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, bind( 1, ::testing::TypedEq< decltype( exModelA2.pid ) >( exModelA2.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 2, ::testing::TypedEq< const std::string& >( exModelA2.token ) ) );
                return mockStatement.release();
            } ) );

            return mockConnection.release();
        } ) );

        InstallMockDriverGuard mockDriverGuard( "mocksql", mockDriver );
        cppdb::session cppdbSession( "mocksql:" );
        cppdb::statement cppdbStatement1( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 1" ) );
        cppdb::statement cppdbStatement2( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 2" ) );

        LHModelDbUtilNS::BindMembersToStatement( exModelA1,
            allMembers.cbegin(),
            allMembers.cend(),
            cppdbStatement1 );

        LHModelDbUtilNS::BindMembersToStatement( exModelA1,
            noMembers.cbegin(), noMembers.cend(),
            cppdbStatement2 );

        LHModelDbUtilNS::BindMembersToStatement( exModelA2,
            someMembers.cbegin(), someMembers.cend(),
            cppdbStatement2 );
    }

    TEST( TestLHModelCppdb, TestBindMembersToStatementRange )
    {
        std::vector< ExampleModelA > exModelAs( 2 );

        ExampleModelA& exModelA1( exModelAs.at( 0 ) );
        exModelA1.pid = 9999999;
        exModelA1.token = "abc";
        exModelA1.enabled = false;

        ExampleModelA& exModelA2( exModelAs.at( 1 ) );
        exModelA2.pid = 88;
        exModelA2.token = "ggg";
        exModelA2.enabled = true;

        std::vector< std::string > noMembers;
        std::vector< std::string > someMembers{ "pid", "token" };

        std::unique_ptr< MockDriver > mockDriver( new MockDriver() );
        EXPECT_CALL( *mockDriver, open( ::testing::_ ) )
            .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const cppdb::connection_info& ci )
        {
            std::unique_ptr< MockConnection > mockConnection( new MockConnection( ci ) );
            EXPECT_CALL( *mockConnection, create_statement( ::testing::_ ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                return mockStatement.release();
            } ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, bind( 1, ::testing::TypedEq< decltype( exModelA1.pid ) >( exModelA1.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 2, ::testing::TypedEq< const std::string& >( exModelA1.token ) ) );
                EXPECT_CALL( *mockStatement, bind( 3, ::testing::TypedEq< decltype( exModelA2.pid ) >( exModelA2.pid ) ) );
                EXPECT_CALL( *mockStatement, bind( 4, ::testing::TypedEq< const std::string& >( exModelA2.token ) ) );
                return mockStatement.release();
            } ) );

            return mockConnection.release();
        } ) );

        InstallMockDriverGuard mockDriverGuard( "mocksql", mockDriver );
        cppdb::session cppdbSession( "mocksql:" );
        cppdb::statement cppdbStatement1( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 1" ) );
        cppdb::statement cppdbStatement2( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 2" ) );

        LHModelDbUtilNS::BindMembersToStatement( exModelAs.begin(), exModelAs.begin() + 1,
            noMembers.cbegin(), noMembers.cend(),
            cppdbStatement1 );
        LHModelDbUtilNS::BindMembersToStatement( exModelAs.begin(), exModelAs.end(),
            someMembers.cbegin(), someMembers.cend(),
            cppdbStatement2 );
    }

    namespace
    {
        int nameToIndex( const std::string& name )
        {
            if ( name == "pid" )
                return 0;
            else if ( name == "pit" )
                return 1;
            else if ( name == "token" )
                return 2;
            else if ( name == "enabled" )
                return 3;

            return -1;
        }
    }

    TEST( TestLHModelCppdb, TestQueryFromStatementSingle )
    {
        ExampleModelA exModelA1;
        ExampleModelA exModelA2;

        std::unique_ptr< MockDriver > mockDriver( new MockDriver() );
        EXPECT_CALL( *mockDriver, open( ::testing::_ ) )
            .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const cppdb::connection_info& ci )
        {
            std::unique_ptr< MockConnection > mockConnection( new MockConnection( ci ) );
            EXPECT_CALL( *mockConnection, create_statement( ::testing::_ ) )
                .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, query() )
                    .WillOnce( ::testing::Invoke( [&exModelA1, &exModelA2]()
                {
                    std::unique_ptr< MockResult > mockResult( new MockResult() );

                    EXPECT_CALL( *mockResult, name_to_column( ::testing::_ ) )
                        .Times( 8 )
                        .WillRepeatedly( ::testing::Invoke( nameToIndex ) );

                    EXPECT_CALL( *mockResult, fetch( 0,
                        ::testing::SafeMatcherCast< unsigned long& >(
                            ::testing::Ref( exModelA1.pid ) ) ) )
                        .WillOnce( ::testing::Return( true ) );

                    EXPECT_CALL( *mockResult, fetch( 2,
                        ::testing::SafeMatcherCast< std::string& >(
                            ::testing::Ref( exModelA1.token ) ) ) )
                        .WillOnce( ::testing::Return( true ) );

                    EXPECT_CALL( *mockResult, fetch( 0,
                        ::testing::SafeMatcherCast< unsigned long& >(
                            ::testing::Ref( exModelA2.pid ) ) ) )
                        .WillOnce( ::testing::Return( true ) );

                    EXPECT_CALL( *mockResult, fetch( 2,
                        ::testing::SafeMatcherCast< std::string& >(
                            ::testing::Ref( exModelA2.token ) ) ) )
                        .WillOnce( ::testing::Return( true ) );

                    EXPECT_CALL( *mockResult, fetch( 1, ::testing::A< std::string& >() ) )
                        .Times( 2 )
                        .WillRepeatedly( ::testing::Return( true ) );

                    EXPECT_CALL( *mockResult, fetch( 3, ::testing::An< int& >() ) )
                        .Times( 2 )
                        .WillRepeatedly( ::testing::Return( true ) );

                    EXPECT_CALL( *mockResult, next() )
                        .Times( 2 )
                        .WillRepeatedly( ::testing::Return( true ) );

                    return mockResult.release();
                } ) );

                return mockStatement.release();
            } ) );

            return mockConnection.release();
        } ) );

        InstallMockDriverGuard mockDriverGuard( "mocksql", mockDriver );
        cppdb::session cppdbSession( "mocksql:" );
        cppdb::statement cppdbStatement1( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 1" ) );
        cppdb::result cppdbResult1( cppdbStatement1.query() );

        if ( cppdbResult1.next() )
            LHModelDbUtilNS::FetchFromResult( exModelA1, cppdbResult1 );
        if ( cppdbResult1.next() )
            LHModelDbUtilNS::FetchFromResult( exModelA2, cppdbResult1 );
    }

    TEST( TestLHModelCppdb, TestQueryFromStatementMultiple )
    {
        std::vector< ExampleModelA > exModelAs;

        std::unique_ptr< MockDriver > mockDriver( new MockDriver() );
        EXPECT_CALL( *mockDriver, open( ::testing::_ ) )
            .WillOnce( ::testing::Invoke( []( const cppdb::connection_info& ci )
        {
            std::unique_ptr< MockConnection > mockConnection( new MockConnection( ci ) );
            EXPECT_CALL( *mockConnection, create_statement( ::testing::_ ) )
                .WillOnce( ::testing::Invoke( []( const std::string& statementStr )
            {
                (void)statementStr;
                std::unique_ptr< MockStatement > mockStatement( new MockStatement() );
                EXPECT_CALL( *mockStatement, query() )
                    .WillOnce( ::testing::Invoke( []()
                {
                    std::unique_ptr< MockResult > mockResult( new MockResult() );

                    EXPECT_CALL( *mockResult, name_to_column( ::testing::_ ) )
                        .Times( 8 )
                        .WillRepeatedly( ::testing::Invoke( nameToIndex ) );

                    EXPECT_CALL( *mockResult, fetch( 0, ::testing::An< unsigned long& >() ) )
                        .WillOnce( ::testing::DoAll( ::testing::SetArgReferee<1>( 123 ), ::testing::Return( true ) ) )
                        .WillOnce( ::testing::DoAll( ::testing::SetArgReferee<1>( 456 ), ::testing::Return( true ) ) );

                    EXPECT_CALL( *mockResult, fetch( 1, ::testing::A< std::string& >() ) )
                        .WillOnce( ::testing::Return( true ) )
                        .WillOnce( ::testing::Return( true ) );

                    EXPECT_CALL( *mockResult, fetch( 2, ::testing::A< std::string& >() ) )
                        .WillOnce( ::testing::DoAll( ::testing::SetArgReferee<1>( "a" ), ::testing::Return( true ) ) )
                        .WillOnce( ::testing::DoAll( ::testing::SetArgReferee<1>( "b" ), ::testing::Return( true ) ) );

                    EXPECT_CALL( *mockResult, fetch( 3, ::testing::An< int& >() ) )
                        .WillOnce( ::testing::DoAll( ::testing::SetArgReferee<1>( 1 ), ::testing::Return( true ) ) )
                        .WillOnce( ::testing::DoAll( ::testing::SetArgReferee<1>( 0 ), ::testing::Return( true ) ) );

                    EXPECT_CALL( *mockResult, next() )
                        .WillOnce( ::testing::Return( true ) )
                        .WillOnce( ::testing::Return( true ) )
                        .WillOnce( ::testing::Return( false ) );

                    return mockResult.release();
                } ) );

                return mockStatement.release();
            } ) );

            return mockConnection.release();
        } ) );

        InstallMockDriverGuard mockDriverGuard( "mocksql", mockDriver );
        cppdb::session cppdbSession( "mocksql:" );
        cppdb::statement cppdbStatement1( cppdbSession.create_statement( "NOT A REAL STATEMENT CASE 1" ) );
        cppdb::result cppdbResult1( cppdbStatement1.query() );

        auto outputIt( std::back_inserter( exModelAs ) );
        LHModelDbUtilNS::FetchFromResult< ExampleModelA, decltype( outputIt ) >( outputIt, cppdbResult1 );

        ASSERT_EQ( 2, exModelAs.size() );

        ASSERT_EQ( 123, exModelAs.front().pid );
        ASSERT_STREQ( "a", exModelAs.front().token.c_str() );
        ASSERT_EQ( true, exModelAs.front().enabled );

        ASSERT_EQ( 456, exModelAs.back().pid );
        ASSERT_STREQ( "b", exModelAs.back().token.c_str() );
        ASSERT_EQ( false, exModelAs.back().enabled );
    }
}
