#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

struct ScoreComponent {
	public:
		int score;

		ScoreComponent(int score = 0) {
			this->score = score;
		}
};

#endif