#ifndef SLANGPROGRAM_H
#define SLANGPROGRAM_H


#include <godot_cpp/classes/object.hpp>

#include "slang_session.h"

namespace godot {

class SlangProgram : public Object {
    GDCLASS(SlangProgram, Object)

private:
    String module_name;
    slang::IModule *module;
    TypedArray<String> &entry_point_names;

protected:
    static void _bind_methods();

public:
    Ref<RDShaderSPIRV> compile_spirv(
        const String &entry_point_compute,
        const String &entry_point_vertex = "",
        const String &entry_point_fragment = "",
        const String &entry_point_tesselation_control = "",
        const String &entry_point_tesselation_evaluation = ""
    );

    SlangProgram();
	~SlangProgram();
};

}

#endif