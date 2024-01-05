local skynet = require "skynet"
require "skynet.manager"	-- import skynet.register
require "myfile"
local db = {}

local command = {}

function command.GET(key)
	return db[key]
end

function command.SET(key, value)
	local last = db[key]
	db[key] = value
	return last
end

function command.SHOW()
	print("this is myservice show")
end

local function test_pr() 
	while true do
		pr()
		skynet.sleep(500)
	end
end

skynet.start(function()
	skynet.dispatch("lua", function(session, address, cmd, ...)
		cmd = cmd:upper()
		local f = command[cmd]
		if f then
			skynet.ret(skynet.pack(f(...)))
		else
			error(string.format("Unknown command %s", tostring(cmd)))
		end
	end)
--	skynet.traceproto("lua", false)	-- true off tracelog
	skynet.register "myservice"
	skynet.timeout(300, test_pr)
end)

