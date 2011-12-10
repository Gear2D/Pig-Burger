#if 0
#!/bin/bash
g++ -fPIC -I/opt/local/include/ -lgear2d -shared -o libnoSheet.so noSheet.cc
exit
#endif
/*
 *  noSheet.cc
 *  
 *
 *  Created by Anderson Campos Cardoso on 11/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "gear2d/gear2d.h"
using namespace gear2d;

#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

class noSheet : public component::base {
	
private:
	bool animating;
	float timer;
	int frameIndex;
	string path;
	string extension;
	vector<string> textureNames;
	
	
	void startAnimation() {
		startAnimation(read<int>("animation.beginFrame"));
	}
	
	void startAnimation(int initialFrame) {
		//if (!read<bool>("full")) {
			//write<float>("frame1.position.z",read<float>("empty.position.z"));
			//write<float>("empty.position.z",read<float>("defaulZ"));
			animating = true;
			timer = 0.0f;
			frameIndex = initialFrame;//read<int>("animation.beginFrame");
		
			stringstream out;
			out << frameIndex;
			write<bool>(out.str() + ".render", true);
			//write<float>(out.str() + ".position.z", -1.0f * read<float>("animation.z"));
		//}
	}
	
	void finishAnimation() {
		//write<float>("complete.position.z",read<float>("frame3.position.z"));
		//write<float>("frame3.position.z",read<float>("defaulZ"));
		stringstream out;
		out << frameIndex-1;
		write<bool>(out.str() + ".render", false);
		animating = false;
	}
	
public:
	
	virtual component::type type() {return "noSheet";}
	virtual component::family family() {return "animation";}
	virtual std::string depends() {return "renderer/renderer";}
	virtual void setup(object::signature & sig) {
		init("animation.frameRate", sig["animation.frameRate"], 1.0f);
		path = sig["animation.path"];
		extension = sig["animation.extension"];
		init("animation.finalFrame", sig["animation.finalFrame"], 0);
		init("animation.beginFrame", sig["animation.beginFrame"], 0);
		init("animation.x", sig["animation.x"], 0.0f);
		init("animation.y", sig["animation.y"], 0.0f);
		init("animation.z", sig["animation.z"], 0.0f);
		
		// length of the finalFrame
		//stringstream out;
		//out << read<int>("animation.finalFrame");
		//int finalFrameLength = out.str().size();
		
		// string of files
		//string fileNames;
		
		// load images
		/*for (int i = read<int>("animation.beginFrame"); i <= read<int>("animation.finalFrame"); i++) {
			// index
			stringstream out;
			out << i;
			
			// left zeros
			string leftZeros = "";
			for (int y = 0; y < finalFrameLength - out.str().size(); y++) {
				leftZeros.append("0");
			}
			cout << out.str() + "=" + path + leftZeros + out.str() + extension << endl;
			fileNames.append(out.str() + "=" + path + leftZeros + out.str() + extension + " ");
			textureNames.push_back(out.str() + "=" + path + leftZeros + out.str() + extension);
			//write<string>("renderer.surfaces",textureNames[i]);
			//string test = out.str() + ".position.z";
			//write<int>(out.str() + ".position.z", 10001);
		}*/
		//write<string>("renderer.surfaces",fileNames);
		//cout << fileNames << endl;
		
		for (int i = read<int>("animation.beginFrame"); i <= read<int>("animation.finalFrame"); i++) {
			// index
			stringstream out;
			out << i;
			write<float>(out.str() + ".position.x", read<float>("animation.x"));
			write<float>(out.str() + ".position.y", read<float>("animation.y"));
			write<float>(out.str() + ".position.z", read<float>("animation.z"));
			write<bool>(out.str() + ".render", false);
		}
		animating = false;
		//startAnimation();
		write<int>("startAnimation",read<int>("animation.beginFrame"));
		hook("startAnimation");
	}
	
	virtual void update(timediff de) {
		if (animating) {
			if (timer >= read<float>("animation.frameRate")) {
				++frameIndex;	//increment
				if (frameIndex > read<int>("animation.finalFrame")) {
					finishAnimation();
				}
				else {
					// go to next frame
					timer -= read<float>("animation.frameRate");
					stringstream previous, current;
					previous << (frameIndex-1);
					current << frameIndex;
					write<bool>(previous.str() + ".render", false);
					write<bool>(current.str() + ".render", true);
					//cout << frameIndex << " \t " << previous.str() << " " << current.str() << " " << " \t " << read<bool>(previous.str() + ".render") << " " << read<bool>(current.str() + ".render") << endl;
				}
			}
			else {
				timer += de;
			}
		}
	}
	
	virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
		if (pid == "startAnimation") {
			startAnimation(read<int>("startAnimation"));
		}
	} 
};

extern "C" {
	component::base * build( ) {
		return new noSheet;
	}
}