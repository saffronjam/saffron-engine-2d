---@diagnostic disable: undefined-global

local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

module = {}

module.Project = "Box2D"
module.Include = function ()
	includedirs {
		GetBasePath() .. "include"
	}
end
module.Link = function ()
	filter "configurations:Debug or Release or Dist"
		libdirs {
			GetBasePath() .. "lib"
		}
		links {
			"Box2D.lib"
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