/**
 * e8yes demo web server.
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
#ifndef SQL_QUERY_BUILDER_H
#define SQL_QUERY_BUILDER_H

#include <string>
#include <vector>

#include "sql/connection/connection_interface.h"
#include "sql/reflection/sql_primitive_interface.h"

namespace e8 {

/**
 * @brief The SqlQueryBuilder class Builder class to construct a parameterized SQL query from query
 * string pieces and variable placeholders.
 */
class SqlQueryBuilder {
  public:
    SqlQueryBuilder() = default;
    ~SqlQueryBuilder() = default;
    SqlQueryBuilder(SqlQueryBuilder const &) = delete;

    /**
     * @brief Append a query string piece.
     *
     * @param piece Query string piece
     * @return The current builder.
     */
    SqlQueryBuilder &query_piece(std::string const &piece);

    /**
     * @brief Represents a variable placeholder
     *
     * @param <Type> The type of variable the place holder represents for.
     */
    template <typename Type> class Placeholder {
      private:
        std::vector<ConnectionInterface::QueryParams::SlotId> slots;
    };

    /**
     * @brief Append a variable placeholder.
     *
     * @param holder Variable placeholder
     * @return The current builder.
     */
    template <typename Type> SqlQueryBuilder &placeholder(Placeholder<Type> *holder) {
        holder->slots.add(params_.allocate_slot());
        query_.append("?");
        return *this;
    }

    /**
     * @brief Collectively assigns the value to the holder to all the query positions.
     *
     * @param <Type>
     * @param holder Placeholder to be assigned a value.
     * @param val The value to be assigned.
     */
    template <typename Type>
    void set_value_to_placeholder(Placeholder<Type> const &holder,
                                  SqlPrimitiveInterface const &val) {
        for (auto slot : holder.slots) {
            params_.set_param(slot, val);
        }
    }

    /**
     * @brief Export a postgres-compliant query from the previously appended pieces.
     *
     * @return postgres-compliant query
     */
    std::string psql_query();

    /**
     * @brief Export the assigned query parameters.
     *
     * @return The assigned query parameters
     */
    ConnectionInterface::QueryParams const &query_params() const;

  private:
    std::string query_;
    ConnectionInterface::QueryParams params_;
};

} // namespace e8

#endif // SQL_QUERY_BUILDER_H
