ext = ".jpg"
ppt = 32 -- How many pixels per tile to use in the screenshot. Game textures are not bigger than 64 pixels per tile.
--border = 128

image_resolution = 32 * ppt
zoom = ppt / 32
state = -1

function capture(layer)
	n = (2 ^ (layer - 1)) -- 1, 2, 3, 4, 5, 6 -> 1, 2, 4, 8, 16, 32, ...
	box_size = n * 32
	for c in game.surfaces[1].get_chunks() do
		if (c.x % n == 0) and (c.y % n == 0) then
			tl = {c.x * 32               , c.y * 32               } -- top left
			br = {c.x * 32 + box_size    , c.y * 32 + box_size    } -- bottom right
			cn = {c.x * 32 + box_size / 2, c.y * 32 + box_size / 2} -- center
			if game.surfaces[1].count_entities_filtered{
				force=game.players[1].force, 
				area={tl, br},
				limit=1
			} > 0 then
				game.take_screenshot{
					resolution = {image_resolution, image_resolution},
					position = cn,
					path = "fotograf/images/" .. 0 .. "/" .. layer .. "/" .. c.x / n .. "_" .. c.y / n .. ext,
					zoom = zoom / n,
					show_entity_info = true,
					quality = 90,
					daytime = 1.0,
					allow_in_replay = false,
					show_gui = false,
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
		for i = 1, 6 do
			capture(i)
		end
		game.write_file("fotograf/mapInfo.js", "mapInfo = '" .. game.table_to_json({ppt = ppt}) .. "'\n")
		game.set_wait_for_screenshots_to_finish()
		state = state + 1
	elseif state == 2 then
		game.print("Done")
		state = -1
	end
end)

commands.add_command("foto", nil, function(command)
	state = 0
end)

