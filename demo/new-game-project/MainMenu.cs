using Godot;
using System;

public partial class MainMenu : Node
{
	private Button _playButton;

	public override void _Ready()
	{
		_playButton = GetNode<Button>("Play");
		_playButton.Pressed += OnPlayPressed;
	}

	private void OnPlayPressed()
	{
		GetTree().ChangeSceneToFile("res://main.tscn");
	}
}
