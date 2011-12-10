#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libpig.so pig.cc
exit
#endif
/*
 *  button.cc
 *  
 *
 *  Created by Anderson Campos Cardoso on 11/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "gear2d/gear2d.h"
using namespace gear2d;

#include <iostream>
#include <time.h>
#include <vector>
#include <stack>
#include <string>

using namespace std;

enum PigState { MOVING_PIG, WAITING_ORDER, WAITING_NEXT_PIG };

class pig : public component::base {
	
	static int maxSandSize;
	static int minSandSize;
	static int maxTime2NextPig;
	static int minTime2NextPig;
	static int maxTime2explode;
	static int minTime2explode;
	static int stackYdefault;
	static int stackPostionYOffset;
	static bool seed;
	
	
private:
	
	object::id orderManager;
	object::id myPig;
	object::id myBalloon;
	object::id myExplosion;
	object::id life;
	
	object::id refri;
	object::id juice;
	object::id iceCream;
	bool sanduich;
	stack<object::id> gameObjectStack;
	string pigStackId;
	
	int pigIndex;
	float time2something;
	float timer;
	float velocity;
	
	enum PigState pigState;
	vector<string> pigNames;
	vector<string> pigAngryNames;
	bool angry;
	
	
	void movingPig(timediff de) {
		if (myPig->component("renderer")->read<float>("texture.position.x") <= read<float>("finalX")) {
			myPig->component("renderer")->write<float>("texture.position.x",read<float>("finalX"));

			// make order
			makeOrder();
			
			// change pig state
			pigState = WAITING_ORDER;
			time2something = (rand() % (maxTime2explode - minTime2explode)) + minTime2explode;
			timer = 0;
		}
		else {
			myPig->component("renderer")->add<float>("texture.position.x",velocity*de);
		}
		timer += de;
	}
	
	void waitingOrder(timediff de) {
		if (timer >= time2something) {
			// explode pig
			
			// play explode sound
			write<bool>("boom.playing",true);
			// play squeal
			write<bool>("squeal.playing",true);
			
			// play explostion animation
			myExplosion->component("animation")->write<int>("startAnimation",0);
			
			// lose life
			life->component("life")->write<int>("changeLife", -1);
			
			destroyPig();
			//change pig state
			pigState = WAITING_NEXT_PIG;
			timer = 0;
			time2something = (rand() % (maxTime2NextPig - minTime2NextPig)) + minTime2NextPig;
		}
		else if (!angry && timer >= time2something/2) {
			angry = true;
			// spawn angry pig
			// play angry pig sound
			write<bool>("snort.playing",true);
			float x,y;
			x = myPig->component("renderer")->read<float>("texture.position.x");
			y = myPig->component("renderer")->read<float>("texture.position.y");
			myPig->destroy();
			myPig = spawn(pigAngryNames[pigIndex]);
			myPig->component("renderer")->write<float>("texture.position.x",x);
			myPig->component("renderer")->write<float>("texture.position.y",y);
		}
		timer += de;
	}
	
	void mouseClicked() {
		if ((pigState == WAITING_ORDER) && (myPig != NULL) && (myBalloon != NULL)) {
			myPig->component("button")->write<bool>("hitTest",true);
			myBalloon->component("button")->write<bool>("hitTest",true);
			if((read<int>("mouse.1") == 1) && (myPig->component("button")->read<bool>("hitTest.return") || myBalloon->component("button")->read<bool>("hitTest.return") )) {
				//cout << "pig: " << pigStackId << endl;
				orderManager->component("orderManager")->write<string>("compareStacks",pigStackId);
				orderManager->component("orderManager")->write<bool>("compareRefri",refri == NULL);
				orderManager->component("orderManager")->write<bool>("compareJuice",juice == NULL);
				orderManager->component("orderManager")->write<bool>("compareIceCream",iceCream == NULL);
				if ((!sanduich || orderManager->component("orderManager")->read<bool>("compareStacks.return")) 
					&& orderManager->component("orderManager")->read<bool>("compareRefri.return") 
					&& orderManager->component("orderManager")->read<bool>("compareJuice.return")
					&& orderManager->component("orderManager")->read<bool>("compareIceCream.return")) {
					// play victory sound
					write<bool>("victory.playing",true);
					// add 200 to score
					if (sanduich) {
						orderManager->component("orderManager")->write<int>("process",8);
					}
					if (refri != NULL) {
						orderManager->component("orderManager")->write<bool>("cleanRefri",false);
					}
					if (juice != NULL) {
						orderManager->component("orderManager")->write<bool>("cleanJuice",false);
					}
					if (iceCream != NULL) {
						orderManager->component("orderManager")->write<bool>("cleanIceCream",false);
					}
					destroyPig();
					// change pig state
					pigState = WAITING_NEXT_PIG;
					time2something = (rand() % (maxTime2NextPig - minTime2NextPig)) + minTime2NextPig;
					timer = 0;
				}
				else {
					write<bool>("error.playing",true);
				}

			}
		}
	}
	
	
	void waitingNextPig(timediff de) {
		if (timer >= time2something) {
			//cout << pigNames.size() << endl;
			createPig();
			pigState = MOVING_PIG;
			timer = 0;
			time2something = 1;
		}
		timer += de;
	}
	
	void makeOrder() {
		myBalloon = spawn("balloon");
		myBalloon->component("renderer")->write<float>("texture.position.x",read<float>("finalX") -90);
		// refri
		if ((rand() % 2 ) == 1) {
			refri = spawn("orderRefriFull");
			float x,y;
			x = (float) read<float>("finalX")-40.0f;
			y = (float) stackYdefault - 100.0f;
			refri->component("renderer")->write<float>("texture.position.x", x);
			refri->component("renderer")->write<float>("texture.position.y", y);
		}
		// juice
		if ((rand() % 2 ) == 1) {
			juice = spawn("orderJuiceFull");
			float x,y;
			x = (float) read<float>("finalX")-80.0f;
			y = (float) stackYdefault - 100.0f;
			juice->component("renderer")->write<float>("texture.position.x", x);
			juice->component("renderer")->write<float>("texture.position.y", y);
		}
		// iceCream
		if ((rand() % 2 ) == 1) {
			iceCream = spawn("orderIceCreamFull");
			float x,y;
			x = (float) read<float>("finalX")-60.0f;
			y = (float) stackYdefault - 120.0f;
			iceCream->component("renderer")->write<float>("texture.position.x", x);
			iceCream->component("renderer")->write<float>("texture.position.y", y);
		}
		
		// sanduich
		if ((rand() % 2 ) == 1 || ((iceCream == NULL) && (juice == NULL) && (refri == NULL))) {
			object::id go;
			sanduich = true;
			go = spawn("breaddownStack");
			pigStackId.append("4");
			positioningStackItem(go);
		
			int sandSize = (rand() % (maxSandSize-minSandSize)) + minSandSize;
			for (int i = 0; i< sandSize; i++) {
				switch (rand() % 4) {
					case 0:
						go = spawn("lettuceStack");
						pigStackId.append("0");
						break;
					case 1:
						go = spawn("hamburgerStack");
						pigStackId.append("1");
						break;
					case 2:
						go = spawn("tomatoStack");
						pigStackId.append("2");
						break;
					case 3:
						go = spawn("cheeseStack");
						pigStackId.append("3");
						break;
					default:
						break;
				}
				positioningStackItem(go);
			}
		
			go = spawn("breadupStack");
			pigStackId.append("4");
			positioningStackItem(go);
		}
	}
	
	void positioningStackItem(object::id go) {
		float x,y;
		x = (float) read<float>("finalX")-86.0f;
		y = (float) stackYdefault - (gameObjectStack.size() * stackPostionYOffset);
		go->component("renderer")->write<float>("texture.position.x", x);
		go->component("renderer")->write<float>("texture.position.y", y);
		//cout << go->component("renderer")->read<float>("texture.position.z") << endl;
		go->component("renderer")->add<float>("texture.position.z", gameObjectStack.size());
		//cout << go->component("renderer")->read<float>("texture.position.z") << endl;
		gameObjectStack.push(go);
	}
	
	void createPig() {
		pigIndex = rand() % pigNames.size();
		myPig = spawn(pigNames[pigIndex]);
		angry = false;
		sanduich = false;
	}
	
	void destroyPig() {
		myPig->destroy();
		myBalloon->destroy();
		
		myPig = NULL;
		myBalloon = NULL;
		
		if (refri != NULL)
			refri->destroy();
		if (juice != NULL)
			juice->destroy();
		if (iceCream != NULL)
			iceCream->destroy();
		
		refri = NULL;
		juice = NULL;
		iceCream = NULL;
		
		// destroy stack
		while (!gameObjectStack.empty()) {
			gameObjectStack.top()->destroy();
			gameObjectStack.pop();
		}
		sanduich = false;
		pigStackId = "";
	}
	
public:
	
	pig() {
		
		// pignames
		pigNames.push_back("pigDefault");
		pigNames.push_back("pigBlond");
		pigNames.push_back("pigReggae");
		pigNames.push_back("pigHarry");
		pigNames.push_back("pigPunk");
		pigNames.push_back("pigSpider");
		
		// pigAngrynames
		pigAngryNames.push_back("pigAngryDefault");
		pigAngryNames.push_back("pigAngryBlond");
		pigAngryNames.push_back("pigAngryReggae");
		pigAngryNames.push_back("pigAngryHarry");
		pigAngryNames.push_back("pigAngryPunk");
		pigAngryNames.push_back("pigAngrySpider");

		
		// init pig variables
		pigIndex = -1;
		refri = NULL;
		juice = NULL;
		iceCream = NULL;
		sanduich = false;
		angry = false;
		
		pigStackId = "";
	}
	
	virtual component::type type() {return "button";}
	virtual component::family family() {return "button";}
	virtual std::string depends() {return "mouse/mouse";} //TODO: add renderer/renderer
	virtual void setup(object::signature & sig) {
		//write<string>("renderer.surfaces","Textures/porco_humor2/porco_padrao_raiva.png");
		init("finalX", sig["finalX"], 0.0f);
		velocity = read<float>("finalX") - 800.0f;
		
		orderManager = locate("orderManager");
		life = locate("head_pig_life"); 
		myExplosion = spawn(sig["explosion"]);
		
		if (!seed) {
			srand ( time(NULL) );
			seed = true;
		}
		time2something = (rand() % (maxTime2NextPig - minTime2NextPig)) + minTime2NextPig;
		timer = 0;
		pigState = WAITING_NEXT_PIG;
		
		hook("mouse.1");
	}
	
	virtual void update(timediff de) {
		switch (pigState) {
			case WAITING_NEXT_PIG:
				waitingNextPig(de);
				break;
			case MOVING_PIG:
				movingPig(de);
				break;
			case WAITING_ORDER:
				waitingOrder(de);
				break;
			default:
				break;
		}
	}
	
	virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
		if (pid == "mouse.1") {
			mouseClicked();
		}
	} 
};

// static initialization
int pig::maxSandSize = 3;
int pig::minSandSize = 1;
int pig::maxTime2NextPig = 10;
int pig::minTime2NextPig = 3;
int pig::minTime2explode = 10;
int pig::maxTime2explode = 30;
int pig::stackYdefault = 200;
int pig::stackPostionYOffset = 8;
bool pig::seed = false;


extern "C" {
	component::base * build( ) {
		return new pig;
	}
}