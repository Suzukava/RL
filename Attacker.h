class Attacker : public Persistent {
public:
	float power; //hp given

	Attacker(float power);
	void attack(Actor *owner, Actor *target);
	int rollDamage(char *dice);

	void load(TCODZip &zip);
	void save(TCODZip &zip);
};