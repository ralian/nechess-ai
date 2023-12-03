#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class NAIMeta : public Node {
	GDCLASS(NAIMeta, Node)

protected:
	static void _bind_methods();

public:
	static String get_version() { return String{"v0.1.0"}; }
};

};
