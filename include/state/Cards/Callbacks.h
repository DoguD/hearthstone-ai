#pragma once

namespace FlowControl { class Manipulate; }

namespace state
{
	namespace Events { class Manager; }

	namespace Cards
	{
		struct ZoneChangedContext {
			FlowControl::Manipulate const& manipulate_;
			Events::Manager & event_mgr_;
			CardRef card_ref_;
		};

		typedef void AddedToPlayZoneCallback(ZoneChangedContext const&);
		typedef void AddedToHandZoneCallback(ZoneChangedContext const&);
	}
}