//  ===================================================================================================================
//
//      COMMON (internal, project) - Lua xcode config
//
//  * See the comomn global xcode config file (in Internal) for a detailled explanation about this system.
//  * See the user debug project xcode config file (in ../glitchConfig/xcconifg) for a detailled explanation about
//    user customisable configuration.
//
//  ===================================================================================================================

// Search Paths
COMMON_HEADER_SEARCH_PATHS = ../../../extern/Lua/src

// Preprocessor Definitions
COMMON_GCC_PREPROCESSOR_DEFINITIONS=MACOSX IPHONEOS TARGET_OS_IPHONE

// Precompile Headers
GCC_PREFIX_HEADER = Lua_Prefix.pch							// Precompile headers.
GCC_INCREASE_PRECOMPILED_HEADER_SHARING = NO				// The precompiled header was not designed to be shared.
GCC_PRECOMPILE_PREFIX_HEADER = YES							// Yes, use precompiled headers.
