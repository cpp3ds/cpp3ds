#ifndef CPP3DS_I18N_HPP
#define CPP3DS_I18N_HPP

#ifndef EMULATION
#include <3ds.h>
#endif
#include <iostream>
#include <stdio.h>
#include <map>
#include <cstring>
#include <memory>
#include <cpp3ds/System/String.hpp>
#include <fmt/format.h>

#define _(key, ...) (cpp3ds::I18n::getInstance().translate(key, ##__VA_ARGS__))


namespace cpp3ds {

enum Language {
	Japanese    = 0,
	English     = 1,
	French,
	German,
	Italian,
	Spanish,
	ChineseSimplified,
	Korean,
	Dutch,
	Portuguese,
	Russian,
	ChineseTraditional,

	COUNT,
};

class I18n {
public:

	static I18n& getInstance();

	static void loadLanguage(Language language);
	static void loadLanguageFile(const std::string& filename);
	static void clearLoadedLanguage();
	static Language getLanguage();

	template<typename ... Args>
	const String translate(const char* key, Args ... args) const {
		std::string trans;
		TranslationMap::const_iterator it = m_content.find(key);
		if (it == m_content.end())
			trans = std::string(key);
		else
			trans = it->second;
		return fmt::sprintf(trans, args ...);
	}

	template<typename ... Args>
	const String translate(const std::string& key, Args ... args) const {
		return translate(key.c_str(), args ...);
	}

	const std::string getLangString(const Language language) const;

private:
	I18n();

	I18n(const I18n &);

	bool loadFromFile(const std::string filename);

	bool loadFromLanguage(const Language language);

	typedef std::map <std::string, std::string> TranslationMap;
	TranslationMap m_content;
	Language m_language;
};

} // namespace cpp3ds

#endif // CPP3DS_I18N_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::I18n
/// \ingroup system
////////////////////////////////////////////////////////////