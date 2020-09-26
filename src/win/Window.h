#pragma once
#include "glue.h"
#include "Commons.h"
#include <string_view>
#include <optional>
#include <wil/win32_helpers.h>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

class Window
{
	class WindowClass
	{
	public:
		constexpr static const wchar_t* GetName()noexcept;
		constexpr static HINSTANCE GetInstance()noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"Veritas Direct3D Window";
		static Window::WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(unsigned int width = win_def_width, unsigned int height = win_def_height, std::wstring_view name = L"Test");
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
public:
	HWND GetWindowHandle()const noexcept;
	void Show()const noexcept;
	void SetTitle(std::string_view title);
	static std::optional<WPARAM> ProcessMessages()noexcept;
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	wil::unique_hwnd hWnd;
};
