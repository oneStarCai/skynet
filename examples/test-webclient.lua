local webclient = require "webclient"

local url = "http://127.0.0.1:9638/game/system/gameswitch" --"/game/system/gameswitch"
local ok, body = skynet.call(webclient, "lua", "request", url, nil, nil, false, 3000)

print("ok:", ok, "body:", body)
