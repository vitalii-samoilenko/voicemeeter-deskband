#pragma once

#include "../../Cherry.h"
#include "../../Multiclient/Manager.h"
#include "../Network.h"
#include "Bus.h"
#include "Line.h"
#include "Strip.h"

namespace Voicemeeter {
	namespace Adapters {
		namespace Multiclient {
			struct CherryDeclaration {
				using Line = Line<CherrySpecification>;
				struct Input {
					struct Physical {
						using Strip = Strip<CherrySpecification, CherrySpecification::Input::Physical::Strip, Line>;
						using Bus = Bus<CherrySpecification, CherrySpecification::Input::Physical, Line, Strip>;
					};
					struct Virtual {
						using Strip = Strip<CherrySpecification, CherrySpecification::Input::Virtual::Strip, Line>;
						using Bus = Bus<CherrySpecification, CherrySpecification::Input::Virtual, Line, Strip>;
					};
				};
				struct Output {
					struct Physical {
						using Strip = Strip<CherrySpecification, CherrySpecification::Output::Physical::Strip, Line>;
						using Bus = Bus<CherrySpecification, CherrySpecification::Output::Physical, Line, Strip>;
					};
					struct Virtual {
						using Strip = Strip<CherrySpecification, CherrySpecification::Output::Virtual::Strip, Line>;
						using Bus = Bus<CherrySpecification, CherrySpecification::Output::Virtual, Line, Strip>;
					};
				};
				using Mixer = Mixer<CherrySpecification,
					Line, Input::Physical::Strip, Input::Physical::Bus,
					Line, Input::Virtual::Strip, Input::Virtual::Bus,
					Line, Output::Physical::Strip, Output::Physical::Bus,
					Line, Output::Virtual::Strip, Output::Virtual::Bus>;
				using Cherry = Network<CherrySpecification,
					Line, Input::Physical::Strip, Input::Physical::Bus,
					Line, Input::Virtual::Strip, Input::Virtual::Bus,
					Line, Output::Physical::Strip, Output::Physical::Bus,
					Line, Output::Virtual::Strip, Output::Virtual::Bus,
					Mixer>;
			};

			using CherrySubscription = ::Voicemeeter::Multiclient::Subscription<CherrySpecification>;

			class Cherry
				: public ::Voicemeeter::Multiclient::Manager<CherrySpecification>
				, private CherryDeclaration::Cherry {
				using BaseManager = ::Voicemeeter::Multiclient::Manager<CherrySpecification>;
				using BaseMixer = CherryDeclaration::Cherry;

			public:
				Cherry();
				Cherry(const Cherry&) = delete;
				Cherry(Cherry&&) = delete;

				inline ~Cherry() = default;

				Cherry& operator=(const Cherry&) = delete;
				Cherry& operator=(Cherry&&) = delete;

				using BaseMixer::get_Plug;
				template<typename TClient, typename TInput, typename TOutput,
					::std::enable_if_t<
						(::std::is_same_v<CherryDeclaration::Input::Physical::Strip, TInput> || ::std::is_same_v<CherryDeclaration::Input::Virtual::Strip, TInput>)
						&& (::std::is_same_v<CherryDeclaration::Output::Physical::Strip, TOutput> || ::std::is_same_v<CherryDeclaration::Output::Virtual::Strip, TOutput>),
						bool> = true>
				inline void set_Plug(TInput& input, TOutput& output, bool value) {
					if (BaseMixer::get_Plug(input, output) == value) {
						return;
					}
					BaseMixer::set_Plug(input, output, value);
					for (const auto& client_subscription : *this) {
						if (client_subscription.first == &typeid(TClient)) {
							continue;
						}
						client_subscription.second
							.on_Plug(input.get_Id(), output.get_Id())
								(value);
					}
				};
				using BaseMixer::get_Vban;
				template<typename TClient>
				inline void set_Vban(bool value) {
					if (BaseMixer::get_Vban() == value) {
						return;
					}
					BaseMixer::set_Vban(value);
					for (const auto& client_subscription : *this) {
						if (client_subscription.first == &typeid(TClient)) {
							continue;
						}
						client_subscription.second
							.on_Vban()
								(value);
					}
				};

				using BaseMixer::get_PhysicalInput;
				using BaseMixer::get_VirtualInput;
				using BaseMixer::get_PhysicalOutput;
				using BaseMixer::get_VirtualOutput;
			};
		}
	}
}