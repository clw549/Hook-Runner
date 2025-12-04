using Godot;
using System;

public partial class MainCharacter : CharacterBody2D
{
	public const float Speed = 300.0f;
	public const float JumpVelocity = -400.0f;

	private AnimatedSprite2D _anim; // Reference to your animated sprite

	public override void _Ready()
	{
		// Change the node path if your AnimatedSprite2D has a different name or is deeper in the tree
		_anim = GetNode<AnimatedSprite2D>("AnimatedSprite2D");
	}

	public override void _PhysicsProcess(double delta)
	{
		Vector2 velocity = Velocity;

		// Add the gravity.
		if (!IsOnFloor())
		{
			velocity += GetGravity() * (float)delta;
		}

		// Handle Jump.
		if (Input.IsActionJustPressed("jump") && IsOnFloor())
		{
			velocity.Y = JumpVelocity;
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
}
