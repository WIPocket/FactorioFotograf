-- settings --
block_size      = 20  -- how long the side of one screenshot should be. This affect if you want 10 100x100 screnshots or 100 10x10 screenshots.
pixels_per_tile = 32  -- how many pixels per tile to use in the screenshot. I dont think that game textures are bigger than 64 pixels per tile.
add_to_border   = 0   -- how many more blocks to add around the border. Use this if you want extra space around you factory in the map
-- end of settings --
image_resolution = block_size * pixels_per_tile
zoom = pixels_per_tile / 32 -- not sure if this number is different on different graphics settings
maxx,maxy,minx,miny = -9999999999,-9999999999,9999999999,9999999999
done = false
script.on_event(defines.events.on_tick, function(event)
	if done == false then -- do only once when game is loaded
		for _, e in pairs(game.surfaces[1].find_entities_filtered{force = game.players[1].force}) do -- calculate box that contains all player's entities
			maxx = math.max(maxx,math.floor(e.position.x/block_size))
			maxy = math.max(maxy,math.floor(e.position.y/block_size))
			minx = math.min(minx,math.floor(e.position.x/block_size))
			miny = math.min(miny,math.floor(e.position.y/block_size))
		end
		maxx = maxx + add_to_border
		maxy = maxy + add_to_border
		minx = minx - add_to_border
		miny = miny - add_to_border
		game.write_file("mapInfo.json", "mapInfo = '" .. game.table_to_json({
			maxx = maxx,
			maxy = maxy,
			minx = minx,
			miny = miny,
			block_size = block_size,
			pixels_per_tile = pixels_per_tile,
		}) .. "'\n")
		for x = minx,maxx,1 do -- loop over all blocks in the calculated box and take a screenshot
			for y = miny,maxy,1 do
				game.take_screenshot{
					resolution = {image_resolution, image_resolution},
					position = {x * block_size + block_size / 2, y * block_size + block_size / 2}, -- position defines the middle of the screenshot
					show_entity_info = true,
					--      images timestamp   surface name        time zoom    x           y
					path = "images/0/" .. game.surfaces[1].name .. "/day/0/" .. x .. "/" .. y .. ".png",
					zoom = zoom
				}
			end
		end
		dx = math.abs(minx) + math.abs(maxx) -- just print some stats
		dy = math.abs(miny) + math.abs(maxy)
		rendering.draw_rectangle{ -- visualize the exported rectangle
			color = {0, 0.5, 0, 0.1},
			filled = true,
			left_top = {minx * block_size, miny * block_size},
			right_bottom = {(maxx + 1) * block_size, (maxy + 1) * block_size},
			time_to_live = 60 * 30,
			surface = game.surfaces[1],
		}
		game.print("took " .. (dx * dy) .. " " .. image_resolution .. "x" .. image_resolution .. " screenshots in a " .. dx .. "x" .. dy .. " grid at " .. pixels_per_tile .. " pixels per tile\nnow exit the game")
		done = true
	end
end)

