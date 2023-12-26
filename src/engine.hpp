#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

// I'm not sure if we're actually going to use polymorphism here, but we may as well cover our bases.
// I don't think we care about having a virtual destructor here because the way godot stores nodes, the correct destructor will be called.
class NAIEngine : public Node {
	GDCLASS(NAIEngine, Node)

protected:
	static void _bind_methods();

public:
	// This class should never be constructed. Originally these were pure virtual, but that breaks some gdextension things as the macros expect the constructor to have an implementation
	virtual void set_state(String input);
	virtual String get_best_move();
	virtual int search_moves(double t_max);
};

};
