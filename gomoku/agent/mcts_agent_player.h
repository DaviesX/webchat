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

#ifndef MCTS_AGENT_PLAYER_H
#define MCTS_AGENT_PLAYER_H

#include <memory>
#include <optional>

#include "gomoku/agent/heuristics/evaluator.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/agent/search/mct_search.h"
#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"

namespace e8 {

/**
 * @brief The MctsAgentPlayer class An artificial intelligent agent that uses MCTS to derive optimal
 * board actions.
 */
class MctsAgentPlayer : public GomokuPlayerInterface {
  public:
    /**
     * @brief MctsAgentPlayer Constructs an agent with using an underlying MCTS algorithm.
     *
     * @param player_side The player side the agent represents.
     * @param searcher The search algorithm.
     * @param shared_searcher Whether the search algorithm states are shared by the oppponent as
     * well.
     */
    MctsAgentPlayer(PlayerSide const player_side, std::shared_ptr<MctSearcher> const &searcher,
                    bool shared_searcher);
    ~MctsAgentPlayer() override = default;

    GomokuActionId NextPlayerAction(GomokuBoardState const &board_state) override;

    void BeforeGomokuActionApplied(GomokuBoardState const &board_state,
                                   PlayerSide const action_performed_by,
                                   GomokuActionId const &incoming_action_id) override;

    virtual void OnGomokuGameBegin(GomokuBoardState const &board_state) override;

    virtual void AfterGomokuActionApplied(GomokuBoardState const &board_state) override;

    virtual void OnGameEnded(GomokuBoardState const &board_state) override;

    virtual bool WantAnotherGame() override;

  private:
    PlayerSide const player_side_;
    std::shared_ptr<MctSearcher> searcher_;
    bool const shared_searcher_;
};

} // namespace e8

#endif // MCTS_AGENT_PLAYER_H
