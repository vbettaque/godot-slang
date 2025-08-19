
#include <godot_cpp/classes/project_settings.hpp>

#include "slang_session.h"

using namespace godot;
using namespace slang;

void godot::SlangSession::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("load_shader", "module_name", "entry_point_name"),
        &SlangSession::load_shader
    );
    // ClassDB::bind_method(
    //     D_METHOD("get_rendering_device"),
    //     &SlangDevice::get_rendering_device
    // );
    // ClassDB::bind_static_method(
    //     "SlangDevice",
    //     D_METHOD("from_rendering_device", "rendering_device"),
    //     &SlangDevice::from_rendering_device
    // );
}


void godot::SlangSession::initialize_global_session() {
    SlangGlobalSessionDesc global_desc;
    SlangResult res = createGlobalSession(
        &global_desc, global_session.writeRef()
    );
    is_global_session_initialized = res >= 0;
    ERR_FAIL_COND_MSG(
        !is_global_session_initialized,
        "Couldn't initialize Slang global session."
    );
}


Ref<RDShaderSPIRV> godot::SlangSession::load_shader(
    const String &p_module_name,
    const String &p_entry_point_name
) {
    Slang::ComPtr<IBlob> diagnostics;
    IModule* module = session->loadModule(p_module_name.utf8().get_data(), diagnostics.writeRef());
    ERR_FAIL_COND_V_MSG(diagnostics, nullptr, (const char*) diagnostics->getBufferPointer());
    print_verbose("Module loaded.");
    Slang::ComPtr<IEntryPoint> entry_point;
    module->findEntryPointByName(
        p_entry_point_name.utf8().get_data(),
        entry_point.writeRef()
    );
    IComponentType* components[] = { module, entry_point };
    Slang::ComPtr<IComponentType> program;
    session->createCompositeComponentType(components, 2, program.writeRef());
    // slang::ProgramLayout* layout = program->getLayout();
    Slang::ComPtr<IComponentType> linked_program;
    program->link(linked_program.writeRef(), diagnostics.writeRef());
    int entry_point_index = 0; // How to get?
    int target_index = 0; // How to get?
    Slang::ComPtr<IBlob> shader_code;
    linked_program->getEntryPointCode(
        entry_point_index,
        target_index,
        shader_code.writeRef(),
        diagnostics.writeRef()
    );
    const uint8_t *code_buffer = (const uint8_t *) shader_code->getBufferPointer();
    PackedByteArray code_byte_array = PackedByteArray();
    code_byte_array.resize(shader_code->getBufferSize());
    for (int i = 0; i < shader_code->getBufferSize(); i++) {
        code_byte_array.set(i, code_buffer[i]);
    }
    Ref<RDShaderSPIRV> shader_spirv = memnew(RDShaderSPIRV);
    shader_spirv->set_stage_bytecode(
        RenderingDevice::ShaderStage::SHADER_STAGE_COMPUTE,
        code_byte_array
    );
    return shader_spirv;
}



void godot::SlangSession::initialize_session() {
    if (!is_global_session_initialized) {
        ERR_FAIL_MSG(
            "Trying to create slang session with uninitialized global session."
        );
        return;
    }

    SessionDesc session_desc;

    const String project_path = 
        ProjectSettings::get_singleton()->globalize_path("res://");
    const char* search_paths[] = { project_path.utf8().get_data() };
    session_desc.searchPaths = search_paths;
    session_desc.searchPathCount = 1;
    
    TargetDesc target_desc;
    target_desc.format = SLANG_SPIRV;
    target_desc.profile = global_session->findProfile("glsl_450");
    session_desc.targets = &target_desc;
    session_desc.targetCount = 1;

    SlangResult res = global_session->createSession(
        session_desc, session.writeRef()
    );
    is_initialized = res >= 0;
    ERR_FAIL_COND_MSG(
        !is_initialized,
        "Couldn't initialize Slang session."
    );
}


godot::SlangSession::SlangSession() {
    initialize_global_session();
    initialize_session();
    ERR_FAIL_COND_MSG(
        !is_initialized,
        "Initialization of SlangDevice failed."
    );
}


godot::SlangSession::~SlangSession() {
	if (is_initialized) { 
        session.setNull();
		global_session.setNull();
	}
}
