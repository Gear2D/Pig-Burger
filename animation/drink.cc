#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libdrink.so drink.cc
exit
#endif
/*
 *  drink.cc
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

/*
 // Declare nested class Drink
 class Drink
 {
 public:
 bool full;
 
 void update(timediff de) {
 if (anim && !full && timer >= time2changeFrame) {
 i += 1;
 if (i >= frames.size())
 full = true;
 else {
 time2changeFrame += frameRate;
 currentFrame->destroy();
 currentFrame = spawn(frames[i]);
 }
 
 }
 timer += de;
 }
 
 void startAnimation() {
 anim = true;
 }
 
 Drink(vector<string> p_frames, float p_frameRate) {
 frames = p_frames;
 time2changeFrame = p_frameRate;
 frameRate = p_frameRate;
 timer = 0;
 i = 0;
 currentFrame = spawn(frames[i]);
 full = false;
 anim = false;
 }
 
 ~Drink() {
 if (currentFrame != NULL) {
 currentFrame->destroy();
 }
 }
 
 private:
 float frameRate;
 float timer;
 float time2changeFrame;
 float i;
 bool anim;
 object::id currentFrame;
 vector<string> frames;
 };
 */

class drink : public component::base {
	
private:
	bool animating;
	float timer;
	int frameIndex;
	
	bool HitTest() {
		// test for mouse position
		if (read<float>("empty.position.x") <= read<int>("mouse.x") && (read<float>("empty.position.x") + read<int>("empty.position.w")) >= read<int>("mouse.x") ) {
			if (read<float>("empty.position.y") <= read<int>("mouse.y") && (read<float>("empty.position.y") + read<int>("empty.position.h")) >= read<int>("mouse.y") ) {
				if (!animating) {
					startAnimation();
					return true;
				} else {
// 					frameIndex++;
// 					timer = 0;
// 					update(0);
				}
			}
		}
		return false;
	}
	
	void startAnimation() {
		if (!read<bool>("full")) {
			write<float>("frame1.position.z",read<float>("empty.position.z"));
			write<float>("empty.position.z",read<float>("defaulZ"));
			animating = true;
			timer = 0.0f;
			frameIndex = 1;
			
		}
	}
	
	void finishAnimation() {
		write<float>("complete.position.z",read<float>("frame3.position.z"));
		write<float>("frame3.position.z",read<float>("defaulZ"));
		animating = false;
		write<bool>("full",true);
	}
	
	void reset() {
		write<float>("empty.position.z",read<float>("complete.position.z"));
		write<float>("complete.position.z",read<float>("defaulZ"));
		write<bool>("full",false);
	}
	
public:
	bool hitTest;
	
	virtual component::type type() {return "drink";}
	virtual component::family family() {return "animation";}
	virtual std::string depends() {return "mouse/mouse renderer/renderer";} //TODO: add renderer/renderer
	virtual void setup(object::signature & sig) {
		//write<bool>("hitTest", false);
		//hook("hitTest");
		
		write<bool>("reset",false);
		hook("reset");
		
		hook("mouse.1");
		init("defaulZ", sig["defaulZ"], -2000.0f);
		init("frameRate", sig["frameRate"], 1.0f);
		init("full", sig["full"], false);
		
		// set frame 2 position
		/*write<float>("frame2.position.x",read<float>("frame1.position.x"));
		write<float>("frame2.position.y",read<float>("frame1.position.y"));
		write<float>("frame2.position.z",read<float>("defaulZ"));
		
		//set frame 3 position
		write<float>("frame3.position.x",read<float>("frame1.position.x"));
		write<float>("frame3.position.y",read<float>("frame1.position.y"));
		write<float>("frame3.position.z",read<float>("defaulZ"));*/
		
		animating = false;
		timer = 0.0f;
		frameIndex = 1;
	}
	
	virtual void update(timediff de) {
		if (animating) {
			if (timer >= read<float>("frameRate")) {
				++frameIndex;	//increment
				switch (frameIndex) {
					case 2:
						write<float>("frame2.position.z",read<float>("frame1.position.z"));
						write<float>("frame1.position.z",read<float>("defaulZ"));
						break;
					case 3:
						write<float>("frame3.position.z",read<float>("frame2.position.z"));
						write<float>("frame2.position.z",read<float>("defaulZ"));
						break;
					case 4:
						finishAnimation();
						break;
					default:
						break;
				}
				timer = 0;
			}
			else {
				timer += de;
			}
		}
	}
	
	virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
		if (pid == "mouse.1" && (read<int>("mouse.1") == 1)) {
			write<bool>("hitTest.return",HitTest());
		}
		if (pid == "reset") {
			reset();
		}
	} 
};

extern "C" {
	component::base * build( ) {
		return new drink;
	}
}