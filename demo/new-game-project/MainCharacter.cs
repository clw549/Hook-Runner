using Godot;
using System;

public partial class MainCharacter : CharacterBody2D
{
	[Export] public Node2D Hook;
	[Export] public Camera2D Camera;

	public const float Speed = 300.0f;
	public const float JumpVelocity = -400.0f;

	private AnimatedSprite2D _anim; // Reference to your animated sprite

	public override void _Ready()
	{
		// Change the node path if your AnimatedSprite2D has a different name or is deeper in the tree
		_anim = GetNode<AnimatedSprite2D>("AnimatedSprite2D");

		// Initialize grappling hook
		if (Hook != null)
		{
			// Move hook off-screen initially
			Hook.GlobalPosition = new Vector2(-10000, -10000);
			Hook.Visible = true; // Force visible for debugging
			Hook.Call("SetClimbSpeed", 100.0f);
			GD.Print("Hook initialized");
		}
		else
		{
			GD.Print("ERROR: Hook node not assigned!");
		}
	}

	public override void _PhysicsProcess(double delta)
	{
		Vector2 velocity = Velocity;

		// Camera follow player
		if (Camera != null)
		{
			Camera.GlobalPosition = GlobalPosition;
		}

		// Add the gravity.
		if (!IsOnFloor())
		{
			velocity += GetGravity() * (float)delta;
		}

		// Handle Jump and hook reel in/out
		if (Input.IsActionPressed("jump"))
		{
			if (IsOnFloor())
			{
				velocity.Y = JumpVelocity;
			}
			else if (Hook != null && (bool)Hook.Call("GetHooked"))
			{
				// Reel in when hooked and jump is pressed
				Hook.Call("IncrementCurrentLength", -(float)delta);
			}
		}
		else if (Input.IsActionPressed("lower"))
		{
			// Lower the rope when 'lower' action is pressed
			if (Hook != null)
			{
				Hook.Call("IncrementCurrentLength", (float)delta);
			}
		}

		// Get the input direction and handle the movement/deceleration.
		Vector2 direction = Input.GetVector("left", "right", "up", "down");

		if (direction.X != 0)
		{
			velocity.X = direction.X * Speed;

			// Flip sprite when going left
			if (_anim != null)
				_anim.FlipH = direction.X < 0;
		}
		else
		{
			velocity.X = Mathf.MoveToward(velocity.X, 0, 10);
		}

		Velocity = velocity;
		MoveAndSlide();

		// --- Animation logic ---
		if (_anim != null)
		{
			if (!IsOnFloor())
			{
				_anim.Play("Jumping");
			}
			else if (Mathf.Abs(Velocity.X) > 1f)
			{
				_anim.Play("Running");
			}
			else
			{
				_anim.Play("Idle");
			}
		}
	}

	public override void _Input(InputEvent @event)
	{
		// Handle mouse clicks for grappling hook
		if (@event is InputEventMouseButton mouseEvent)
		{
			if (mouseEvent.ButtonIndex == MouseButton.Left && mouseEvent.Pressed)
			{
				ShootHook();
			}
			else if (mouseEvent.ButtonIndex == MouseButton.Right && mouseEvent.Pressed)
			{
				RetractHook();
			}
		}

		// ESC to quit (useful for testing)
		if (@event is InputEventKey keyEvent)
		{
			if (keyEvent.Pressed && keyEvent.Keycode == Key.Escape)
			{
				GetTree().Quit();
			}
		}
	}

	private void ShootHook()
	{
		GD.Print("Shoot hook called!");
		if (Hook == null)
		{
			GD.Print("ERROR: Hook node is null!");
			return;
		}

		Vector2 mousePos = GetGlobalMousePosition();
		Vector2 direction = (mousePos - GlobalPosition).Normalized();
		GD.Print("Shooting hook toward: ", mousePos);

		// Call the hook's ShootHook method
		Hook.Call("ShootHook", direction);
	}

	private void RetractHook()
	{
		GD.Print("Releasing hook!");
		if (Hook == null)
		{
			GD.Print("ERROR: Hook node is null!");
			return;
		}

		// Call the hook's RetractHook method
		Hook.Call("RetractHook");
	}
}
