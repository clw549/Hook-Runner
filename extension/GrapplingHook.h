#ifndef GRAPPLINGHOOK
#define GRAPPLINGHOOK

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <cstdlib>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/line2d.hpp>
#include <godot_cpp/classes/kinematic_collision2d.hpp>


namespace godot {
  class GrapplingHook : public CharacterBody2D {
    GDCLASS(GrapplingHook, CharacterBody2D)

    private:
      Vector2 _hook_position;
      float _maxLength = 800;
      float _currentLength = 400;
      float _minRopeLen = 50;
      bool _showGuide = false;
      Vector2 _shoot_direction = Vector2(1, 0);
      CharacterBody2D *_player = nullptr;
      Line2D *_rope_line =nullptr;
      Line2D *_guideLine = nullptr;

      //deals with hitting a object, colliding, and sticking to it
      void OnHookHitBody(KinematicCollision2D *collision);
      void CorrectPlayerPhysics();
      void DrawAimGuide();


      

    protected:
      static void _bind_methods();

    public:
      Node2D* grapplingHook;
      float speed;
      float climbSpeed = 10;
      CollisionShape2D *collider;
      Vector2 velocity = Vector2(1,0);
      Vector2 position;
      bool hooked = false;

      void _physics_process(double delta) override;

      void _ready() override;
      //creation/destruction methods
      GrapplingHook();
      ~GrapplingHook();
      //shoots hook in direction given (creates child)
      void ShootHook(Vector2 direction);
      //deletes child that was created
      void RetractHook();
      void AimGuide(bool show);
      void SetMaxHookLength(float length);

      //get and setter for the grappling hook reference
      void SetGrapplingHook(Node2D *hook);
      Node2D *GetGrapplingHook();

      //get and setter for speed of the grappling hook
      void SetSpeed(float speed);
      float GetSpeed();

      //get and setter for player for the grappling hook to manipulate
      // may be used with other instances for enemies or traps?
      CharacterBody2D *GetPlayer();
      void SetPlayer(CharacterBody2D *newPlayer);

      //getter and setter with an incremental seting of the current length
      // this helps with extending the grappling hook rope length.
      void SetCurrentLength(float length);
      float GetCurrentLength();
      void IncrementCurrentLength(float addLength);

      void SetHooked(bool newHooked);
      bool GetHooked();

      void SetClimbSpeed(float newClimbSpeed);
      float GetClimbSpeed();
  };
}

#endif