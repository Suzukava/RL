class Actor {
	public :
		int x, y; //position on map
		int ch; //ascii code
		const char *name; //the actors name
		TCODColor col;

		bool blocks; //can we walk on this actor?
		Attacker *attacker; // something that deals damages
		Destructible *destructible; //something that can be damaged
		Ai *ai; // something self-update
		Pickable *pickable;
		Container *container;

		float getDistance(int cx, int cy) const;

		Actor(int x, int y, int ch, const char *name, const TCODColor &col);
		~Actor();
		void update();
		//bool moveOrAttack(int x, int y);
		void render() const;

};