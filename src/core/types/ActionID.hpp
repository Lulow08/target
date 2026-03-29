#pragma once

#include <QString>
#include <QHashFunctions>

namespace core::types {

struct ActionID {
    QString value;

    [[nodiscard]] bool operator==(const ActionID& o) const noexcept {
        return value == o.value;
    }
    [[nodiscard]] bool isValid() const noexcept { return !value.isEmpty(); }
};

} // namespace core::types

// Qt6: seed type is size_t
inline size_t qHash(const core::types::ActionID& key, size_t seed = 0) noexcept {
    return qHash(key.value, seed);
}