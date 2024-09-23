#include <iostream>
#include <string>
#include <map>
#include <tinyxml2.h>

class LanguageManager {
public:
    LanguageManager(const std::string& languageFile) {
        loadLanguages(languageFile);
    }

    void setLanguage(const std::string& languageId) {
        currentLanguage = languageId;
    }

    std::string getText(const std::string& key) const {
        if (languages.find(currentLanguage) != languages.end() &&
            languages.at(currentLanguage).find(key) != languages.at(currentLanguage).end()) {
            return languages.at(currentLanguage).at(key);
        }
        return key; // Fallback: Return the key if no translation is found
    }

private:
    void loadLanguages(const std::string& languageFile) {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(languageFile.c_str()) != tinyxml2::XML_SUCCESS) {
            std::cerr << "Failed to load language file: " << languageFile << std::endl;
            return;
        }

        tinyxml2::XMLElement* root = doc.FirstChildElement("languages");
        if (!root) {
            std::cerr << "Invalid language file format." << std::endl;
            return;
        }

        for (tinyxml2::XMLElement* languageElem = root->FirstChildElement("language"); languageElem != nullptr; languageElem = languageElem->NextSiblingElement("language")) {
            std::string languageId = languageElem->Attribute("id");
            std::map<std::string, std::string> translations;

            for (tinyxml2::XMLElement* textElem = languageElem->FirstChildElement(); textElem != nullptr; textElem = textElem->NextSiblingElement()) {
                translations[textElem->Name()] = textElem->GetText();
            }

            languages[languageId] = translations;
        }
    }

    std::map<std::string, std::map<std::string, std::string>> languages;
    std::string currentLanguage = "en"; // Default language
};
