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

#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/chat_message_entity.h"
#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/chat_message.pb.h"
#include "proto_cc/file.pb.h"
#include "proto_cc/pagination.pb.h"

namespace e8 {

/**
 * @brief The SendChatMessageResult struct Contains the sent message and the file location accesses
 * requested from the SendChatMessage()'s arguments.
 */
struct SendChatMessageResult {
    ChatMessageEntry message;
    std::vector<FileTokenAccess> binary_file_location_accesses;
    std::vector<FileTokenAccess> media_file_location_accesses;
};

/**
 * @brief SendChatMessage Send a chat message entry to the specified chat message group which
 * belongs to a message channel. People in that message channel will be notified and receive the
 * chat message. The sender will be checked against the access rules provided by the PBAC.
 *
 * TOOD: Add supports for handling binary files and media files.
 *
 * @param sender_id ID of the sender (user) of this chat message.
 * @param group_id ID of the chat message group created in a chat message channel.
 * @param texts A list of text to be sent, if any.
 * @param media_file_formats Requests a list of media file location access of the specified file
 * formats, if any.
 * @param binary_file_formats Requests a list of general binary file location access of the
 * specified file formats, if any.
 * @param pbac Policy based access controller for the associated message channel.
 * @param key_gen Key generator for signing the avatar path as well as file paths associated with
 * the chat message.
 * @param conns Database connections.
 * @return The sent messages and corresponding file location accesses if the group ID is valid and
 * the sender pass the evaluation provided by the PBAC.
 */
std::optional<SendChatMessageResult> SendChatMessage(
    UserId const sender_id, ChatMessageGroupId const group_id,
    std::vector<std::string> const &texts, std::vector<FileFormat> const &media_file_formats,
    std::vector<FileFormat> const &binary_file_formats, MessageChannelPbacInterface *pbac,
    KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns);

/**
 * @brief GetChatMessages Get chat message entries from the specified chat message group which
 * belongs to a message channel. The viewer will be checked against the access rules provided by the
 * PBAC in that message channel.
 *
 * @param viewer_id ID of the viewer attempted to read from the chat message group.
 * @param group_id ID of the chat message group to read from.
 * @param pagination Optionally paginate the message entries.
 * @param pbac Policy based access controller for the associated message channel.
 * @param key_gen Key generator for signing the avatar path as well as file paths associated with
 * the chat message.
 * @param conns Database connections.
 * @return The message entries returned based on the criteria specified by the arguments. If the
 * message group doesn't exist or the viewer doesn't have the privilege to read from the message
 * group, it will return an empty list.
 */
std::vector<ChatMessageEntry>
GetChatMessages(UserId const viewer_id, ChatMessageGroupId const group_id,
                std::optional<Pagination> const &pagination, MessageChannelPbacInterface *pbac,
                KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns);

} // namespace e8

#endif // CHAT_MESSAGE_H
