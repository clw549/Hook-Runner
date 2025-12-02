#ifndef GRAPPLINGHOOK
#define GRAPPLINGHOOK

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <cstdlib>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

namespace godot {
  class GrapplingHook : public CharacterBody2D {
    GDCLASS(GrapplingHook, CharacterBody2D)

    private:
      Vector2 _hookPosition;
      float _maxLength = 200;
      float _currentLength = 50;
      bool _showGuide;
      bool _hooked = false;
      void OnHookHitBody(KinematicCollision2D *collision);
      Vector2 _shoot_direction = Vector2(1, 0);
      Node2D *_player;

      

    protected:
      static void _bind_methods();

    public:
      Node2D* grapplingHook;
      float speed;
      CollisionShape2D *collider;
      Vector2 velocity = Vector2(1,0);

      void _physics_process(double delta);

      void _ready();
      GrapplingHook();
      ~GrapplingHook();
      void ShootHook(Vector2 direction);
      void RetractHook();
      void AimGuide(bool show);
      void SetMaxHookLength(float length);

      void SetGrapplingHook(Node2D *hook);
      Node2D *GetGrapplingHook();

      void SetSpeed(float speed);
      float GetSpeed();

      Node2D *GetPlayer();
      void SetPlayer(Node2D newPlayer);
  };
}

#endif