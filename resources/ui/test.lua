window = {dimmed = false}

mylabel = {pos = {200, 200},
		   size = {300, 100},
		   bgclr = {0.0, 1.0, 0.0, 0.5},
		   fgclr = {1.0, 1.0, 0.0, 1.0},
		   text = "Hello, Lua UI!"}

unload_flag = false

function init()
	ui_add("label", "mylabel")
end

function update(ms)
	local opacity = mylabel.bgclr[4]
	opacity = opacity + ms / 5000.0

	if opacity > 1.0 then
		opacity = opacity - 1.0
		if ui_unload then
			ui_close()
		end
	end

	mylabel.bgclr[4] = opacity
end
