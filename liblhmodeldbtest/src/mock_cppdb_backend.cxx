#include <lhmodeldbtest/mock_cppdb_backend.h>

#include <cppdb/driver_manager.h>

namespace MockCppdbBackendNS
{
    MockConnection::MockConnection( const cppdb::connection_info& ci )
    :   cppdb::backend::connection( ci )
    {
    }

    MockDriver::MockDriver()
    {
        // so that it is always removed by collect_unused()
        ON_CALL( *this, in_use() ).WillByDefault( ::testing::Return( false ) );
    }

    InstallMockDriverGuard::InstallMockDriverGuard( const std::string& backendName,
                                                    std::unique_ptr< MockDriver >& mockDriver )
    {
        cppdb::driver_manager::instance().install_driver( backendName, mockDriver.get() );
        mockDriver.release();
    }

    InstallMockDriverGuard::~InstallMockDriverGuard()
    {
        cppdb::driver_manager::instance().collect_unused();
    }
}
