#include <libtcod.hpp>
#include "main.h"


Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10), screenWidth(screenWidth),screenHeight(screenHeight) {
	
	TCODConsole::initRoot(screenWidth,screenHeight, "RL on libtcod", false);
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	player->destructible = new PlayerDestructible(30, 2, "you cadaver");
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	actors.push(player);
	map = new Map(80, 50);
	gui = new Gui();
	gui->message(TCODColor::red, "Welcome stranger!\nPrepare to die.");
}


Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
	delete gui;
}

void Engine::update(){
	
	if(gameStatus == STARTUP) map->computeFov();
	gameStatus=IDLE;

	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
	player->update();
	if (gameStatus == NEW_TURN) {
		for(Actor **iterator=actors.begin(); iterator != actors.end(); iterator++){
				Actor *actor = *iterator;
				if(actor != player) {
					actor->update();
				}
		}
	}	
}

void Engine::sendToBack(Actor *actor) {
	actors.remove(actor);
	actors.insertBefore(actor, 0);
}



void Engine::render() {
		TCODConsole::root->clear();
		//draw the map
		map->render();

		for(Actor **iterator=actors.begin();
			iterator != actors.end(); iterator++){
			
				Actor *actor = *iterator;
				if(map->isInFov(actor->x, actor->y)){
					actor->render();
				}
		}
		player->render();
		//show the player stats
		gui->render();
		//TCODConsole::root->print(1, screenHeight-2, "HP : %d/%d", (int)player->destructible->hp, (int) player->destructible->maxHp);
		
}
