class Actor {
	public :
		int x, y; //position on map
		int ch; //ascii code
		const char *name; //the actors name
		TCODColor col;

		Actor(int x, int y, int ch, const char *name, const TCODColor &col);
		void update();
		bool moveOrAttack(int x, int y);
		void render() const;

};