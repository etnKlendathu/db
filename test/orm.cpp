#include <catch2/catch.hpp>
#include <fty/db/orm/table.h>
#include <iostream>
#include <fty/db/utils.h>

using namespace fty::db;
using namespace fty::orm;

struct Test : public Table<Test>
{
    static constexpr Int32  id   = DBFIELD("id", Option::Autoincrement);
    static constexpr String name = DBFIELD("name", Option::NotNull);

    ORM_META("test", id, name);
};

TEST_CASE("Orm")
{
    auto sql = Test::select(Test::name).where((Test::id == ":id"_p || Test::id == nullptr) && Test::name == "some");
    std::cerr << sql.toString() << std::endl;
}
