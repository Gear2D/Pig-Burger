#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o liborderManager.so orderManager.cc
exit
#endif
/*
 *  orderManager.cc
 *  
 *
 *  Created by Anderson Campos Cardoso on 11/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "gear2d/gear2d.h"
using namespace gear2d;

#include <iostream>
#include <stack>
using namespace std;

class orderManager : public component::base {
	
private:
	stack<object::id> gameObjectStack;
	string sandStackId;
	int stackPostionYOffset;
	int stackYdefault;
	bool breadUp;
	
	object::id refri;
	object::id juice;
	object::id iceCream;
	
	void process() {
		int value = read<int>("process");
		object::id go;
		switch (value) {
			case 8:
				cout << "trash" << endl;
				while (!gameObjectStack.empty()) {
					gameObjectStack.top()->destroy();
					gameObjectStack.pop();
				}
				sandStackId = "";
				breadUp = false;
				break;
			case 4:
				cout << "tomato" << endl;
				go = spawn("tomatoStack");
				sandStackId.append("2");
				positioningStackItem(go);
				gameObjectStack.push(go);
				break;
			case 3:
				cout << "cheese" << endl;
				go = spawn("cheeseStack");
				sandStackId.append("3");
				positioningStackItem(go);
				gameObjectStack.push(go);
				break;
			case 2:
				cout << "bread" << endl;
				if (breadUp) {
					go = spawn("breadupStack");
				}
				else {
					go = spawn("breaddownStack");
				}
				sandStackId.append("4");
				positioningStackItem(go);
				gameObjectStack.push(go);
				breadUp = true;
				break;
			case 1:
				cout << "hamburgers" << endl;
				go = spawn("hamburgerStack");
				sandStackId.append("1");
				positioningStackItem(go);
				gameObjectStack.push(go);
				break;
			case 0:
				cout << "lettuce" << endl;
				go = spawn("lettuceStack");
				sandStackId.append("0");
				positioningStackItem(go);
				gameObjectStack.push(go);
				break;
			default:
				break;
		}
	}
	
	void cleanRefri() {
		refri->component("animation")->write<bool>("reset",true);
	}
	
	
	void cleanJuice() {
		juice->component("animation")->write<bool>("reset",true);
	}
	
	void cleanIceCream() {
		iceCream->component("animation")->write<bool>("reset",true);
	}
	
	void compareStacks() {
		//cout << sandStackId << endl;
		if (read<string>("compareStacks") == sandStackId) {
			write<bool>("compareStacks.return",true);
		}
		else {
			write<bool>("compareStacks.return",false);
		}

	}
	
	void compareRefri() {
		//cout << refri->component("animation")->read<bool>("full") << endl;
		if (read<bool>("compareRefri") || refri->component("animation")->read<bool>("full")) {
			write<bool>("compareRefri.return",true);
		}
		else {
			write<bool>("compareRefri.return",false);
		}
		
	}
	
	void compareJuice() {
		cout << read<bool>("compareJuice") << " " << juice->component("animation")->read<bool>("full") << endl;
		if (read<bool>("compareJuice") || juice->component("animation")->read<bool>("full")) {
			write<bool>("compareJuice.return",true);
		}
		else {
			write<bool>("compareJuice.return",false);
		}
		
	}
	
	void compareIceCream() {
		cout << read<bool>("compareIceCream") << " " << iceCream->component("animation")->read<bool>("full") << endl;
		if (read<bool>("compareIceCream") || iceCream->component("animation")->read<bool>("full")) {
			write<bool>("compareIceCream.return",true);
		}
		else {
			write<bool>("compareIceCream.return",false);
		}
		
	}
	
	void positioningStackItem(object::id go) {
		float y;
		y = (float) stackYdefault - (gameObjectStack.size() * stackPostionYOffset);
		go->component("renderer")->write<float>("texture.position.y", y);
		//cout << go->component("renderer")->read<float>("texture.position.z") << endl;
		go->component("renderer")->add<float>("texture.position.z", gameObjectStack.size());
		//cout << go->component("renderer")->read<float>("texture.position.z") << endl;
	}
	
public:
	virtual component::type type() {return "orderManager";}
	virtual component::family family() {return "orderManager";}
	virtual std::string depends() {return "";}
	virtual void setup(object::signature & sig) {
		write<int>("process",-1);
		hook("process");
		
		write<bool>("cleanRefri",false);
		hook("cleanRefri");
		
		write<bool>("cleanJuice",false);
		hook("cleanJuice");
		
		write<bool>("cleanIceCream",false);
		hook("cleanIceCream");
		
		stackPostionYOffset = 8;
		stackYdefault = 400;
		breadUp = false;
		
		sandStackId = "";
		write<string>("compareStacks","");
		hook("compareStacks");
		
		refri = locate("refri");
		write<bool>("compareRefri",false);
		hook("compareRefri");
		
		juice = locate("juice");
		write<bool>("compareJuice",false);
		hook("compareJuice");
		
		iceCream = locate("iceCream");
		write<bool>("compareIceCream",false);
		hook("compareIceCream");
	}
	
	virtual void update(timediff de) {
		
	}
	virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
		if (pid == "process") {
			process();
		}
		
		if (pid == "compareStacks") {
			compareStacks();
		}
		if (pid == "compareRefri") {
			compareRefri();
		}
		if (pid == "compareJuice") {
			compareJuice();
		}
		if (pid == "compareIceCream") {
			compareIceCream();
		}
		
		if (pid == "cleanRefri") {
			cleanRefri();
		}
		if (pid == "cleanJuice") {
			cleanJuice();
		}
		if (pid == "cleanIceCream") {
			cleanIceCream();
		}
	}
};

extern "C" {
	component::base * build( ) {
		return new orderManager;
	}
}