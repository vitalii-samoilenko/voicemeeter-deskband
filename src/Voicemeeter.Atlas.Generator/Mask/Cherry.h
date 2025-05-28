#pragma once

#include "Dynamic.h"

namespace Voicemeeter {
	namespace Atlas {
		namespace Generator {
			namespace Mask {
				struct CherrySpecification {
					struct Block {
						static constexpr FLOAT Width{ 26.F };
						static constexpr FLOAT Height{ 17.F };
					};
					static constexpr size_t Width{ 8 };
					static constexpr size_t Height{ 14 };
					struct Font {
						static constexpr WCHAR Name[]{ L"Arial" };
						static constexpr FLOAT Size{ 16.F };
					};
					struct Vban {
						struct Frame {
							struct Point {
								static constexpr size_t X{ 0 };
								static constexpr size_t Y{ 0 };
							};
							static constexpr FLOAT Width{ 39.F };
							static constexpr FLOAT Height{ 22.F };
							static constexpr FLOAT Stroke{ 1.F };
						};
						struct Side {
							static constexpr FLOAT Width{ 8.F };
						};
					};
					struct Knob {
						struct Frame {
							struct Point {
								static constexpr size_t X{ 4 };
								static constexpr size_t Y{ 0 };
							};
							static constexpr FLOAT Radius{ 24.F };
							static constexpr FLOAT Stroke{ 3.F };
						};
						struct Indicator {
							struct Point {
								static constexpr size_t X{ 6 };
								static constexpr size_t Y{ 0 };
							};
							static constexpr FLOAT Radius{ 2.75F };
						};
						struct Label {
							struct Strip {
								struct Point {
									static constexpr size_t X{ 0 };
									static constexpr size_t Y{ 4 };
								};
							};
							struct Gain {
								struct Point {
									static constexpr size_t X{ 0 };
									static constexpr size_t Y{ 5 };
								};
							};
						};
					};
					struct Plug {
						struct Frame {
							struct Point {
								static constexpr size_t X{ 6 };
								static constexpr size_t Y{ 1 };
							};
							static constexpr FLOAT Width{ 41.F };
							static constexpr FLOAT Height{ 19.F };
							static constexpr FLOAT Radius{ 7.F };
							static constexpr FLOAT Stroke{ 1.5F };
						};
						struct Indicator {
							struct Start {
								static constexpr FLOAT X{ 7.F };
								static constexpr FLOAT Y{ 3.5F };
							};
							struct Tip {
								static constexpr FLOAT X{ 13.F };
								static constexpr FLOAT Y{ 9.F };
							};
							struct End {
								static constexpr FLOAT X{ 7.F };
								static constexpr FLOAT Y{ 15.5F };
							};
						};
						struct Label {
							struct Strip {
								struct Point {
									static constexpr size_t X{ 0 };
									static constexpr size_t Y{ 3 };
								};
							};
						};
					};
				};

				class Cherry : public Dynamic<CherrySpecification> {
					friend struct RenderTextW;

				public:
					Cherry(
						bool horizontal,
						const ::std::valarray<double>& scale
					);
					Cherry() = delete;
					Cherry(const Cherry&) = delete;
					Cherry(Cherry&&) = delete;

					~Cherry() = default;

					Cherry& operator=(const Cherry&) = delete;
					Cherry& operator=(Cherry&&) = delete;
				};
			}
		}
	}
}