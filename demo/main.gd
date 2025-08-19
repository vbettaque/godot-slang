@tool
extends Node

var rd: RenderingDevice
@export_tool_button("Test") var test_action = _run_test

func _ready() -> void:
	rd = RenderingServer.get_rendering_device()
	

func _run_test() -> void:
 	#SlangServer
	var session = SlangSession.new()
	var spirv: RDShaderSPIRV = session.load_shader("MyShaders", "computeMain")
	print(spirv.bytecode_compute)
	var shader = rd.shader_compile_binary_from_spirv(spirv)
	print("shader:")
	print(shader)

func _process(delta: float) -> void:
	pass
