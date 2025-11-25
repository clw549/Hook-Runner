#ifndef GRAPPLINGHOOK
#define GRAPPLINGHOOK

#include <godot_cpp/classes/node2d.hpp>
#include <cstdlib>
#include <godot_cpp/godot.hpp>

namespace godot {
  class GrapplingHook : public Node2D {
    GDCLASS(GrapplingHook, Node2D)

    private:
      Vector2 _hookPosition;
      float _maxLength;
      float _currentLength;
      bool _showGuide;
      Node2D* _graplingHook;
    
    protected:
      static void _bind_methods();

    public:
      GrapplingHook();
      ~GrapplingHook();
      Node2D* ShootHook(Vector2 direction);
      void RetractHook();
      void AimGuide(bool show);
      void SetMaxHookLength(float length);
  };
}

#endif