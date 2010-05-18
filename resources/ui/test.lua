-- load helper library
dofile "resources/ui/uilib.lua"

window.dimmed = false

mylabel1 = {pos = {0, 0},
		    size = {300, 100},
		    bgclr = {1.0, 0.0, 0.0, 0.5},
		    fgclr = {1.0, 1.0, 0.0, 1.0},
		    text = "Label 1!"}

mylabel2 = {pos = {0, 0},
		    size = {300, 100},
		    bgclr = {0.0, 1.0, 0.0, 0.5},
		    fgclr = {1.0, 1.0, 1.0, 1.0},
		    text = "Label 2!"}

mylabel3 = {pos = {0, 0},
		    size = {300, 100},
		    bgclr = {0.0, 0.0, 1.0, 0.5},
		    fgclr = {1.0, 1.0, 1.0, 1.0},
		    text = "Label 3!"}

mylabel4 = {pos = {0, 0},
		    size = {300, 100},
		    bgclr = {1.0, 1.0, 0.0, 0.5},
		    fgclr = {1.0, 1.0, 1.0, 1.0},
		    text = "Label 4!"}

mylabel5 = {pos = {0, 0},
		    size = {300, 100},
		    bgclr = {0.0, 1.0, 1.0, 0.5},
		    fgclr = {1.0, 1.0, 1.0, 1.0},
		    text = "Label 5!"}

mylabel6 = {pos = {0, 0},
		    size = {300, 100},
		    bgclr = {1.0, 0.0, 1.0, 0.5},
		    fgclr = {1.0, 1.0, 1.0, 1.0},
		    text = "Label 6!"}

in_duration = 1000
in_offset = 50
out_duration = 500
out_offset = 150
running_time = 0
in_start_time = -1
out_start_time = -1

function init()
	mylabel1.pos = {window.width, window.height - mylabel1.size[2] - 20}
	mylabel2.pos = {window.width, window.height - mylabel2.size[2] - 140}
	mylabel3.pos = {window.width, window.height - mylabel3.size[2] - 260}
	mylabel4.pos = {window.width, window.height - mylabel3.size[2] - 380}
	mylabel5.pos = {window.width, window.height - mylabel3.size[2] - 500}
	mylabel6.pos = {window.width, window.height - mylabel3.size[2] - 620}

	ui_add("label", "mylabel1")
	ui_add("label", "mylabel2")
	ui_add("label", "mylabel3")
	ui_add("label", "mylabel4")
	ui_add("label", "mylabel5")
	ui_add("label", "mylabel6")
end

function update(ms)
	running_time = running_time + ms

	-- animate in
	if not ui_unload then
		if in_start_time < 0 then
			in_start_time = running_time
			out_start_time = -1
		end
		if running_time < in_start_time + in_duration + (5 * in_offset) then
			mylabel1.pos[1] = uilib.bounce(window.width, window.width - 320, in_start_time, in_start_time + in_duration, running_time)
			mylabel2.pos[1] = uilib.bounce(window.width, window.width - 320, in_start_time + (1 * in_offset), in_start_time + (1 * in_offset) + in_duration, running_time)
			mylabel3.pos[1] = uilib.bounce(window.width, window.width - 320, in_start_time + (1 * in_offset), in_start_time + (2 * in_offset) + in_duration, running_time)
			mylabel4.pos[1] = uilib.bounce(window.width, window.width - 320, in_start_time + (1 * in_offset), in_start_time + (3 * in_offset) + in_duration, running_time)
			mylabel5.pos[1] = uilib.bounce(window.width, window.width - 320, in_start_time + (1 * in_offset), in_start_time + (4 * in_offset) + in_duration, running_time)
			mylabel6.pos[1] = uilib.bounce(window.width, window.width - 320, in_start_time + (1 * in_offset), in_start_time + (5 * in_offset) + in_duration, running_time)
		end
	end

	-- animate out
	if ui_unload then
		if out_start_time < 0 then
			out_start_time = running_time
			in_start_time = -1
		end
		if running_time < out_start_time + out_duration + (5 * out_offset) then
			mylabel1.pos[1] = uilib.cubic_in(window.width - 320, window.width, out_start_time, out_start_time + out_duration, running_time)
			mylabel2.pos[1] = uilib.cubic_in(window.width - 320, window.width, out_start_time + (1 * out_offset), out_start_time + (1 * out_offset) + out_duration, running_time)
			mylabel3.pos[1] = uilib.cubic_in(window.width - 320, window.width, out_start_time + (1 * out_offset), out_start_time + (2 * out_offset) + out_duration, running_time)
			mylabel4.pos[1] = uilib.cubic_in(window.width - 320, window.width, out_start_time + (1 * out_offset), out_start_time + (3 * out_offset) + out_duration, running_time)
			mylabel5.pos[1] = uilib.cubic_in(window.width - 320, window.width, out_start_time + (1 * out_offset), out_start_time + (4 * out_offset) + out_duration, running_time)
			mylabel6.pos[1] = uilib.cubic_in(window.width - 320, window.width, out_start_time + (1 * out_offset), out_start_time + (5 * out_offset) + out_duration, running_time)
		else
			ui_close()
		end
	end
end
