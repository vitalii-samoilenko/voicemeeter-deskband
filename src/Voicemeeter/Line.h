#pragma once

namespace Voicemeeter {
	class Line {
	public:
		inline Line() = default;
		Line(const Line&) = delete;
		inline Line(Line&&) = default;

		inline ~Line() = default;

		Line& operator=(const Line&) = delete;
		Line& operator=(Line&&) = delete;

		inline double get_Level() const {
			return m_level;
		};
		inline void set_Level(double value) {
			m_level = value;
		};

	private:
		double m_level;
	};
}