#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libtutorial.so tutorial.cc
exit
#endif
/*
 *  tutorial.cc
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

class tutorial : public component::base {
private:
	timediff deltaTime;
	object::id click2play;
	
public:
	
	tutorial() {
		deltaTime = 0.0f;
		click2play = NULL;
	}
	
	virtual component::type type() {return "tutorial";}
	virtual component::family family() {return "splash";}
	virtual std::string depends() {return "mouse/mouse";}
	virtual void setup(object::signature & sig) {
		
	}
	
	virtual void update(timediff de) {
		deltaTime += de;
		if (deltaTime >= 5.0f) {
			if (!click2play) {
				click2play = spawn("click2play");
			}
			if (read<int>("mouse.1") == 1) {
				load("gameScene");
			}
		}
	}
};

extern "C" {
	component::base * build( ) {
		return new tutorial;
	}
}