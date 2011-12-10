#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libsplash.so splash.cc
exit
#endif
/*
 *  splash.cc
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

class splash : public component::base {
private: 
	string nextScene;
	bool hooked;
	float deltaTime;
	
	virtual void MouseClicked() {
		if (read<int>("mouse.1") == 1) {
			load(nextScene);
		}
	}
	
public:
	virtual component::type type() {return "splash";}
	virtual component::family family() {return "splash";}
	virtual std::string depends() {return "mouse/mouse";}
	virtual void setup(object::signature & sig) {
		init("splash.time", sig["splash.time"], 1.0f);
		nextScene = sig["splash.nextScene"];
		
		deltaTime = 0.0f;
		hooked = false;
		
	}
	
	virtual void update(timediff de) {
		deltaTime += de;
		if (deltaTime >= read<float>("splash.time")) {
			if (!hooked) {
				hook("mouse.1");
				hooked = true;
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
		return new splash;
	}
}