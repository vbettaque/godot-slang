#ifndef SLANGPIMPORTLUGIN_H
#define SLANGIMPORTPLUGIN_H

#include <godot_cpp/classes/editor_import_plugin.hpp>

namespace godot {

class SlangImportPlugin : public EditorImportPlugin {
	GDCLASS(SlangImportPlugin, EditorImportPlugin)

private:
	
	
protected:

public:
	SlangImportPlugin();
	~SlangImportPlugin();
};

}

#endif