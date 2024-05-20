#include "PlaceBombComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "WorldGrid.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "SoundServiceLocator.h"
#include "GameManager.h"
#include "ColliderComponent.h"

#include <iostream>


Jotar::PlaceBombComponent::PlaceBombComponent(GameObject* owner)
	: Component(owner)
	, m_MaxAmountOfBombs{ 1 }
	, m_AmountOfFlames{ 1 }
	, m_BombTimer{ 5.f }
	, m_HasDetonator{ false }
{
}

void Jotar::PlaceBombComponent::OnNotify(const ExplosionEvent&)
{
	//std::cout << "BombRemoved" << '\n';
	m_BombsPlaced.erase(m_BombsPlaced.begin());
}

void Jotar::PlaceBombComponent::UpgradeExplosionRange()
{
	++m_AmountOfFlames;
}

void Jotar::PlaceBombComponent::UpgradeMaxAmountOfBombs()
{
	++m_MaxAmountOfBombs;
}

void Jotar::PlaceBombComponent::GiveDetonator()
{
	m_HasDetonator = true;
}

void Jotar::PlaceBombComponent::PlaceBomb()
{
	if (static_cast<int>(m_BombsPlaced.size()) >= m_MaxAmountOfBombs) return;

	auto& cell = GameManager::GetInstance().GetWorldGrid()->GetGridCellByPosition(GetOwner()->GetTransform()->GetLocalPosition());

	if (cell.ObjectOnCell.expired())
	{
		auto bomb = CreateBombGameObject(cell);
	}

}

void Jotar::PlaceBombComponent::DetonateBomb()
{
	if (!m_HasDetonator) return;

	for (size_t i = 0; i < m_BombsPlaced.size(); i++)
	{
		if (m_BombsPlaced[i] != nullptr)
		{
			m_BombsPlaced[i]->Explode();
			return;
		}
	}
}

std::shared_ptr<Jotar::GameObject> Jotar::PlaceBombComponent::CreateBombGameObject(GridCell& cell)
{
	Scene& scene = SceneManager::GetInstance().GetCurrentScene();
	auto bombObj = scene.CreateGameObject("Bomb");
	auto size = GameManager::GetInstance().GetWorldGrid()->GetCellSize();
	glm::vec2 sizeVec = { size, size };

	bombObj->GetTransform()->SetSize({ sizeVec });
	auto colliderComp = bombObj->AddComponent<ColliderComponent>(true, true);
	colliderComp->SetTag("Bomb");

	auto bombComp = bombObj->AddComponent<BombComponent>(GetOwner(), m_BombTimer, m_AmountOfFlames);

	colliderComp->AddObserver(bombComp);

	bombObj->GetTransform()->SetPosition(cell.CenterCellPosition);
	cell.ObjectOnCell = bombObj;
	SoundServiceLocator::GetSoundSystem().Play(0);


	bombComp->AddObserver(this);
	m_BombsPlaced.emplace_back(bombComp);

	bombObj->AddComponent<TextureComponent>(ResourceManager::GetInstance().GetSharedTexture("Bomb"));


	bombObj->Start();
	return bombObj;
}


