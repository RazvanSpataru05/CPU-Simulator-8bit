#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <functional>

#include "InstructionDef.h"
#include "ISAEntry.h"
#include "Token.h"

using Handler = std::function<void()>;

class Lexer
{
public:
	Lexer(const std::string& sourceCode);

	void Tokenize();

private:
	void ConsumeWord();
	void ConsumeNumber();
	void ConsumeNewLine();
	void ConsumeComment();

	Token BuildToken(const std::string& word);
	Token BuildNumericToken(const std::string& number, uint8_t prefix);

private:
	std::string m_sourceCode;
	uint32_t m_lineNumber;
	unsigned m_currentIndex;

	std::vector<Token> m_tokens;

	const std::vector<std::pair<std::function<bool(char)>, Handler>> m_handlers = {
		{[](char c) { return isalpha(c); },[this] {ConsumeWord(); }},
		{[](char c) {return isdigit(c); }, [this] {ConsumeNumber(); }},
		{[](char c) {return c == '\n'; }, [this] {ConsumeNewLine(); }},
		{[](char c) {return c == ';'; }, [this] {ConsumeComment(); }},
		{[](char c) {return c == ' ' || c == '\t'; }, [this] {m_currentIndex++; }}
	};
};

