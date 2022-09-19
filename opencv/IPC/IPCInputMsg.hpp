#ifndef IPCINPUTMSG_HPP
#define IPCINPUTMSG_HPP

#include "opencv_wrapper/Match.hpp"
#include "nlohmann/json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json;

struct IPCInputMsg {
protected:
	json in;

public:
	IPCInputMsg(json in) {
		this->in = in;
	}
};

struct LoadTemplatesMsg : public IPCInputMsg {
	static bool IsMsg(json in) {
		return in.contains("loadTemplates");
	}

	LoadTemplatesMsg(json in) : IPCInputMsg(in) {}

	std::vector<std::string> GetTemplates() {
		return in["loadTemplates"]["templates"];
	}
};

struct CompareSingleMsg : public IPCInputMsg {
	static bool IsMsg(json in) {
		return in.contains("compareSingle");
	}

	CompareSingleMsg(json in) : IPCInputMsg(in) {}

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




#endif
