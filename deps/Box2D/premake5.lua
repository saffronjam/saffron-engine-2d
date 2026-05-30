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
	filter { "system:windows", "configurations:Debug or Release or Dist" }
		libdirs {
			GetBasePath() .. "lib"
		}
		links {
			"Box2D.lib"
		}
	-- Linux: link the static lib built from Box2D 2.3.1 source (same API as
	-- the vendored headers the engine compiles against)
	filter "system:linux"
		libdirs {
			GetBasePath() .. "lib/linux"
		}
		links {
			"Box2D"
		}
	filter {}
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