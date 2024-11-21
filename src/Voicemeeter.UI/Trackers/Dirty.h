#pragma once

#include <unordered_set>

#include "Environment/IDirtyTracker.h"

#include "../Graphics/IGlyph.h"
#include "IDirty.h"

namespace Voicemeeter {
	namespace UI {
		namespace Trackers {
			class Dirty final : public IDirty {
			public:
				explicit Dirty(
					::Environment::IDirtyTracker& envDirtyTracker
				);
				Dirty() = delete;
				Dirty(const Dirty&) = delete;
				Dirty(Dirty&&) = delete;

				~Dirty() = default;

				Dirty& operator=(const Dirty&) = delete;
				Dirty& operator=(Dirty&&) = delete;

				virtual const ::std::valarray<double>& get_Position() const override;
				virtual const ::std::valarray<double>& get_Size() const override;
				virtual void set_Dirty(Graphics::IGlyph& glyph, bool value) override;

				virtual void Redraw(const ::std::valarray<double>& point, const ::std::valarray<double>& vertex) override;

				inline ::std::unordered_set<Graphics::IGlyph*>::const_iterator begin() const {
					return m_cpGlyph.begin();
				}
				inline ::std::unordered_set<Graphics::IGlyph*>::const_iterator end() const {
					return m_cpGlyph.end();
				}

			private:
				::Environment::IDirtyTracker& m_envDirtyTracker;
				mutable ::std::valarray<double> m_point;
				mutable ::std::valarray<double> m_vertex;
				::std::unordered_set<Graphics::IGlyph*> m_cpGlyph;
			};
		}
	}
}