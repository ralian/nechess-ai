#pragma once

#include "engine.hpp"
#include "state.hpp"

namespace godot {

class NAIEngineStandard : public NAIEngine {
	GDCLASS(NAIEngineStandard, NAIEngine)

protected:
	nai::local_board<8, 8> board{};
	static void _bind_methods();
	static inline float piece_value(nai::piece::underlying_type p);
	inline float score();

public:
	void set_state(String input);
	String get_best_move();
	int search_moves(double t_max);
};

};
