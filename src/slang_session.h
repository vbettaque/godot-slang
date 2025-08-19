#ifndef SLANGSESSION_H
#define SLANGSESSION_H

#include <slang.h>
#include <slang-com-ptr.h>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>

#include "slang_program.h"

namespace godot {

class SlangSession : public Object {
	GDCLASS(SlangSession, Object)

private:
	Slang::ComPtr<slang::IGlobalSession> global_session;
    Slang::ComPtr<slang::ISession> session;
	void initialize_global_session();
	void initialize_session();
	bool is_global_session_initialized;
	bool is_initialized;
protected:
	static void _bind_methods();

public:
	// SlangProgram *load_program(
	// 	const String &p_module_name,
	// 	const TypedArray<String> &p_entry_point_names
	// );
	Ref<RDShaderSPIRV> load_shader(
		const String &p_module_name,
		const String &p_entry_point_name
	);
	SlangSession();
	~SlangSession();
};

}

#endif