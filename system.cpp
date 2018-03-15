#include "system.h"
#include "control_handler.h"
#include "state.h"
#include "main/main_state.h"
#include "loading/loading_state.h"
#include "user/user_profile.h"
#include "user/setting.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "background.h"

fr::System *fr::System::m_instance = 0;

void fr::System::init()
{
	window_width = 720;
	window_heigh = 1280;
	rotation = WINDOWROTATION_PORTRAIT;
	bg_r = 0xFF;
	bg_g = 0xFF;
	bg_b = 0xFF;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 20);

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	screen_width = displayMode.w;
	screen_heigh = displayMode.h;
	//for fullscreen
	scale = float(screen_width) / float(window_width);
	window_heigh = screen_heigh / scale;
	window_depth = 1980;
	rotation = window_width > window_heigh ? WINDOWROTATION_LANDSCAPE : WINDOWROTATION_PORTRAIT;

	system_window = SDL_CreateWindow("Force Rush", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_heigh, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	gles_context = SDL_GL_CreateContext(system_window);

	std::string vertex_shader_src_str =
		"attribute vec4 a_position;\n"
		"attribute vec2 a_texCoord;\n"
		"varying vec2 v_texCoord;\n"
		"uniform mat4 mvp_matrix;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = mvp_matrix * a_position;\n"
		"	v_texCoord = a_texCoord;\n"
		"}\n";
	std::string fragment_shader_src_str =
		"precision mediump float;\n"
		"varying vec2 v_texCoord;\n"
		"uniform sampler2D s_texture;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = texture2D(s_texture, v_texCoord);\n"
		"}\n";
	const char *vertex_shader_src = vertex_shader_src_str.c_str();
	const char *fragment_shader_src = fragment_shader_src_str.c_str();
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint vertex_compiled;
	GLuint fragment_compiled;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	program_object = glCreateProgram();
	glAttachShader(program_object, vertex_shader);
	glAttachShader(program_object, fragment_shader);
	glLinkProgram(program_object);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	TTF_Init();
	IMG_Init(0);
	TextureManager::instance()->init(program_object);
	SoundManager::instance()->init();
	ControlHandler::instance()->init();
	Setting::instance()->init();
	UserProfile::instance()->init();
	Background::instance()->init();
	MainState::instance()->init();
	LoadingState::instance()->init();
	current_state = MainState::instance();
	//将理论分辨率的宽高比例与实际屏幕分辨率的宽高比例吻合
}

void fr::System::run()
{
	while (!ControlHandler::instance()->IsQuit())
	{
		glViewport(0, 0, screen_width, screen_heigh);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program_object);
		TextureManager::instance()->update();
		is_window_modified = false;
		ControlHandler::instance()->update();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		if (!LoadingState::instance()->IsLoading())
		{
			current_state->update();
		}
		LoadingState::instance()->update();
		UserProfile::instance()->update();
		SDL_GL_SwapWindow(system_window);
	}
}

void fr::System::clear()
{
	ControlHandler::instance()->clear();
	TextureManager::instance()->clear();
	SDL_DestroyWindow(system_window);
	system_window = NULL;
	TTF_Quit();
	IMG_Quit();
}

void fr::System::RefreshWindowSize()
{
	int w, h;
	SDL_GetWindowSize(system_window, &w, &h);
	screen_width = w;
	screen_heigh = h;
	rotation = window_width > window_heigh ? WINDOWROTATION_LANDSCAPE : WINDOWROTATION_PORTRAIT;
	window_width = rotation == WINDOWROTATION_PORTRAIT ? 720 : 1280;
	//屏幕縮放以720p 16:9為基準(也就是我手機屏幕的分辨率了)
	//是720 * 1280的分辨率
	scale = float(screen_width) / float(window_width);
	window_heigh = screen_heigh / scale;
	window_depth = 1980;
	is_window_modified = true;
}

bool fr::System::IsWindowModified()
{
	return is_window_modified;
}

void fr::System::SetBackgroundColor(char r, char g, char b)
{
	bg_r = r;
	bg_g = g;
	bg_b = b;
}

int fr::System::GetWindowWidth()
{
	return window_width;
}
int fr::System::GetWindowHeigh()
{
	return window_heigh;
}
int fr::System::GetWindowDepth()
{
	return window_depth;
}
int fr::System::GetScreenWidth()
{
	return screen_width;
}
int fr::System::GetScreenHeigh()
{
	return screen_heigh;
}
float fr::System::GetScale()
{
	return scale;
}
fr::WindowRotation fr::System::GetWindowRotation()
{
	return rotation;
}