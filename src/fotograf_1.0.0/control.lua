ext = ".jpg"
pixels_per_tile = 32  -- How many pixels per tile to use in the screenshot. Game textures are not bigger than 64 pixels per tile.
border          = 128

image_resolution = 32 * pixels_per_tile
zoom = pixels_per_tile / 32
state = -1
capture_caller = nil

function capture(layer)
	n = (2 ^ (layer - 1)) -- 1, 2, 4, 8, 16, 32, ...
	box_size = n * 32
	for c in game.surfaces[1].get_chunks() do
		--game.print(c.x .. " " .. c.y .. " " .. layer)
		if (c.x % n == 0) and (c.y % n == 0) then
			tl = {c.x * 32           , c.y * 32}
			br = {c.x * 32 + box_size, c.y * 32 + box_size}
			if game.surfaces[1].count_entities_filtered{
				force=game.players[1].force, 
				area={tl, br},
				limit=1
			} > 0 then
				--[[game.take_screenshot{
					resolution = {image_resolution, image_resolution},
					position = {c.x * box_size + box_size / 2, c.y * box_size + box_size / 2}, -- position defines the middle of the screenshot
					path = "fotograf/images/" .. 0 .. "/" .. layer .. "/" .. c.x .. "_" .. c.y .. ext,
					zoom = zoom,
					show_entity_info = true,
					quality = 90,
					daytime = 1.0,
					allow_in_replay = false,
					show_gui = false,
				}]]
				rendering.draw_rectangle{
					color = {1.0, 1.0, 0.0},
					filled = false,
					left_top = tl,
					right_bottom = br,
					time_to_live = 600,
					surface = game.surfaces[1],
				}
				rendering.draw_rectangle{
					color = {0.0, 0.1, 0.0, 0.1},
					filled = true,
					left_top = tl,
					right_bottom = br,
					time_to_live = 600,
					surface = game.surfaces[1],
				}
			end
		end
	end
end

script.on_event(defines.events.on_tick, function(event)
	if state == 0 then
		game.print("Taking screenshots...")
		state = state + 1
	elseif state == 1 then
		capture(2)
		--capture(2)
		game.write_file("mapInfo.json", game.table_to_json({
			pixels_per_tile = pixels_per_tile,
		}))
		game.set_wait_for_screenshots_to_finish()
		state = state + 1
	elseif state == 2 then
		game.print("done")
		game.write_file("done", "done")
		state = -1
	end
end)

commands.add_command("foto1", nil, function(command)
	--state = 0
	capture(2)
end)

commands.add_command("foto2", nil, function(command)
	--state = 0
	capture(3)
end)

