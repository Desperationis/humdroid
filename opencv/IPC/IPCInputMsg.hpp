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

struct LoadTemplateMsg : public IPCInputMsg {
	static bool IsMsg(json in) {
		return in.contains("loadTemplate");
	}

	LoadTemplateMsg(json in) : IPCInputMsg(in) {}

	std::string GetPath() {
		return in["loadTemplate"]["path"];
	}

	int GetID() {
		return in["loadTemplate"]["id"];
	}

	int GetGroup() {
		return in["loadTemplate"]["group"];
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
