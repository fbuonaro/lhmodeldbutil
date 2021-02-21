#ifndef __MOCK_CPPDB_BACKEND_H__
#define __MOCK_CPPDB_BACKEND_H__

#include <cppdb/backend.h>
#include <cppdb/utils.h>

#include <gmock/gmock.h>

#include <memory>

namespace MockCppdbBackendNS
{
    class MockConnection : public cppdb::backend::connection
    {
        public:
            MockConnection( const cppdb::connection_info& ci );
            MOCK_METHOD0( begin,  void () );
            MOCK_METHOD0( commit,  void () );
            MOCK_METHOD0( rollback,  void () );
            MOCK_METHOD1( prepare_statement,  cppdb::backend::statement* (std::string const &) );
            MOCK_METHOD1( create_statement,  cppdb::backend::statement* (std::string const &) );
            MOCK_METHOD1( escape,  std::string (std::string const &) );
            MOCK_METHOD1( escape,  std::string (char const *) );
            MOCK_METHOD2( escape,  std::string (char const *, char const *) );
            MOCK_METHOD0( driver,  std::string () );
            MOCK_METHOD0( engine,  std::string () );
    };

    class MockStatement : public cppdb::backend::statement
    {
        public:
            MOCK_METHOD0( reset,  void () );
            MOCK_METHOD0( sql_query,  std::string const& () );
            MOCK_METHOD2( bind,  void (int, std::string const &) );
            MOCK_METHOD2( bind,  void (int, char const *) );
            MOCK_METHOD3( bind,  void (int, char const *, char const *) );
            MOCK_METHOD2( bind,  void (int, std::tm const &) );
            MOCK_METHOD2( bind,  void (int, std::istream &) );
            MOCK_METHOD2( bind,  void (int, int) );
            MOCK_METHOD2( bind,  void (int, unsigned) );
            MOCK_METHOD2( bind,  void (int, long) );
            MOCK_METHOD2( bind,  void (int, unsigned long) );
            MOCK_METHOD2( bind,  void (int, long long) );
            MOCK_METHOD2( bind,  void (int, unsigned long long) );
            MOCK_METHOD2( bind,  void (int, double) );
            MOCK_METHOD2( bind,  void (int, long double) );
            MOCK_METHOD1( bind_null,  void (int) );
            MOCK_METHOD1( sequence_last,  long long (std::string const &) );
            MOCK_METHOD0( affected,  unsigned long long () );
            MOCK_METHOD0( query,  cppdb::backend::result* () );
            MOCK_METHOD0( exec,  void () );
    };

    class MockResult : public cppdb::backend::result
    {
        public:
            MOCK_METHOD0( has_next,  next_row () );
            MOCK_METHOD0( next,  bool () );
            MOCK_METHOD2( fetch,  bool (int, short&) );
            MOCK_METHOD2( fetch,  bool (int, unsigned short&) );
            MOCK_METHOD2( fetch,  bool (int, int&) );
            MOCK_METHOD2( fetch,  bool (int, unsigned&) );
            MOCK_METHOD2( fetch,  bool (int, long&) );
            MOCK_METHOD2( fetch,  bool (int, unsigned long&) );
            MOCK_METHOD2( fetch,  bool (int, long long&) );
            MOCK_METHOD2( fetch,  bool (int, unsigned long long&) );
            MOCK_METHOD2( fetch,  bool (int, float&) );
            MOCK_METHOD2( fetch,  bool (int, double&) );
            MOCK_METHOD2( fetch,  bool (int, long double&) );
            MOCK_METHOD2( fetch,  bool (int, std::string&) );
            MOCK_METHOD2( fetch,  bool (int, std::ostream&) );
            MOCK_METHOD2( fetch,  bool (int, std::tm&) );
            MOCK_METHOD1( is_null,  bool (int) );
            MOCK_METHOD0( cols,  int () );
            MOCK_METHOD1( name_to_column,  int (std::string const &) );
            MOCK_METHOD1( column_to_name,  std::string (int) );
    };

    class MockDriver : public cppdb::backend::driver
    {
        public:
            MockDriver();

            MOCK_METHOD1( open,  cppdb::backend::connection* ( const cppdb::connection_info& ) );
            MOCK_METHOD0( in_use,  bool () );
    };

    class InstallMockDriverGuard
    {
        public:
            InstallMockDriverGuard( const std::string& backendName, std::unique_ptr< MockDriver >& mockDriver );
            ~InstallMockDriverGuard();
    };
}

#endif
