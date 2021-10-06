#include <catch2/catch.hpp>
#include "fty/db/connection.h"
#include <iostream>

TEST_CASE("Query")
{
    fty::db::Connection conn;
    try {
        conn.execute(R"(CREATE TABLE test (
            id   INT UNSIGNED   NOT NULL AUTO_INCREMENT,
            name VARCHAR(50)    NOT NULL,
            PRIMARY KEY (id)
        ))");

        using namespace fty::db;
        auto cnt = conn.execute("INSERT INTO test VALUES(:id, :name)", "id"_p = 1, "name"_p = "aaaaa");
        CHECK(cnt == 1);
    } catch (const fty::db::Error& err) {
        FAIL(err.what());
    }
    std::cerr << "Done" << std::endl;

    auto res = conn.select("SELECT * FROM test");
    REQUIRE(res.size() == 1);
    for(const auto& row: res) {
        CHECK(row.get<int>("id") == 1);
        CHECK(row.get("name") == "aaaaa");
    }
}
