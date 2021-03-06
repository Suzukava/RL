#include "main.h"
#include <stdarg.h>
#include <stdio.h>


static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_X = BAR_WIDTH +2;
static const int MSG_HEIGHT = PANEL_HEIGHT - 1;


Gui::Gui() {
	con = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);

}

Gui::~Gui() {
	delete con;
	clear();
}

void Gui::clear() {
	log.clearAndDelete();
}

Gui::Message::Message(const char *text, const TCODColor &col) : text(strdup(text)), col(col) {
	//this->text = new char[strlen(text)];
	//strcpy(this->text, text);
}

Gui::Message::~Message() {
	//delete [] text;
	free(text);
}

void Gui::message(const TCODColor &col, const char *text, ...) {
	//build the text
	va_list ap;
	char buf[512];
	va_start(ap, text);
	vsnprintf(buf, 512, text, ap);
	va_end(ap);

	char *lineBegin = buf;
	char *lineEnd;

	do {
		// make room for the new message
		if(log.size() == MSG_HEIGHT){
			Message *toRemove = log.get(0);
			log.remove(toRemove);
			delete toRemove;
		}


		//detect end of the line
		lineEnd = strchr(lineBegin, '\n');
		if (lineEnd) {
			*lineEnd = '\0';
		}

		//add a new message to the log
		Message *msg = new Message(lineBegin, col);
		log.push(msg);

		//go to the next line
		lineBegin = lineEnd + 1;

		}while (lineEnd);

}

void Gui::renderMouseLook() {
	if (! engine.map->isInFov(engine.mouse.cx, engine.mouse.cy)){
		//if mouse is out of fov, nothing to render
		return;
	}

	char buf[128] = "";
	bool first=true;
	for (Actor **it = engine.actors.begin(); it != engine.actors.end(); it++){
		Actor *actor = *it;
		// find actors under the mouse corsor
		if (actor->x == engine.mouse.cx && actor->y == engine.mouse.cy){
			if (! first) {
				strcat(buf, ", ");
			} 
			else {
				first = false;
			}
			strcat(buf, actor->name);
		}
	}
	con->setDefaultForeground(TCODColor::lightGrey);
	con->print(1,0,buf);
}


void Gui::render() {
	//clear the Gui console
	con->setDefaultBackground(TCODColor::black);
	con->clear();

	//draw the health bar
	renderBar(1, 1, BAR_WIDTH, "HP", engine.player->destructible->hp, engine.player->destructible->maxHp, TCODColor::lightRed, TCODColor::darkRed);

	//draw the log
	int y = 1;
	float colorCoef = 0.4f;

	for(Message **it = log.begin(); it != log.end(); it++){
		Message *message = *it;
		con->setDefaultForeground(message->col * colorCoef);
		con->print(MSG_X, y, message->text);
		y++;

		if(colorCoef < 1.0f){
			colorCoef += 0.3f;
		}
	}

	renderMouseLook();

	// blit the Gui console on the rot console
	TCODConsole::blit(con, 0, 0, engine.screenWidth, PANEL_HEIGHT, TCODConsole::root, 0, engine.screenHeight - PANEL_HEIGHT);



}


void Gui::renderBar(int x, int y, int width, const char *name, float value, float maxValue, const TCODColor &barColor, const TCODColor &backColor) {
	con->setDefaultBackground(backColor);
	con->rect(x, y, width, 1, false, TCOD_BKGND_SET);

	int barWidth = (int)(value / maxValue * width);

	if (barWidth > 0) {
		con->setDefaultBackground(barColor);
		con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
	}

	//print tect on top of the bar
	con->setDefaultForeground(TCODColor::white);
	con->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name, value, maxValue);


}

Menu::~Menu() {
	clear();
}

void Menu::clear() {
	items.clearAndDelete();
}

void Menu::addItem(MenuItemCode code, const char *label) {
	MenuItem *item = new MenuItem();
	item->code = code;
	item->label = label;
	items.push(item);
}

Menu::MenuItemCode Menu::pick() {
	static TCODImage img ("menu_background1.png");

	int selectedItem = 0;
	while (! TCODConsole::isWindowClosed() ) {
		img.blit2x(TCODConsole::root, 0, 0);
		int currentItem = 0;
		for (MenuItem **it = items.begin(); it != items.end(); it++){
			if (currentItem == selectedItem) {
				TCODConsole::root->setDefaultForeground(TCODColor::lightOrange);
			} else {
				TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);
			}
			TCODConsole::root->print(10, 10 + currentItem * 3, (*it)->label);
			currentItem++;
		}
		TCODConsole::flush();
		// check key press
		TCOD_key_t key;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
		switch(key.vk){
			case TCODK_UP:
				selectedItem--;
				if (selectedItem < 0) {
					selectedItem = items.size() - 1;
				} 
			break;

			case TCODK_DOWN:
				selectedItem = (selectedItem + 1) % items.size();
			break;

			case TCODK_ENTER:
				return items.get(selectedItem)->code;

			default : break;
		}
	}
	return NONE;
}
