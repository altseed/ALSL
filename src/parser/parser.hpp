#pragma once
#ifndef ALSL_PARSER_HPP
#define ALSL_PARSER_HPP

#include <memory>
#include <string>
#include <boost/optional.hpp>
#include "../AST/ast.hpp"

namespace ALSL {
	boost::optional<std::shared_ptr<Node>> parse(std::string const& fileName, std::string::const_iterator beg, std::string::const_iterator end);
	boost::optional<std::shared_ptr<Node>> parse(std::string const& fileName, std::string const& src);
	//boost::optional<std::shared_ptr<Node>> parse(std::istream_iterator<char> beg, std::istream_iterator<char> end);
	boost::optional<std::shared_ptr<Node>> parseFile(std::string const& fileName, std::string const& src);

}

#endif