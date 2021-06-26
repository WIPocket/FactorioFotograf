-- settings --
block_size      = 20  -- how long the side of one screenshot should be in tiles. This affect if you want 10 100x100 screnshots or 100 10x10 screenshots.
pixels_per_tile = 32  -- how many pixels per tile to use in the screenshot. I dont think that game textures are bigger than 64 pixels per tile.
min_dist_to_smt = 60  -- how far away a player's structure can be from the block center for it to be included in the map.
-- end of settings --
image_resolution = block_size * pixels_per_tile
add_to_border = math.ceil(min_dist_to_smt / block_size) + 1
zoom = pixels_per_tile / 32 -- not sure if this number is different on different graphics settings
maxx,maxy,minx,miny = -9999999999,-9999999999,9999999999,9999999999
tick = 0
script.on_event(defines.events.on_tick, function(event)
	if tick == 0 then -- do only one time when game is loaded
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
		game.write_file("mapInfo.json", game.table_to_json({
			maxx = maxx,
			maxy = maxy,
			minx = minx,
			miny = miny,
			block_size = block_size,
			pixels_per_tile = pixels_per_tile,
			image_resolution = image_resolution,
		}))
		for x = minx,maxx,1 do -- loop over all blocks in the calculated box and take a screenshot
			for y = miny,maxy,1 do
				position = {x * block_size + block_size / 2, y * block_size + block_size / 2}
				if #game.surfaces[1].find_entities_filtered{force=game.players[1].force, position=position, radius=min_dist_to_smt, limit=1} > 0 then
					game.take_screenshot{
						resolution = {image_resolution, image_resolution},
						position = position, -- position defines the middle of the screenshot
						show_entity_info = true,
						--      images, tick, zoom, x, y,
						path = "images/0/8/" .. x .. "/" .. y .. ".png",
						zoom = zoom
					}
				end
			end
		end
		--[[rendering.draw_rectangle{ -- visualize the exported rectangle
			color = {0, 0.5, 0, 0.1},
			filled = true,
			left_top = {minx * block_size, miny * block_size},
			right_bottom = {(maxx + 1) * block_size, (maxy + 1) * block_size},
			time_to_live = 60 * 30,
			surface = game.surfaces[1]
		}]]
		done = true
	elseif tick == 10 then -- wait a bit
		game.write_file("done", "done")
	end
	tick = tick + 1
end)

