#include "register_types.h"

#include "GrapplingHook.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/string_name.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
  //REGISTER CLASSES
	GDREGISTER_RUNTIME_CLASS(GrapplingHook);
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT adv_mvmnt_library_init(
  GDExtensionInterfaceGetProcAddress p_get_proc_address, 
  GDExtensionClassLibraryPtr library, 
  GDExtensionInitialization *r_initialization
) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, library, r_initialization);

	init_obj.register_initializer(initialize_example_module);
	init_obj.register_terminator(uninitialize_example_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}