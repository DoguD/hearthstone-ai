#include "engine/Game-impl.h"

#include <stdlib.h>

#include <chrono>
#include <iostream>
#include <random>
#include <sstream>

#include "Cards/PreIndexedCards.h"
#include "TestStateBuilder.h"
#include "judge/Judger.h"
#include "agents/MCTSAgent.h"

static void Initialize(unsigned int rand_seed)
{
	std::cout << "Reading json file...";
	if (!Cards::Database::GetInstance().Initialize("cards.json")) assert(false);
	Cards::PreIndexedCards::GetInstance().Initialize();
	std::cout << " Done." << std::endl;

	srand(rand_seed);
}

class AgentCallback
{
public:
	AgentCallback(int max_iterations) :
		first_time_(true), max_iterations_(max_iterations), last_shown_()
	{}

	void BeforeThink(engine::view::BoardRefView const& board_view) {
		first_time_ = true;
	}

	void Thinking(engine::view::BoardRefView const& board_view, uint64_t iteration) {
		if (first_time_) {
			std::cout << "Turn: " << board_view.GetTurn() << std::endl;
			last_shown_ = std::chrono::steady_clock::now();
			first_time_ = false;
		}

		auto now = std::chrono::steady_clock::now();
		auto after_last_shown = std::chrono::duration_cast<std::chrono::seconds>(now - last_shown_).count();
		if (after_last_shown > 5) {
			double percent = (double)iteration / max_iterations_;
			std::cout << "Iterations: " << iteration << " (" << percent * 100.0 << "%)" << std::endl;
			last_shown_ = now;
		}
	}

	void AfterThink(uint64_t iteration) {
		std::cout << "Total iterations: " << iteration << std::endl;
	}

private:
	bool first_time_;
	uint64_t max_iterations_;
	std::chrono::steady_clock::time_point last_shown_;
};

void SaveJson(Json::Value const& json) {
	time_t now;
	time(&now);

	struct tm timeinfo;
#ifdef _MSC_VER
	localtime_s(&timeinfo, &now);
#else
	localtime_r(&now, &timeinfo);
#endif

	char buffer[80];
	strftime(buffer, 80, "%Y%m%d-%H%M%S", &timeinfo);

	std::ostringstream ss;
	int postfix = rand() % 90000 + 10000;
	ss << buffer << "-" << postfix << ".json";
	std::string filename = ss.str();

	std::ofstream fs(filename, std::ofstream::trunc);
	Json::StyledStreamWriter json_writer;
	json_writer.write(fs, json);
	fs.close();
}

int main(int argc, char *argv[])
{
	auto seed = std::random_device()();

	std::cout << "Initialize with random seed: " << seed << std::endl;
	std::mt19937 rand(seed);

	Initialize(rand());

	if (argc != 3) {
		std::cout << "Usage: "
			<< argv[0]
			<< " (threads)"
			<< " (iterations)"
			<< std::endl;
		return 0;
	}

	agents::MCTSAgentConfig config;
	config.tree_samples = 10;
	config.threads = 1;
	config.iterations_per_action = 1000;
	config.mcts.SetNeuralNetPath("neural_net");

	{
		std::istringstream ss(argv[1]);
		ss >> config.threads;
	}
	{
		std::istringstream ss(argv[2]);
		ss >> config.iterations_per_action;
	}

	std::cout << "Parameters: " << std::endl;
	std::cout << "\tThreads: " << config.threads << std::endl;
	std::cout << "\tIterations: " << config.iterations_per_action << std::endl;
	std::cout << "\tSeed: " << seed << std::endl;

	using MCTSAgent = agents::MCTSAgent<AgentCallback>;
	judge::json::Recorder recorder(rand);
	judge::Judger<MCTSAgent, judge::json::Recorder> judger(rand, recorder);
	MCTSAgent first(config, AgentCallback(config.iterations_per_action));
	MCTSAgent second(config, AgentCallback(config.iterations_per_action));

	judger.SetFirstAgent(&first);
	judger.SetSecondAgent(&second);

	state::State start_state = TestStateBuilder().GetStateWithRandomStartCard(rand(), rand);
	judger.Start(start_state, rand);

	SaveJson(recorder.GetJson());

	return 0;
}
