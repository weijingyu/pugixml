project "pugixml"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/pugiconfig.hpp",
        "src/pugixml.hpp",
        "src/pugixml.cpp"

    }

    includedirs
    {
        "src",
    }

    exceptionhandling "off"
    callingconvention "FastCall"

    defines {
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"
        staticruntime "Off"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++20"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
