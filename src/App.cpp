#include "App.h"

int App::Start()
{
	//float dt = 0.005f;
	while (true)
	{
		const auto a = Window::ProcessMessages();
		if (a)
		{
			return (int)a.value();
		}
		//ProcessInput(dt);
		//DoFrame(dt);
	}
}
