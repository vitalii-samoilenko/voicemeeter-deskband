#pragma once

namespace Voicemeeter {
	namespace Decorators {
		class IAmplifier {
		public:
			IAmplifier(const IAmplifier&) = delete;
			IAmplifier(IAmplifier&&) = delete;

			IAmplifier& operator=(const IAmplifier&) = delete;
			IAmplifier& operator=(IAmplifier&&) = delete;

			virtual double get_Gain() const = 0;
			virtual void set_Gain(double value) = 0;
			virtual bool get_Mute() const = 0;
			virtual void set_Mute(bool value) = 0;

		protected:
			IAmplifier() = default;

			~IAmplifier() = default;
		};
	}
}