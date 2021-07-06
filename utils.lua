local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

module = {}

module.CopyCmd = function (from, to)
	return "{COPY} " .. from .. " " .. to
end

module.CopyAssetsToOutput = function (Configuration, BinaryOutputDir, ProjectDir)
    filter ("configurations:" .. Configuration)
		if Configuration == "Debug" or Configuration == "Release" then
			local resFrom = GetBasePath() .. AstFol
            local resBinTo = BinaryOutputDir .. AstFol
            local resProjTo = ProjectDir .. AstFol
            postbuildcommands {
                Utils.CopyCmd(resFrom, resBinTo),
                Utils.CopyCmd(resFrom, resProjTo)
            }
		elseif Configuration == "Dist" then
            local resFrom = GetBasePath() .. AstFol
            local resBinTo = BinaryOutputDir .. AstFol
            local resProjTo = ProjectDir .. AstFol
            postbuildcommands {
                Utils.CopyCmd(resFrom, resBinTo),
                Utils.CopyCmd(resFrom, resProjTo)
            }
		end
end

return module