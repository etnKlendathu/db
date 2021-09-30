#include <catch2/catch.hpp>
#include "fty/db/connection.h"
#include <iostream>

TEST_CASE("Query")
{
    try {
        fty::db::Connection conn;

        conn.execute(R"(CREATE TABLE test (
            id   INT UNSIGNED   NOT NULL AUTO_INCREMENT,
            name VARCHAR(50)    NOT NULL,
            PRIMARY KEY (id)
        ))");

        using namespace fty::db;
        auto cnt = conn.execute("INSERT INTO test VALUES(:id, :name)", "id"_p = 1, "name"_p = "aaaaa");
        CHECK(cnt == 1);

        auto res = conn.select("SELECT * FROM test");
        REQUIRE(res.size() == 1);
    } catch (const fty::db::Error& err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }
    std::cerr << "Done" << std::endl;
}
