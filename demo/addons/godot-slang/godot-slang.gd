@tool
extends EditorPlugin

var import_plugin: EditorImportPlugin

var already_had_slang_extension: bool

func _enter_tree():
	import_plugin = preload("import/import_plugin.gd").new()
	#add_slang_extension()
	add_import_plugin(import_plugin)
	


func _exit_tree():
	#remove_slang_extension()
	remove_import_plugin(import_plugin)
	import_plugin = null
	
#func add_slang_extension():
	#var settings := EditorInterface.get_editor_settings()
	#var file_extensions := settings.get_setting(
		#"docks/filesystem/textfile_extensions"
	#) as String
	#already_had_slang_extension = "slang" in file_extensions
	#if already_had_slang_extension: return
	#file_extensions += ",slang"
	#settings.set_setting(
		#"docks/filesystem/textfile_extensions",
		#file_extensions
	#)
#
#func remove_slang_extension():
	#if already_had_slang_extension: return
	#var settings := EditorInterface.get_editor_settings()
	#var file_extensions := settings.get_setting(
		#"docks/filesystem/textfile_extensions"
	#) as String
	#file_extensions = file_extensions.replace(",slang", "")
	#settings.set_setting(
		#"docks/filesystem/textfile_extensions",
		#file_extensions
	#)
	
	
