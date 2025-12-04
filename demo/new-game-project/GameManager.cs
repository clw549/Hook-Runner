using Godot;
using System;

public partial class GameManager : Node
{
	public static GameManager Instance { get; private set; }

	public int CherryCount { get; private set; } = 0;

	public override void _Ready()
	{
		// Make this easily accessible from other scripts
		Instance = this;
	}

	public void AddCherry()
	{
		CherryCount++;
		GD.Print($"Cherries collected: {CherryCount}");
		// Later you can update UI here instead of just printing
	}
}
