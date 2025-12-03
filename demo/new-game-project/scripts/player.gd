extends CharacterBody2D

@export var hook: Node2D
@export var camera: Camera2D

const SPEED = 300.0
const JUMP_VELOCITY = -500.0
#HOOK PARAMETERS HERE IS NONO! 
# There are setters and getters for most of the things that were here...
var gravity = ProjectSettings.get_setting("physics/2d/default_gravity")

func _ready():
	if hook:
		# Move hook off-screen initially
		hook.global_position = Vector2(-10000, -10000)
		hook.set_visible(true)  # Force visible for debugging
		print("Hook initialized")
	else:
		print("ERROR: Hook node not assigned!")

func _physics_process(delta):
	# Camera follow player
	if camera:
		camera.global_position = global_position


##TODO convert to C++
	## Handle hook shooting physics
	#if is_hook_shooting and hook:
		#var prev_pos = hook.global_position
		## Move hook
		#hook.global_position = hook.global_position + hook_velocity * delta
#
		## Check if hook hit max distance
		#var distance = hook.global_position.distance_to(hook_start_pos)
		#if distance > MAX_HOOK_DISTANCE:
			#print("Hook reached max distance, retracting")
			#retract_hook()
			#return

		# Check if hook hit something (raycast from previous to current position)
		# I DID THIS -c
		var space_state = get_world_2d().direct_space_state
#		var query = PhysicsRayQueryParameters2D.create(prev_pos, hook.global_position)
		#query.exclude = [self]  # Don't collide with player
		#var result = space_state.intersect_ray(query)
		## DONE IN PHYSICS PROCESS OF HOOK (FAST)
		#if result:
			## Hook hit something!
			#is_hook_shooting = false
			#is_hooked = true
			#is_reeling_in = true  # Auto start reeling in
			#hook.global_position = result.position  # Snap to collision point
			#current_rope_length = hook.global_position.distance_to(global_position)
			#print("Hook latched! Distance: ", current_rope_length, " on ", result.collider.name)

	# Gravity
	if not is_on_floor():
		velocity.y += gravity * delta

	# Jump - uses "jump" action (SPACE, W, or UP arrow)
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	# Horizontal movement - uses custom move_left/move_right actions (A/D or Arrow Keys)
	var direction = Input.get_axis("move_left", "move_right")
	if direction:
		velocity.x = direction * SPEED
	else:
		velocity.x = move_toward(velocity.x, 0, SPEED * 0.1)

	# Handle grappling hook physics when hooked
	#if is_hooked and hook and not is_hook_shooting:
		#var hook_pos = hook.get_global_position()
		#var to_hook = hook_pos - global_position
		#var distance = to_hook.length()

		# Reel in: shorten rope over time
		#if is_reeling_in and current_rope_length > MIN_ROPE_LENGTH:
			#current_rope_length -= REEL_IN_SPEED * delta

		# Constrain player to rope length
		#if distance > current_rope_length:
			## Pull player toward hook to maintain rope length
			#var pull_direction = to_hook.normalized()
			#var overshoot = distance - current_rope_length
			#global_position += pull_direction * overshoot

			# Apply velocity constraint for swing physics
			#var radial_velocity = velocity.dot(pull_direction)
			#if radial_velocity < 0:  # Moving away from hook
				#velocity -= pull_direction * radial_velocity  # Remove outward component

	move_and_slide()

func shoot_hook():
	print("Shoot hook called!")
	if not hook:
		print("ERROR: Hook node is null!")
		return
		
	#if is_hooked or is_hook_shooting:
		#print("Already hooked/shooting, ignoring")
		#return

	var mouse_pos = get_global_mouse_position()
	var direction = (mouse_pos - global_position).normalized()
	print("Shooting hook toward: ", mouse_pos)
	##############################################
	## call this function instead
	hook.ShootHook(direction);

	# Position hook at player and shoot it out
	hook.global_position = global_position
	#hook_start_pos = global_position
	#hook_velocity = direction * HOOK_SHOOT_SPEED
	#is_hook_shooting = true

	# Make hook and rope visible
	hook.set_visible(true)
	var rope_line = hook.get_node_or_null("Line")
	if rope_line:
		rope_line.set_visible(true)

func retract_hook():
	print("Releasing hook!")
	if not hook:
		print("ERROR: Hook node is null!")
		return

	# Release the hook completely
	# this should be done by module, there is infastructure there
	#is_hooked = false
	#is_hook_shooting = false
	#is_reeling_in = false
	#hook_velocity = Vector2.ZERO
	#current_rope_length = 0.0
	hook.RetractHook();

	# Hide the hook sprite and rope
	#hook.set_visible(false)
	#var rope_line = hook.get_node_or_null("Line")
	#if rope_line:
		#rope_line.set_visible(false)

	# Position hook at player to avoid C++ constraint (within 400 unit threshold)
	hook.global_position = global_position

func _input(event):
	# Handle mouse clicks for grappling hook
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT and event.pressed:
			shoot_hook()
		elif event.button_index == MOUSE_BUTTON_RIGHT and event.pressed:
			retract_hook()

	# ESC to quit (useful for testing)
	if event is InputEventKey:
		if event.pressed and event.keycode == KEY_ESCAPE:
			get_tree().quit()
