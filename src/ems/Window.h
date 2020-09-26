#pragma once
#include <string_view>
#include <optional>
#include "Commons.h"

using HWND = void*;
using WPARAM = uintptr_t;

class Window
{
public:
	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
public:
	HWND GetWindowHandle()const noexcept;
	void Show()const noexcept;
public:
	static std::optional<WPARAM> ProcessMessages() noexcept;
private:
	static struct HandleImpl {} DUMMY;
	//bool exit_status = false;
};
