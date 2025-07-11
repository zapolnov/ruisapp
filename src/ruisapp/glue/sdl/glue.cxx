/*
ruisapp - ruis GUI adaptation layer

Copyright (C) 2016-2025  Ivan Gagis <igagis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ================ LICENSE END ================ */

#include <atomic>

#include <utki/config.hpp>
#include <utki/enum_array.hpp>
#include <utki/unicode.hpp>

#include "../../application.hpp"

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
#	include <emscripten.h>
#	include <emscripten/html5.h>
#endif

#if CFG_COMPILER == CFG_COMPILER_MSVC
#	include <SDL.h>
#else
#	include <SDL2/SDL.h>
#endif

#ifdef RUISAPP_RENDER_OPENGL
#	include <GL/glew.h>
#	include <ruis/render/opengl/context.hpp>
#elif defined(RUISAPP_RENDER_OPENGLES)
#	include <GLES2/gl2.h>
#	include <ruis/render/opengles/context.hpp>
#else
#	error "Unknown graphics API"
#endif

#include "../friend_accessors.cxx" // NOLINT(bugprone-suspicious-include)

using namespace std::string_view_literals;

using namespace ruisapp;

namespace {
const std::array<ruis::key, utki::byte_mask + 1> key_map = {
	{
     ruis::key::unknown, // 0
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::a,
     ruis::key::b, // x5
		ruis::key::c,
     ruis::key::d,
     ruis::key::e,
     ruis::key::f,
     ruis::key::g, // 10
		ruis::key::h,
     ruis::key::i,
     ruis::key::j,
     ruis::key::k,
     ruis::key::l, // x5
		ruis::key::m,
     ruis::key::n,
     ruis::key::o,
     ruis::key::p,
     ruis::key::q, // 20
		ruis::key::r,
     ruis::key::s,
     ruis::key::t,
     ruis::key::u,
     ruis::key::v, // x5
		ruis::key::w,
     ruis::key::x,
     ruis::key::y,
     ruis::key::z,
     ruis::key::one, // 30
		ruis::key::two,
     ruis::key::three,
     ruis::key::four,
     ruis::key::five,
     ruis::key::six, // x5
		ruis::key::seven,
     ruis::key::eight,
     ruis::key::nine,
     ruis::key::zero,
     ruis::key::enter, // 40
		ruis::key::escape,
     ruis::key::backspace,
     ruis::key::tabulator,
     ruis::key::space,
     ruis::key::minus, // x5
		ruis::key::equals,
     ruis::key::left_square_bracket,
     ruis::key::right_square_bracket,
     ruis::key::backslash,
     ruis::key::backslash, // 50
		ruis::key::semicolon,
     ruis::key::apostrophe,
     ruis::key::grave,
     ruis::key::comma,
     ruis::key::period, // x5
		ruis::key::slash,
     ruis::key::capslock,
     ruis::key::f1,
     ruis::key::f2,
     ruis::key::f3, // 60
		ruis::key::f4,
     ruis::key::f5,
     ruis::key::f6,
     ruis::key::f7,
     ruis::key::f8, // x5
		ruis::key::f9,
     ruis::key::f10,
     ruis::key::f11,
     ruis::key::f12,
     ruis::key::print_screen, // 70
		ruis::key::unknown,
     ruis::key::pause,
     ruis::key::insert,
     ruis::key::home,
     ruis::key::page_up, // x5
		ruis::key::deletion,
     ruis::key::end,
     ruis::key::page_down,
     ruis::key::arrow_right,
     ruis::key::arrow_left, // 80
		ruis::key::arrow_down,
     ruis::key::arrow_up,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 90
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 100
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::f13,
     ruis::key::f14, // x5
		ruis::key::f15,
     ruis::key::f16,
     ruis::key::f17,
     ruis::key::f18,
     ruis::key::f19, // 110
		ruis::key::f20,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 120
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 130
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 140
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 150
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 160
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 170
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 180
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 190
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 200
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 210
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 220
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::left_control,
     ruis::key::left_shift, // x5
		ruis::key::left_alt,
     ruis::key::unknown,
     ruis::key::right_control,
     ruis::key::right_shift,
     ruis::key::right_alt, // 230
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 240
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // x5
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown, // 250
		ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown,
     ruis::key::unknown // 255
	}
};

ruis::key sdl_scancode_to_ruis_key(SDL_Scancode sc)
{
	if (size_t(sc) >= key_map.size()) {
		return ruis::key::unknown;
	}

	return key_map[sc];
}
} // namespace

namespace {
ruis::real get_dpi(int display_index = 0)
{
	float dpi = ruis::units::default_dots_per_inch;
	if (SDL_GetDisplayDPI(display_index, &dpi, nullptr, nullptr) != 0) {
		throw std::runtime_error(utki::cat("Could not get SDL display DPI, SDL Error: ", SDL_GetError()));
	}
	// std::cout << "get_dpi(): dpi = " << dpi << std::endl;
	return ruis::real(dpi);
}

ruis::real get_display_scaling_factor(int display_index = 0)
{
	using std::round;
	return round(get_dpi(display_index) / ruis::units::default_dots_per_inch);
}
} // namespace

namespace {
class window_wrapper : public utki::destructable
{
	class sdl_wrapper
	{
	public:
		sdl_wrapper()
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				throw std::runtime_error(utki::cat("Could not initialize SDL, SDL_Error: ", SDL_GetError()));
			}
		}

		~sdl_wrapper()
		{
			SDL_Quit();
		}

		sdl_wrapper(const sdl_wrapper&) = delete;
		sdl_wrapper& operator=(const sdl_wrapper&) = delete;
		sdl_wrapper(sdl_wrapper&&) = delete;
		sdl_wrapper& operator=(sdl_wrapper&&) = delete;
	} sdl;

	class sdl_cursor_wrapper
	{
		SDL_Cursor* sdl_cursor = nullptr;

	public:
		sdl_cursor_wrapper() = default;

		sdl_cursor_wrapper(const sdl_cursor_wrapper&) = delete;
		sdl_cursor_wrapper& operator=(const sdl_cursor_wrapper&) = delete;

		sdl_cursor_wrapper(sdl_cursor_wrapper&&) = delete;
		sdl_cursor_wrapper& operator=(sdl_cursor_wrapper&&) = delete;

		void set(ruis::mouse_cursor cursor)
		{
			ASSERT(!this->sdl_cursor)

			utki::enum_array<SDL_SystemCursor, ruis::mouse_cursor> cursor_mapping = {
				SDL_SYSTEM_CURSOR_ARROW, // none
				SDL_SYSTEM_CURSOR_ARROW, // arrow
				SDL_SYSTEM_CURSOR_SIZEWE, // left_right_arrow
				SDL_SYSTEM_CURSOR_SIZENS, // up_down_arrow
				SDL_SYSTEM_CURSOR_SIZEALL, // all_directions_arrow
				SDL_SYSTEM_CURSOR_SIZEWE, // left_side
				SDL_SYSTEM_CURSOR_SIZEWE, // right_side
				SDL_SYSTEM_CURSOR_SIZENS, // top_side
				SDL_SYSTEM_CURSOR_SIZENS, // bottom_side
				SDL_SYSTEM_CURSOR_SIZEALL, // top_left_corner
				SDL_SYSTEM_CURSOR_SIZEALL, // top_right_corner
				SDL_SYSTEM_CURSOR_SIZEALL, // bottom_left_corner
				SDL_SYSTEM_CURSOR_SIZEALL, // bottom_right_corner
				SDL_SYSTEM_CURSOR_CROSSHAIR, // index_finger
				SDL_SYSTEM_CURSOR_HAND, // grab
				SDL_SYSTEM_CURSOR_IBEAM, // caret
				// TODO:
				// SDL_SYSTEM_CURSOR_WAIT
				// SDL_SYSTEM_CURSOR_NO
			};

			this->sdl_cursor = SDL_CreateSystemCursor(cursor_mapping[cursor]);
		}

		~sdl_cursor_wrapper()
		{
			if (this->sdl_cursor) {
				SDL_FreeCursor(this->sdl_cursor);
			}
		}

		bool empty() const noexcept
		{
			return this->sdl_cursor == nullptr;
		}

		void activate()
		{
			ASSERT(!this->empty())

			SDL_SetCursor(this->sdl_cursor);
		}
	};

	utki::enum_array<sdl_cursor_wrapper, ruis::mouse_cursor> mouse_cursors;

public:
	void set_cursor(ruis::mouse_cursor cursor)
	{
		auto& c = this->mouse_cursors[cursor];
		if (c.empty()) {
			c.set(cursor);
		}
		c.activate();
	}

	class sdl_window_wrapper
	{
	public:
		const ruis::real scale_factor;

		SDL_Window* const window;

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
		static bool on_emscripten_canvas_size_changed_callback(int event_type, const void* reserved, void* user_data)
		{
			if (!user_data) {
				std::cout << "emscripten_get_canvas_element_size(#canvas): user_data is nullptr" << std::endl;
				return false;
			}
			auto* ww = reinterpret_cast<sdl_window_wrapper*>(user_data);

			double width = 0;
			double height = 0;

			if (auto res = emscripten_get_element_css_size("#canvas", &width, &height);
				res != EMSCRIPTEN_RESULT_SUCCESS)
			{
				std::cout << "emscripten_get_canvas_element_size(#canvas): failed, error = " << res << std::endl;
				return false;
			}

			// std::cout << "emscripten_get_canvas_element_size(#canvas): new canvas size = " << width << " " << height << std::endl;

			SDL_SetWindowSize(
				ww->window, //
				int(width),
				int(height)
			);

			return true;
		}
#endif

		sdl_window_wrapper(const window_parameters& wp) :
			scale_factor(get_display_scaling_factor()),
			window([&]() {
#ifdef RUISAPP_RENDER_OPENGL
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#elif defined(RUISAPP_RENDER_OPENGLES)
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
#	error "Unknown graphics API"
#endif
				{
					auto ver = wp.graphics_api_version;
					if (ver.major == 0 && ver.minor == 0) {
						// default OpenGL version is 2.0
						// TODO: set default version for non-OpenGL APIs
						ver.major = 2;
					}
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ver.major);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ver.minor);
				}

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
				// Lock orientation if requested
				if (wp.orientation != ruisapp::orientation::dynamic) {
					int emsc_orient = [&]() {
						switch (wp.orientation) {
							default:
							case ruisapp::orientation::landscape:
								return EMSCRIPTEN_ORIENTATION_LANDSCAPE_PRIMARY;
							case ruisapp::orientation::portrait:
								return EMSCRIPTEN_ORIENTATION_PORTRAIT_PRIMARY;
						}
					}();
					emscripten_lock_orientation(emsc_orient);
				}

				// Change to soft fullscreen mode before creating the window to set correct OpenGL viewport initially.
				{
					EmscriptenFullscreenStrategy strategy{};
					strategy = {
						.scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH,
						.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_HIDEF,
						.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_NEAREST,
						.canvasResizedCallback = &sdl_window_wrapper::on_emscripten_canvas_size_changed_callback,
						.canvasResizedCallbackUserData = this
					};
					emscripten_enter_soft_fullscreen("#canvas", &strategy);
				}

				auto dims = []() {
					double width;
					double height;

					if (auto res = emscripten_get_element_css_size("#canvas", &width, &height);
						res != EMSCRIPTEN_RESULT_SUCCESS)
					{
						throw std::runtime_error(
							utki::cat(
								"emscripten_get_canvas_element_size(#canvas): failed, error = ", //
								res
							)
						);
					}
					return ruis::vec2(
						ruis::real(width), //
						ruis::real(height)
					);
				}();
#else
				auto dims = wp.dims.to<ruis::real>();
				dims *= this->scale_factor;
#endif

				// std::cout << "dims = " << dims << std::endl;

				SDL_Window* window = SDL_CreateWindow(
					wp.title.c_str(),
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					int(dims.x()),
					int(dims.y()),
					SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
				);
				if (!window) {
					std::runtime_error(utki::cat("Could not create SDL window, SDL_Error: ", SDL_GetError()));
				}

				return window;
			}())
		{}

		~sdl_window_wrapper()
		{
			SDL_DestroyWindow(this->window);
		}

		sdl_window_wrapper(const sdl_window_wrapper&) = delete;
		sdl_window_wrapper& operator=(const sdl_window_wrapper&) = delete;
		sdl_window_wrapper(sdl_window_wrapper&&) = delete;
		sdl_window_wrapper& operator=(sdl_window_wrapper&&) = delete;
	} window;

	class gl_context_wrapper
	{
		SDL_GLContext context;

	public:
		gl_context_wrapper(sdl_window_wrapper& sdl_window) :
			context([&]() {
				SDL_GLContext c = SDL_GL_CreateContext(sdl_window.window);
				if (!c) {
					throw std::runtime_error(utki::cat("Could not create OpenGL context, SDL Error: ", SDL_GetError()));
				}
				return c;
			}())
		{}

		~gl_context_wrapper()
		{
			SDL_GL_DeleteContext(this->context);
		}

		gl_context_wrapper(const gl_context_wrapper&) = delete;
		gl_context_wrapper& operator=(const gl_context_wrapper&) = delete;
		gl_context_wrapper(gl_context_wrapper&&) = delete;
		gl_context_wrapper& operator=(gl_context_wrapper&&) = delete;
	} gl_context;

	Uint32 user_event_type;

	std::atomic_bool quit_flag = false;

	window_wrapper(const window_parameters& wp) :
		window(wp),
		gl_context(this->window),
		user_event_type([]() {
			Uint32 t = SDL_RegisterEvents(1);
			if (t == (Uint32)(-1)) {
				throw std::runtime_error(
					utki::cat("Could not create SDL user event type, SDL Error: ", SDL_GetError())
				);
			}
			return t;
		}())
	{
#ifdef RUISAPP_RENDER_OPENGL
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Could not initialize GLEW");
		}
#endif
		SDL_StartTextInput();
	}

	~window_wrapper() override
	{
		SDL_StopTextInput();
	}

	window_wrapper(const window_wrapper&) = delete;
	window_wrapper& operator=(const window_wrapper&) = delete;
	window_wrapper(window_wrapper&&) = delete;
	window_wrapper& operator=(window_wrapper&&) = delete;
};
} // namespace

namespace {
ruisapp::application::directories get_application_directories(std::string_view app_name)
{
	char* base_dir = SDL_GetPrefPath("", std::string(app_name).c_str());
	utki::scope_exit base_dir_scope_exit([&]() {
		SDL_free(base_dir);
	});

	ruisapp::application::directories dirs;

	dirs.cache = utki::cat(base_dir, "cache/"sv);
	dirs.config = utki::cat(base_dir, "config/"sv);
	dirs.state = utki::cat(base_dir, "state/"sv);

	// std::cout << "cache dir = " << dirs.cache << std::endl;
	// std::cout << "config dir = " << dirs.config << std::endl;
	// std::cout << "state dir = " << dirs.state << std::endl;

	return dirs;
}
} // namespace

namespace {
window_wrapper& get_impl(const std::unique_ptr<utki::destructable>& pimpl)
{
	ASSERT(dynamic_cast<window_wrapper*>(pimpl.get()))
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
	return static_cast<window_wrapper&>(*pimpl);
}

window_wrapper& get_impl(application& app)
{
	return get_impl(get_window_pimpl(app));
}
} // namespace

namespace {
ruis::mouse_button button_number_to_enum(Uint8 number)
{
	switch (number) {
		case SDL_BUTTON_LEFT:
			return ruis::mouse_button::left;
		case SDL_BUTTON_X1:
			// TODO:
		case SDL_BUTTON_X2:
			// TODO:
		default:
		case SDL_BUTTON_MIDDLE:
			return ruis::mouse_button::middle;
		case SDL_BUTTON_RIGHT:
			return ruis::mouse_button::right;
	}
}
} // namespace

application::application(std::string name, const window_parameters& wp) :
	name(std::move(name)),
	window_pimpl(std::make_unique<window_wrapper>(wp)),
	gui(utki::make_shared<ruis::context>(
		utki::make_shared<ruis::style_provider>( //
			utki::make_shared<ruis::resource_loader>( //
				utki::make_shared<ruis::render::renderer>(
#ifdef RUISAPP_RENDER_OPENGL
					utki::make_shared<ruis::render::opengl::context>()
#elif defined(RUISAPP_RENDER_OPENGLES)
					utki::make_shared<ruis::render::opengles::context>()
#else
#	error "Unknown graphics API"
#endif
				)
			)
		),
		utki::make_shared<ruis::updater>(),
		ruis::context::parameters{
			.post_to_ui_thread_function =
				[this](std::function<void()> procedure) {
					auto& ww = get_impl(*this);

					SDL_Event e;
					SDL_memset(&e, 0, sizeof(e));
					e.type = ww.user_event_type;
					e.user.code = 0;
					// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
					e.user.data1 = new std::function<void()>(std::move(procedure));
					e.user.data2 = nullptr;
					SDL_PushEvent(&e);
				},
			.set_mouse_cursor_function =
				[this](ruis::mouse_cursor c) {
					auto& ww = get_impl(*this);
					ww.set_cursor(c);
				},
			.units = ruis::units(
				get_dpi(), //
				get_impl(*this).window.scale_factor
			)
		}
	)),
	directory(get_application_directories(this->name))
{
	auto& ww = get_impl(*this);

	// Get actual window size, as it can differ from requested one.
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(ww.window.window, &width, &height);

	auto dims = ruis::vec2(ruis::real(width), ruis::real(height));

	// std::cout << "actual window size = " << dims << std::endl;

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
	dims *= ww.window.scale_factor;
#endif

	this->update_window_rect(
		ruis::rect(
			{0, 0}, //
			dims
		)
	);
}

void application::quit() noexcept
{
	auto& ww = get_impl(this->window_pimpl);

	ww.quit_flag.store(true);
}

void application::swap_frame_buffers()
{
	auto& ww = get_impl(this->window_pimpl);

	SDL_GL_SwapWindow(ww.window.window);
}

void application::set_fullscreen(bool enable)
{
	auto& ww = get_impl(this->window_pimpl);

	auto flags = [&]() -> uint32_t {
		if (enable) {
			return SDL_WINDOW_FULLSCREEN_DESKTOP;
		} else {
			return 0;
		}
	}();

	auto error = SDL_SetWindowFullscreen(
		ww.window.window, //
		flags
	);

	if (error != 0) {
		throw std::runtime_error(
			utki::cat(
				"application::set_fullscreen(): could not switch fullscreen mode, error: ", //
				SDL_GetError()
			)
		);
	}
}

void application::set_mouse_cursor_visible(bool visible)
{
	int mode = [&]() {
		if (visible) {
			return SDL_ENABLE;
		} else {
			return SDL_DISABLE;
		}
	}();

	int error = SDL_ShowCursor(mode);

	if (error < 0) {
		throw std::runtime_error(
			utki::cat(
				"application::set_mouse_cursor_visible(): could not show/hide mouse cursor, error: ", //
				SDL_GetError()
			)
		);
	}
}

namespace {
void main_loop_iteration(void* user_data)
{
	ASSERT(user_data)
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	auto app = reinterpret_cast<ruisapp::application*>(user_data);

	auto& ww = get_impl(*app);

	// loop iteration sequence:
	// - update updateables
	// - render
	// - wait for events and handle them/next cycle

#if CFG_OS_NAME != CFG_OS_NAME_EMSCRIPTEN
	auto to_wait_ms =
#endif
		app->gui.update();

	render(*app);

#if CFG_OS_NAME != CFG_OS_NAME_EMSCRIPTEN
	// clamp to_wait_ms to max of int as SDL_WaitEventTimeout() accepts int type
	to_wait_ms = std::min(to_wait_ms, uint32_t(std::numeric_limits<int32_t>::max()));

	if (SDL_WaitEventTimeout(nullptr, int(to_wait_ms)) == 0) {
		// No events or error. In case of error not much we can do, just ignore it.
		return;
	}
#endif

	ruis::vector2 new_win_dims(-1, -1);

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT:
				ww.quit_flag.store(true);
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event) {
					default:
						break;
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						// squash all window resize events into one, for that store the new
						// window dimensions and update the viewport later only once
						new_win_dims.x() = ruis::real(e.window.data1);
						new_win_dims.y() = ruis::real(e.window.data2);
#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
						new_win_dims *= ww.window.scale_factor;
#endif
						// std::cout << "new window dims = " << new_win_dims << std::endl;
						break;
					case SDL_WINDOWEVENT_ENTER:
						handle_mouse_hover(*app, true, 0);
						break;
					case SDL_WINDOWEVENT_LEAVE:
						handle_mouse_hover(*app, false, 0);
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				{
					int x = 0;
					int y = 0;
					SDL_GetMouseState(&x, &y);

					ruis::vector2 pos(x, y);

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
					pos *= ww.window.scale_factor;
#endif

					handle_mouse_move(
						*app, //
						pos,
						0
					);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				[[fallthrough]];
			case SDL_MOUSEBUTTONUP:
				{
					int x = 0;
					int y = 0;
					SDL_GetMouseState(&x, &y);

					ruis::vector2 pos(x, y);

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
					pos *= ww.window.scale_factor;
#endif

					handle_mouse_button(
						*app, //
						e.button.type == SDL_MOUSEBUTTONDOWN,
						pos,
						button_number_to_enum(e.button.button),
						0 // pointer id
					);
				}
				break;
			case SDL_KEYDOWN:
				[[fallthrough]];
			case SDL_KEYUP:
				{
					auto key = sdl_scancode_to_ruis_key(e.key.keysym.scancode);
					if (e.key.repeat == 0) {
						handle_key_event(
							*app, //
							e.key.type == SDL_KEYDOWN,
							key
						);
					}
					if (e.type == SDL_KEYDOWN) {
						struct sdl_dummy_input_string_provider : public ruis::gui::input_string_provider {
							std::u32string get() const override
							{
								return {};
							}
						};

						handle_character_input(
							*app, //
							sdl_dummy_input_string_provider(),
							key
						);
					}
				}
				break;
			case SDL_TEXTINPUT:
				{
					struct sdl_input_string_provider : public ruis::gui::input_string_provider {
						const char* text;

						sdl_input_string_provider(const char* text) :
							text(text)
						{}

						std::u32string get() const override
						{
							return utki::to_utf32(this->text);
						}
					} sdl_input_string_provider(
						// save pointer to text, the ownership of text buffer is not taken!
						&(e.text.text[0])
					);

					handle_character_input(
						*app, //
						sdl_input_string_provider,
						ruis::key::unknown
					);
				}
				break;
			default:
				if (e.type == ww.user_event_type) {
					std::unique_ptr<std::function<void()>> f(
						// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
						reinterpret_cast<std::function<void()>*>(e.user.data1)
					);
					f->operator()();
				}
				break;
		}
	}

	if (new_win_dims.is_positive_or_zero()) {
		update_window_rect(*app, ruis::rect(0, new_win_dims));
	}

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
	if (ww.quit_flag.load()) {
		std::unique_ptr<ruisapp::application> p(app);
		emscripten_cancel_main_loop();
	}
#endif
}
} // namespace

int main(int argc, const char** argv)
{
	try {
		// std::cout << "main(): enter" << std::endl;
		std::unique_ptr<ruisapp::application> app = ruisapp::application_factory::make_application(argc, argv);
		if (!app) {
			// Not an error. The application just did not show any GUI to the user.
			return 0;
		}
		// std::cout << "main(): app created" << std::endl;

#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
		emscripten_set_main_loop_arg(&main_loop_iteration, app.release(), 0, false);
		// std::cout << "main(): emscripten loop is set up" << std::endl;
		return 0;
#else
		while (!get_impl(*app).quit_flag.load()) {
			main_loop_iteration(app.get());
		}
#endif
	} catch (std::exception& e) {
#if CFG_OS_NAME == CFG_OS_NAME_EMSCRIPTEN
		std::cout << "uncaught " << utki::to_string(e) << std::endl;
#endif
		throw;
	}

	return 0;
}
