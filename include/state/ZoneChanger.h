#pragma once

#include <assert.h>
#include "state/Types.h"
#include "state/board/Board.h"
#include "state/Cards/Manager.h"
#include "state/detail/PlayerDataStructureMaintainer.h"
#include "state/detail/InvokeCallback.h"

namespace state {
	template <CardZone ChangingCardZone, CardType ChangingCardType>
	class ZoneChanger
	{
	public:
		ZoneChanger(board::Board & board, Cards::Manager & cards_mgr, IRandomGenerator & random,CardRef card_ref, Cards::Card &card)
			: board_(board), cards_mgr_(cards_mgr), random_(random),card_ref_(card_ref), card_(card)
		{
			assert(card.GetZone() == ChangingCardZone);
			assert(card.GetCardType() == ChangingCardType);
		}

		template <CardZone ChangeToZone,
			typename = std::enable_if_t<detail::PlayerDataStructureMaintainer<ChangingCardType, ChangeToZone>::SpecifyAddPosition == false>>
			void ChangeTo(PlayerIdentifier player_identifier)
		{
			detail::PlayerDataStructureMaintainer<ChangingCardType, ChangingCardZone>::Remove(board_, cards_mgr_, random_, card_ref_, card_);
			detail::InvokeCallback<ChangingCardType, ChangingCardZone>::Removed(card_ref_, card_);
			card_.SetZone()(player_identifier, ChangeToZone);
			detail::PlayerDataStructureMaintainer<ChangingCardType, ChangeToZone>::Add(board_, cards_mgr_, random_, card_ref_, card_);
			detail::InvokeCallback<ChangingCardType, ChangeToZone>::Added(card_ref_, card_);
		}

		template <CardZone ChangeToZone,
			typename = std::enable_if_t<detail::PlayerDataStructureMaintainer<ChangingCardType, ChangeToZone>::SpecifyAddPosition == true>>
			void ChangeTo(PlayerIdentifier player_identifier, int pos)
		{
			detail::PlayerDataStructureMaintainer<ChangingCardType, ChangingCardZone>::Remove(board_, cards_mgr_, random_, card_ref_, card_);
			detail::InvokeCallback<ChangingCardType, ChangingCardZone>::Removed(card_ref_, card_);
			card_.SetZone()(player_identifier, ChangeToZone);
			detail::PlayerDataStructureMaintainer<ChangingCardType, ChangeToZone>::Add(board_, cards_mgr_, random_, card_ref_, card_, pos);
			detail::InvokeCallback<ChangingCardType, ChangeToZone>::Added(card_ref_, card_);
		}

	private:
		board::Board& board_;
		Cards::Manager& cards_mgr_;
		IRandomGenerator & random_;
		CardRef card_ref_;
		Cards::Card & card_;
	};

	template <CardType ChangingCardType>
	class ZoneChangerWithUnknownZone
	{
	public:
		ZoneChangerWithUnknownZone(board::Board & board, Cards::Manager & cards_mgr, IRandomGenerator & random,CardRef card_ref, Cards::Card &card)
			: board_(board), cards_mgr_(cards_mgr), random_(random),card_ref_(card_ref), card_(card)
		{
			assert(card.GetCardType() == ChangingCardType);
		}

		template <CardZone ChangeToZone,
			typename = std::enable_if_t<detail::PlayerDataStructureMaintainer<ChangingCardType, ChangeToZone>::SpecifyAddPosition == false>>
			void ChangeTo(PlayerIdentifier player_identifier)
		{
			switch (card_.GetZone())
			{
			case kCardZoneDeck:
				return ZoneChanger<kCardZoneDeck, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardZoneGraveyard:
				return ZoneChanger<kCardZoneGraveyard, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardZoneHand:
				return ZoneChanger<kCardZoneHand, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardZonePlay:
				return ZoneChanger<kCardZonePlay, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardZoneSetASide:
				return ZoneChanger<kCardZoneSetASide, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardZoneRemoved:
				return ZoneChanger<kCardZoneRemoved, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardZoneInvalid:
				return ZoneChanger<kCardZoneInvalid, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			default:
				throw std::exception("Unknown card zone");
			}
		}

		template <CardZone ChangeToZone,
			typename = std::enable_if_t<detail::PlayerDataStructureMaintainer<ChangingCardType, ChangeToZone>::SpecifyAddPosition == true>>
			void ChangeTo(PlayerIdentifier player_identifier, int pos)
		{
			switch (card_.GetZone())
			{
			case kCardZoneDeck:
				return ZoneChanger<kCardZoneDeck, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardZoneGraveyard:
				return ZoneChanger<kCardZoneGraveyard, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardZoneHand:
				return ZoneChanger<kCardZoneHand, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardZonePlay:
				return ZoneChanger<kCardZonePlay, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardZoneSetASide:
				return ZoneChanger<kCardZoneSetASide, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardZoneRemoved:
				return ZoneChanger<kCardZoneRemoved, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardZoneInvalid:
				return ZoneChanger<kCardZoneInvalid, ChangingCardType>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			default:
				throw std::exception("Unknown card zone");
			}
		}

	private:
		board::Board& board_;
		Cards::Manager& cards_mgr_;
		IRandomGenerator & random_;
		CardRef card_ref_;
		Cards::Card & card_;
	};

	template <CardZone ChangingCardZone>
	class ZoneChangerWithUnknownType
	{
	public:
		ZoneChangerWithUnknownType(board::Board & board, Cards::Manager & cards_mgr, IRandomGenerator & random,CardRef card_ref, Cards::Card &card)
			: board_(board), cards_mgr_(cards_mgr), random_(random),card_ref_(card_ref), card_(card)
		{
			assert(card.GetZone() == ChangingCardZone);
		}

		template <CardZone ChangeToZone>
		void ChangeTo(PlayerIdentifier player_identifier)
		{
			switch (card_.GetCardType())
			{
			case kCardTypeHero:
				return ZoneChanger<ChangingCardZone, kCardTypeHero>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeHeroPower:
				return ZoneChanger<ChangingCardZone, kCardTypeHeroPower>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeMinion:
				return ZoneChanger<ChangingCardZone, kCardTypeMinion>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeSecret:
				return ZoneChanger<ChangingCardZone, kCardTypeSecret>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeSpell:
				return ZoneChanger<ChangingCardZone, kCardTypeSpell>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeWeapon:
				return ZoneChanger<ChangingCardZone, kCardTypeWeapon>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeEnchantment:
				return ZoneChanger<ChangingCardZone, kCardTypeEnchantment>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			default:
				throw std::exception("unknown card type");
			}
		}

		template <CardZone ChangeToZone>
		void ChangeTo(PlayerIdentifier player_identifier, int pos)
		{
			switch (card_.GetCardType())
			{
			case kCardTypeHero:
				return ZoneChanger<ChangingCardZone, kCardTypeHero>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeHeroPower:
				return ZoneChanger<ChangingCardZone, kCardTypeHeroPower>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeMinion:
				return ZoneChanger<ChangingCardZone, kCardTypeMinion>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeSecret:
				return ZoneChanger<ChangingCardZone, kCardTypeSecret>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeSpell:
				return ZoneChanger<ChangingCardZone, kCardTypeSpell>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeWeapon:
				return ZoneChanger<ChangingCardZone, kCardTypeWeapon>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeEnchantment:
				return ZoneChanger<ChangingCardZone, kCardTypeEnchantment>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			default:
				throw std::exception("unknown card type");
			}
		}

		template <>
		void ChangeTo<kCardZonePlay>(PlayerIdentifier player_identifier)
		{
			static constexpr CardZone ChangeToZone = kCardZonePlay;
			switch (card_.GetCardType())
			{
			case kCardTypeHero:
				return ZoneChanger<ChangingCardZone, kCardTypeHero>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeHeroPower:
				return ZoneChanger<ChangingCardZone, kCardTypeHeroPower>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeMinion:
				assert(false); // should specify position
			case kCardTypeSecret:
				return ZoneChanger<ChangingCardZone, kCardTypeSecret>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeSpell:
				return ZoneChanger<ChangingCardZone, kCardTypeSpell>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeWeapon:
				return ZoneChanger<ChangingCardZone, kCardTypeWeapon>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeEnchantment:
				return ZoneChanger<ChangingCardZone, kCardTypeEnchantment>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			default:
				throw std::exception("unknown card type");
			}
		}
		template <>
		void ChangeTo<kCardZonePlay>(PlayerIdentifier player_identifier, int pos)
		{
			static constexpr CardZone ChangeToZone = kCardZonePlay;
			switch (card_.GetCardType())
			{
			case kCardTypeHero:
				assert(false); // should not specify position
			case kCardTypeHeroPower:
				assert(false); // should not specify position
			case kCardTypeMinion:
				return ZoneChanger<ChangingCardZone, kCardTypeMinion>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeSecret:
				assert(false); // should not specify position
			case kCardTypeSpell:
				assert(false); // should not specify position
			case kCardTypeWeapon:
				assert(false); // should not specify position
			case kCardTypeEnchantment:
				assert(false); // should not specify position
			default:
				throw std::exception("unknown card type");
			}
		}

	private:
		board::Board& board_;
		Cards::Manager& cards_mgr_;
		IRandomGenerator & random_;
		CardRef card_ref_;
		Cards::Card & card_;
	};

	class ZoneChangerWithUnknownZoneUnknownType
	{
	public:
		ZoneChangerWithUnknownZoneUnknownType(board::Board& board, Cards::Manager& cards_mgr, IRandomGenerator & random,CardRef card_ref, Cards::Card &card)
			: board_(board), cards_mgr_(cards_mgr), random_(random),card_ref_(card_ref), card_(card)
		{}

		template <CardZone ChangeToZone>
		void ChangeTo(PlayerIdentifier player_identifier)
		{
			switch (card_.GetCardType())
			{
			case kCardTypeMinion:
				return ZoneChangerWithUnknownZone<kCardTypeMinion>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeHeroPower:
				return ZoneChangerWithUnknownZone<kCardTypeHeroPower>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeSecret:
				return ZoneChangerWithUnknownZone<kCardTypeSecret>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeSpell:
				return ZoneChangerWithUnknownZone<kCardTypeSpell>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			case kCardTypeWeapon:
				return ZoneChangerWithUnknownZone<kCardTypeWeapon>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier);
			default:
				throw std::exception("unknown card type");
			}
		}

		template <CardZone ChangeToZone>
		void ChangeTo(PlayerIdentifier player_identifier, int pos)
		{
			switch (card_.GetCardType())
			{
			case kCardTypeHero:
				return ZoneChangerWithUnknownZone<kCardTypeHero>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeMinion:
				return ZoneChangerWithUnknownZone<kCardTypeMinion>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeHeroPower:
				return ZoneChangerWithUnknownZone<kCardTypeHeroPower>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeSecret:
				return ZoneChangerWithUnknownZone<kCardTypeSecret>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeSpell:
				return ZoneChangerWithUnknownZone<kCardTypeSpell>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			case kCardTypeWeapon:
				return ZoneChangerWithUnknownZone<kCardTypeWeapon>(board_, cards_mgr_, random_, card_ref_, card_).ChangeTo<ChangeToZone>(player_identifier, pos);
			default:
				throw std::exception("unknown card type");
			}
		}

	private:
		board::Board& board_;
		Cards::Manager& cards_mgr_;
		IRandomGenerator & random_;
		CardRef card_ref_;
		Cards::Card & card_;
	};
}