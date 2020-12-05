/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#include <chrono>

#include "common/time_util/time_util.h"

namespace e8 {
namespace {

template <typename Precision, typename Timestamp> Timestamp CurrentEpoch() {
    auto now = std::chrono::high_resolution_clock::now();
    auto casted = std::chrono::time_point_cast<Precision>(now);
    auto dura = std::chrono::duration_cast<Precision>(casted.time_since_epoch());
    return static_cast<Timestamp>(dura.count());
}

} // namespace

TimestampSecs CurrentTimestampSecs() { return CurrentEpoch<std::chrono::seconds, TimestampSecs>(); }

TimestampMillis CurrentTimestampMillis() {
    return CurrentEpoch<std::chrono::milliseconds, TimestampMillis>();
}

TimestampMicros CurrentTimestampMicros() {
    return CurrentEpoch<std::chrono::microseconds, TimestampMicros>();
}

} // namespace e8
