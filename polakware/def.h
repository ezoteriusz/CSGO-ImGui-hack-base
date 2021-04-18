 //  files {
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "kiero.h"
#include "xorstr.h"
#include "offsets 18.04.2021 .h"
// }
// 
// ImGui base{
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
//}

// others 
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "winmm.lib")
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern LPDIRECT3DDEVICE9 pDevice;