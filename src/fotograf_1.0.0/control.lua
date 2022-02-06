ext = ".jpg"
pixels_per_tile = 32  -- How many pixels per tile to use in the screenshot. Game textures are not bigger than 64 pixels per tile.
min_dist_to_smt = 128 -- How far away from player's structures should the tiles still be rendered

image_resolution = 32 * pixels_per_tile
zoom = pixels_per_tile / 32 -- not sure if this number is different on different graphics settings
maxx, maxy, minx, miny = -9999999999, -9999999999, 9999999999, 9999999999
tick = 0
script.on_event(defines.events.on_tick, function(event)
	if tick == 0 then
		game.surfaces[1].daytime = 0
		game.print("Taking screenshots...")
	elseif tick == 1 then
		for c in game.surfaces[1].get_chunks() do
			position = {c.x * 32 + 16, c.y * 32 + 16}
			if #game.surfaces[1].find_entities_filtered{force=game.players[1].force, position=position, radius=min_dist_to_smt, limit=1} > 0 then
				maxx = math.max(maxx, c.x)
				maxy = math.max(maxy, c.y)
				minx = math.min(minx, c.x)
				miny = math.min(miny, c.y)
				game.take_screenshot{
					resolution = {image_resolution, image_resolution},
					position = position, -- position defines the middle of the screenshot
					show_entity_info = true,
					path = "images/0/8/" .. c.x .. "_" .. c.y .. ext,
					quality = 90,
					zoom = zoom,
					daytime = 1.0
				}
			end
		end
		game.write_file("mapInfo.json", game.table_to_json({
			maxx = maxx, maxy = maxy, minx = minx, miny = miny,
			pixels_per_tile = pixels_per_tile,
			image_resolution = image_resolution,
		}))
		game.set_wait_for_screenshots_to_finish()
	elseif tick == 2 then
		game.print("done")
		game.write_file("done", "done")
	end
	tick = tick + 1
end)

