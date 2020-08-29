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

#ifndef USER_ENTITY_H
#define USER_ENTITY_H

#include <cstdint>

#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

using UserId = int64_t;

/**
 * @brief The UserEntity class C++ class representation of the database table "auser".
 */
class UserEntity : public SqlEntityInterface {
  public:
    UserEntity();
    UserEntity(UserEntity const &other);
    ~UserEntity() = default;

    UserEntity &operator=(UserEntity const &other);

    SqlLong id = SqlLong("id");
    SqlStr id_str = SqlStr("id_str");
    SqlStrArr emails = SqlStrArr("emails");
    SqlStr alias = SqlStr("alias");
    SqlStr biography = SqlStr("biography");
    SqlStr avatar_path = SqlStr("avatar_path");
    SqlStr avatar_preview_path = SqlStr("avatar_preview_path");
    SqlStr security_key_hash = SqlStr("security_key_hash");
    SqlStrArr group_names = SqlStrArr("group_names");
    SqlInt active_level = SqlInt("active_level");
    SqlTimestamp created_at = SqlTimestamp("created_at");
};

} // namespace e8

#endif // USER_ENTITY_H
