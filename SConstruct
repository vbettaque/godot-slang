#!/usr/bin/env python
import os
import sys

from methods import print_error

LIB_NAME = "godot-slang"
ADDON_DIR = "demo/addons/{}".format(LIB_NAME)

SLANG_LIB_NAME = "slang"
SLANG_RELEASE_INCLUDE_DIR = "slang/build/Release/include"
SLANG_DEBUG_INCLUDE_DIR = "slang/build/RelWithDebInfo/include"
SLANG_RELEASE_LIB_DIR = "slang/build/Release/lib/"
SLANG_DEBUG_LIB_DIR = "slang/build/RelWithDebInfo/lib/"

################################################################################

local_env = Environment(tools=["default"], PLATFORM="")

# Build profiles can be used to decrease compile times.
# You can either specify "disabled_classes", OR
# explicitly specify "enabled_classes" which disables all other classes.
# Modify the example file as needed and uncomment the line below or
# manually specify the build_profile parameter when running SCons.

# localEnv["build_profile"] = "build_profile.json"

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(local_env)

Help(opts.GenerateHelpText(local_env))

env = local_env.Clone()

if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
    print_error(
        """godot-cpp is not available within this folder, as Git submodules haven't been initialized.
        Run the following command insider the folder to download godot-cpp:

        git submodule update --init --recursive
        
        Then follow the guidelines on how to build it."""
    )
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

if not (os.path.isdir("slang") and os.listdir("slang")):
    print_error(
        """slang is not available within this folder, as Git submodules haven't been initialized.
        Run the following command inside the folder to download slang:

        git submodule update --init --recursive
        
        Then follow the guidelines on how to build it."""
    )
    sys.exit(1)

if env["target"] in ["editor", "template_debug"]:
    env.Append(CPPPATH=[SLANG_DEBUG_INCLUDE_DIR])
    env.Append(LIBPATH=[SLANG_DEBUG_LIB_DIR])
elif env["target"] == "template_release":
    env.Append(CPPPATH=[SLANG_RELEASE_INCLUDE_DIR])
    env.Append(LIBPATH=[SLANG_RELEASE_LIB_DIR])
env.Append(LIBS=[SLANG_LIB_NAME])

# Tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# .dev doesn't inhibit compatibility, so we don't need to key it.
# .universal just means "compatible with all relevant arches" so we don't need to key it.
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")

file_name = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), LIB_NAME, suffix, env.subst('$SHLIBSUFFIX'))

library = env.SharedLibrary(
    "bin/{}/{}".format(env['platform'], file_name),
    source=sources,
)

copy = env.Install("{}/bin/{}/".format(ADDON_DIR, env["platform"]), library)

default_args = [library, copy]
Default(*default_args)