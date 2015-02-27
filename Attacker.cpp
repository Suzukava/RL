#include <stdio.h>
#include "main.h"

Attacker::Attacker(float power) : power(power) {

}

void Attacker:: attack(Actor *owner, Actor *target) {
	if (target->destructible && ! target->destructible->isDead()){
		if(power - target->destructible->defense > 0){
			engine.gui->message(owner == engine.player ? TCODColor::red : TCODColor::lightGrey, "%s attack %s for %g hit points.\n", owner->name, target->name, power-target->destructible->defense);
		}
		else {
			engine.gui->message(TCODColor::lightGrey, "%s attack %s but it has no effect!\n", owner->name, target->name);
		}
		target->destructible->takeDamage(target, power);
	}
	else {
		engine.gui->message(TCODColor::lightGrey, "%s is attack %s in vain.\n", owner->name, target->name);
	}
}
