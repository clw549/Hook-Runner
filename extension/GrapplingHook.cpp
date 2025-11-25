#include <godot_cpp/core/class_db.hpp>
#include "GrapplingHook.h"

using namespace godot;

void GrapplingHook::_bind_methods() {
  ClassDB::bind_method(D_METHOD("ShootHook", "direction"), &GrapplingHook::ShootHook);
  ClassDB::bind_method(D_METHOD("RetractHook"), &GrapplingHook::RetractHook);
  ClassDB::bind_method(D_METHOD("SetMaxHookLength", "length"), &GrapplingHook::SetMaxHookLength);
}

GrapplingHook::GrapplingHook() {
  _graplingHook = nullptr;

}

GrapplingHook::~GrapplingHook() {

}

Node2D* GrapplingHook::ShootHook(Vector2 direction) {
  //todo: instantiate child pointing in the direction of direction
  return NULL;
}

void GrapplingHook::RetractHook() {

}

void GrapplingHook::AimGuide(bool show) {

}

void GrapplingHook::SetMaxHookLength(float length) {

}