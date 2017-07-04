#pragma once

#include <optional>
#include "MCTS/Types.h"

namespace mcts
{
	namespace board
	{
		class ActionParameterGetter;
		class RandomGenerator;

		class BoardActionAnalyzer
		{
		private:
			enum MainActions
			{
				kActionPlayCard,
				kActionAttack,
				kActionHeroPower,
				kActionEndTurn,
				kActionMax // Should be at last
			};

		public:
			int GetActionsCount(state::State const& board);
			Result ApplyAction(
				state::State & board,
				int action,
				RandomGenerator & random,
				ActionParameterGetter & action_parameters);

		private:
			Result PlayCard(state::State & board, RandomGenerator & random, ActionParameterGetter & action_parameters) const;
			Result Attack(state::State & board, RandomGenerator & random, ActionParameterGetter & action_parameters);
			Result HeroPower(state::State & board, RandomGenerator & random, ActionParameterGetter & action_parameters) const;
			Result EndTurn(state::State & board, RandomGenerator & random, ActionParameterGetter & action_parameters) const;

		private:
			std::optional<std::vector<int>> attackers_;
		};
	}
}