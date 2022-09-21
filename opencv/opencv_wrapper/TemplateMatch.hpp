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
	virtual void addTemplate(int id, std::string file, int group) = 0;


	/**
	 * Sets the background that all templates will be matched with.
	 *
	 * @param file Path to the file.
	*/ 
	virtual void setBackground(std::string file) = 0;

	/**
	 * Returns a list of template matches. 
	 *
	 * match(-1, -1) -> Match with all templates
	 * match(234, -1) -> Templates w/ id == 234
	 * match(10, 3) -> Templates w/ id == 10 && group == 3
	 * match(-1, 0) -> All templates in group 0
	 *
	 * @id The specific ID of a template. -1 if the ID doesn't matter.
	 * @group The group to check against. If combined with `id`, only returns
	 * templates that fit both `id` and `group`. If -1, ignore.
	*/ 
	virtual std::vector<Match> match(int id, int group) = 0;
};

TemplateMatch::~TemplateMatch() {}

#endif
