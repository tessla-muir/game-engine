#ifndef PARTICLECOMPONENT_H
#define PARTICLECOMPONENT_H

struct ParticleComponent {
	bool isFriendly;
	// int damage;

	ParticleComponent(bool isFriendly = false) {
		this->isFriendly = isFriendly;
	}
};

#endif