#pragma once

#include "alphazero/neural_net/neural_net.h"
#include "alphazero/evaluation/competition_result.h"
#include "alphazero/evaluation/evaluator_options.h"

namespace alphazero
{
	namespace evaluation
	{
		class Evaluator
		{
		public:
			void ClearCompetitionResult() {
				result_.Clear();
			}

			// Thread safety: No
			void RunOnce(neural_net::NeuralNet const& baseline, neural_net::NeuralNet const& competitor, EvaluatorRunOptions const& options) {
				// TODO: compete several games
			}

			CompetitionResult GetCompetitionResult()
			{
				// TODO: fill result_
				return result_;
			}

		private:
			CompetitionResult result_;
		};
	}
}