class Engine {
public:
	TCODList<Actor *> actors;
	Actor *player;
	Map *map;
	int fovRadius;
	Engine();
	~Engine();
	void update();
	void render();

	enum GameStaus{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	}gameStatus;
private:
	bool computeFov;
};

extern Engine engine;