class Engine {
public:
	enum GameStaus{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	}gameStatus;
	TCODList<Actor *> actors;
	Actor *player;
	Map *map;
	int fovRadius;
	int screenWidth;
	int screenHeight;
	TCOD_key_t lastKey;		
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