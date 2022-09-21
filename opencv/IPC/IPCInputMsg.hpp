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

	json GetJSON() {
		return in;
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

struct CompareIDMsg : public IPCInputMsg {
	static bool IsMsg(json in) {
		return in.contains("compareID");
	}

	CompareIDMsg(json in) : IPCInputMsg(in) {}

	std::string GetPhoto() {
		return in["compareID"]["photo"];
	}

	int GetID() {
		return in["compareID"]["id"];
	}

	double GetMinConfidence() {
		return in["compareID"]["minConfidence"];
	}
};


struct CompareGroupMsg : public IPCInputMsg {
	static bool IsMsg(json in) {
		return in.contains("compareGroup");
	}

	CompareGroupMsg(json in) : IPCInputMsg(in) {}

	std::string GetPhoto() {
		return in["compareGroup"]["photo"];
	}

	int GetGroup() {
		return in["compareGroup"]["group"];
	}

	double GetMinConfidence() {
		return in["compareGroup"]["minConfidence"];
	}
};


#endif
