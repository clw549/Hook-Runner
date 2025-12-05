using Godot;
using System;

public partial class GameManager : Node
{
	public static GameManager Instance { get; private set; }

	[Export]
	public Label CherryLabel { get; set; }

	public int CherryCount { get; private set; } = 0;

	public override void _Ready()
	{
		Instance = this;
		UpdateLabel();
	}

	public void AddCherry()
	{
		CherryCount++;
		UpdateLabel();
	}

	private void UpdateLabel()
	{
		if (CherryLabel != null)
			CherryLabel.Text = $"Cherries Collected: {CherryCount}";
	}
}
