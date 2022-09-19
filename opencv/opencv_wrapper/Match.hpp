#ifndef MATCH_HPP
#define MATCH_HPP

#include <string>

/**
 * A match of an image through image detection.
*/ 
struct Match {
public:
	enum ORIGIN { TOPLEFT, CENTER };

private:
	int id;
	int x, y;
	double confidence;
	ORIGIN origin;

public:
	Match(int id, int x, int y, ORIGIN origin, double confidence) {
		this->id = id;
		this->x = x;
		this->y = y;
		this->origin = origin;
		this->confidence = confidence;
	}

	int getID() {
		return id;
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	ORIGIN getOrigin() {
		return origin;
	}

	double getConfidence() {
		return confidence;
	}
};

#endif
