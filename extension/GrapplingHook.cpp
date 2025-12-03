#include <godot_cpp/core/class_db.hpp>
#include "GrapplingHook.h"
#include <godot_cpp/core/binder_common.hpp>  

using namespace godot;

void GrapplingHook::_bind_methods() {
  ClassDB::bind_method(D_METHOD("ShootHook", "direction"), &GrapplingHook::ShootHook);
  ClassDB::bind_method(D_METHOD("RetractHook"), &GrapplingHook::RetractHook);
  ClassDB::bind_method(D_METHOD("SetMaxHookLength", "length"), &GrapplingHook::SetMaxHookLength);

  ClassDB::bind_method(D_METHOD("GetGrapplingHook"), &GrapplingHook::GetGrapplingHook);
  ClassDB::bind_method(D_METHOD("SetGrapplingHook", "hook"), &GrapplingHook::SetGrapplingHook);
  ClassDB::bind_method(D_METHOD("SetSpeed", "newSpeed"), &GrapplingHook::SetSpeed);
  ClassDB::bind_method(D_METHOD("GetSpeed"), &GrapplingHook::GetSpeed);
  ClassDB::bind_method(D_METHOD("SetPlayer", "newPlayer"), &GrapplingHook::SetPlayer);
  ClassDB::bind_method(D_METHOD("GetPlayer"), &GrapplingHook::GetPlayer);

  ClassDB::add_property("GrapplingHook", PropertyInfo(Variant::OBJECT, "grapplingHook", PROPERTY_HINT_NODE_TYPE, "Node2D"), "SetGrapplingHook", "GetGrapplingHook");

  ClassDB::add_property("GrapplingHook", PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_NODE_TYPE, "float"), "SetSpeed", "GetSpeed");

  ClassDB::add_property("GrapplingHook", PropertyInfo(Variant::OBJECT, "_player", PROPERTY_HINT_NODE_TYPE, "Node2D"), "SetPlayer", "GetPlayer");

  ClassDB::bind_method(D_METHOD("OnHookHitBody", "body"), &GrapplingHook::OnHookHitBody);

}

void GrapplingHook::_ready() {
  collider = get_node<CollisionShape2D>("CollisionShape2D");
  // Note: CollisionShape2D doesn't have body_entered signal
  // We use move_and_collide() for collision detection instead
  _rope_line = get_node<Line2D>("Line");
    if (_rope_line) {
        _rope_line->clear_points();
        _rope_line->add_point(Vector2(0,0)); // start
        _rope_line->add_point(Vector2()); // end
    }
}

GrapplingHook::GrapplingHook() {
  speed = 500;
  set_visible(false); // Start hidden

}

GrapplingHook::~GrapplingHook() {

}

void GrapplingHook::_physics_process(double delta) {
  _hook_position = get_position();
  Vector2 player_pos = _player->get_global_position();

  
    if (!_hooked) {
        // Move hook linearly
        velocity = _shoot_direction * speed;

        Ref<KinematicCollision2D> collision = move_and_collide(velocity * delta);
        if (collision.is_valid()) {
            OnHookHitBody(collision.ptr());
        }
    } else {
        // Hook is static, maybe rotate/animate it if needed
        velocity = Vector2(0,0);

        if (_player) {
            // Constrain player around hook
            Vector2 rope = _player->get_global_position() - _hook_position;
            float dist = rope.length();
            if (dist > _maxLength) {
                Vector2 corrected = _hook_position + rope.normalized() * _maxLength;
                _player->set_global_position(corrected);
            }
        }
    }
    if (_rope_line && _player) {
      _rope_line->set_point_position(1, _rope_line->to_local(_player->get_global_position()));
      _rope_line->set_visible(true);
      _rope_line->set_width(4.0);
      _rope_line->set_default_color(Color(1,0,0));
    }

}


void GrapplingHook::ShootHook(Vector2 direction) {
  if (_hooked) return; // Don't shoot if already hooked

  _shoot_direction = direction.normalized();
  _hooked = false;

  // Position hook at player position to start shooting
  if (_player) {
    set_global_position(_player->get_global_position());
  }

  set_visible(true);
}

void GrapplingHook::RetractHook() {
  _hooked = false;
  _shoot_direction = Vector2(0, 0);
  set_visible(false);

  if (_rope_line) {
    _rope_line->set_visible(false);
  }
}

void GrapplingHook::OnHookHitBody(KinematicCollision2D *collision) {
  _shoot_direction = Vector2(0,0);
  UtilityFunctions::print("Hook touched body");
  _hooked=true;


}

void GrapplingHook::AimGuide(bool show) {

}

void GrapplingHook::SetMaxHookLength(float length) {
  _maxLength = length;
}

void GrapplingHook::SetGrapplingHook(Node2D *hook) {
  grapplingHook = hook;
}

Node2D *GrapplingHook::GetGrapplingHook(){
  return grapplingHook;
}

void GrapplingHook::SetSpeed(float newSpeed) {
  speed = newSpeed;
}
float GrapplingHook::GetSpeed() {
  return speed;
}

void GrapplingHook::SetPlayer(Node2D *newPlayer) {
  _player = newPlayer;
}
Node2D *GrapplingHook::GetPlayer() {
  return _player;
}