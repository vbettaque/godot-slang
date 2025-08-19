@tool
extends EditorImportPlugin

func _get_importer_name() -> String:
	return "godot-slang"

func _get_visible_name() -> String:
	return "Slang File"
	
func _get_recognized_extensions() -> PackedStringArray:
	return ["slang"]
	
func _get_save_extension() -> String:
	return "res"

func _get_resource_type() -> String:
	return "SlangFile"
	
func _get_priority() -> float:
	return 2.

func _get_preset_count() -> int:
	return 0
	
func _get_preset_name(preset_index: int) -> String:
	return "Default"
			
func _get_import_options(path: String, preset_index: int) -> Array[Dictionary]:
	return []

func _get_import_order() -> int:
	return 0
	

func _import(source_file, save_path, options, r_platform_variants, r_gen_files):
	var file = FileAccess.open(source_file, FileAccess.READ)
	if file == null:
		return FileAccess.get_open_error()
	
	var slang_file := SlangFile.new()
	return ResourceSaver.save(
		slang_file,
		"%s.%s" % [save_path, _get_save_extension()]
	)
	 
