#ifndef SLANGMODULE_H
#define SLANGMODULE_H

#include <godot_cpp/classes/object.hpp>

#include <slang.h>
#include <slang-com-ptr.h>

#include "slang_session.h"

namespace godot {

class SlangModule : public Object {
    GDCLASS(SlangModule, Object)

private:
    SlangSession *session;
    slang::IModule *module;
    String name;
    TypedArray<String> &entry_point_names;

protected:
    static void _bind_methods();

public:

};

}

#endif