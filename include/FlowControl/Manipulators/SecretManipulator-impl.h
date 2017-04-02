#pragma once

#include "FlowControl/Manipulators/SecretManipulator.h"
#include "FlowControl/Manipulate.h"

namespace FlowControl
{
	namespace Manipulators
	{
		inline void SecretManipulator::Remove()
		{
			assert(GetCard().GetZone() == state::kCardZonePlay);
			state_.GetZoneChanger<state::kCardZonePlay, state::kCardTypeSecret>(Manipulate(state_, flow_context_), card_ref_)
				.ChangeTo<state::kCardZoneGraveyard>(GetCard().GetPlayerIdentifier());
		}
	}
}