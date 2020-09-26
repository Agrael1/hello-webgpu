#include "window.h"
#include <emscripten/html5.h>

Window::HandleImpl Window::DUMMY;

HWND Window::GetWindowHandle()const noexcept
{
	return (void*)&DUMMY;
}
std::optional<WPARAM> Window::ProcessMessages() noexcept
{
	return{};
}
void Window::Show() const noexcept
{

}

//namespace window {
//EM_BOOL em_redraw(double /*time*/, void *userData) {
//	window::Redraw redraw = (window::Redraw)userData;
//	return redraw(); // If this returns true, rAF() will continue, otherwise it will terminate
//}
//
//}
//
//void window::loop(HWND /*wHnd*/, window::Redraw func) {
//	emscripten_request_animation_frame_loop(window::em_redraw, (void*)func);
//}
