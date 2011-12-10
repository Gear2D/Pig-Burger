#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libbuttonManager.so buttonManager.cc
exit
#endif
/*
 *  buttonManager.cc
 *  
 *
 *  Created by Anderson Campos Cardoso on 11/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "gear2d/gear2d.h"
using namespace gear2d;

#include <iostream>
using namespace std;

class buttonManager : public component::base {
private:
	object::id orderManager;
	
	//buttons
	object::id trash;
	object::id lettuce;
	object::id tomato;
	object::id bread;
	object::id cheese;
	object::id hamburgers;
	object::id reload;
	
public:
	virtual component::type type() {return "buttonManager";}
	virtual component::family family() {return "buttonManager";}
	virtual std::string depends() {return "mouse/mouse";} 
	virtual void setup(object::signature & sig) {
		orderManager = spawn("orderManager");
		
		trash = spawn("trash");
		lettuce = spawn("lettuce");
		tomato = spawn("tomato");
		bread = spawn("bread");
		cheese = spawn("cheese");
		hamburgers = spawn("hamburgers");
		reload = spawn("reload");
		
		hook("mouse.1");
	}
	
	virtual void update(timediff de) {

	}
	
	void MouseClicked() {
		if (read<int>("mouse.1") == 1){
			trash->component("button")->write<bool>("hitTest",true);
			lettuce->component("button")->write<bool>("hitTest",true);
			tomato->component("button")->write<bool>("hitTest",true);
			bread->component("button")->write<bool>("hitTest",true);
			cheese->component("button")->write<bool>("hitTest",true);
			hamburgers->component("button")->write<bool>("hitTest",true);
			reload->component("button")->write<bool>("hitTest",true);
			if (reload->component("button")->read<bool>("hitTest.return")) {
				load("gear2d");
			}
			else if(trash->component("button")->read<bool>("hitTest.return")) {
				orderManager->component("orderManager")->write<int>("process", 8);
			}
			else if (lettuce->component("button")->read<bool>("hitTest.return")) {
				orderManager->component("orderManager")->write<int>("process", 0);
			}
			else if (tomato->component("button")->read<bool>("hitTest.return")) {
				orderManager->component("orderManager")->write<int>("process", 4);
			}
			else if (bread->component("button")->read<bool>("hitTest.return")) {
				orderManager->component("orderManager")->write<int>("process", 2);
			}
			else if (cheese->component("button")->read<bool>("hitTest.return")) {
				orderManager->component("orderManager")->write<int>("process", 3);
			}
			else if (hamburgers->component("button")->read<bool>("hitTest.return")) {
				orderManager->component("orderManager")->write<int>("process", 1);
			}
		}
	}
	
	virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
		if (pid == "mouse.1") {
			MouseClicked();
		}
	}
};

extern "C" {
	component::base * build( ) {
		return new buttonManager;
	}
}