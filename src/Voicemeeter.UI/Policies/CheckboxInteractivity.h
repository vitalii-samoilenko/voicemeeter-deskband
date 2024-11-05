#pragma once

#include "estd/linear_algebra.h"

#include "../Controls/Checkbox.h"
#include "../IFocusTracker.h"
#include "IInteractivity.h"

namespace Voicemeeter {
	namespace UI {
		namespace Policies {
			template<typename TGlyph>
			class CheckboxInteractivity final : public IInteractivity<Controls::Checkbox<TGlyph>> {
			public:
				explicit CheckboxInteractivity(
					IFocusTracker& focusTracker
				) : m_focusTracker{ focusTracker } {

				};
				CheckboxInteractivity(const CheckboxInteractivity&) = delete;
				CheckboxInteractivity(CheckboxInteractivity&&) = delete;

				~CheckboxInteractivity() = default;

				CheckboxInteractivity& operator=(const CheckboxInteractivity&) = delete;
				CheckboxInteractivity& operator=(CheckboxInteractivity&&) = delete;

				virtual void set_Focus(Controls::Checkbox<TGlyph>& component, bool value) const override {

				};

				virtual void MouseLDown(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					m_focusTracker.set_Track(component, true);

					component.SetNext();
				};
				virtual void MouseLDouble(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					component.SetNext();
				};
				virtual void MouseMDown(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {

				};
				virtual void MouseMDouble(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {
					
				};
				virtual void MouseRDown(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {

				};
				virtual void MouseRDouble(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {

				};
				virtual void MouseWheel(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point, int delta) const override {

				};
				virtual void MouseMove(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {

				};
				virtual void MouseLUp(Controls::Checkbox<TGlyph>& component, const ::linear_algebra::vectord& point) const override {

				};

			private:
				IFocusTracker& m_focusTracker;
			};
		}
	}
}