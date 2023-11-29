#include "FinalRenderer.h"
#include "../Platform/OpenGL/FinalRendererGL.h"

namespace Eunoia {

	FinalRenderer* FinalRenderer::CreateFinalRenderer(RenderAPI renderAPI)
	{
		switch (renderAPI)
		{
		case EU_RENDER_API_OPENGL: return new FinalRendererGL();
		}

		return EU_NULL;
	}

}
