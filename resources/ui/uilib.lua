--Useful UI helpers for making UI programming easier

-- make sure window global is declared
window = {}

-- uilib namespace
uilib = {}

-- base ui control table with some handy accessors
--[[function uilib.new(settings)
	settings.x = function
		return settings.pos[1]
	end
end]]--

-- bounce easing (out only)
function uilib.bounce(a, b, start_t, end_t, t)
	-- some basic time sanity checks
	if t < start_t then
		return a
	elseif t > end_t then
		return b
	end

	-- calculate the distance we have to travel
    local d = b - a

    -- how far are we into the time period?
    local at = (t - start_t) / (end_t - start_t)

	-- do the easing
    if (at < 1 / 2.75) then
        return d * (7.5625 * at * at) + a
    elseif (at < 2 / 2.75) then
        at = at - 1.5 / 2.75
        return d * (7.5625 * at * at + 0.75) + a
    elseif (at < 2.5 / 2.75) then
        at = at - 2.25 / 2.75
        return d * (7.5625 * at * at + 0.9375) + a
    end

    at = at - 2.625 / 2.75
    return d * (7.5625 * at * at + 0.984375) + a
end

-- cubic in easing
function uilib.cubic_in(a, b, start_t, end_t, t)
    -- some basic time sanity checks
	if t < start_t then
		return a
	elseif t > end_t then
		return b
	end

	-- calculate the distance we have to travel
    local d = b - a

    -- how far are we into the time period?
    local at = (t - start_t) / (end_t - start_t)

    return (d * at * at * at) + a
end
