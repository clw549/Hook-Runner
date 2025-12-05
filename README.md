# File Structure:
## ./
- SConstruct is the build script
- use `scons` in terminal to build the module into the demo/bin folder
## demo
- holds the demo game we will make inside demo/new-game-project
- holds the compiled extension in demo/bin
- holds adv_mvmnt.gdextension which is for linking in the module
## extension 
- holds the source files, the files responsible for the extension and features
## godot-cpp
- holds files for building in the godot libraries for the extension, do not change!
## docs
- holds files for documentation, use, and compiling the module.
