class Attacker {
public:
	float power; //hp given

	Attacker(float power);
	void attack(Actor *owner, Actor *target);
	int rollDamage(char dice);
};