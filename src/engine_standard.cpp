#include "engine_standard.hpp"

#include <atomic>
#include <chrono>
using std::chrono::high_resolution_clock;

using namespace nai::piece;

void godot::NAIEngineStandard::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_state", "state"),	&NAIEngineStandard::set_state);
	ClassDB::bind_method(D_METHOD("get_best_move"),			&NAIEngineStandard::get_best_move);
	ClassDB::bind_method(D_METHOD("search_moves", "t_max"), &NAIEngineStandard::search_moves);
}

inline float godot::NAIEngineStandard::piece_value(underlying_type p)
{
	switch (p & static_cast<underlying_type>(type::ANY))
	{
	case static_cast<underlying_type>(type::PAWN):
		return 1.0f;
	case static_cast<underlying_type>(type::KNIGHT):
		return 3.0f;
	case static_cast<underlying_type>(type::BISHOP):
		return 3.3f;
	case static_cast<underlying_type>(type::ROOK):
		return 4.6f;
	case static_cast<underlying_type>(type::QUEEN):
		return 8.9f;
	case static_cast<underlying_type>(type::KING):
		return 1000.0f;
	default:
		return 0.0f;
	}
}

inline float godot::NAIEngineStandard::score()
{
	float base_score = 0.0f;

	for (auto column : this->board.guts)
		for (auto piece : column)
			base_score += (nai::piece::is_self_or_ally(piece) ? 1.0f : -1.0f) * piece_value(piece);

	return base_score;
}

void godot::NAIEngineStandard::set_state(String input)
{
	std::array<int, 2> where{};
	nai::piece::underlying_type what;
	if (nai::from_string<2>(input.utf8().get_data(), where, what))
		board.guts[where[0]][where[1]] = what;

	// else throw something
}

godot::String godot::NAIEngineStandard::get_best_move()
{
	return godot::String{};
}

int godot::NAIEngineStandard::search_moves(double t_max)
{
	const auto start = high_resolution_clock::now();
	const std::chrono::duration<double> duration_max{ t_max };

	std::atomic_int states_searched{ 0 }; // no concurrency now, but easier to do atomic now

	do {
		constexpr int burst = 100;

		for (int i = 0; i < burst; i++);

		states_searched += burst;

	} while (duration_max > high_resolution_clock::now() - start);

	return states_searched;
}
