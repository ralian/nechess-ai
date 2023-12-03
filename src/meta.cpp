#include "meta.hpp"

void godot::NAIMeta::_bind_methods()
{
	ClassDB::bind_static_method(StringName{"NAIMeta"}, D_METHOD("get_version"), &NAIMeta::get_version);
}

