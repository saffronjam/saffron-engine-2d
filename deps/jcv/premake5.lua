---@diagnostic disable: undefined-global

local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

module = {}

module.Project = "jcv"
module.Include = function ()
	includedirs {
		GetBasePath() .. "include"
	}
end

project (module.Project)
    kind "None"
    language "C++"
	
	targetdir (OutBin)
	objdir (OutObj)
	location (OutLoc)

	files {
        "include/**.h",
		"include/**.c",
		"include/**.hpp",
		"include/**.cpp",
    }

return module