#include <godot_cpp/classes/editor_import_plugin.hpp>
#include "slang_editor_plugin.h"
#include "slang_import_plugin.h"

using namespace godot;

void SlangEditorPlugin::_enter_tree() {
    import_plugin.instantiate();
    add_import_plugin(import_plugin);
}

void SlangEditorPlugin::_exit_tree() {
    remove_import_plugin(import_plugin);
    import_plugin.unref();
}