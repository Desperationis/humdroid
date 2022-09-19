#ifndef IPCOUTPUTMSG_HPP
#define IPCOUTPUTMSG_HPP

#include "opencv_wrapper/Match.hpp"
#include "nlohmann/json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json;



struct IPCOutputMsg {
protected:
	json out;
public:
	virtual std::string Serialize() {
		return out.dump();
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
