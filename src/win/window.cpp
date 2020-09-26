#include "window.h"
#include <algorithm>


//namespace impl {
////**************************** Windows Event Loop ****************************/
//
///**
// * Helper to toggle between windowed mode and fullscreen.
// * 
// * \todo tested only on Windows 10 so far
// * 
// * \param[in] hWnd main application window handle
// * \param[in] exit \c true if the toggle can only \e exit fullscreen
// */
//static void toggleFullscreen(HWND const hWnd, bool const exit = false) {
//	/*
//	 * Note: stores the *single* window state of the main application (and
//	 * doesn't track that the window handle is the same one).
//	 */
//	static struct FSState {
//		FSState() : fullscrn(false) {}
//		bool fullscrn; // whether the last call set fullscreen
//		LONG wndStyle; // the windowed mode style (to restore)
//		LONG extStyle; // the windowed mode extended style (to restore)
//		RECT origRect; // the windowed mode original size and position
//	} state;
//	if (state.fullscrn == false) {
//		if (!exit) {
//			/*
//			 * Stores the current window styles, unsets the menu bar and any
//			 * frame features, then fits the undecorated content to the
//			 * nearest monitor bounds.
//			 */
//			state.wndStyle = GetWindowLong(hWnd, GWL_STYLE);
//			state.extStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
//			GetWindowRect(hWnd, &state.origRect);
//			MONITORINFO info; info.cbSize = sizeof(info);
//			if (GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &info)) {
//				SetWindowLong(hWnd, GWL_STYLE,   state.wndStyle & ~WS_OVERLAPPEDWINDOW);
//				SetWindowLong(hWnd, GWL_EXSTYLE, state.extStyle & ~WS_EX_OVERLAPPEDWINDOW);
//				SetWindowPos(hWnd, HWND_TOP, info.rcMonitor.left, info.rcMonitor.top,
//					info.rcMonitor.right - info.rcMonitor.left,
//						info.rcMonitor.bottom - info.rcMonitor.top, SWP_FRAMECHANGED);
//				state.fullscrn = !state.fullscrn;
//			}
//		}
//	} else {
//		/*
//		 * Restores styles and size.
//		 */
//		SetWindowLong(hWnd, GWL_STYLE,   state.wndStyle);
//		SetWindowLong(hWnd, GWL_EXSTYLE, state.extStyle);
//		SetWindowPos (hWnd, NULL, state.origRect.left, state.origRect.top,
//			state.origRect.right - state.origRect.left,
//				state.origRect.bottom - state.origRect.top, SWP_FRAMECHANGED);
//		state.fullscrn = !state.fullscrn;
//	}
//}
//


UINT GetTrueWindowDpi(HWND hWnd = nullptr) 
{
	UINT dpi = USER_DEFAULT_SCREEN_DPI;

#if _WIN32_WINNT >= 0x0605
	if (hWnd) 
	{
		dpi = GetDpiForWindow(hWnd);
	} 
	else
	{
		dpi = GetDpiForSystem();
	}
#else 
	wil::unique_hdc hdc{ GetDC(hWnd) };
	if (hdc) dpi = std::max(
		GetDeviceCaps(hdc.get(), LOGPIXELSX),
		GetDeviceCaps(hdc.get(), LOGPIXELSY)
	);
#endif
	if (!dpi) return USER_DEFAULT_SCREEN_DPI;
	return dpi;
}
//
///**
// * Standard \c WndProc function for Windows events, passing key presses, mouse
// * events, etc., to the different \c impl internal functions.
// * 
// * \param[in] hWnd window handle
// * \param[in[ uMsg message type
// */
//LRESULT CALLBACK windowEvents(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam) {
//	switch (uMsg) {
//	case WM_SIZE:
//		/*
//		 * See note about the swap in surfaceResize and WM_WINDOWPOSCHANGED,
//		 * so look into a better way of doing this.
//		 */
//		if (wParam != SIZE_MINIMIZED) {
//			//impl::surfaceResize(LOWORD(lParam), HIWORD(lParam), dpiScale);
//		}
//		break;
//	case WM_CLOSE:
//		PostQuitMessage(0);
//		return 0;
//	case WM_ERASEBKGND:
//		/*
//		 * Return non-zero to tell Windows we handled the erase (a no-op).
//		 */
//		return 1;
//	case WM_WINDOWPOSCHANGED:
//		/*
//		 * Early on, when first experimenting with ANGLE, moving the window
//		 * was causing smearing artifacts. The workaround was to recopy the
//		 * colour buffer for every message. This no longer appears necessary
//		 * for moving (at a guess, the DX11 backend changed) but resizing and
//		 * DPI changes look better. The (SWP_NOSIZE | SWP_NOMOVE) corresponds
//		 * to window activation and DPI/monitor changes.
//		 * 
//		 * A true fix would be to redraw, not just swap buffers, allowing for
//		 * live resizing. This is a lightweight compromise.
//		 */
//		if ((reinterpret_cast<WINDOWPOS*>(lParam)->flags &  SWP_NOSIZE) == 0 ||
//			(reinterpret_cast<WINDOWPOS*>(lParam)->flags & (SWP_NOSIZE | SWP_NOMOVE)) == (SWP_NOSIZE | SWP_NOMOVE))
//		{
//			/*
//			 * Hmm, I'm not happy still with this. It looks to cause massive
//			 * flickering when going from different DPI monitors in some
//			 * configs. The swap is commented out for now.
//			 * 
//			 * if (eglDisplay != EGL_NO_DISPLAY && eglSurface != EGL_NO_SURFACE) {
//			 *     eglSwapBuffers(eglDisplay, eglSurface);
//			 * }
//			 */
//		}
//		break;
//	case WM_KEYUP:
//		/*
//		 * F11 toggles between fullscreen and windowed mode; ESC exits
//		 * fullscreen (but does nothing if windowed).
//		 * 
//		 * TODO: other function keys for 4:3, 16:9, etc.?
//		 */
//		if (wParam == VK_F11) {
//			toggleFullscreen(hWnd);
//		} else {
//			if (wParam == VK_ESCAPE) {
//				toggleFullscreen(hWnd, true);
//			}
//		}
//		break;
//	#ifdef WM_DPICHANGED
//		/*
//		 * Handles the DPI change when moving between monitors (Win8.1+). The
//		 * incoming RECT contains the suggested size, which works better than
//		 * taking the DPI and calculating (given, when switching between
//		 * normal to high to normal, increasingly shrinks the window). It's
//		 * still not 100%, but is at least consistent when moving between
//		 * monitors.
//		 * 
//		 * TODO: older Windows (pre-Win10 1703) appear to get the size calculation wrong?
//		 */
//	case WM_DPICHANGED: {
//		const RECT* rect = reinterpret_cast<RECT*>(lParam);
//		SetWindowPos(hWnd, NULL, rect->left, rect->top,
//			rect->right - rect->left,
//			rect->bottom - rect->top,
//				SWP_NOZORDER | SWP_NOACTIVATE);
//		return 0;
//	}
//#endif
//#ifdef WM_GETDPISCALEDSIZE
//	/*
//	 * Windows gets this wrong (still as of Feb 2018), so we calculate it
//	 * manually. The original size and scale work because 'hWnd' is still at
//	 * the old DPI and 'wParam' is the new. Return TRUE to say we handled it
//	 * ('lParam' contains the returned size).
//	 * 
//	 * TODO: grab the menu parameter
//	 */
//	case WM_GETDPISCALEDSIZE: {
//		float scale = static_cast<float>(wParam) / getWindowDpi(hWnd);
//		RECT rect;
//		GetClientRect(hWnd, &rect);
//		rect.right  = static_cast<LONG>((rect.right - rect.left) * scale + 0.5f);
//		rect.bottom = static_cast<LONG>((rect.bottom - rect.top) * scale + 0.5f);
//		rect.left   = 0;
//		rect.top    = 0;
//		if (AdjustWindowRectExForDpi != NULL) {
//			AdjustWindowRectExForDpi(&rect,
//				GetWindowLong(hWnd, GWL_STYLE), FALSE,
//				GetWindowLong(hWnd, GWL_EXSTYLE),
//					static_cast<UINT>(wParam));
//		} else {
//			/*
//			 * TODO: test this fallback on older Windows
//			 */
//			AdjustWindowRectEx(&rect,
//				GetWindowLong(hWnd, GWL_STYLE), FALSE,
//				GetWindowLong(hWnd, GWL_EXSTYLE));
//		}
//		SIZE* size = reinterpret_cast<SIZE*>(lParam);
//		size->cx = rect.right - rect.left;
//		size->cy = rect.bottom - rect.top;
//		return TRUE;
//	}
//#endif
//	}
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}
//
///**
// * \return \c true if the application is still running (i.e. did not quit)
// */
//bool yield() {
//	bool running = true;
//	MSG msg;
//	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//		if (msg.message == WM_QUIT) {
//			running = false;
//		}
//		TranslateMessage(&msg);
//		DispatchMessage (&msg); 
//	}
//	Sleep(WINDOW_SLEEP_PERIOD);
//	return running; 
//}
//}
//
////******************************** Public API ********************************/
//
//window::Handle window::create(unsigned winW, unsigned winH, const char* /*name*/) 
//{
//	Window x;
//	return nullptr;
//}
////
//void window::destroy(window::Handle wHnd) 
//{
////	DestroyWindow(TO_WIN(wHnd));
//}
////
//void window::show(window::Handle wHnd, bool show) 
//{
////	ShowWindow(TO_WIN(wHnd), (show) ? SW_SHOWDEFAULT : SW_HIDE);
//}
////
//void window::loop(window::Handle /*wHnd*/, window::Redraw func) {
//	//while (impl::yield()) {
//	//	if (func) {
//	//		if (!func()) {
//	//			break;
//	//		}
//	//	}
//	//}
//}

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:hInst(GetModuleHandle(nullptr))
{
	const WNDCLASSEX wcWindow 
	{ 
	.cbSize = sizeof(wcWindow),
	.style = CS_OWNDC,
	.lpfnWndProc = HandleMsgSetup,
	.cbClsExtra = 0,
	.cbWndExtra = 0,
	.hInstance = GetInstance(),
	.hIcon = nullptr,
	.hCursor = nullptr,
	.hbrBackground = nullptr,
	.lpszMenuName = nullptr,
	.lpszClassName = GetName(),
	.hIconSm = nullptr
	};
	RegisterClassEx(&wcWindow);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}
constexpr const wchar_t* Window::WindowClass::GetName() noexcept
{
	return Window::WindowClass::wndClassName;
}
constexpr HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(unsigned int in_width, unsigned int in_height, std::wstring_view name)
	:width(in_width), height(in_height)
{
	const uint32_t dpi = GetTrueWindowDpi();
#if WINDOW_WIN_RESIZE
	constexpr DWORD dwWndStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
#else
	constexpr DWORD dwWndStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
#endif
	constexpr DWORD dwExtStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE /*| WS_EX_ACCEPTFILES*/;

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = MulDiv(in_width, dpi, USER_DEFAULT_SCREEN_DPI);
	rect.bottom = MulDiv(in_height, dpi, USER_DEFAULT_SCREEN_DPI);

#if (_WIN32_WINNT >= 0x0605) 
		AdjustWindowRectExForDpi(&rect, dwWndStyle, FALSE, dwExtStyle, dpi);
#else 
		AdjustWindowRectEx(&rect, dwWndStyle, FALSE, dwExtStyle);
#endif

	hWnd.reset(CreateWindowEx(dwExtStyle,
		WindowClass::GetName(), name.data(),
		dwWndStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, WindowClass::GetInstance(), nullptr));

	if (hWnd) 
	{
		RegisterHotKey(hWnd.get(), VK_SNAPSHOT, 0, VK_SNAPSHOT);
	}
}

LRESULT __stdcall Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Create routine initializer
	if (msg == WM_NCCREATE)
	{
		// Extract data from creation of window
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to win class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set msgproc to to non setup handle
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT __stdcall Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to win class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward msg to class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE: {
		PostQuitMessage(0);// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND Window::GetWindowHandle() const noexcept
{
	return hWnd.get();
}

void Window::Show() const noexcept
{
	ShowWindow(hWnd.get(), SW_SHOWDEFAULT);
}

std::optional<WPARAM> Window::ProcessMessages() noexcept
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return{};
}
