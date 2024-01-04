local skynet = require "skynet"
local harbor = require "skynet.harbor"
require "skynet.manager"	-- import skynet.monitor


skynet.start(function()
	print("Share server start")

	--local webclient = require "webclient"
	local webclient = skynet.newservice("webclient")

	local url = "http://127.0.0.1:9638/game/system/gameswitch" --"/game/system/gameswitch"
	local ok, body = skynet.call(webclient, "lua", "request", url, nil, nil, false, 3000)

	print("ok:", ok, "body:", body)
	--local sharehost = skynet.newservice("share", "host")
	
end)
