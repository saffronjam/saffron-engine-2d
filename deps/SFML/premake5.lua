---@diagnostic disable: undefined-global

local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

module = {}

module.Project = "SFML"

module.Include = function ()
	-- Windows uses the vendored SFML 2.5.1 headers; Linux uses the system
	-- SFML headers (default /usr/include) so they match the system lib.
	filter "system:windows"
		includedirs {
			GetBasePath() .. "include"
		}
	filter {}
end

module.Link = function ()
	-- Windows: vendored SFML 2.5.1 prebuilts + Win32 system libs
	filter { "system:windows", "configurations:Debug or Release or Dist" }
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

	filter { "system:windows", "configurations:Debug" }
		links {
			"sfml-graphics-d",
			"sfml-window-d",
			"sfml-audio-d",
			"sfml-network-d",
			"sfml-system-d",
		}
	filter { "system:windows", "configurations:Release or Dist" }
		links {
			"sfml-graphics",
			"sfml-window",
			"sfml-audio",
			"sfml-network",
			"sfml-system",
		}

	-- Linux: link the distro's system SFML (2.6.x, source-compatible with 2.5) + GL
	filter "system:linux"
		links {
			"sfml-graphics",
			"sfml-window",
			"sfml-audio",
			"sfml-network",
			"sfml-system",
			"GL",
		}

	filter {}
end

module.PostBuild = function (Configuration, BinaryOutputDir, ProjectDir)
	-- Only Windows ships SFML DLLs alongside the executable; on Linux the
	-- shared objects come from the system and need no copy step.
	filter { "system:windows", "configurations:" .. Configuration }
		if Configuration == "Debug" then
			local from = GetBasePath() .. "bin/msvc/Debug"
			postbuildcommands {
				Utils.CopyCmd(from, BinaryOutputDir)
			}			
		elseif Configuration == "Release" or Configuration == "Dist" then

			local from = GetBasePath() .. "bin/msvc/Release"
			postbuildcommands {
				Utils.CopyCmd(from, BinaryOutputDir)
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