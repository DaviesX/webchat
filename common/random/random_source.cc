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

#include <random>

#include "common/random/random_source.h"

namespace e8 {

RandomSource::RandomSource() : uniform_(0, 1) {
    std::random_device rd;
    unsigned seed = rd();
    engine_.seed(seed);
}

RandomSource::RandomSource(unsigned seed) : engine_(seed), uniform_(0, 1) {}

double RandomSource::Draw() { return uniform_(engine_); }

} // namespace e8
