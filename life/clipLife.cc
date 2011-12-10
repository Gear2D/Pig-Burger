#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libclipLife.so clipLife.cc
exit
#endif
/*
 *  clipLife.cc
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

class clipLife : public component::base {
	
private:
	string gameOverScene;
	float timer;
	
	void changeLife(int value) {
		//cout << "antes" << endl;
		add<int>("life.current",value);
		float clipSize = read<float>("life.clipSize") * read<int>("life.current");
		write<float>("texture.clip.w", clipSize);
		cout << read<float>("texture.clip.w") <<  " " << clipSize << endl;
		//cout << "depois" << endl;
	}
	
	
public:
	
	virtual component::type type() {return "clipLife";}
	virtual component::family family() {return "life";}
	virtual std::string depends() {return "renderer/renderer";}
	virtual void setup(object::signature & sig) {
		init("life.clipSize", sig["life.clipSize"], 10.0f);
		init("life.units", sig["life.units"], 3);
		init("life.current", sig["life.current"], 3);
		init("life.time2loadScene", sig["life.time2loadScene"], 4.0f);
		gameOverScene = sig["life.gameOverScene"];

		write<int>("changeLife",0);
		hook("changeLife");
		timer = 0.0f;
	}
	
	virtual void update(timediff de) {
		// Dead?
		if (read<int>("life.current") <= 0) {
			timer+=de;
		}
		if (timer >= read<float>("life.time2loadScene")) {
			load(gameOverScene);
		}
	}
	
	virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
		//cout << "teste" << endl;
		if (pid == "changeLife") {
			changeLife(read<int>("changeLife"));
		}
	} 
};

extern "C" {
	component::base * build( ) {
		return new clipLife;
	}
}