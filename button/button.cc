#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libbutton.so button.cc
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
using namespace std;

class button : public component::base {
	
private: 
	bool HitTest() {
		// test for mouse position
		if (read<float>("texture.position.x") <= read<int>("mouse.x") && (read<float>("texture.position.x") + read<int>("texture.position.w")) >= read<int>("mouse.x") ) {
			if (read<float>("texture.position.y") <= read<int>("mouse.y") && (read<float>("texture.position.y") + read<int>("texture.position.h")) >= read<int>("mouse.y") ) {
				return true;
			}
		}
		return false;
	}
	
public:
	bool hitTest;
	
	virtual component::type type() {return "button";}
	virtual component::family family() {return "button";}
	virtual std::string depends() {return "mouse/mouse";} //TODO: add renderer/renderer
	virtual void setup(object::signature & sig) {
		write<bool>("hitTest", false);
		hook("hitTest");
	}
	
	virtual void update(timediff de) {

	}
	
	virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
		if (pid == "hitTest") {
			write<bool>("hitTest.return",HitTest());
		}
	} 
};

extern "C" {
	component::base * build( ) {
		return new button;
	}
}