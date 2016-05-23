#include <clocale>
#include <cstring>
#include <fstream>

#include <cpp3ds/System/Err.hpp>
#include <cpp3ds/System/I18n.hpp>
#include <cpp3ds/System/String.hpp>
#include <cpp3ds/System/Service.hpp>

#define TOKEN_COMMENT  '#'


namespace {

static bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if(from.empty())
		return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

} // namespace

namespace cpp3ds {


I18n& I18n::getInstance()
{
	static I18n self;
	return self;
}


I18n::I18n()
{
	Uint8 langcode;
	Int32 ret;
	Service::enable(Config);
#ifdef EMULATION
	langcode = 1; // TODO: get actual locale of PC
#else
	ret = CFGU_GetSystemLanguage(&langcode);
	if (!ret) {
		// If the syscall fails, as it does with Citra-emu, default to English
		langcode = 1;
	}
#endif
	m_language = static_cast<Language>(langcode);
	loadFromLanguage(m_language);
}


const std::string I18n::getLangString(const Language langcode) const
{
	switch (langcode) {
		case Japanese:           return "jp";
		case English:            return "en";
		case French:             return "fr";
		case German:             return "de";
		case Italian:            return "it";
		case Spanish:            return "es";
		case ChineseSimplified:  return "zh";
		case Korean:             return "ko";
		case Dutch:              return "nl";
		case Portuguese:         return "pt";
		case Russian:            return "ru";
		case ChineseTraditional: return "tw";
		default:                 return "en";
	}
}


void I18n::loadLanguage(Language language)
{
	getInstance().loadFromLanguage(language);
}


Language I18n::getLanguage()
{
	return static_cast<Language>(getInstance().m_language);
}


bool I18n::loadFromLanguage(const Language language)
{
	std::string filename = "lang/" + getLangString(language) + ".lang";
	return loadFromFile(filename);
}


void I18n::loadLanguageFile(const std::string& filename)
{
	getInstance().loadFromFile(filename);
}


bool I18n::loadFromFile(const std::string filename)
{
	std::ifstream file(filename);
	if (file)
	{
		m_content.clear();
		std::string line;
		std::string content;
		const std::map<std::string, std::string> replaceList = {
				{"\\n", "\n"}, {"\\'", "\'"}
		};

		while (std::getline(file, line)) {
			if (line.empty() || line[0] == TOKEN_COMMENT)
				continue;

			std::string key = line;
			while (std::getline(file, content)) {
				if (!content.empty() && content[0] != TOKEN_COMMENT)
					break;
			}

			replaceAll(key, "\\\\", "\\");
			replaceAll(content, "\\\\", "\\");

			for (auto& s : replaceList) {
				replaceAll(key, s.first, s.second);
				replaceAll(content, s.first, s.second);
			}

			m_content[key] = content;
		}
		return true;

	} else {
		err() << "Failed to load language file: " << filename << std::endl;
	}
	return false;
}

}
