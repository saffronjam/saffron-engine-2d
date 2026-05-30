local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

module = {}

module.CopyCmd = function (from, to)
	if os.target() == "windows" then
		return "{COPY} " .. from .. " " .. to
	end
	-- On POSIX, `cp -rf SRC DST` nests (DST/SRC) when DST already exists, so a
	-- second asset copy into the same dir produces assets/Assets. Copy the
	-- directory *contents* into the target instead, which merges correctly.
	return "mkdir -p " .. to .. " && cp -rf " .. from .. ". " .. to
end

function CopyAssetsToOutput (configuration, from, binaryOutputDir, projectDir)
    filter ("configurations:" .. configuration)
        postbuildcommands {
            Utils.CopyCmd(from, binaryOutputDir),
            Utils.CopyCmd(from, projectDir)
        }
end

return module