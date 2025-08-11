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

group "App"
include "PackedHeader"
group ""