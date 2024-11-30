#include "Cherry.h"

using namespace ::Voicemeeter;
using namespace ::Voicemeeter::Adapters::Multiclient;

static constexpr size_t PIStripStart{ 0 };
static constexpr size_t PILineStart{ 0 };
static constexpr size_t VIStripStart{ PIStripStart + CherrySpecification::Input::Physical::Width };
static constexpr size_t VILineStart{ PILineStart + CherrySpecification::Input::Physical::Width * CherrySpecification::Input::Physical::Strip::Width };
static constexpr size_t POStripStart{ VIStripStart + CherrySpecification::Input::Virtual::Width };
static constexpr size_t POLineStart{ VILineStart + CherrySpecification::Input::Virtual::Width * CherrySpecification::Input::Virtual::Strip::Width };
static constexpr size_t VOStripStart{ POStripStart + CherrySpecification::Output::Physical::Width };
static constexpr size_t VOLineStart{ POLineStart + CherrySpecification::Output::Physical::Width * CherrySpecification::Output::Physical::Strip::Width };

Cherry::Cherry(
) : BaseManager{}
  , BaseMixer{
		CherryDeclaration::Input::Physical::Bus{
			CherryDeclaration::Input::Physical::Strip{
				*this, PIStripStart + 0,
				CherryDeclaration::Line{ *this, PILineStart + CherrySpecification::Input::Physical::Width * 0 + 0 },
				CherryDeclaration::Line{ *this, PILineStart + CherrySpecification::Input::Physical::Width * 0 + 1 }
			}
		},
		CherryDeclaration::Input::Virtual::Bus{
			CherryDeclaration::Input::Virtual::Strip{
				*this, VIStripStart + 0,
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 0 },
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 1 },
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 2 },
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 3 },
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 4 },
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 5 },
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 6 },
				CherryDeclaration::Line{ *this, VILineStart + CherrySpecification::Input::Virtual::Width * 0 + 7 }
			}
		},
		CherryDeclaration::Output::Physical::Bus{
			CherryDeclaration::Output::Physical::Strip{
				*this, POStripStart + 0,
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 0 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 1 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 2 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 3 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 4 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 5 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 6 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 0 + 7 }
			},
			CherryDeclaration::Output::Physical::Strip{
				*this, POStripStart + 1,
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 0 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 1 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 2 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 3 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 4 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 5 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 6 },
				CherryDeclaration::Line{ *this, POLineStart + CherrySpecification::Output::Physical::Strip::Width * 1 + 7 }
			}
	  },
	  CherryDeclaration::Output::Virtual::Bus{
			CherryDeclaration::Output::Virtual::Strip{
				*this, VOStripStart + 0,
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 0 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 1 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 2 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 3 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 4 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 5 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 6 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 0 + 7 }
			},
			CherryDeclaration::Output::Virtual::Strip{
				*this, VOStripStart + 1,
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 0 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 1 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 2 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 3 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 4 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 5 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 6 },
				CherryDeclaration::Line{ *this, VOLineStart + CherrySpecification::Output::Virtual::Strip::Width * 1 + 7 }
			}
	  }
} {

};