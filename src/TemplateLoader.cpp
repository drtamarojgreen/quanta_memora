#include "TemplateLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

TemplateLoader::TemplateLoader(const std::string& sql_file_path) {
    std::ifstream file(sql_file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open SQL file: " + sql_file_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Simple check to see if the line is an INSERT statement
        if (line.find("INSERT INTO templates") != std::string::npos) {
            parseAndLoad(line);
        }
    }
}

std::string TemplateLoader::getTemplate(const std::string& key) const {
    try {
        return templates_.at(key);
    } catch (const std::out_of_range&) {
        throw std::out_of_range("Template with key '" + key + "' not found.");
    }
}

std::string TemplateLoader::getAndSubstitute(const std::string& key, const std::map<std::string, std::string>& substitutions) const {
    std::string content = getTemplate(key);
    for (const auto& pair : substitutions) {
        size_t pos = content.find(pair.first);
        while (pos != std::string::npos) {
            content.replace(pos, pair.first.length(), pair.second);
            pos = content.find(pair.first, pos + pair.second.length());
        }
    }
    return content;
}

void TemplateLoader::parseAndLoad(const std::string& sql_line) {
    // This is a fragile parser. It assumes a very specific format.
    // e.g., INSERT ... VALUES (..., 'file_path', ..., 'content');

    // Find the VALUES clause
    size_t values_pos = sql_line.find("VALUES");
    if (values_pos == std::string::npos) {
        return; // Not a valid insert statement for our purpose
    }

    // Find the opening and closing parentheses of the values list
    size_t open_paren = sql_line.find('(', values_pos);
    size_t close_paren = sql_line.rfind(')');
    if (open_paren == std::string::npos || close_paren == std::string::npos) {
        return;
    }

    std::string values_str = sql_line.substr(open_paren + 1, close_paren - open_paren - 1);

    // We expect 3 values: 'project_name', 'file_path', 'content'
    // Let's find the single quotes to extract the values
    std::vector<std::string> parsed_values;
    std::stringstream ss(values_str);
    std::string item;
    char delimiter = '\'';

    // This logic is simplistic. It finds content between single quotes.
    // It will break if there are escaped quotes inside the content, but for this
    // specific format, we assume the content is clean after C++ escaping.
    // A robust solution would need a proper SQL parsing library.

    // A better simple approach: find the single quotes manually
    size_t current_pos = 0;
    while(parsed_values.size() < 3) {
        size_t start_quote = values_str.find(delimiter, current_pos);
        if (start_quote == std::string::npos) break;
        size_t end_quote = values_str.find(delimiter, start_quote + 1);
        if (end_quote == std::string::npos) break;

        // Handle escaped quotes ''
        while (values_str[end_quote + 1] == '\'') {
            end_quote = values_str.find(delimiter, end_quote + 2);
            if (end_quote == std::string::npos) break;
        }
        if (end_quote == std::string::npos) break;

        std::string value = values_str.substr(start_quote + 1, end_quote - start_quote - 1);
        parsed_values.push_back(value);
        current_pos = end_quote + 1;
    }


    if (parsed_values.size() == 3) {
        std::string project_name = parsed_values[0]; // Not used yet, but parsed
        std::string file_path = parsed_values[1];
        std::string content = parsed_values[2];

        // Un-escape SQL's double single-quotes back to a single quote
        size_t pos = content.find("''");
        while (pos != std::string::npos) {
            content.replace(pos, 2, "'");
            pos = content.find("''", pos + 1);
        }

        templates_[file_path] = content;
        std::cout << "Loaded template: " << file_path << std::endl;
    } else {
        std::cerr << "Warning: Could not parse SQL line: " << sql_line << std::endl;
    }
}
