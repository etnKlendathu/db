cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

############################################################################################################################################
project(fty-db
    VERSION 1.0.0
    DESCRIPTION "Provides database tools for agents"
)
############################################################################################################################################
find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR}/fty-cmake)
############################################################################################################################################

etn_target(shared ${PROJECT_NAME}
    PUBLIC
        fty/db.h
        fty/db/connection.h
        fty/db/statement.h
        fty/db/row.h
        fty/db/rows.h
        fty/db/utils.h
        fty/db/transaction.h

        fty/db/orm/table.h
        fty/db/orm/query.h
    SOURCES
        src/db.cpp
        src/connection.cpp
        src/statement.cpp
        src/utils.cpp
        src/rows.cpp
        src/private/connection.cpp
        src/private/connection.h
        src/private/statement.cpp
        src/private/statement.h
        src/private/bind.cpp
        src/private/bind.h
        src/private/rows.cpp
        src/private/rows.h
        src/private/row.cpp
        src/private/row.h
    USES
        fty_common_logging
        mariadb
    USES_PUBLIC
        fty-utils
)

############################################################################################################################################

etn_target(static ${PROJECT_NAME}-embedded
    PUBLIC
        fty/db/embedded.h
    SOURCES
        src/embedded.cpp
    USES
        fty-utils
        mariadbd
)

############################################################################################################################################

etn_test_target(${PROJECT_NAME}
    SOURCES
        test/main.cpp
        test/sql.cpp
        test/query.cpp
        test/orm.cpp
    USES
        pthread
        ${PROJECT_NAME}-embedded
        fty-pack
    SUBDIR
        test
)

############################################################################################################################################
