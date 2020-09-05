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
#include <cstdint>
#include <exception>
#include <ios>
#include <memory>
#include <string>
#include <unordered_set>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/orm/query_completion.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/resultset/result_set_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace {

int64_t reverse_bytes(int64_t original) {
    return (original & 0xFF) << 56 | ((original >> 8) & 0xFF) << 48 |
           ((original >> 16) & 0xFF) << 40 | ((original >> 24) & 0xFF) << 32 |
           ((original >> 32) & 0xFF) << 24 | ((original >> 40) & 0xFF) << 16 |
           ((original >> 48) & 0xFF) << 8 | ((original >> 56) & 0xFF);
}

} // namespace

uint64_t Update(SqlEntityInterface const &entity, std::string const &table_name, bool override,
                ConnectionReservoirInterface *reservoir) {
    InsertQueryAndParams query_and_params = GenerateInsertQuery(table_name, entity, override);

    ConnectionInterface *conn = reservoir->Take();
    uint64_t numRowsUpdated =
        conn->RunUpdate(query_and_params.query, query_and_params.query_params);
    reservoir->Put(conn);

    return numRowsUpdated;
}

uint64_t Delete(std::string const &table_name, SqlQueryBuilder const &query,
                ConnectionReservoirInterface *reservoir) {
    std::string completed_query = "DELETE FROM " + table_name + " " + query.PsqlQuery();

    ConnectionInterface *conn = reservoir->Take();
    uint64_t numRowsUpdated = conn->RunUpdate(completed_query, query.QueryParams());
    reservoir->Put(conn);

    return numRowsUpdated;
}

bool Exists(SqlQueryBuilder const &query, ConnectionReservoirInterface *reservoir) {
    std::string exists_query = "SELECT TRUE FROM " + query.PsqlQuery();

    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs = conn->RunQuery(exists_query, query.QueryParams());
    bool exists = rs->HasNext();

    reservoir->Put(conn);

    return exists;
}

std::unordered_set<std::string> Tables(ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();
    std::string reflection_query =
        "SELECT tb.table_name FROM information_schema.tables tb WHERE tb.table_schema='public'";
    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery(reflection_query, ConnectionInterface::QueryParams());

    std::unordered_set<std::string> table_names;
    SqlStr table_name("table_name");
    for (; rs->HasNext(); rs->Next()) {
        rs->SetField(0, &table_name);
        assert(table_name.Value().has_value());
        table_names.insert(table_name.Value().value());
    }

    reservoir->Put(conn);

    return table_names;
}

bool SendHeartBeat(ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery("SELECT 1", ConnectionInterface::QueryParams());

    if (!rs->HasNext()) {
        reservoir->Put(conn);
        return false;
    }

    SqlInt one("heart_beat");
    rs->SetField(0, &one);
    if (!one.Value().has_value() || one.Value().value() != 1) {
        reservoir->Put(conn);
        return false;
    }

    reservoir->Put(conn);

    return true;
}

bool SendHeartBeat(ConnectionInterface *conn) {
    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery("SELECT 1", ConnectionInterface::QueryParams());

    if (!rs->HasNext()) {
        return false;
    }

    SqlInt one("heart_beat");
    rs->SetField(0, &one);
    if (!one.Value().has_value() || one.Value().value() != 1) {
        return false;
    }

    return true;
}

int64_t TimeId(unsigned host_id) {
    auto now = std::chrono::high_resolution_clock::now();
    auto micros = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto dura = std::chrono::duration_cast<std::chrono::microseconds>(micros.time_since_epoch());
    int64_t timestamp = dura.count() - 1588490444394000L;
    int64_t unique_id = host_id * 0xFFFFFFFFFFFFF + timestamp;
    return reverse_bytes(unique_id);
}

int64_t SeqId(std::string const &seq_table, ConnectionReservoirInterface *reservoir) {
    ConnectionInterface *conn = reservoir->Take();

    std::unique_ptr<ResultSetInterface> rs =
        conn->RunQuery("SELECT nextval('" + seq_table + "')", ConnectionInterface::QueryParams());
    assert(rs->HasNext());

    SqlLong id("id");
    rs->SetField(0, &id);
    assert(id.Value().has_value());

    reservoir->Put(conn);

    return id.Value().value();
}

void ClearAllTables(ConnectionReservoirInterface *reservoir) {
    std::unordered_set<std::string> table_names = Tables(reservoir);
    SqlQueryBuilder constraint;
    constraint.QueryPiece("CASCADE");
    for (std::string const &tb_name : table_names) {
        Delete(tb_name, constraint, reservoir);
    }
}

} // namespace e8
