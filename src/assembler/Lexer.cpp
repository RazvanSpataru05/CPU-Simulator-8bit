#include "Lexer.h"

Lexer::Lexer(const std::string& sourceCode) :
	m_sourceCode{ sourceCode },
	m_lineNumber{ 0u },
	m_currentIndex{ 0u }
{
}

void Lexer::Tokenize()
{
	if (m_sourceCode.empty()) return;

	while (m_currentIndex < m_sourceCode.size())
	{
		const char current = m_sourceCode[m_currentIndex];

		auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [current](const auto& pair)
			{return pair.first(current); }
		);

		if (it != m_handlers.end())
		{
			it->second();
		}
		else
		{
			// TO-DO: Handle Lexical Errors 
		}
	}

	m_tokens.push_back(Token{
		TokenType::END,
		"HLT",
		m_lineNumber });
}

void Lexer::ConsumeWord()
{
	std::string word;
	while (m_currentIndex < m_sourceCode.size() &&
		(isalnum(m_sourceCode[m_currentIndex]) || m_sourceCode[m_currentIndex] == '_'))
	{
		word += m_sourceCode[m_currentIndex];
		m_currentIndex++;
	}
	Token token = BuildToken(word);
	m_tokens.emplace_back(token);
}

void Lexer::ConsumeNumber()
{
	std::string number;
	std::string prefix;
	uint8_t numberPrefix = 10u;
	while (m_currentIndex < m_sourceCode.size() &&
		(m_sourceCode[m_currentIndex] == '0' || m_sourceCode[m_currentIndex] == 'b' || m_sourceCode[m_currentIndex] == 'x'))
	{
		prefix += m_sourceCode[m_currentIndex];
		m_currentIndex++;
	}
	switch (prefix.size())
	{
	case 0:
		break;

	case 1:
		number = std::move(prefix);
		break;

	case 2:
		if (prefix[1] == 'b') numberPrefix = 2;
		else if (prefix[1] == 'x') numberPrefix = 16;
		number = std::move(prefix);
		break;
		
	default:
		// TO-DO HANDLE LEXICAL ERROR (multiple b's or x's);
		break;
	}

	auto isValidDigit = [](char c, uint8_t base) -> bool
		{
			if (base == 2) return c == '0' || c == '1';
			if (base == 16) return isxdigit(c);
			return isdigit(c);
		};

	while (m_currentIndex < m_sourceCode.size() && isValidDigit(m_sourceCode[m_currentIndex], numberPrefix))
	{
		number += m_sourceCode[m_currentIndex];
		m_currentIndex++;
	}
	Token token = BuildNumericToken(number, numberPrefix);
	m_tokens.emplace_back(token);
}

void Lexer::ConsumeNewLine()
{
	Token token = Token(
		TokenType::NEWLINE,
		"\n",
		m_lineNumber
	);
	m_tokens.emplace_back(token);
	m_lineNumber++;
	m_currentIndex++;
}

void Lexer::ConsumeComment()
{
	while (m_currentIndex < m_sourceCode.size() && m_sourceCode[m_currentIndex] != '\n')
	{
		m_currentIndex++;
	}
}

Token Lexer::BuildToken(const std::string& word)
{
	Token token;
	token.line = m_lineNumber;
	token.value = word;
	
	auto it = nameToSelector.find(word);
	if (it != nameToSelector.end())
	{
		token.type = TokenType::REGISTER;
		return token;
	}

	if (IsMnemonic(word))
	{
		token.type = TokenType::MNEMONIC;
		return token;
	}

	if (m_sourceCode[m_currentIndex] == ':')
	{
		token.type = TokenType::LABEL_DEF;
		m_currentIndex++;
		return token;
	}
	token.type = TokenType::LABEL_REF;
	return token;
}

Token Lexer::BuildNumericToken(const std::string& number, uint8_t prefix)
{
	Token token;
	token.line = m_lineNumber;
	token.value = std::to_string(std::stoi(number, nullptr, prefix));
	token.type = TokenType::NUMBER;

	return token;
}
