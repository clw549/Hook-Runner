# Classes:
- # GrapplingHook
  ## - `void ShootHook(Vector2 direction);`
    Shoots the grappling hook in the direction of the vector 2.
    give this the mouse position to aim with the mouse when the screen is centered on the player.

  ## - `void RetractHook();`
    Hides the hook and turns all functions off including processing until ShootHook() is called again. 
    ShootHook() -> RetractHook() -> ShootHook()...

  ## - `void SetMaxHookLength(float length);`
    Sets max hook length to clamp the max distance the player can be from the GrapplingHook.

  ## - `void SetSpeed(float speed);`
  ## - `float GetSpeed();`
    getter and setter for speed, how fast the grappling hook flies.

  ## - `CharacterBody2D *GetPlayer();`
  ## - `void SetPlayer(CharacterBody2D *newPlayer);`
    Getter and setter of the `player` variable. This `player` variable is what is attatched to the `GrapplingHook`, what swings from `it` and has their movement alterd by `it`. The player can be any `CharacterBody2D`, not just a controlled `CharacterBody2d`.

  ## - `void SetCurrentLength(float length);`
  ## - `float GetCurrentLength();`
    Getter and setter for the `currentLength` variable. 
    It handles the length the rope is holding the `player` at, or the max distance between the two.

  ## - `void IncrementCurrentLength(float addLength);`
    Increments the length by a float amount, reccommended to be delta, but it could be multiplied, but there is a multiplier: `climbSpeed` getter and setter below.
    Use positive values to lengthen (lower) the rope and negatigve ones to shorten (climb).

  ## - `void SetHooked(bool newHooked);`
    Builder function for adding property. No functionality.
  ## - `bool GetHooked();`
    Getter of the `hooked` boolean state. 
    This is to find when the grappling hook is hooked on to something.

  ## - `void SetClimbSpeed(float newClimbSpeed);`
  ## - `float GetClimbSpeed();`
    Getter and setter of the `climbSpeed` variable. 
    This variable multiplies the `length` climbed by `IncrementCurrentLength()` thus increasing the speed of "climbing" the rope.