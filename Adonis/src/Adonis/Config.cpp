#include "pch.h"
#include "Config.h"


namespace Adonis{

	Config::Config(const std::string& filename) :
		m_filename(filename)
	{
		std::ifstream file(filename);
		if (!file.good()) {
			default_config();
		}
		else {
			file >> (*this);
		}
	}

	Config::~Config() {
		std::ofstream file(m_filename);
		file << (*this).dump(4) << std::endl;
	}

	void Config::default_config() {
		auto& gui_cfg = (*this)["gui"];
		gui_cfg["theme"] = 0;
		gui_cfg["font"] = 0;

		auto& tool_window_cfg = gui_cfg["tool_window"];
		tool_window_cfg["show"] = true;

		auto& style_chooser_cfg = gui_cfg["style_chooser"];
		style_chooser_cfg["show"] = false;
		style_chooser_cfg["selected_theme"] = 0;
		style_chooser_cfg["selected_font"] = 0;

		auto& render_window_cfg = gui_cfg["render_window"];
		render_window_cfg["res"]["w"] = 1280;
		render_window_cfg["res"]["h"] = 720;

		auto& demo_window_cfg = gui_cfg["demo_window"];
		demo_window_cfg["show"] = false;

		auto& window_cfg = (*this)["window"];
		window_cfg["title"] = "Adonis";
		window_cfg["vsync"] = true;
		window_cfg["pos"]["x"] = 100;
		window_cfg["pos"]["y"] = 100;
		window_cfg["res"]["w"] = 1280;
		window_cfg["res"]["h"] = 720;

		auto& assets_cfg = (*this)["assets"];
		assets_cfg["path"] = "../assets/";

		auto& font_cfg = assets_cfg["fonts"];
		font_cfg["path"] = assets_cfg["path"].get<std::string>() + "fonts/";
		font_cfg["font_size"] = 15.0f;
		font_cfg["font_scale"] = 1.0f;
		font_cfg["current_font"] = "RoboSans";

		auto& fonts_cfg = font_cfg["font-list"];

		fonts_cfg["RoboSans"]["name"] = "RoboSans";
		fonts_cfg["RoboSans"]["path"] = font_cfg["path"].get<std::string>() + "RoboSans/RoboSans-Regular.ttf";
		
		fonts_cfg["Consolas"]["name"] = "Consolas";
		fonts_cfg["Consolas"]["path"] = font_cfg["path"].get<std::string>() + "Consolas/consola.ttf";


		std::ofstream file(m_filename);

		file << (*this).dump(4) << std::endl;
	}

}