#include "Main.h"
#include "Includes.h"
#include "Utility.h"
#include "Driver.h"
#include "DX11Window.h"
#include "DX11.h"
#include "Data.h"
#include "Esp.h"
#include "Memory.h"
#include "Aimbot.h"
#include"loadDrv.h"
#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_win32.h"
#include "imgui-docking/imgui_impl_dx11.h"
#include "imgui-docking/imgui_internal.h"

#include "ShObjIdl.h"
#include "xorstr.hpp"



#define XTREME_DRIVER_FILE  "Helios.sys"
#define XTREME_SERVICE_NAME "Helios"
#define XTREME_DEVICE_NAME  "\\Device\\Helios"
DRIVER* m_drv = nullptr;




inline bool existsDrv(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
bool WriteDataToFile(const UCHAR pBuffer[], const DWORD dwSize, const std::string& strFileName, const DWORD dwCreationDisposition = CREATE_NEW)
{
	const auto hFile = CreateFileA(strFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) return false;
	DWORD dwNumberOfBytesWritten = NULL;
	const auto bWriteFile = WriteFile(hFile, pBuffer, dwSize, &dwNumberOfBytesWritten, nullptr);
	CloseHandle(hFile);
	return !(!bWriteFile || dwNumberOfBytesWritten != dwSize);
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
void CustomImGUIStyle()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(4, 9);
	style.WindowRounding = 5.000f;
	style.FramePadding = ImVec2(3, 3);
	style.FrameRounding = 1.000f; // Make all elements (checkboxes, etc) circles
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(1, 6);
	style.CellPadding = ImVec2(5, 5);
	style.IndentSpacing = 23.0f;
	style.ScrollbarSize = 11.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f; // Make grab a circle
	style.GrabRounding = 3.0f;
	style.PopupRounding = 2.f;
	style.Alpha = 1.0;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.TabRounding = 7.0f;

	style.ChildRounding = 7.0f;

	style.ButtonTextAlign = ImVec2(0.50f, 0.50f);

	style.ChildBorderSize = 0.100f;

	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.f;
	style.TabBorderSize = 0.000f;
	ImVec4* colors = style.Colors;

	style.Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 235);
	style.Colors[ImGuiCol_Border] = ImColor(200, 200, 200, 255);
	// style.Colors[ImGuiCol_ChildBg] = ImVec4(5.03399e-07, 4.41989e-07, 1e-06, 0.84530);
	//style.Colors[ImGuiCol_Border] = ImColor(255, 0, 0, 255);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_TitleBg] = ImColor(66, 66, 66, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(66, 66, 66, 255);


	style.Colors[ImGuiCol_CheckMark] = ImColor(0, 255, 50, 255);
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_SliderGrab] = ImColor(61, 133, 224, 250);
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(61, 133, 224, 250);
	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.97f, 0.93f, 1.00f);

	style.Colors[ImGuiCol_Header] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_TabHovered] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_SliderGrabActive] = ImColor(61, 133, 224, 250);

	style.Colors[ImGuiCol_Button] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(158, 158, 158, 250);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_Header] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_HeaderHovered] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_HeaderActive] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_SeparatorHovered] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_SeparatorActive] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_ResizeGrip] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_ResizeGripHovered] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_ResizeGripActive] = ImColor(66, 66, 66, 250);

	style.Colors[ImGuiCol_PlotHistogram] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImColor(66, 66, 66, 250);
	style.Colors[ImGuiCol_PlotLines] = ImColor(66, 66, 66, 250);

	colors[ImGuiCol_PlotHistogram] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_PlotHistogramHovered] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_PlotLinesHovered] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_PlotLines] = ImColor(66, 66, 66, 250);

	style.Colors[ImGuiCol_Tab] = ImColor(33, 33, 33, 255);
	style.Colors[ImGuiCol_TabHovered] = ImColor(66, 66, 66, 255);
	style.Colors[ImGuiCol_TabActive] = ImColor(66, 66, 66, 255);
	// colors[ImGuiCol_TabUnfocused] = ImVec4(0.038f, 0.050f, 0.136f, 1.000f);

	colors[ImGuiCol_TabUnfocusedActive] = ImColor(66, 66, 66, 250);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
	colors[ImGuiCol_NavHighlight] = ImColor(66, 66, 66, 250);
	//style.Colors[ImGuiCol_Node]
}

int main()
{
	int choice = 2;
	/*HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);*/
	//std::cout << XorStr("Please Choose Your Emulator Version... \n\n");
	printf("Checking Version...\n");
	Sleep(500);
	
	
	printf("Done\n");
	Sleep(500);
	printf("Initializing data...\n");
	Sleep(500);
	printf("Done\n");
	Sleep(500);
	printf("Injecting...");
	Sleep(2000);


	printf("\n");
	

	//std::cout << XorStr("Please Choose Your Version... \n\n");
	//std::cout << XorStr("[+] 0. Free Version \n");
	//std::cout << XorStr("[+] 1. Pro Version \n");
	//std::cout << ("Enter Your Choice & Press Enter: \n");


	//std::cin >> choice;
	//if (choice == 0 || choice == 1);
	//std::cout << "\n";

	//if (choice == 0) {
	//	Setting::profree = false;

	//}
	//else if (choice == 1) {
	//	Setting::profree = true;

	//}

	m_drv = new DRIVER(XTREME_DRIVER_FILE, XTREME_DEVICE_NAME, XTREME_SERVICE_NAME, Driver, DriverSize);
	m_drv->Load();



	TCHAR procid[256] = L"aow_exe.exe";
	TCHAR procidss[256] = L"AndroidProcess.exe";
	Game::Id = Utility::GetTrueProcessId(procidss);
	if (Game::Id > 0)
	{
		Emulator::IsSmartGaGa = TRUE;
	}
	else
	{
		Game::Id = Utility::GetTrueProcessId(procid);
		if (Game::Id > 0)
		{
			Emulator::IsGameLoop = TRUE;
		}
	}

	if (Game::Id == 0)
	{
		//	Driver::UnloadDriver(DriverName.c_str());
		MessageBox(NULL, L"Failed to inject hack", L"Error", MB_OKCANCEL);
		system("pause >nul");
		ExitProcess(0);
	}

	Game::hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Game::Id);

	if (Game::hProcess == 0)
	{
		//Driver::UnloadDriver(DriverName.c_str());
		cout << "Can't find game process, press any key to exit hack" << endl;
		system("pause >nul");
		ExitProcess(0);
	}

	if (Emulator::IsSmartGaGa == true)
	{
		EnumWindows(EnumWindowsProc, 0);
		if (IsIconic(Game::hWindow) != 0) ShowWindow(Game::hWindow, SW_RESTORE);
		Game::hWindow = FindWindowExW(Game::hWindow, 0, L"TitanRenderWindowClass", L"SmartGaGa RenderWindow");
		Game::hWindow = FindWindowEx(FindWindow(0, L"TitanRenderWindowClass"), NULL, L"TitanRenderWindowClass", L"SmartGaGa RenderWindow");

	}

	else if (Emulator::IsGameLoop == true)
	{
		EnumWindows(EnumWindowsProc, 0);
		if (IsIconic(Game::hWindow) != 0) ShowWindow(Game::hWindow, SW_RESTORE);
		Game::hWindow = FindWindowExW(Game::hWindow, 0, L"AEngineRenderWindowClass", L"AEngineRenderWindow");
	}

	if (Game::hWindow == 0)
	{
		//m_drv->Unload();
		MessageBox(NULL, L"Failed to inject hack", L"Error", MB_OKCANCEL);
		system("pause >nul");
		ExitProcess(0);
	}
	int scrWidthh = GetSystemMetrics(SM_CXSCREEN);
	int scrHeightt = GetSystemMetrics(SM_CYSCREEN);
	RECT Rect;
	GetWindowRect(Game::hWindow, &Rect);
	if (Rect.left < 0 || Rect.top < 0 || Rect.right < 0 || Rect.bottom < 0)
	{

		RECT tSize;
		GetWindowRect(Game::hWindow, &tSize);
		scrWidthh = tSize.right - tSize.left;
		scrHeightt = tSize.bottom - tSize.top;
	/*	DX11Window::Width = tSize.right - tSize.left;
		DX11Window::Height = tSize.bottom - tSize.top;*/
		//Driver::UnloadDriver(DriverName.c_str());
		/*cout << "Can't find game rect" << endl;
		system("pause >nul");
		ExitProcess(0);*/
		SetWindowPos(DX11Window::hWindow, HWND_TOPMOST, tSize.left, tSize.top, scrWidthh, scrHeightt, SWP_NOMOVE | SWP_NOSIZE);
	}

	if (Emulator::IsSmartGaGa == true)
	{
		Data::ViewMatrixBase = Data::GetViewMatrixBase(0x80000000, 0x90000000);

		if (Data::ViewMatrixBase == 0)
		{
			Data::ViewMatrixBase = Data::GetViewMatrixBase(0x90000000, 0xA0000000);
			Emulator::IsSmartGaGa4 = true;
		}
		else
		{
			Emulator::IsSmartGaGa7 = true;
		}
	}
	else if (Emulator::IsGameLoop == true)
	{
		Data::ViewMatrixBase = Data::GetViewMatrixBase(0x3D000000, 0x60000000);

		if (Data::ViewMatrixBase == 0)
		{
			Data::ViewMatrixBase = Data::GetViewMatrixBase(0x20000000, 0x30000000);
			Emulator::IsGameLoop4 = true;
		}
		else
		{
			Emulator::IsGameLoop7 = true;
		}
	}

	if (Data::ViewMatrixBase == 0)
	{
	//	m_drv->Unload();
		MessageBox(NULL,L"Khoi dong driver that bai", L"Error", MB_OKCANCEL);
		ExitProcess(0);
	}
	//	driver


	cout << "PID: " << dec << uppercase << Game::Id << " | " << hex << Game::Id << endl;
	m_drv->Unload();
	FreeConsole();
	
	// Main font and render font path here
	string MainFontPath = "C:\\Windows\\Fonts\\arial.ttf";
	string RenderFontPath = "C:\\Windows\\Fonts\\arial.ttf";

	DX11Window::Instantiate(Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top);
	DX11::Instantiate(DX11Window::hWindow, DX11Window::Width, DX11Window::Height);
	ImGui::CreateContext();
	ImGuiIO& Io = ImGui::GetIO();
	CustomImGUIStyle();


	Io.WantSaveIniSettings = true;
	
	//Io.Fonts->AddFontFromFileTTF("C:\\windows\\Fonts\\segoeuib.ttf", 14.5f);
	// Setup Dear ImGui style
	CustomImGUIStyle();

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;


	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 2.5;
	icons_config.OversampleV = 2.5;




	//
	Io.IniFilename = nullptr; //crutial for not leaving the imgui.ini file
	//Io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	Esp::LoadConfig("‪C:\\helios_Key.ini"); //‪C:\khni_Key.ini
	Io.Fonts->AddFontFromFileTTF(MainFontPath.c_str(), 15.0f);


	Io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 16.0f, &icons_config, icons_ranges);

	ImGui_ImplWin32_Init(DX11Window::hWindow);
	ImGui_ImplDX11_Init(DX11::pDevice, DX11::pImmediateContext);
	DX11::ImGui_DX11::special = Io.Fonts->AddFontFromFileTTF(RenderFontPath.c_str(), 15.0f);
	DX11::ImGui_DX11::pRegularFont = Io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 13.f, &CustomFont);
	//DX11::ImGui_DX11::my_texture = Io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(logoo), sizeof(logoo), 13.f, &CustomFont);

	Thread::hQueryThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Data::QueryThread, 0, 0, 0);
	CloseHandle(Thread::hQueryThread);
	Thread::hEspThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Esp::EspThread, 0, 0, 0);
	CloseHandle(Thread::hEspThread);
	Thread::Target = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DX11Window::SetWindowToTarget, 0, 0, 0);
	CloseHandle(Thread::Target);
	Thread::hAimbotThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Aimbot::AimbotThread, 0, 0, 0);
	CloseHandle(Thread::hAimbotThread);
	Thread::hMemoryThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::MemoryThread, 0, 0, 0);
	CloseHandle(Thread::hMemoryThread);
	Thread::MagicX = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::MagicX, 0, 0, 0);
	CloseHandle(Thread::MagicX);
	Thread::instanthitThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::Instant, 0, 0, 0);
	CloseHandle(Thread::instanthitThread);
	Thread::fastrunThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::FastRunn, 0, 0, 0);
	CloseHandle(Thread::fastrunThread);
	Thread::speedcarThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::FastCar, 0, 0, 0);
	CloseHandle(Thread::speedcarThread);
	Thread::norecoilThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::norecoil, 0, 0, 0);
	CloseHandle(Thread::norecoilThread);
	Thread::CarFlyThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::flyCar, 0, 0, 0);
	CloseHandle(Thread::CarFlyThread);
	Thread::MagicBct = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Memory::MagicBct, 0, 0, 0);
	CloseHandle(Thread::MagicBct);
	Memory::flyman();
	Memory::magnatbullot();
	MSG Message;
	while (true)
	{
		
		Sleep(10);

		if (PeekMessage(&Message, DX11Window::hWindow, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		
		if (Setting::ShowMenu)
		{
			SetForegroundWindow(DX11Window::hWindow);
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			if (DX11Window::IsClickable == true)
			{
				Utility::ChangeClickability(FALSE, DX11Window::hWindow);
				DX11Window::IsClickable = !DX11Window::IsClickable;
				Setting::ShowMenu = !Setting::ShowMenu;
			}
			else
			{
				Utility::ChangeClickability(TRUE, DX11Window::hWindow);
				DX11Window::IsClickable = !DX11Window::IsClickable;
				Setting::ShowMenu = !Setting::ShowMenu;
			}
		}
		if (GetAsyncKeyState(VK_END) & 1)
		{
			if (Setting::MagicBullet)
			{
				Memory::RestoreHook();
			}
			
			Sleep(100);
			m_drv->Unload();
			ExitProcess(1);
		}

	}

	return EXIT_SUCCESS;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	char Buffer[1000];
	GetClassNameA(hWnd, Buffer, 1000);

	string WindowClassName = Buffer;
	if (Emulator::IsSmartGaGa == TRUE)
	{
		if (WindowClassName.find("TitanRenderWindowClass") != string::npos)
		{
			wchar_t Name[1000];
			GetWindowTextW(hWnd, Name, 1000);
			Game::hWindow = FindWindowW(L"TitanRenderWindowClass", Name);
		}
	}
	else if (Emulator::IsGameLoop == TRUE)
	{
		if (WindowClassName.find("TXGuiFoundation") != string::npos)
		{
			wchar_t Name[1000];
			GetWindowTextW(hWnd, Name, 1000);
			Game::hWindow = FindWindowW(L"TXGuiFoundation", Name);
		}
	}
	else if (Emulator::IsGameLoop7 == TRUE)
	{
		if (WindowClassName.find("TXGuiFoundation") != string::npos)
		{
			wchar_t Name[1000];
			GetWindowTextW(hWnd, Name, 1000);
			Game::hWindow = FindWindowW(L"TXGuiFoundation", Name);
			if (!Game::hWindow)
			{
				Game::hWindow = FindWindowEx(NULL, NULL, L"TXGuiFoundation", NULL);
			}
			
		}
	}
	return TRUE;
	//return 0;
}