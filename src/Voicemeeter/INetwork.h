#pragma once

namespace Voicemeeter {
	class INetwork {
	public:
		INetwork(const INetwork&) = delete;
		INetwork(INetwork&&) = delete;

		INetwork& operator=(const INetwork&) = delete;
		INetwork& operator=(INetwork&&) = delete;

		virtual bool get_Vban() const = 0;
		virtual void set_Vban(bool value) = 0;

	protected:
		INetwork() = default;

		~INetwork() = default;
	};
}