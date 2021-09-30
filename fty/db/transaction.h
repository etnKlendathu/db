#pragma once
#include <memory>

namespace fty::db {

// =========================================================================================================================================

class Connection;
class Transaction
{
public:
    Transaction(Connection& con);
    ~Transaction();

    void commit();
    void rollback();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

// =========================================================================================================================================

} // namespace fty::db
