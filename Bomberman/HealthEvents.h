#pragma once
#include "Event.h"
#include "GameObject.h"

namespace Jotar
{
	class HealthEvent : public Event
	{
	public:
		HealthEvent(int currentHealth)
			: m_Health{ currentHealth } {}
		virtual ~HealthEvent() = default;

		int GetHealth() const { return m_Health; }

	protected:
		int m_Health;
	};



	class DamageHealthEvent final : public HealthEvent
	{
	public:
		DamageHealthEvent(int currentHealth, GameObject* attacker)
			:HealthEvent(currentHealth)
			, m_Attacker{ attacker }
		{}
		virtual ~DamageHealthEvent() = default;

		GameObject* GetAttacker() const { return m_Attacker; }
	private:
		GameObject* m_Attacker;
	};


	class UpdateHealthEvent final : public HealthEvent
	{
	public:
		UpdateHealthEvent(int currentHealth)
			:HealthEvent(currentHealth) {}
		virtual ~UpdateHealthEvent() = default;
	};

}