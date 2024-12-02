#pragma once

#include <type_traits>
#include <unordered_set>

#include "windows.h"

#include "Environment/IDirtyTracker.h"
#include "Environment/IInputTracker.h"
#include "Environment/ITimer.h"
#include "Voicemeeter/Adapters/Multiclient/Cherry.h"
#include "Voicemeeter.UI.D2D/Scene.h"
#include "Voicemeeter.UI/Direction.h"

namespace Voicemeeter {
	namespace Clients {
		namespace UI {
			namespace D2D {
				template<typename TMixer>
				class Builder {
					static_assert(
						::std::is_same_v<Adapters::Multiclient::Cherry, TMixer>,
						"TMixer is not supported");

				public:
					inline Builder(
						HWND hWnd,
						::Environment::IDirtyTracker& dirtyTracker,
						::Environment::IInputTracker& inputTracker,
						::Environment::ITimer& compositionTimer,
						::Environment::ITimer& dirtyTimer,
						TMixer& mixer
					) : m_hWnd{ hWnd }
					  , m_dirtyTracker{ dirtyTracker }
					  , m_inputTracker{ inputTracker }
					  , m_compositionTimer{ compositionTimer }
					  , m_dirtyTimer{ dirtyTimer }
					  , m_mixer{ mixer }
					  , m_direction{ ::Voicemeeter::UI::Direction::Right }
					  , m_cIgnoredStrip{}
					  , m_network{ true } {

					};
					Builder() = delete;
					Builder(const Builder&) = delete;
					Builder(Builder&&) = delete;

					inline ~Builder() = default;

					Builder& operator=(const Builder&) = delete;
					Builder& operator=(Builder&&) = delete;

					inline Builder& WithDirection(::Voicemeeter::UI::Direction direction) {
						m_direction = direction;
						return *this;
					};
					inline Builder& WithIgnoredStrip(size_t id) {
						m_cIgnoredStrip.insert(id);
						return *this;
					};
					inline Builder& WithNetwork(bool network) {
						m_network = network;
						return *this;
					}
					::Voicemeeter::UI::D2D::Scene* Build();

				private:
					HWND m_hWnd;
					::Environment::IDirtyTracker& m_dirtyTracker;
					::Environment::IInputTracker& m_inputTracker;
					::Environment::ITimer& m_compositionTimer;
					::Environment::ITimer& m_dirtyTimer;
					TMixer& m_mixer;
					::Voicemeeter::UI::Direction m_direction;
					::std::unordered_set<size_t> m_cIgnoredStrip;
					bool m_network;
				};
			}
		}
	}
}