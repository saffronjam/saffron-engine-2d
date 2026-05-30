---@diagnostic disable: undefined-global, undefined-field

OutBin     = _MAIN_SCRIPT_DIR .. "/build/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/"
OutObj     = _MAIN_SCRIPT_DIR .. "/build/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/"
OutBinDist = _MAIN_SCRIPT_DIR .. "/build/dist/%{cfg.system}/"
OutLoc     = _MAIN_SCRIPT_DIR .. "/build/"
PrjLoc     = _MAIN_SCRIPT_DIR .. "/build/"
WrkLoc     = _MAIN_SCRIPT_DIR .. "/build/"
AstFol     = "assets/"

local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

Inspect = dofile(GetBasePath() .. "inspect.lua")
Utils = dofile(GetBasePath() .. "utils.lua")

local function RequireAll()
    local result = {}
    result["ImGui"]  = dofile(GetBasePath() .. "deps/imgui/premake5.lua")
    result["SFML"]   = dofile(GetBasePath() .. "deps/SFML/premake5.lua")
    result["Glad"]   = dofile(GetBasePath() .. "deps/Glad/premake5.lua")
    result["Jcv"]    = dofile(GetBasePath() .. "deps/jcv/premake5.lua")
    result["Spdlog"] = dofile(GetBasePath() .. "deps/spdlog/premake5.lua")
    result["Box2D"]  = dofile(GetBasePath() .. "deps/Box2D/premake5.lua")
    return result
end

local ThirdParties

local function LinkAll()
    for k, v in pairs(ThirdParties) do
        if v.Link ~= nil then
            v.Link()            
        end
    end
end

local function IncludeAll()
    for k, v in pairs(ThirdParties) do
        if v.Include ~= nil then
            v.Include()            
        end
    end
end

--[[
 Called by Client, because only Client knows
 the BinaryOutputDir for sure
]]
local function PreBuildAll(Configuration, BinaryOutputDir, ProjectDir)
    for k, v in pairs(ThirdParties) do
        if v.PreBuild ~= nil then
            v.PreBuild(Configuration, BinaryOutputDir, ProjectDir)            
        end
    end
end

--[[
 Called by Client, because only Client knows
 the BinaryOutputDir for sure
]]
local function PostBuildAll(Configuration, BinaryOutputDir, ProjectDir)
    for k, v in pairs(ThirdParties) do
        if v.PostBuild ~= nil then
            v.PostBuild(Configuration, BinaryOutputDir, ProjectDir)            
        end
    end
end


-- Start module

local module = {}

module.Project = "saffron-engine-2d"

module.Include = function ()
    includedirs {
        GetBasePath() .. "src"
    }    
    -- Include third parties
    IncludeAll()
end

module.Link = function ()
    links {
        module.Project
    }
end

module.PreBuild = function (Configuration, BinaryOutputDir, ProjectDir)
    disablewarnings { 
        "4244",
        "4267"
    }
    PreBuildAll(Configuration, BinaryOutputDir, ProjectDir)
end

module.PostBuild = function (Configuration, BinaryOutputDir, ProjectDir)    
	filter ("configurations:" .. Configuration)
	    local resFrom = GetBasePath() .. AstFol
        local resBinTo = BinaryOutputDir .. AstFol
        local resProjTo = ProjectDir .. AstFol
        postbuildcommands {
            Utils.CopyCmd(resFrom, resBinTo),
            Utils.CopyCmd(resFrom, resProjTo),
        }
    PostBuildAll(Configuration, BinaryOutputDir, ProjectDir)
end

module.SetStartUpProject = function (projectName)
    workspace "saffron"
        startproject (projectName)
end


group "Engine/deps"
ThirdParties = RequireAll()
group "Engine"
module.PostBuildSfml = ThirdParties["SFML"].PostBuild

module.AddDefines = function()
	filter "configurations:Debug or Release or Dist"
        defines {
            "SE_IMGUI_INI_PATH=\"assets/imgui.ini\""
        }
    filter "system:windows"
        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "SE_PLATFORM_WINDOWS",
        }
    filter "system:linux"
        defines {
            "SE_PLATFORM_LINUX",
        }
    filter {}
    filter "configurations:Debug"
        defines { "DEBUG", "SE_DEBUG" }
    filter "configurations:Release"
        defines { "NDEBUG", "SE_RELEASE" }
    filter "configurations:Dist"
        defines { "NDEBUG", "SE_DIST" }
end

project (module.Project)
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	staticruntime "On"

	filter "system:linux"
		buildoptions { "-std=c++23" }
	filter "system:windows"
		buildoptions { "/std:c++latest" }
	filter {}

	-- MSVC-style PCH: every .cpp already #includes "saffron_pch.h" explicitly.
	-- gcc's force-include PCH double-defines pragma-once headers, so only
	-- enable the precompiled header on Windows; Linux just compiles the
	-- explicit includes normally.
	filter "system:windows"
		pchheader ("saffron_pch.h")
		pchsource ("src/saffron_pch.cpp")
	filter {}

	targetdir (OutBin)
	objdir (OutObj)
	location (OutLoc)

    files {
        "src/**.h",
		"src/**.c",
		"src/**.hpp",
		"src/**.cpp",
    }

    -- Compile only the matching platform implementation
    filter "system:not windows"
        removefiles { "src/saffron/platform/windows/**" }
    filter "system:not linux"
        removefiles { "src/saffron/platform/linux/**" }
    filter {}

    disablewarnings {
        "4244",
        "4267"
    }

    -- Include self and third parties
    module.Include()

    -- Link third parties
    LinkAll()
    
    module.AddDefines()

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"
    filter "configurations:Dist"
        optimize "On"

group ""

return module
