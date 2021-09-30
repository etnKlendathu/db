#pragma once
#include "fty/convert.h"
#include <any>
#include <fmt/format.h>
#include <fty/db/utils.h>
#include <fty/string-utils.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <typeindex>
#include <variant>
#include <vector>

namespace fty::orm {

template <class T, class F>
inline std::pair<const std::type_index, std::function<std::string(std::any const&)>> toAnyVisitor(const F& f)
{
    return {std::type_index(typeid(T)), [g = f](const std::any& a) {
                if constexpr (std::is_null_pointer_v<T> || std::is_void_v<T>) {
                    return g();
                } else {
                    return g(std::any_cast<const T&>(a));
                }
            }};
}

class Condition;
class ConditionGroup;

using CondType = std::variant<Condition, ConditionGroup>;

class Condition
{
public:
    enum class Operator
    {
        Equal
    };

    Condition(const std::string& name, Operator op, const std::any& val)
        : m_name(name)
        , m_op(op)
        , m_val(val)
    {
    }

    std::string toString() const
    {
        static std::unordered_map<std::type_index, std::function<std::string(std::any const&)>> anyVisitor{
            toAnyVisitor<std::nullptr_t>([] {
                return "IS NULL";
            }),
            toAnyVisitor<void>([] {
                return "IS NULL";
            }),
            toAnyVisitor<int>([](int x) {
                return fty::convert<std::string>(x);
            }),
            toAnyVisitor<unsigned>([](unsigned x) {
                return fty::convert<std::string>(x);
            }),
            toAnyVisitor<float>([](float x) {
                return fty::convert<std::string>(x);
            }),
            toAnyVisitor<double>([](double x) {
                return fty::convert<std::string>(x);
            }),
            toAnyVisitor<char const*>([](char const* s) {
                return fmt::format("'{}'", s);
            }),
            toAnyVisitor<const std::string&>([](const std::string& s) {
                return fmt::format("'{}'", s);
            }),
            toAnyVisitor<const fty::db::details::Arg&>([](const fty::db::details::Arg& s) {
                return fmt::format("{}", s.str);
            }),
        };

        std::string val = anyVisitor[m_val.type()](m_val);

        std::string op = [&]() {
            switch (m_op) {
                case Operator::Equal:
                    return "=";
            }
        }();

        return fmt::format("{} {} {}", m_name, op, val);
    }

private:
    std::string m_name;
    Operator    m_op;
    std::any    m_val;
};

class ConditionGroup
{
public:
    enum class Operator
    {
        Or,
        And
    };

    ConditionGroup(const ConditionGroup& other)
        : m_conditions(other.m_conditions)
        , m_op(other.m_op)
    {
    }

    ConditionGroup(const Condition& l, const Condition& r, Operator op)
        : m_conditions({l, r})
        , m_op(op)
    {
    }

    ConditionGroup(const ConditionGroup& l, const Condition& r, Operator op)
        : m_conditions({l, r})
        , m_op(op)
    {
    }

    std::string toString() const
    {
        std::string op = [&]() {
            switch (m_op) {
                case Operator::Or:
                    return " OR ";
                case Operator::And:
                    return " AND ";
            }
        }();

        return fmt::format("({})", implode(m_conditions, op, [](const auto& cond) {
                               return std::visit(
                                   [](const auto& arg) -> std::string {
                                       return arg.toString();
                                   },
                                   cond);
                           }));
    }

    Operator op() const
    {
        return m_op;
    }

    ConditionGroup& append(const Condition& cond)
    {
        m_conditions.push_back(cond);
        return *this;
    }

private:
    std::vector<CondType> m_conditions;
    Operator              m_op;
};

ConditionGroup operator||(const Condition& l, const Condition& r)
{
    return ConditionGroup(l, r, ConditionGroup::Operator::Or);
}

ConditionGroup operator||(const ConditionGroup& l, const Condition& r)
{
    if (l.op() == ConditionGroup::Operator::Or) {
        ConditionGroup g(l);
        g.append(r);
        return g;
    }
    return ConditionGroup(l, r, ConditionGroup::Operator::Or);
}

ConditionGroup operator&&(const Condition& l, const Condition& r)
{
    return ConditionGroup(l, r, ConditionGroup::Operator::And);
}

ConditionGroup operator&&(const ConditionGroup& l, const Condition& r)
{
    if (l.op() == ConditionGroup::Operator::And) {
        ConditionGroup g(l);
        g.append(r);
        return g;
    }
    return ConditionGroup(l, r, ConditionGroup::Operator::And);
}

class Where
{
public:
    Where& set(const Condition& cond)
    {
        m_conditions.push_back(cond);
        return *this;
    }

    Where& set(const ConditionGroup& cond)
    {
        m_conditions.push_back(cond);
        return *this;
    }

    std::string toString() const
    {
        return implode(m_conditions, "AND", [](const auto& cond) {
            return std::visit(
                [](const auto& arg) -> std::string {
                    return arg.toString();
                },
                cond);
        });
    }

    operator bool() const
    {
        return !m_conditions.empty();
    }

private:
    std::vector<CondType> m_conditions;
};

template <typename Q>
class Query
{
public:
    virtual ~Query() = default;

    virtual std::string toString() const
    {
        return "query";
    }

    template <typename... T>
    Q& where(const T&... cond)
    {
        m_where.set(cond...);
        return static_cast<Q&>(*this);
    }

protected:
    Where m_where;
};

class Select : public Query<Select>
{
public:
    Select(const std::vector<std::string>& fields, const std::string& table)
        : m_fields(fields)
        , m_table(table)
    {
    }

    std::string toString() const override
    {
        auto sql = fmt::format("SELECT {} FROM {}", implode(m_fields, ", "), m_table);
        if (m_where) {
            sql += " WHERE " + m_where.toString();
        }
        return sql;
    }

private:
    std::vector<std::string> m_fields;
    std::string              m_table;
};

} // namespace fty::orm
