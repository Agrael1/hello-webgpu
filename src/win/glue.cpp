#include "glue.h"
#include <shellapi.h>
/**
 * Entry point for the 'real' application.
 *
 * \param[in] argc count of program arguments in argv
 * \param[in] argv program arguments (excluding the application)
 */
extern "C" int __main__(int /*argc*/, wchar_t* /*argv*/[]);

/**
 * Entry point. Workaround for Emscripten needing an \c async start.
 */
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);
    return __main__(argc, argv);
}
