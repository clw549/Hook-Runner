using Godot;
using System;

public partial class Cherry : Area2D
{
	[Export]
	public string PlayerGroup = "Player";

	public override void _Ready()
	{
		BodyEntered += OnBodyEntered;
	}

	private void OnBodyEntered(Node body)
	{
		if (body.IsInGroup(PlayerGroup))
		{
			// Tell GameManager we collected a cherry
			GameManager.Instance?.AddCherry();

			// Remove the cherry from the scene
			QueueFree();
		}
	}
}
