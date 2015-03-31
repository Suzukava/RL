class Engine {
public:
	enum GameStaus{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	}gameStatus;
	TCOD_key_t lastKey;
	TCOD_mouse_t mouse;
	TCODList<Actor *> actors;
	Actor *player;
	Map *map;
	int fovRadius;
	int screenWidth;
	int screenHeight;
	Actor *getClosesMonster(int x, int y, float range) const;
	bool pickATile(int *x, int *y, float maxRange = 0.0f);
	void init();
	void load();
	void save();

	Actor *getActor(int x, int y) const;
	Gui *gui;

	Engine(int screenWidth, int screenHeight);
	~Engine();
	void update();
	void render();
	void sendToBack(Actor *owner);

	
private:
	bool computeFov;
};

extern Engine engine;