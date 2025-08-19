#ifndef SLANGEDITORPLUGIN_H
#define SLANGEDITORPLUGIN_H

#include <godot_cpp/classes/editor_plugin.hpp>
#include "slang_import_plugin.h"

namespace godot {

class SlangEditorPlugin : public EditorPlugin {
	GDCLASS(SlangEditorPlugin, EditorPlugin)

private:
	Ref<SlangImportPlugin> import_plugin;
	
protected:

public:
	SlangEditorPlugin();
	~SlangEditorPlugin();

    void _enter_tree() override;
    void _exit_tree() override;
};

}

#endif