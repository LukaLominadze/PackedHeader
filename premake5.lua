workspace "PackedHeader"
	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	architecture "x64"

	startproject "PackedHeader"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "vendor/nfd/build/Build-nfd-Only.lua"
group ""

group "App"
include "PackedHeader"
group ""