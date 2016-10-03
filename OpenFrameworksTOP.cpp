#include "OpenFrameworksTOP.h"

#include <windows.h>
#include <gl/glew.h>

#include <cstdio>
#include <assert.h>
#include <stdio.h>
#include <string.h>

// These functions are basic C function, which the DLL loader can find
// much easier than finding a C++ Class.
// The DLLEXPORT prefix is needed so the compile exports these functions from the .dll
// you are creating
extern "C"
{

DLLEXPORT
int
GetTOPAPIVersion(void)
{
	// Always return TOP_CPLUSPLUS_API_VERSION in this function.
	return TOP_CPLUSPLUS_API_VERSION;
}

DLLEXPORT
TOP_CPlusPlusBase*
CreateTOPInstance(const OP_NodeInfo* info, TOP_Context *context)
{
	// Return a new instance of your class every time this is called.
	// It will be called once per TOP that is using the .dll
	return new OpenFrameworksTOP(info);
}

DLLEXPORT
void
DestroyTOPInstance(TOP_CPlusPlusBase* instance, TOP_Context *context)
{
	// Delete the instance here, this will be called when
	// Touch is shutting down, when the TOP using that instance is deleted, or
	// if the TOP loads a different DLL
	delete (OpenFrameworksTOP*)instance;
}

};

OpenFrameworksTOP::OpenFrameworksTOP(const OP_NodeInfo* info) : myNodeInfo(info)
{
	isSetup = false;

	// ofAppNoWindow doesn't provide a usable renderer so calling any interfacing
	// functions that use a renderer (ie. ofShader::begin) will either crash or do absolutely nothing.
	// Instead make our own renderer and call the renderer's functions directly.
	renderer = new ofGLProgrammableRenderer(&myWindow);

	//#error Requires the relative path from the TouchDesigner executable to desired data directory
	// In the below example my installed TouchDesigner executable is in C:/Program Files/Derivative/TouchDesigner088/bin/ and 
	// my data directory is in C:/of_v0.9.3_vs_release/apps/myApps/inputTexture/bin/data/
	// So below is the relative path from the installed TouchDesigner executable to my data directory:
	ofSetDataPathRoot("../../../../Development/openframeworks/apps/myApps/TD-InputTexture/bin/data/");
}

OpenFrameworksTOP::~OpenFrameworksTOP()
{
	delete renderer;
}

void OpenFrameworksTOP::setup() 
{
	glewInit();
	renderer->setup(3, 2);

#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer())
	{
		shader.load("shadersGL3/shader");
	}
	else
	{
		shader.load("shadersGL2/shader");
	}
#endif

	isSetup = true;
}

void
OpenFrameworksTOP::getGeneralInfo(TOP_GeneralInfo* ginfo)
{
	// Uncomment this line if you want the TOP to cook every frame even
	// if none of it's inputs/parameters are changing.
	ginfo->cookEveryFrame = true;
}

bool
OpenFrameworksTOP::getOutputFormat(TOP_OutputFormat* format)
{
	// In this function we could assign variable values to 'format' to specify
	// the pixel format/resolution etc that we want to output to.
	// If we did that, we'd want to return true to tell the TOP to use the settings we've
	// specified.
	// In this example we'll return false and use the TOP's settings
	return false;
}

void
OpenFrameworksTOP::begin()
{
	renderer->startRender();
	renderer->setupScreen();
    renderer->bind(shader);
	renderer->pushMatrix();
}

void
OpenFrameworksTOP::end()
{
	renderer->popMatrix();
    renderer->unbind(shader);
	renderer->finishRender();
}


void
OpenFrameworksTOP::execute(const TOP_OutputFormatSpecs* outputFormat,
							OP_Inputs* inputs,
							TOP_Context *context)
{
	double xPos = inputs->getParDouble("Translation");

	int width = outputFormat->width;
	int height = outputFormat->height;

	// Use the first input TOP (here we assume it exists but in reality it might not)
	const OP_TOPInput *topInput = inputs->getInputTOP(0);
	ofTexture texture;
	texture.setUseExternalTextureID(topInput->textureIndex);
	texture.texData.width = topInput->width;
	texture.texData.height = topInput->height;
	texture.texData.tex_w = topInput->width;
	texture.texData.tex_h = topInput->height;
	texture.texData.tex_t = 1.0f;
	texture.texData.tex_u = 1.0f;
	texture.texData.textureTarget = topInput->textureType;
	texture.texData.bFlipTexture = true;

  const OP_TOPInput *topInput1 = inputs->getInputTOP(1);
	ofTexture texture1;
	texture1.setUseExternalTextureID(topInput1->textureIndex);
	texture1.texData.width = topInput1->width;
	texture1.texData.height = topInput1->height;
	texture1.texData.tex_w = topInput1->width;
	texture1.texData.tex_h = topInput1->height;
	texture1.texData.tex_t = 1.0f;
	texture1.texData.tex_u = 1.0f;
	texture1.texData.textureTarget = topInput1->textureType;
	texture1.texData.bFlipTexture = true;

	// Need to use a ofAppNoWindow so that openFrameworks doesn't create a conflicting
	// OpenGL context. We want to use TouchDesigner's context in ::execute
	ofSetupOpenGL(&myWindow, width, height, OF_WINDOW);

	context->beginGLCommands();

	// shaders must be loaded within the TouchDesigner context
	if (!isSetup)
	{
		setup();
	}

	begin();

	shader.setUniform1f("xPos", (float)-xPos);
  shader.setUniformTexture("tex0", texture, 0);
  shader.setUniformTexture("backbuffer", texture1, 1);
	renderer->draw(texture, 0, 0, 0, 1280, 720, 0, 0, 1280, 720);

	end();

	context->endGLCommands();
}

void
OpenFrameworksTOP::setupParameters(OP_ParameterManager* manager)
{

	OP_NumericParameter	np;

	np.name = "Translation";
	np.label = "Translation";
	
	ParAppendResult res = manager->appendFloat(np);
	assert(res == PARAMETER_APPEND_SUCCESS);

}
