#ifndef TEMPLATE_LOADER_H
#define TEMPLATE_LOADER_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

class TemplateLoader {
public:
    /**
     * @brief Constructs a TemplateLoader and loads templates from the given SQL file.
     * @param sql_file_path The path to the .sql file containing template data.
     */
    explicit TemplateLoader(const std::string& sql_file_path);

    /**
     * @brief Retrieves the raw content of a template.
     * @param key The key of the template (e.g., "LICENSE", "README.md").
     * @return The template content as a string.
     * @throws std::out_of_range if the key is not found.
     */
    std::string getTemplate(const std::string& key) const;

    /**
     * @brief Retrieves a template and performs placeholder substitution.
     * @param key The key of the template.
     * @param substitutions A map of placeholders (e.g., "{{author}}") to their values.
     * @return The template content with all placeholders replaced.
     */
    std::string getAndSubstitute(const std::string& key, const std::map<std::string, std::string>& substitutions) const;


private:
    std::map<std::string, std::string> templates_;

    /**
     * @brief Parses a simple SQL INSERT statement to extract file_path and content.
     * This is a very basic parser and not a full SQL parser.
     * It expects a specific format: INSERT INTO templates (..., file_path, ..., content) VALUES (..., 'path', ..., 'content');
     * @param sql_line A line containing a SQL INSERT statement.
     */
    void parseAndLoad(const std::string& sql_line);
};

#endif // TEMPLATE_LOADER_H
