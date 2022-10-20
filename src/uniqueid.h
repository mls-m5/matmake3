#pragma once

using Id = long;
#include <algorithm>

namespace uniqueidimpl {

inline Id id = 0;

}

inline Id uniqueId() {
    return ++uniqueidimpl::id;
}

inline void forceMinId(Id id) {
    uniqueidimpl::id = std::max(id, uniqueidimpl::id);
}
