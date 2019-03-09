#pragma once

#include "pch.h"
#include "Adonis/Core.h"

namespace Adonis {

	class Config : public nlohmann::json {
	public:

		Config(const std::string& filename);
		~Config();

		inline auto filename()->std::string { return m_filename; };

	private:
		std::string m_filename;

		auto default_config()->void;
	};

}
