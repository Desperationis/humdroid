#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "opencv_wrapper/Match.hpp"
#include "nlohmann/json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json;

struct IPCMessage {
protected:
	json in;

public:
	IPCMessage(json in) {
		this->in = in;
	}
};

struct LoadTemplatesMsg : public IPCMessage {
	static bool IsMsg(json in) {
		return in.contains("loadTemplates");
	}

	LoadTemplatesMsg(json in) : IPCMessage(in) {}

	std::vector<std::string> GetTemplates() {
		return in["loadTemplates"]["templates"];
	}
};

struct CompareSingleMsg : public IPCMessage {
	static bool IsMsg(json in) {
		return in.contains("compareSingle");
	}

	CompareSingleMsg(json in) : IPCMessage(in) {}

	std::string GetPhoto() {
		return in["compareSingle"]["photo"];
	}

	int GetID() {
		return in["compareSingle"]["id"];
	}

	double GetMinConfidence() {
		return in["compareSingle"]["minConfidence"];
	}
};



struct IPCOutputMsg {
protected:
	json out;
public:
	virtual json ToJSON() {
		return out;
	};
};



struct MatchesMsg : public IPCOutputMsg {
public:
	MatchesMsg() {
		out["matches"] = json::array();
	}

	void AddMatch(Match match) {
		std::string origin = "topleft";
		if(match.getOrigin() == Match::ORIGIN::CENTER)
			origin = "center";

		out["matches"].push_back( {
			{ "id", match.getID() },
			{ "x", match.getX() },
			{ "y", match.getY() },
			{ "confidence", match.getConfidence() },
			{ "origin",  origin}
		} );
	}
};

#endif
