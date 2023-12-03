#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct",
    SCONS_CXX_STANDARD="c++20")

env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

library = env.SharedLibrary(
    "bin/libnai{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
