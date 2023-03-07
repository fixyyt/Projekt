#include "gui.h"
#include "cheats.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include"globals.h"
#include"imguipp.h"
#include <fstream>
#include<iostream>

void saveConfig(globals::Settings currentSettings)
{
	std::ofstream file("config.txt");

	if (!file)
	{
		std::cout << "Error: Unable to open file for writing" << std::endl;
		return;
	}

	file << currentSettings.glow << std::endl;
	file << currentSettings.glowColor[0] << " " << currentSettings.glowColor[1] << " " << currentSettings.glowColor[2] << " " << currentSettings.glowColor[3] << std::endl;
	file << currentSettings.glowColorTeam[0] << " " << currentSettings.glowColorTeam[1] << " " << currentSettings.glowColorTeam[2] << " " << currentSettings.glowColorTeam[3] << std::endl;
	file << currentSettings.chamsTeam[0] << " " << currentSettings.chamsTeam[1] << " " << currentSettings.chamsTeam[2] << std::endl;

	for (int i = 0; i < 4; i++)
	{
		file << currentSettings.hitbox[i] << " ";
	}
	file << std::endl;

	file << currentSettings.radar << std::endl;
	file << currentSettings.trigger << std::endl;
	file << currentSettings.aimassist << std::endl;
	file << currentSettings.bhop << std::endl;
	file << currentSettings.noFlash << std::endl;
	file << currentSettings.noRecoil << std::endl;
	file << currentSettings.aimassistSmooth << std::endl;
	file << currentSettings.aimassist2 << std::endl;
	file << currentSettings.fovChanger << std::endl;
	file << currentSettings.chams << std::endl;
	file << currentSettings.addBright << std::endl;
	file << currentSettings.teamGlow << std::endl;
	file << currentSettings.removeFlash << std::endl;
	file << currentSettings.changeFlash << std::endl;
	file << currentSettings.spray << std::endl;
	file << currentSettings.crossh << std::endl;
	file << currentSettings.autoPistol << std::endl;

	file << currentSettings.brightness << std::endl;
	file << currentSettings.fov << std::endl;
	file << currentSettings.aimbotfov << std::endl;
	file << currentSettings.tab << std::endl;
	file << currentSettings.ms << std::endl;
	file << currentSettings.strenght << std::endl;
	file << currentSettings.sprayTime << std::endl;
	file << currentSettings.a << std::endl;

	file.close();
}

void loadConfig(globals::Settings& currentSettings)
{
	std::ifstream file("config.txt");

	if (!file)
	{
		std::cout << "Error: Unable to open file for reading" << std::endl;
		return;
	}

	file >> currentSettings.glow;
	file >> currentSettings.glowColor[0] >> currentSettings.glowColor[1] >> currentSettings.glowColor[2] >> currentSettings.glowColor[3];
	file >> currentSettings.glowColorTeam[0] >> currentSettings.glowColorTeam[1] >> currentSettings.glowColorTeam[2] >> currentSettings.glowColorTeam[3];
	file >> currentSettings.chamsTeam[0] >> currentSettings.chamsTeam[1] >> currentSettings.chamsTeam[2];

	for (int i = 0; i < 4; i++)
	{
		file >> currentSettings.hitbox[i];
	}

	file >> currentSettings.radar;
	file >> currentSettings.trigger;
	file >> currentSettings.aimassist;
	file >> currentSettings.bhop;
	file >> currentSettings.noFlash;
	file >> currentSettings.noRecoil;
	file >> currentSettings.aimassistSmooth;
	file >> currentSettings.aimassist2;
	file >> currentSettings.fovChanger;
	file >> currentSettings.chams;
	file >> currentSettings.addBright;
	file >> currentSettings.teamGlow;
	file >> currentSettings.removeFlash;
	file >> currentSettings.changeFlash;
	file >> currentSettings.spray;
	file >> currentSettings.crossh;
	file >> currentSettings.autoPistol;

	file >> currentSettings.brightness;
	file >> currentSettings.fov;
	file >> currentSettings.aimbotfov;
	file >> currentSettings.tab;
	file >> currentSettings.ms;
	file >> currentSettings.strenght;
	file >> currentSettings.sprayTime;
	file >> currentSettings.a;

	file.close();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}
void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;
	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();
	
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

void gui::Render() noexcept
{

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Fixy legit cheat",
		&isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);
	ImGui::GetStyle().FrameRounding = 3.f;
	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 300);
	{
		ImGui::Spacing();
		if (ImGui::Button("Visuals", ImVec2(300 - 15, 39)))
		{
			globals::Settings::tab = 1;
		}

		ImGui::Spacing();
		if (ImGui::Button("Triggerbot", ImVec2(300 - 15, 39)))
		{
			globals::Settings::tab = 2;
		}
		ImGui::Spacing();
		if (ImGui::Button("Aimassist", ImVec2(300 - 15, 39)))
		{
			globals::Settings::tab = 3;
		}
		ImGui::Spacing();
		if (ImGui::Button("Misc", ImVec2(300 - 15, 39)))
		{
			globals::Settings::tab = 4;
		}
		ImGui::Spacing();
		if (ImGui::Button("Config", ImVec2(300 - 15, 39)))
		{
			globals::Settings::tab = 5;
		}
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
		imguipp::center_text_ex("Fixy", 300, 1, false);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("i hate niggers");
	}
	ImGui::NextColumn();
	{
		if(globals::Settings::tab == 1)
		{
			ImGui::Checkbox("Glow", &globals::Settings::glow);
			if (globals::Settings::glow) {
				ImGui::Text("ENEMY:");
				ImGui::Spacing();
				ImGui::ColorEdit4("Enemy glow color", globals::Settings::glowColor);
				ImGui::Spacing();
				ImGui::Checkbox("Enable team glow", &globals::Settings::teamGlow);
				if (globals::Settings::teamGlow)
				{
					ImGui::Text("TEAM:");
					ImGui::Spacing();
					ImGui::ColorEdit4("Team glow color", globals::Settings::glowColorTeam);

				}
			}
			else
			{
				globals::Settings::teamGlow = false;
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Chams", &globals::Settings::chams);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Brightness", &globals::Settings::addBright);
			if (globals::Settings::addBright)
			{
				ImGui::SliderFloat("Bright level", &globals::Settings::brightness, 0, 100);
			}
			if(!globals::Settings::addBright) {
				globals::Settings::brightness = 0;
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Fov changer", &globals::Settings::fovChanger);
			if (globals::Settings::fovChanger)
			{
				ImGui::SliderInt("Fov", &globals::Settings::fov, 50, 150);
			}
		}
		else if (globals::Settings::tab == 2)
		{
			ImGui::Checkbox("Triggerbot [ mouse 4 ]", &globals::Settings::trigger);
			ImGui::Spacing();
			ImGui::SliderInt("Delay in ms", &globals::Settings::ms, 1, 100);
			ImGui::Spacing();
			ImGui::Checkbox("Spray", &globals::Settings::spray);
			if (globals::Settings::spray)
			{
				ImGui::SliderInt("SprayTime", &globals::Settings::sprayTime, 1, 6);
			}
			else {
				globals::Settings::sprayTime = 1;
			}
			ImGui::Spacing();
			ImGui::Checkbox("AutoPistol", &globals::Settings::autoPistol);
		}
		else if (globals::Settings::tab == 3)
		{
			ImGui::Checkbox("Aimassist [ Left Mouse Button ]", &globals::Settings::aimassist);
			if (globals::Settings::aimassist) {
				globals::Settings::aimassist2 = false;
				ImGui::SliderFloat("Aimassist smooth", &globals::Settings::aimassistSmooth, 1, 100);
				ImGui::Spacing();
				ImGui::SliderFloat("Fov", &globals::Settings::aimbotfov, 1, 180);
				
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Aimassist [ always ON ]", &globals::Settings::aimassist2);
			if (globals::Settings::aimassist2)
			{
				globals::Settings::aimassist = false;
				ImGui::SliderFloat("Aimassist smooth", &globals::Settings::aimassistSmooth, 1, 100);
				ImGui::Spacing();
				ImGui::SliderFloat("Fov", &globals::Settings::aimbotfov, 1, 180);
			}
		}
		else if (globals::Settings::tab == 4)
		{
			ImGui::Checkbox("Bunnyhop", &globals::Settings::bhop);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Radar", &globals::Settings::radar);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("No flash", &globals::Settings::noFlash);
			if (globals::Settings::noFlash)
			{
				ImGui::Checkbox("Remove flash", &globals::Settings::removeFlash);
				if (globals::Settings::removeFlash)
				{
					globals::Settings::changeFlash = false;
				}
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Checkbox("Change strenght of flash", &globals::Settings::changeFlash);
				if (globals::Settings::changeFlash)
				{
					globals::Settings::removeFlash = false;
					ImGui::SliderFloat("Strenght", &globals::Settings::strenght, 0, 255);
				}
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("No Recoil", &globals::Settings::noRecoil);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Recoil Crosshair [ Window Mode Only ]", &globals::Settings::crossh);
			
		}
		else if (globals::Settings::tab == 5)
		{
			float buttonWidth = 0.0f;
			float centerOffset = ImGui::GetWindowWidth() / 2.0f - buttonWidth / 2.0f;
			float buttonHeight = 100.0f;
			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();

			ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
			ImGui::SetCursorPosY((windowHeight - buttonHeight) * 0.5f);
			ImGui::SetCursorPosX(centerOffset);
			globals::Settings currentSettings;
			if (ImGui::Button("Save", ImVec2(285, 39)))
			{
				saveConfig(currentSettings);
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SetCursorPosX(centerOffset);
			if (ImGui::Button("Load", ImVec2(300 - 15, 39)))
			{
				loadConfig(currentSettings);
			}
		}
		else {
			imguipp::center_text_ex(" _______  __  ___   ___ ____    ____ ", 500, 0, false);
			imguipp::center_text_ex("|   ____||  | \\  \\ /  / \\   \\  /   / ", 500, 0, false);
			imguipp::center_text_ex("|  |__   |  |  \\  V  /   \\   \\/   /  ", 500, 0, false);
			imguipp::center_text_ex("|   __|  |  |   >   <     \\_    _/   ", 500, 0, false);
			imguipp::center_text_ex("|  |     |  |  /  .  \\      |  |     ", 500, 0, false);
			imguipp::center_text_ex("|__|     |__| /__/ \\__\\     |__|     ", 500, 0, false);
			imguipp::center_text_ex("                                       ", 500, 0, false);
			imguipp::center_text_ex("                                       ", 500, 0, false);
			imguipp::center_text_ex(" _            _ _   ", 500, 0, false);
			imguipp::center_text_ex("| | ___  __ _(_) |_ ", 500, 0, false);
			imguipp::center_text_ex("| |/ _ \\/ _` | | __|", 500, 0, false);
			imguipp::center_text_ex("| |  __/ (_| | | |_ ", 500, 0, false);
			imguipp::center_text_ex("|_|\\___|\\__, |_|\\__|", 500, 0, false);
			imguipp::center_text_ex("        |___/       ", 500, 0, false);
			imguipp::center_text_ex("                                       ", 500, 0, false);
			imguipp::center_text_ex("                                       ", 500, 0, false);
			imguipp::center_text_ex("      _                _   ", 500, 0, false);
			imguipp::center_text_ex("  ___| |__   ___  __ _| |_ ", 500, 0, false);
			imguipp::center_text_ex(" / __| '_ \\ / _ \\/ _` | __|", 500, 0, false);
			imguipp::center_text_ex("| (__| | | |  __/ (_| | |_ ", 500, 0, false);
			imguipp::center_text_ex(" \\___|_| |_|\\___|\\__,_|\\__|", 500, 0, false);
		}
	}
	ImGui::End();
}
