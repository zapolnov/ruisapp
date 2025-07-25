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

#include "application.hpp"

#include <papki/fs_file.hpp>
#include <papki/root_dir.hpp>
#include <utki/config.hpp>
#include <utki/debug.hpp>

using namespace ruisapp;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
application::instance_type application::instance;

void application::render()
{
	// TODO: render only if needed?
	this->gui.context.get().renderer.get().render_context.get().clear_framebuffer_color();

	// no clear of depth and stencil buffers, it will be done by individual widgets if needed

	this->gui.render(this->gui.context.get().renderer.get().render_context.get().initial_matrix);

	this->swap_frame_buffers();
}

void application::update_window_rect(const ruis::rect& rect)
{
	if (this->cur_window_rect == rect) {
		return;
	}

	this->cur_window_rect = rect;

	LOG([&](auto& o) {
		o << "application::update_window_rect(): this->cur_window_rect = " << this->cur_window_rect << std::endl;
	})
	this->gui.context.get().renderer.get().render_context.get().set_viewport(
		r4::rectangle<uint32_t>(
			uint32_t(this->cur_window_rect.p.x()),
			uint32_t(this->cur_window_rect.p.y()),
			uint32_t(this->cur_window_rect.d.x()),
			uint32_t(this->cur_window_rect.d.y())
		)
	);

	this->gui.set_viewport(this->cur_window_rect.d);
}

#if CFG_OS_NAME != CFG_OS_NAME_ANDROID && CFG_OS_NAME != CFG_OS_NAME_IOS
std::unique_ptr<papki::file> application::get_res_file(std::string_view path) const
{
	return std::make_unique<papki::fs_file>(path);
}

void application::show_virtual_keyboard() noexcept
{
	LOG([](auto& o) {
		o << "application::show_virtual_keyboard(): invoked" << std::endl;
	})
	// do nothing
}

void application::hide_virtual_keyboard() noexcept {
	LOG([](auto& o) {
		o << "application::hide_virtual_keyboard(): invoked" << std::endl;
	})
	// do nothing
}
#endif

ruis::real application::get_pixels_per_pp(r4::vector2<unsigned> resolution, r4::vector2<unsigned> screen_size_mm)
{
	utki::log([&](auto& o) {
		o << "screen resolution = " << resolution << std::endl;
	});
	utki::log([&](auto& o) {
		o << "physical screen size, mm = " << screen_size_mm << std::endl;
	});

	// NOTE: for ordinary desktop displays the DP size should be equal to 1 pixel.
	// For high density displays it should be more than one pixel, depending on
	// display dpi. For hand held devices the size of DP should be determined from
	// physical screen size and pixel resolution.

#if CFG_OS_NAME == CFG_OS_NAME_IOS
	return ruis::real(1); // TODO:
#else
	unsigned x_index = [&resolution]() {
		if (resolution.x() > resolution.y()) {
			return 0;
		} else {
			return 1;
		}
	}();

	// using std::max;
	// unsigned max_dim_px = max(resolution.x(), resolution.y());
	// unsigned max_dim_mm = max(screen_size_mm.x(), screen_size_mm.y());

	// ruis::real dpmm = ruis::real(max_dim_px) / ruis::real(max_dim_mm);

	if (screen_size_mm[x_index] < 300) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		return ruis::real(resolution[x_index]) / ruis::real(700); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	} else if (screen_size_mm[x_index] < 150) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
		return ruis::real(resolution[x_index]) / ruis::real(200); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	}

	return ruis::real(1);
#endif
}

void application::handle_key_event(bool is_down, ruis::key key_code)
{
	this->gui.send_key(is_down, key_code);
}

application_factory::factory_type& application_factory::get_factory_internal()
{
	static application_factory::factory_type f;
	return f;
}

const application_factory::factory_type& application_factory::get_factory()
{
	auto& f = get_factory_internal();
	if (!f) {
		throw std::logic_error("no application factory registered");
	}
	return f;
}

std::unique_ptr<application> application_factory::make_application(
	int argc, //
	const char** argv
)
{
	auto cli_args = utki::make_span(argv, argc);

	if (cli_args.empty()) {
		return get_factory()(std::string_view(), {});
	}

	std::string_view executable = cli_args.front();

	std::vector<std::string_view> args;
	for (const auto& a : cli_args.subspan(1)) {
		args.emplace_back(a);
	}

	return get_factory()(executable, args);
}

application_factory::application_factory(factory_type factory)
{
	auto& f = this->get_factory_internal();
	if (f) {
		throw std::logic_error("application factory is already registered");
	}
	f = std::move(factory);
}
