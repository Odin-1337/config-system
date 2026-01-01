#pragma once

/* default / standard includes */
#include <Windows.h> // default os functions
#include <vector> // flexable array
#include <thread> // threading
#include <chrono> // time
#include <filesystem> // file handling 
#include <cstdint> // values
#include <fstream>

/* for imgui dx11 rendering */
#include <d3d11.h>

/* formating and console printing, C++23 or later. */
#include <format>
#include <print>

#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/backend/imgui_impl_win32.h>
#include <dependencies/imgui/backend/imgui_impl_dx11.h>

#include <impl/settings/settings.h>

#include <impl/config-system/config-system.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM );
#include <workspace/overlay/overlay.h>