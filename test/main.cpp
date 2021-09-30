#define CATCH_CONFIG_RUNNER
#include "fty/db/connection.h"
#include "fty/db/embedded.h"
#include <catch2/catch.hpp>

int main(int argc, char* argv[])
{
    Catch::Session session;

    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) {
        return returnCode;
    }

    Catch::ConfigData data = session.configData();
    if (data.listReporters || data.listTestNamesOnly) {
        return session.run();
    }


    fty::db::Embedded::init("testdb");
    int result = 0;
    try {
        result = session.run(argc, argv);
    } catch (const fty::db::Error& err) {
        std::cerr << err.what() << std::endl;
    }
    fty::db::Embedded::destroy();
    return result;
}
