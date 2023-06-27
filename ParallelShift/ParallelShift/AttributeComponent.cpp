#include "stdafx.h"
#include "AttributeComponent.h"

AttributeComponent::AttributeComponent(unsigned level)
{
	this->level = level;
	this->exp = 0;
	this->expNext = 0;
	this->attributePoints = 3;

	this->vitality = 1;
	this->strength = 1;
	this->dexterity = 1;
	this->agility = 1;
	this->intelligence = 1;

	this->updateStats();
}

AttributeComponent::~AttributeComponent()
{
}

void AttributeComponent::calculateExpNext()
{
}

void AttributeComponent::updateStats()
{
}

void AttributeComponent::levelUp()
{
}

void AttributeComponent::update()
{
}
