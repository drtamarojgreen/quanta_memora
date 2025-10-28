#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <stdexcept>
#include <array>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

// --- Helper Function to Execute Shell Commands ---
std::string executeCommand(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// --- Repository Data Structure ---
struct Repository {
    std::string name;
    std::string primary_language;
    std::string file_structure_summary;
    std::string documentation_summary;
    std::string last_update;
};

// --- Analysis Functions ---

// Analyzes the file list to determine primary languages
std::string analyzeLanguages(const std::string& file_list) {
    std::map<std::string, int> lang_counts;
    std::stringstream ss(file_list);
    std::string filename;

    while (std::getline(ss, filename, '\n')) {
        size_t dot_pos = filename.rfind('.');
        if (dot_pos != std::string::npos) {
            std::string ext = filename.substr(dot_pos);
            if (ext == ".cpp" || ext == ".h") lang_counts["C++"]++;
            else if (ext == ".py") lang_counts["Python"]++;
            else if (ext == ".js") lang_counts["JavaScript"]++;
            else if (ext == ".sh") lang_counts["Shell"]++;
            else if (ext == ".ps1") lang_counts["PowerShell"]++;
            else if (ext == ".r") lang_counts["R"]++;
        }
    }

    if (lang_counts.empty()) {
        return "N/A";
    }

    // Sort by count and return the top languages
    std::vector<std::pair<std::string, int>> sorted_langs(lang_counts.begin(), lang_counts.end());
    std::sort(sorted_langs.begin(), sorted_langs.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::string result;
    for (size_t i = 0; i < std::min(sorted_langs.size(), (size_t)2); ++i) {
        if (!result.empty()) result += ", ";
        result += sorted_langs[i].first;
    }
    return result;
}

// Analyzes the file list for common directories and documentation
void analyzeStructureAndDocs(const std::string& file_list, std::string& structure_summary, std::string& doc_summary) {
    std::set<std::string> top_level_dirs;
    bool has_readme = false;
    bool has_docs = false;
    std::stringstream ss(file_list);
    std::string filepath;

    while (std::getline(ss, filepath, '\n')) {
        if (filepath.rfind("README", 0) == 0) has_readme = true;

        size_t first_slash = filepath.find('/');
        if (first_slash != std::string::npos) {
            std::string dir = filepath.substr(0, first_slash);
            top_level_dirs.insert(dir);
        }
    }

    // Summarize Structure
    structure_summary = "Key dirs: ";
    if (top_level_dirs.empty()) {
        structure_summary = "Single directory project.";
    } else {
        for (const auto& dir : top_level_dirs) {
            if (dir == "src" || dir == "docs" || dir == "tests" || dir == "include" || dir == "apps") {
                structure_summary += dir + "/, ";
            }
        }
        if (structure_summary == "Key dirs: ") {
             structure_summary = "Custom structure.";
        } else {
            structure_summary.resize(structure_summary.size() - 2); // trim trailing ", "
        }
    }


    // Summarize Docs
    if (top_level_dirs.count("docs")) {
        doc_summary = "Has README and /docs directory.";
    } else if (has_readme) {
        doc_summary = "Has README.md.";
    } else {
        doc_summary = "No significant documentation found.";
    }
}


// --- Main Analysis Logic ---
int main() {
    // --- Configuration ---
    const std::string GITHUB_USER = "drtamarojgreen";
    const std::string TEMP_CLONE_DIR = "temp_repo_clones";
    const std::string OUTPUT_FILE = "docs/programmatic_project_comparison.md";

    const std::vector<std::string> repo_names = {
        "alignment_map", "greenhouse_org", "multiple_viewer", "prismquanta",
        "quanta_alarma", "quanta_cerebra", "quanta_cogno", "quanta_dorsa",
        "quanta_ethos", "quanta_glia", "quanta_haba", "quanta_lista",
        "quanta_memora", "quanta_occipita", "quanta_pie", "quanta_porto",
        "quanta_pulsa", "quanta_quilida", "quanta_retina", "quanta_sensa",
        "quanta_serene", "quanta_synapse", "quanta_tissu"
    };

    std::vector<Repository> analyzed_repos;

    // --- Step 1: Clone Repositories ---
    std::cout << "Creating temporary directory: " << TEMP_CLONE_DIR << std::endl;
    executeCommand(("mkdir " + TEMP_CLONE_DIR).c_str());

    for (const auto& name : repo_names) {
        std::cout << "Cloning " << name << "..." << std::endl;
        std::string clone_cmd = "git clone https://github.com/" + GITHUB_USER + "/" + name + ".git " + TEMP_CLONE_DIR + "/" + name;
        executeCommand(clone_cmd.c_str());
    }

    // --- Step 2: Analyze Each Repository ---
    std::cout << "\nAnalyzing repositories..." << std::endl;
    for (const auto& name : repo_names) {
        std::cout << "Analyzing " << name << "..." << std::endl;
        Repository repo;
        repo.name = name;
        std::string repo_path = TEMP_CLONE_DIR + "/" + name;

        // Get Last Update Date
        std::string date_cmd = "git -C " + repo_path + " log -1 --format=%cd --date=short";
        repo.last_update = executeCommand(date_cmd.c_str());
        repo.last_update.erase(std::remove(repo.last_update.begin(), repo.last_update.end(), '\n'), repo.last_update.end());

        // Get File List for Analysis
        std::string ls_tree_cmd = "git -C " + repo_path + " ls-tree -r --name-only HEAD";
        std::string file_list = executeCommand(ls_tree_cmd.c_str());

        // Run analysis functions
        repo.primary_language = analyzeLanguages(file_list);
        analyzeStructureAndDocs(file_list, repo.file_structure_summary, repo.documentation_summary);

        analyzed_repos.push_back(repo);
    }

    // --- Step 3: Generate Markdown Report ---
    std::cout << "\nGenerating Markdown report: " << OUTPUT_FILE << std::endl;
    std::ofstream report_file(OUTPUT_FILE);

    report_file << "# Programmatic Project Comparison Report\n\n";
    report_file << "This report was generated automatically.\n\n";
    report_file << "| Repository Name | Primary Language(s) | File Structure Summary | Documentation Summary | Last Update |\n";
    report_file << "|---|---|---|---|---|\n";

    for (const auto& repo : analyzed_repos) {
        report_file << "| `" << repo.name << "` "
                    << "| " << repo.primary_language << " "
                    << "| " << repo.file_structure_summary << " "
                    << "| " << repo.documentation_summary << " "
                    << "| " << repo.last_update << " |\n";
    }

    report_file.close();

    // --- Step 4: Clean Up ---
    std::cout << "\nCleaning up temporary files..." << std::endl;
    executeCommand(("rm -rf " + TEMP_CLONE_DIR).c_str());

    std::cout << "\nAnalysis complete. Report generated at " << OUTPUT_FILE << std::endl;

    return 0;
}
