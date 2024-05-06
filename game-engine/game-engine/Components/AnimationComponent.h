#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

struct AnimationComponent {
	int numFrames;
	int currentFrame;
	int animSpeed;
	bool isLooping;
	int startTime;

	AnimationComponent(int numFrames = 1, int animSpeed = 1, bool isLooping = true) {
		this->numFrames = numFrames;
		this->currentFrame = 1;
		this->animSpeed = animSpeed;
		this->isLooping = isLooping;
		this->startTime = 0;
	}
};

#endif