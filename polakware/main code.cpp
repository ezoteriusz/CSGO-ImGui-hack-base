#include "def.h"
#define _CRT_SECURE_NO_WARNINGS // no errors compilation
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
static EndScene oEndScene = NULL;
LPDIRECT3DDEVICE9 pDevice = nullptr;
WNDPROC oWndProc;
static HWND window = NULL;
bool init = false; // if DEVICE9 is ok
bool menu = false; // menu
bool krzysiek = false; // checkbox 
DWORD gamemodule;
DWORD LocalPlayer;
DWORD engine;
void ImGuiInit(LPDIRECT3DDEVICE9 pDevice)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(xorstr("C:\\Windows\\Fonts\\Consolas.ttf"), 20, NULL, io.Fonts->GetGlyphRangesCyrillic()); // you can change your font
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(pDevice);
}

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
    if (!pDevice) {
        pDevice = o_pDevice;
    }

    if (!init)
    {
        ImGuiInit(pDevice);
        gamemodule = (DWORD)GetModuleHandle(xorstr("client.dll")); // client.dll 
        LocalPlayer = *(DWORD*)(gamemodule + hazedumper::signatures::dwLocalPlayer); // local player
        engine = (DWORD)GetModuleHandle(xorstr("engine.dll")); // engine.dll
        init = true;
    }
    if (GetAsyncKeyState(VK_END)) {

        kiero::shutdown();  // uninject using kiro library
    }
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        menu = !menu;
    }

    if (menu) {  // menu

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        auto flagi = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
        ImGui::Begin(xorstr("ImGui Open Cheat Base by Judasz"), nullptr,flagi);
        ImGui::Checkbox(xorstr("test checkbox"),&krzysiek);
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    return oEndScene(pDevice); // loop
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
    DWORD wndProcId;
    GetWindowThreadProcessId(handle, &wndProcId);
    if (GetCurrentProcessId() != wndProcId)
        return TRUE;
    window = handle;
    return FALSE;
}


HWND GetProcessWindow()
{
    window = NULL;
    EnumWindows(EnumWindowsCallback, NULL);
    return window;
}


int mainThread()
{
    if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success) // render style prefer D3D9
    {

        kiero::bind(42, (void**)&oEndScene, hkEndScene); // EndScene
        window = GetProcessWindow(); // getting current process behind DLL
        oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc); // creating overlay for window SetWindowLongPtr
    }
    return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mainThread, NULL, 0, NULL)); // starting main thread
    }
    return TRUE;
}
