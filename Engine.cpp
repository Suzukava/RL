#include <libtcod.hpp>
#include "Actor.h"
#include "Map.h"
#include "Engine.h"

Engine::Engine() {
	TCODConsole::initRoot(80,50, "libtcod", false);
	player = new Actor(40, 25, '@', TCODColor::green);
	actors.push(player);
	map = new Map(80, 50);
}

Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
}

void Engine::update(){
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
	switch(key.vk){
		case TCODK_UP:
			if(!map->isWall(player->x,player->y-1)){
				player->y--;
			}
			break;
		case TCODK_DOWN:
			if(!map->isWall(player->x, player->y+1)){
				player->y++;
			}
			break;
		case TCODK_LEFT:
			if(!map->isWall(player->x-1,player->y )){
				player->x--;
			}
			break;
		case TCODK_RIGHT:
			if(!map->isWall(player->x+1, player->y)){
				player->x++;
			}
			break;
		default:break;
	}
}

void Engine::render() {
		TCODConsole::root->clear();
		//draw the map
		map->render();

		for(Actor **iterator=actors.begin();
			iterator != actors.end(); iterator++)
			(*iterator)->render();
	}
