#include "pch.h"

#pragma region generalDirectives
// SDL libs
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "SDL2main.lib")

// OpenGL libs
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Glu32.lib")

// SDL and OpenGL Includes
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#pragma endregion generalDirectives

#include <iostream>
#include <string>
#include <ctime>

#pragma region windowInformation
const float g_WindowWidth{ 550.0f };
const float g_WindowHeight{ 450.0f };
const std::string g_WindowTitle{ u8"Beam - Häkkinen, Esa - 1DAE08" };
bool g_IsVSyncOn{ true };
#pragma endregion windowInformation

#pragma region coreDeclarations
// Functions
void Initialize( );
void Cleanup( );
void QuitOnSDLError( );
void QuitOnOpenGlError( );

void DrawBoxes();

// Variables
SDL_Window* g_pWindow{ nullptr }; // The window we'll be rendering to
SDL_GLContext g_pContext; // OpenGL context
#pragma endregion coreDeclarations

#pragma region gameDeclarations
// Functions
void Draw( );
void ClearBackground( );

// Variables
#pragma endregion gameDeclarations



int main( int argc, char* args[] )
{
	// seed the pseudo random number generator
	srand(unsigned int(time(nullptr)));
	
	// Initialize SDL and OpenGL
	Initialize( );

	// Draw on the window
	Draw( );

	// Update window
	SDL_GL_SwapWindow( g_pWindow );


	// Wait for user to quit the program
	std::cout << "Press ENTER to quit\n";
	std::cin.get( );

	// Clean up SDL and OpenGL
	Cleanup( );

	return 0;
}

#pragma region gameImplementations
void ClearBackground( )
{
	glClearColor( 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Draw( )
{
	ClearBackground( );
	// add your drawing code here
	DrawBoxes();
	
}

void DrawBoxes()
{
	const float SquareSize{ 200.0f };
	const float border{ 10.0f };

	// first box, background, lower left
	glColor3f(0.0f / 255.f, 255.0f / 255.f, 0.f / 255.f); //green
	glLineWidth(0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f + border, 0.0f + border);
	glVertex2f(0.0f + border + SquareSize, 0.0f + border);
	glVertex2f(0.0f + border + SquareSize, 0.0f + border + SquareSize);
	glVertex2f(0.0f + border, 0.0f + border + SquareSize);
	glEnd();

	// first box, background, higher right
	glColor3f(0.0f / 255.f, 255.0f / 255.f, 0.f / 255.f); //green
	glLineWidth(0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(g_WindowWidth - border - SquareSize, g_WindowHeight - border - SquareSize);
	glVertex2f(g_WindowWidth - border, g_WindowHeight - border - SquareSize);
	glVertex2f(g_WindowWidth - border, g_WindowHeight - border);
	glVertex2f(g_WindowWidth - border - SquareSize, g_WindowHeight - border);
	glEnd();

	// foreground white lines
	//lower left frame
	glColor3f(255.0f / 255.f, 255.f / 255.f, 255.f / 255.f); //white
	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(border, border);
	glVertex2f(border + SquareSize, border);
	glVertex2f(border + SquareSize, border + SquareSize);
	glVertex2f(border, border + SquareSize);
	glEnd();

	//higher right frame
	glColor3f(255.0f / 255.f, 255.f / 255.f, 255.f / 255.f); //white
	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(g_WindowWidth - border - SquareSize, g_WindowHeight - border - SquareSize);
	glVertex2f(g_WindowWidth - border, g_WindowHeight - border - SquareSize);
	glVertex2f(g_WindowWidth - border, g_WindowHeight - border);
	glVertex2f(g_WindowWidth - border - SquareSize, g_WindowHeight - border);
	glEnd();

	//lines going over
	glColor3f(255.0f / 255.f, 255.f / 255.f, 255.f / 255.f); //white
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	glVertex2f(border, border);
	glVertex2f(g_WindowWidth - border, g_WindowHeight - border);
	
	glVertex2f(border + SquareSize, border);
	glVertex2f(g_WindowWidth - border, g_WindowHeight - border - SquareSize);
	
	glVertex2f(border, border + SquareSize);
	glVertex2f(g_WindowWidth - border - SquareSize, g_WindowHeight-border);
	glEnd();

	return;
}
#pragma endregion gameImplementations

#pragma region coreImplementations
void Initialize( )
{
	//Initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		QuitOnSDLError( );
	}

	//Use OpenGL 2.1
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	//Create window
	g_pWindow = SDL_CreateWindow(
		g_WindowTitle.c_str( ),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int( g_WindowWidth ),
		int( g_WindowHeight ),
		SDL_WINDOW_OPENGL );

	if ( g_pWindow == nullptr )
	{
		QuitOnSDLError( );
	}

	// Create an opengl context and attach it to the window 
	g_pContext = SDL_GL_CreateContext( g_pWindow );
	if ( g_pContext == nullptr )
	{
		QuitOnSDLError( );
	}

	// Set the swap interval for the current OpenGL context synchronized with the vertical retrace
	if ( g_IsVSyncOn )
	{
		if ( SDL_GL_SetSwapInterval( 1 ) < 0 )
		{
			QuitOnSDLError( );
		}
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}


	// Initialize Projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	// Set the clipping (viewing) area's left, right, bottom and top
	gluOrtho2D( 0, g_WindowWidth, 0, g_WindowHeight );

	// The viewport is the rectangular region of the window where the image is drawn.
	// Set it to the entire client area of the created window
	glViewport(0, 0, int(g_WindowWidth), int(g_WindowHeight));

	//Initialize Modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// Enable color blending and use alpha blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void Cleanup( )
{
	SDL_GL_DeleteContext( g_pContext );

	SDL_DestroyWindow( g_pWindow );
	g_pWindow = nullptr;

	SDL_Quit( );
}


void QuitOnSDLError( )
{
	std::cout << "Problem during SDL initialization: ";
	std::cout << SDL_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}
void QuitOnOpenGlError( )
{
	std::cout << "Problem during OpenGL initialization: ";
	std::cout << SDL_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}

#pragma endregion coreImplementations
