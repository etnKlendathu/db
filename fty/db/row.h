#pragma once
#include "utils.h"
#include <memory>
#include <string>

namespace fty::db {

// =========================================================================================================================================

class Row
{
public:
    Row() = default;
    ~Row() = default;

    template <typename T>
    T get(const std::string& col, const details::UseType<T>& defVal = {}) const;

    std::string get(const std::string& col, const std::string& defVal = {}) const;

    template <typename T>
    void get(const std::string& name, T& val) const;

protected:
    std::string getString(const std::string& name, const std::string& defVal = {}) const;
    bool        getBool(const std::string& name, bool defVal = {}) const;
    int8_t      getInt8(const std::string& name, int8_t defVal = {}) const;
    uint8_t     getUint8(const std::string& name, uint8_t defVal = {}) const;
    int16_t     getInt16(const std::string& name, int16_t defVal = {}) const;
    uint16_t    getUint16(const std::string& name, uint16_t defVal = {}) const;
    int32_t     getInt32(const std::string& name, int32_t defVal = {}) const;
    uint32_t    getUint32(const std::string& name, uint32_t defVal = {}) const;
    int64_t     getInt64(const std::string& name, int64_t defVal = {}) const;
    uint64_t    getUint64(const std::string& name, uint64_t defVal = {}) const;
    float       getFloat(const std::string& name, float defVal = {}) const;
    double      getDouble(const std::string& name, double defVal = {}) const;
    bool        isNull(const std::string& name) const;

private:
//    struct Impl;
//    std::shared_ptr<Impl> m_impl;
//    Row(std::shared_ptr<Impl> impl);

    friend class Statement;
    friend class ConstIterator;
    friend class Rows;
};


// =========================================================================================================================================

template <typename T>
inline T Row::get(const std::string& col, const details::UseType<T>& defVal) const
{
    if (isNull(col)) {
        return defVal;
    }

    if constexpr (std::is_same_v<T, std::string>) {
        return getString(col, defVal);
    } else if constexpr (std::is_same_v<T, bool>) {
        return getBool(col, defVal);
    } else if constexpr (std::is_same_v<T, int64_t>) {
        return getInt64(col, defVal);
    } else if constexpr (std::is_same_v<T, int32_t>) {
        return getInt32(col, defVal);
    } else if constexpr (std::is_same_v<T, int16_t>) {
        return getInt16(col, defVal);
    } else if constexpr (std::is_same_v<T, int8_t>) {
        return getInt8(col, defVal);
    } else if constexpr (std::is_same_v<T, uint64_t>) {
        return getUint64(col, defVal);
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        return getUint32(col, defVal);
    } else if constexpr (std::is_same_v<T, uint16_t>) {
        return getUint16(col, defVal);
    } else if constexpr (std::is_same_v<T, uint8_t>) {
        return getUint8(col, defVal);
    } else if constexpr (std::is_same_v<T, float>) {
        return getFloat(col, defVal);
    } else if constexpr (std::is_same_v<T, double>) {
        return getDouble(col, defVal);
    } else {
        static_assert(always_false<T>, "Unsupported type");
    }
}

template <typename T>
inline void Row::get(const std::string& name, T& val) const
{
    val = get<std::decay_t<T>>(name);
}

// =========================================================================================================================================

} // namespace fty::db
