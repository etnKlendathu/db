#include "src/private/statement.h"
#include <catch2/catch.hpp>
#include <iostream>

TEST_CASE("Sql")
{
    //fty::db::StatementImpl impl("SELECT * FROM test WHERE id=:id");
    {
        std::vector<std::string> pholders;
        auto sql = fty::db::parseQuery("str :p1 next(:some_other)", pholders);
        CHECK(sql == "str ? next(?)");
        REQUIRE(pholders.size() == 2);
        CHECK(pholders[0] == "p1");
        CHECK(pholders[1] == "some_other");
    }

    {
        std::vector<std::string> pholders;
        auto sql = fty::db::parseQuery("str {p1} next({some_other})", pholders);
        CHECK(sql == "str ? next(?)");
        REQUIRE(pholders.size() == 2);
        CHECK(pholders[0] == "p1");
        CHECK(pholders[1] == "some_other");
    }

    {
        std::vector<std::string> pholders;
        auto sql = fty::db::parseQuery("str :p1 next({some_other})", pholders);
        CHECK(sql == "str ? next(?)");
        REQUIRE(pholders.size() == 2);
        CHECK(pholders[0] == "p1");
        CHECK(pholders[1] == "some_other");
    }

    {
        std::vector<std::string> pholders;
        auto sql = fty::db::parseQuery("str next", pholders);
        CHECK(sql == "str next");
        REQUIRE(pholders.size() == 0);
    }

    {
        std::vector<std::string> pholders;
        auto sql = fty::db::parseQuery("str : next", pholders);
        CHECK(sql == "str : next");
        REQUIRE(pholders.size() == 0);
    }

    {
        std::vector<std::string> pholders;
        auto sql = fty::db::parseQuery("str {} next", pholders);
        CHECK(sql == "str {} next");
        REQUIRE(pholders.size() == 0);
    }
}
