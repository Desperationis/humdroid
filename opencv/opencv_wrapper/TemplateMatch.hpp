#ifndef TEMPLATEMATCH_HPP
#define TEMPLATEMATCH_HPP

#include "Match.hpp"
#include <string>
#include <vector>

/**
 * Base class for Template Matching.
 */ 
class TemplateMatch {
public:
	TemplateMatch() = default;
	virtual ~TemplateMatch() = 0;

	/**
	 * Adds an image to the template list.
	 *
	 * @param id Int that refers to the template. Bit masks should be used for
	 * grouping multiple templates together.
	 * @param file Path to the file.
	*/
	virtual void addTemplate(int id, std::string file) = 0;


	/**
	 * Sets the background that all templates will be matched with.
	 *
	 * @param file Path to the file.
	*/ 
	virtual void setBackground(std::string file) = 0;

	/**
	 * Returns a list of template matches with an image.
	*/ 
	virtual std::vector<Match> match() = 0;
};

TemplateMatch::~TemplateMatch() {}

#endif
