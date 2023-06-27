#ifndef ATTRIBUTECOMPONENT_H
#define ATTRIBUTECOMPONENT_H

class AttributeComponent
{

public:

	unsigned level;
	unsigned exp;
	unsigned expNext;
	unsigned attributePoints;

	unsigned strength;
	unsigned vitality;
	unsigned dexterity;
	unsigned agility;
	unsigned intelligence;

	int hp;
	int hpMax;
	int damageMin;
	int damageMax;
	int accuracy;
	int defence;
	int luck;

	AttributeComponent(unsigned level);
	virtual ~AttributeComponent();

	void calculateExpNext();
	void updateStats();
	void levelUp();
	void update();
};

#endif