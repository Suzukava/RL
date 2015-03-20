#include <stdio.h>
#include <math.h>
#include "main.h"

static const int TRACKING_TURNS=3;

int round (double value){
	return (int) floor(value + 0.5);
}

MonsterAi::MonsterAi() : moveCount(0) {
}

void PlayerAi::update(Actor *owner) {
	if(owner->destructible && owner->destructible->isDead()){
		return;
	}
	int dx = 0, dy = 0;
	switch(engine.lastKey.vk){
		case TCODK_UP: dy = -1;
			break;
		case TCODK_DOWN: dy = 1;
			break;
		case TCODK_LEFT: dx = -1;
			break;
		case TCODK_RIGHT: dx = 1;
			break;
		case TCODK_CHAR: handleActionKey(owner, engine.lastKey.c);
			break;
		default:break;
	}
	if (dx != 0 || dy != 0) {
		engine.gameStatus = Engine::NEW_TURN;
		if(moveOrAttack(owner, owner->x+dx, owner->y+dy)){
			engine.map->computeFov();
		}
	}
}

void PlayerAi::handleActionKey(Actor *owner, int ascii) {
	switch(ascii) {
		case 'g': 
		{
			bool found = false;
			for (Actor **iterator = engine.actors.begin(); iterator < engine.actors.end(); iterator++ ){
				Actor *actor = *iterator;
				if(actor->pickable && actor->x == owner->x && actor->y == owner->y){
					if(actor->pickable->pick(actor, owner)) {
						found = true;
						engine.gui->message(TCODColor::lightGrey, "You pick the %s.", actor->name);
						break;
					} else if(! found){
						found = true;
						engine.gui->message(TCODColor::red, "You inventory is full");
					}
				}
			}
			if (!found) {
				engine.gui->message(TCODColor::lightGrey, "There's nothing here that you can pick");
			}
			engine.gameStatus = Engine::NEW_TURN;
		}
		break;
		case 'i':
		{
			Actor * actor = choseFromInventory(owner);
			if (actor) {
				actor->pickable->use(actor, owner);
				engine.gameStatus = Engine::NEW_TURN;
			}

		}
		break;

	}
}

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety){
	if (engine.map->isWall(targetx,targety)) {
		return false;
	}
	for (Actor **iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++){
		Actor *actor = *iterator;
		
		if (actor->destructible && !actor->destructible->isDead() && actor->x == targetx && actor->y == targety){
			owner->attacker->attack(owner, actor);
			//engine.gui->message(TCODColor::lightGrey, "The %s laughs at your puny efforts to attack him \n", actor->name);
			return false;
		}
	}
	for (Actor **iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++){
		Actor *actor = *iterator;
		bool corpseOrItem = (actor->destructible && actor->destructible->isDead()) || actor->pickable;
		if (corpseOrItem && actor->x == targetx && actor->y == targety){
			engine.gui->message(TCODColor::lightGrey, "There is %s\n", actor->name);
		}
	}
	owner->x = targetx;
	owner->y = targety;
	return true;
}

Actor *PlayerAi::choseFromInventory(Actor *owner){
	static const int INVENTORT_WIDTH = 50;
	static const int INVENTORY_HEIGHT = 28;
	static TCODConsole con(INVENTORT_WIDTH, INVENTORY_HEIGHT);

	//display the inventory frame
	con.setDefaultForeground(TCODColor(200,180,50));
	con.printFrame(0, 0, INVENTORT_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "inventory");

	//display the items with their keyboard shortcut
	con.setDefaultForeground(TCODColor::white);
	int shortcut = 'a';
	int y = 1;
	for (Actor **it = owner->container->inventory.begin(); it != owner->container->inventory.end(); it++){
		Actor *actor = *it;
		con.print(2, y, "(%c) %s", shortcut, actor->name);
		y++;
		shortcut++;
	}

	TCODConsole::blit(&con, 0, 0, INVENTORT_WIDTH, INVENTORY_HEIGHT, TCODConsole::root, engine.screenWidth/2 - INVENTORT_WIDTH/2, engine.screenHeight/2 - INVENTORY_HEIGHT/2);
	TCODConsole::flush();

	//wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

	if (key.vk == TCODK_CHAR) {
		int actorIndex = key.c - 'a';

		if(actorIndex >= 0 && actorIndex < owner->container->inventory.size()){
			return owner->container->inventory.get(actorIndex);
		}
	}
	return NULL;
}

void MonsterAi::update(Actor *owner){
	if(owner->destructible && owner->destructible->isDead()){
		return;
	}

	if(engine.map->isInFov(owner->x, owner->y)){
		moveCount = TRACKING_TURNS;
	}else {
		moveCount--;
	}

	if(moveCount > 0){
		moveOrAttack(owner, engine.player->x, engine.player->y);
	}
}

void MonsterAi::moveOrAttack(Actor *owner, int targetx, int targety){
	int dx = targetx - owner->x;
	int dy = targety - owner->y;
	int stepdx = (dx > 0 ? 1 : -1);
	int stepdy = (dy > 0 ? 1 : -1);
	float distance = sqrtf(dx*dx + dy*dy);

	if( distance >= 2) {
		dx = (int)(round(dx/distance));
		dy = (int)(round(dy/distance));
		if(engine.map->canWalk(owner->x+dx, owner->y+dy)){
			owner->x += dx;
			owner->y += dy;
		}else if (engine.map->canWalk(owner->x+stepdx, owner->y)){
			owner->x =+ stepdx;
		}else if (engine.map->canWalk(owner->x, owner->y+stepdy)){
			owner->y += stepdy;
		}
	}
	else if(owner->attacker) {
			owner->attacker->attack(owner, engine.player);
		
}
}