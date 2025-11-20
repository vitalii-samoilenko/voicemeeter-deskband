#ifndef VOICEMEETER_UI_ADAPTERS_ANIMATED_HPP
#define VOICEMEETER_UI_ADAPTERS_ANIMATED_HPP

#include <cmath>
#include <utility>
#include <valarray>

namespace Voicemeeter {
	namespace UI {
		namespace Adapters {
			template<
				typename TBundle,
				typename TPalette,
				typename TUpdateFrame>
			class Animated : public TBundle {
			public:
				template<typename... Args>
				inline Animated(
					TPalette &palette,
					::std::valarray<int> &&animationBaseVertex,
					TUpdateFrame &&updateFrame = TUpdateFrame{},
					Args &&...args)
					: TBundle{ ::std::forward<Args>(args)... }
					, _palette{ palette }
					, _beforeExecute{ *this }
					, _afterExecute{ *this }
					, _animationSpeed{ 0 }
					, _animationVertex(0, animationBaseVertex.size())
					, _animationBaseVertex{ ::std::move(animationBaseVertex) }
					, _animationVelocityVertex(0, _animationBaseVertex.size())
					, _updateFrame{ ::std::move(updateFrame) } {

				};
				Animated() = delete;
				Animated(Animated const &) = delete;
				Animated(Animated &&) = delete;

				inline ~Animated() = default;

				Animated & operator()(Animated const &) = delete;
				Animated & operator()(Animated &&) = delete;

				inline ::std::valarray<int> const & get_AnimationSize() const {
					return _animationVertex;
				};
				inline ::std::valarray<int> const & get_AnimationBaseSize() const {
					return _animationBaseVertex;
				};
				inline ::std::valarray<int> const & get_AnimationVelocitySize() const {
					return _animationVelociyVertex;
				};

				inline void set_AnimationVelocitySize(::std::valarray<int> const &vertex) {
					::std::valarray<int> temp{ vertex };
					ClampAnimationVelocitySize(temp);
					if (temp == _animationVelocityVertex) {
						return;
					}
					_animationVelocityVertex = temp;
					if (_animationSpeed == 0) {
						Enqueue();
						UpdateAnimationSpeed();
					}
				};

			private:
				class BeforeExecute {
				public:
					inline explicit BeforeExecute(Animated &bundle)
						: _this{ &bundle } {

					};
					BeforeExecute() = delete;
					BeforeExecute(BeforeExecute const &) = delete;
					BeforeExecute(BeforeExecute &&) = delete;

					inline ~BeforeExecute() = default;

					BeforeExecute & operator()(BeforeExecute const &) = delete;
					BeforeExecute & operator()(BeforeExecute &&) = delete;

					inline void operator()() {
						_this->_animationVertex += _this->_palette.get_Stopwatch()
							.get_Elapsed() * _this->_animationVelocityVertex;
						_this->ClampAnimationSize(_this->_animationVertex);
						_this->_updateFrame(*_this);
					};

				private:
					Animated *_this;
				};

				class AfterExecute {
				public:
				inline explicit AfterExecute(Animated &bundle)
					: _this{ &bundle } {

				};
				AfterExecute() = delete;
				AfterExecute(AfterExecute const &) = delete;
				AfterExecute(AfterExecute &&) = delete;

				inline ~AfterExecute() = default;

				AfterExecute & operator()(AfterExecute const &) = delete;
				AfterExecute & operator()(AfterExecute &&) = delete;

				inline void operator()() {
					_this->ClampAnimationVelocitySize(_this->_animationVelocityVertex);
					_this->UpdateAnimationSpeed();
					if (_this->_animationSpeed != 0) {
						_this->Enqueue();
					}
				};

				private:
					Animated *_this;
				};

				inline void ClampAnimationSize(::std::valarray<int> &vertex) const {
					vertex[vertex < 0] = 0;
					vertex[_animationBaseVertex < vertex] = _animationBaseVertex;
				};
				inline void ClampAnimationVelocitySize(::std::valarray<int> &vertex) const {
					vertex[vertex < 0 && _animationVertex == 0] = 0;
					vertex[0 < vertex && _animationVertex == _animationBaseVertex] = 0;
				};
				inline int UpdateAnimationSpeed() {
					_animationSpeed = ::std::abs(_animationVelocityVertex)
						.sum();
				};
				inline void Enqueue() {
					_palette.get_Queue()
						+= _beforeExecute;
					TBundle::set_Invalid();
					_palette.get_Queue()
						+= _afterExecute;
				};

				friend BeforeExecute;
				friend AfterExecute;

				TPalette &_palette;
				BeforeExecute _beforeExecute;
				AfterExecute _afterExecute;
				int _animationSpeed;
				::std::valarray<int> _animationVertex;
				::std::valarray<int> _animationBaseVertex;
				::std::valarray<int> _animationVelocityVertex;
				TUpdateFrame _updateFrame;
			};
		}
	}
}

#endif
