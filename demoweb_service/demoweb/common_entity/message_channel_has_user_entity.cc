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

#include "demoweb_service/demoweb/common_entity/message_channel_has_user_entity.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

MessageChannelHasUserEntity::MessageChannelHasUserEntity()
    : SqlEntityInterface({&channel_id, &user_id, &ownership, &created_at, &last_interaction_at}) {}

MessageChannelHasUserEntity::MessageChannelHasUserEntity(MessageChannelHasUserEntity const &other)
    : MessageChannelHasUserEntity() {
    channel_id = other.channel_id;
    user_id = other.user_id;
    ownership = other.ownership;
    created_at = other.created_at;
    last_interaction_at = other.last_interaction_at;
}

MessageChannelHasUserEntity &
MessageChannelHasUserEntity::operator=(MessageChannelHasUserEntity const &other) {
    channel_id = other.channel_id;
    user_id = other.user_id;
    ownership = other.ownership;
    created_at = other.created_at;
    last_interaction_at = other.last_interaction_at;
    return *this;
}

} // namespace e8
