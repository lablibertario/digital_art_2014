#pragma once

#include "ofMain.h"
#include "ofxTiming.h"

class BufferedVideo : public ofBaseVideo {

protected:
	vector<ofPixels> images;
	int loaded;
	ofTexture texture;
	bool newFrame, playing;
	bool bRolledOver; 
	int currentFrame;
	DelayTimer timer;
	ofDirectory directory;
	void loadNextAvailable() {
		if(loaded < images.size()) {
			ofLoadImage(images[loaded], directory.getPath(loaded));
			loaded++;
		}
	}
	void updateFrame() {
		if(images[currentFrame].getWidth() > 0) {
			texture.loadData(images[currentFrame]);
		}
		newFrame = true;
	}

public:
	BufferedVideo()
	:currentFrame(0)
	,playing(true)
	,newFrame(false)
	,loaded(0) {
		timer.setFramerate(30);
	}
	void setFrameRate(float frameRate) {
		timer.setFramerate(frameRate);
	}
	void load(string directoryName) {
		loaded = 0;
        currentFrame = 0;
        newFrame = false;
        directory.listDir(directoryName);
		images.clear();
        images.resize(directory.size());
		loadNextAvailable();
		texture.allocate(images[0]);
		bRolledOver = false; 
	}
	void close() {
	}
	unsigned char* getPixels() {
		return images[currentFrame].getPixels();
	}
	ofPixels& getPixelsRef() {
		return images[currentFrame];
	}
	ofPixels& getFrame(int i) {
		return images[i];
	}
	void setPlaying(bool playing) {
		this->playing = playing;
	}
	void goToPrevious() {
		currentFrame = (currentFrame - 1 + images.size()) % images.size();
		updateFrame();
	}
	void goToNext() {
		int prevFrame = currentFrame;
		currentFrame = (currentFrame + 1) % images.size();
		if (prevFrame == (images.size()-1)){
			bRolledOver = true; 
		}
		updateFrame();
	}
	void update() {
		loadNextAvailable();
		if(playing) {
			if(timer.tick()) {
				goToNext();
			}
		}
	}
	bool isRolledOver(){
		return bRolledOver;
	}
	bool isFrameNew() {
		bool cur = newFrame;
		newFrame = false;
		return cur;
	}
	void draw(float x, float y) {
		texture.draw(x, y);
	}
	void draw(float x, float y, float w, float h) {
		texture.draw(x, y, w, h);
	}
	int getCurrentFrameID(){
		return currentFrame;
	}
};