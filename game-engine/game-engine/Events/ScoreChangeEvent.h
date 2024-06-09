#ifndef SCORECHANGEEVENT_H
#define SCORECHANGEEVENT_H

#include "../ECS/ECS.h"
#include "./EventBus/Event.h"

class ScoreChangeEvent : public Event {
	public:
		int score;
		ScoreChangeEvent(int score) {
			this->score = score;
		}
};

#endif