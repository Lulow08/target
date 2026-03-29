#pragma once

#include <QString>
#include <QHashFunctions>

namespace core::types {

struct CommandID {
    QString value;

    [[nodiscard]] bool operator==(const CommandID& o) const noexcept {
        return value == o.value;
    }
    [[nodiscard]] bool isValid() const noexcept { return !value.isEmpty(); }
};

} // namespace core::types

inline size_t qHash(const core::types::CommandID& key, size_t seed = 0) noexcept {
    return qHash(key.value, seed);
}