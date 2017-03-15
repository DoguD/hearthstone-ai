#pragma once

#include <assert.h>
#include <memory>
#include <utility>
#include "state/Cards/Card.h"
#include "state/Cards/EnchantmentAuxData.h"
#include "FlowControl/Contexts.h"

namespace FlowControl
{
	namespace Manipulators
	{
		namespace Helpers
		{
			class EnchantmentHelper
			{
			public:
				EnchantmentHelper(state::State & state, FlowControl::FlowContext & flow_context, state::CardRef card_ref, state::Cards::Card & card) :
					state_(state), flow_context_(flow_context), card_ref_(card_ref), card_(card)
				{
				}

				template <typename T>
				decltype(auto) Add(T&& enchantment)
				{
					state::Cards::EnchantmentAuxData & data = card_.GetMutableEnchantmentAuxDataGetter().Get();

					data.need_update = true;
					return data.enchantments.PushBack(std::forward<T>(enchantment));
				}

				template <typename EnchantmentType, typename T>
				decltype(auto) Remove(T&& id)
				{
					state::Cards::EnchantmentAuxData & data = card_.GetMutableEnchantmentAuxDataGetter().Get();

					data.need_update = true;
					return data.enchantments.Remove<EnchantmentType>(std::forward<T>(id));
				}

				void Update()
				{
					state::Cards::EnchantmentAuxData & data = card_.GetMutableEnchantmentAuxDataGetter().Get();
					if (!data.need_update) return;

					switch (card_.GetCardType()) {
					case state::kCardTypeHero:
						UpdateHero();
						break;
					case state::kCardTypeMinion:
						UpdateMinion();
						break;
					case state::kCardTypeWeapon:
						UpdateWeapon();
						break;
					default:
						throw std::exception("not implemented");
					}

					data.need_update = false;
				}

			private:
				void UpdateHero();
				void UpdateMinion();
				void UpdateWeapon();

				void UpdateCharacter(state::Cards::EnchantableStates const& new_states);

			private:
				void ChangeMinionPlayer(state::PlayerIdentifier player);

			private:
				state::State & state_;
				FlowControl::FlowContext & flow_context_;
				state::CardRef card_ref_;
				state::Cards::Card & card_;
			};
		}
	}
}