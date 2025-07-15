extends Node

func _ready():
	# Spawn a TestCompile node and call its function
	var tc = TestCompile.new()
	tc.compile_slang_to_glsl("shader")
