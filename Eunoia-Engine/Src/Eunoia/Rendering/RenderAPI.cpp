#include "RenderAPI.h"

#include "../Platform/OpenGL/RenderAPIOpenGL.h"

namespace Eunoia {

	RenderAPIInterface* RenderAPIInterface::CreateRenderAPI(RenderAPI api)
	{
		switch (api)
		{
		case EU_RENDER_API_OPENGL: return new RenderAPIOpenGL();
		}

		return EU_NULL;
	}

}
