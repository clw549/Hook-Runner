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
  
  ClassDB::bind_method(D_METHOD("SetCurrentLength", "length"), &GrapplingHook::SetCurrentLength);
  ClassDB::bind_method(D_METHOD("GetCurrentLength"), &GrapplingHook::GetCurrentLength);
  ClassDB::bind_method(D_METHOD("IncrementCurrentLength", "addLength"), &GrapplingHook::IncrementCurrentLength);

  ClassDB::add_property("GrapplingHook", PropertyInfo(Variant::OBJECT, "grapplingHook", PROPERTY_HINT_NODE_TYPE, "CharacterBody2D"), "SetGrapplingHook", "GetGrapplingHook");

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

    set_physics_process(false);
}

GrapplingHook::GrapplingHook() {
  speed = 500;
  set_visible(false); // Start hidden

  //creating new line to use
  _guideLine = memnew(Line2D);

}

GrapplingHook::~GrapplingHook() {
  //deleting old line (prevent mem leak)
  memdelete(_guideLine);
}

void GrapplingHook::_physics_process(double delta) {
    _hook_position = get_global_position();

    Vector2 player_pos = _player->get_global_position();
    Vector2 rope = player_pos - _hook_position;        // hook → player
    Vector2 to_hook = _hook_position - player_pos;     // player → hook
    float dist = rope.length();

    if (!_hooked) {
        // Move the hook forward
        velocity = _shoot_direction * speed;

        Ref<KinematicCollision2D> collision = move_and_collide(velocity * delta);

        // hook overshot max length
        if (dist > _maxLength) {
            RetractHook();
        }

        if (collision.is_valid()) {
            OnHookHitBody(collision.ptr());
            _currentLength = dist;  
        }
    }
    else {
        // Hook is stationary
        velocity = Vector2(0, 0);

        if (_player) {
            // 1. Enforce rope length
            if (dist > _currentLength) {
                Vector2 corrected = _hook_position + rope.normalized() * _currentLength;
                _player->set_global_position(corrected);
            }

            // 2. Remove radial (in/outward) velocity component
            Vector2 player_vel = _player->get_velocity();

            // projection of velocity onto rope direction
            float radial = player_vel.dot(to_hook.normalized());

            if (radial < 0) {
                // remove radial component → keeps tangential velocity for swinging
                Vector2 tangential = player_vel - to_hook.normalized() * radial;
                _player->set_velocity(tangential);
            }
        }
    }

    // Draw rope line
    if (_rope_line && _player) {
        _rope_line->set_point_position(1, _rope_line->to_local(_player->get_global_position()));
        _rope_line->set_visible(true);
        _rope_line->set_width(4.0);
        _rope_line->set_default_color(Color(1, 0.5, 0));
    }
}


void GrapplingHook::CorrectPlayerPhysics() {
  
}


void GrapplingHook::ShootHook(Vector2 direction) {
  if (_hooked) return; // Don't shoot if already hooked
  set_physics_process(true);

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
  _currentLength = 0;

  if (_rope_line) {
    _rope_line->set_visible(false);
  }
  set_physics_process(false);
}

void GrapplingHook::OnHookHitBody(KinematicCollision2D *collision) {
  _shoot_direction = Vector2(0,0);
  UtilityFunctions::print("Hook touched body");
  _hooked=true;


}

void GrapplingHook::AimGuide(bool show) {
  _showGuide = show;
}

void GrapplingHook::DrawAimGuide() {
  Vector2 mousePos;
  if (_showGuide){
    mousePos = get_global_mouse_position();

  }
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

void GrapplingHook::SetPlayer(CharacterBody2D *newPlayer) {
  _player = newPlayer;
}
CharacterBody2D *GrapplingHook::GetPlayer() {
  return _player;
}


void GrapplingHook::SetCurrentLength(float length){
  //clamping values
  if (length < _minRopeLen) _currentLength = _minRopeLen;
  else if (length > _maxLength) _currentLength = _maxLength;
  else _currentLength = length;
}
float GrapplingHook::GetCurrentLength() {
  return _currentLength;
}
void GrapplingHook::IncrementCurrentLength(float addLength) {
  //clamping values min <= current <= max
  addLength = addLength*climbSpeed;
  if (_currentLength+addLength < _minRopeLen) _currentLength = _minRopeLen;
  else if (_currentLength+addLength > _maxLength) _currentLength = _maxLength;
  else _currentLength += addLength;
}