---@diagnostic disable: undefined-global

local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

module = {}

module.Project = "SFML"

module.Include = function ()
	includedirs {
		GetBasePath() .. "include"
	}
end

module.Link = function ()
	filter "configurations:Debug or Release or Dist"
		libdirs {
			GetBasePath() .. "lib/msvc"
		}
		
		links {			
			"opengl32.lib",
			"freetype.lib", 
			"winmm.lib", 
			"gdi32.lib", 
			"openal32.lib",
			"flac.lib", 
			"vorbisenc.lib", 
			"vorbisfile.lib", 
			"vorbis.lib", 
			"ogg.lib", 
			"ws2_32.lib",
		}

	filter "configurations:Debug"
		links {
			"sfml-graphics-d",
			"sfml-window-d",
			"sfml-audio-d",
			"sfml-network-d",
			"sfml-system-d",
		}
	filter "configurations:Release or Dist"
		links {
			"sfml-graphics",
			"sfml-window",
			"sfml-audio",
			"sfml-network",
			"sfml-system",
		}
end

module.PostBuild = function (Configuration, BinaryOutputDir, ProjectDir)
	filter ("configurations:" .. Configuration)
		if Configuration == "Debug" then
			local from = GetBasePath() .. "bin/msvc/Debug"
			postbuildcommands {
				CopyCmd(from, BinaryOutputDir)
			}			
		elseif Configuration == "Release" or Configuration == "Dist" then

			local from = GetBasePath() .. "bin/msvc/Release"
			postbuildcommands {
				CopyCmd(from, BinaryOutputDir)
			}
		end
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