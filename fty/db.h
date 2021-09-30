#pragma once

#include "db/connection.h"
#include "db/row.h"
#include "db/rows.h"
#include "db/statement.h"
#include "db/transaction.h"

namespace fty::db {

// =====================================================================================================================

//inline std::string multiInsert(std::initializer_list<std::string> cols, size_t count)
//{
//    std::vector<std::string> colsStr;
//    for (const auto& col : cols) {
//        colsStr.push_back(":" + col + "_{0}");
//    }

//    std::string out;
//    for (size_t i = 0; i < count; ++i) {
//        out += (i > 0 ? ", " : "") + fmt::format("(" + fty::implode(colsStr, ", ") + ")", i);
//    }
//    return out;
//}

} // namespace fty::db

