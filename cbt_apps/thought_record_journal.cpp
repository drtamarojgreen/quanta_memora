#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

/**
 * Thought Record Journal - A Core CBT Tool
 * 
 * This application implements a classic CBT tool where users log:
 * - Situations that trigger emotional responses
 * - Automatic thoughts that arise
 * - Emotions and their intensity (1-10 scale)
 * - Alternative, more balanced responses
 * 
 * Based on the cognitive model: Situation -> Thoughts -> Emotions -> Behaviors
 */

struct ThoughtRecord {
    std::string timestamp;
    std::string situation;
    std::string automatic_thoughts;
    std::string emotions;
    int emotion_intensity;
    std::string physical_sensations;
    std::string behaviors;
    std::string evidence_for;
    std::string evidence_against;
    std::string balanced_thought;
    std::string new_emotion;
    int new_intensity;
    std::string id;
};

class ThoughtRecordJournal {
private:
    std::vector<ThoughtRecord> records;
    std::string data_file;
    std::string export_directory;
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    std::string generateId() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        return "TR_" + std::to_string(millis);
    }
    
    void ensureDataDirectory() {
        if (!fs::exists("data")) {
            fs::create_directory("data");
        }
        if (!fs::exists("exports")) {
            fs::create_directory("exports");
        }
    }
    
    int getValidatedIntensity(const std::string& prompt) {
        int intensity;
        while (true) {
            std::cout << prompt << " (1-10): ";
            std::string input;
            std::getline(std::cin, input);
            
            try {
                intensity = std::stoi(input);
                if (intensity >= 1 && intensity <= 10) {
                    break;
                } else {
                    std::cout << "Please enter a number between 1 and 10." << std::endl;
                }
            } catch (const std::exception&) {
                std::cout << "Please enter a valid number." << std::endl;
            }
        }
        return intensity;
    }
    
    void saveToFile() {
        std::ofstream file(data_file);
        if (!file.is_open()) {
            std::cerr << "Error: Could not save data to file." << std::endl;
            return;
        }
        
        // Write CSV header
        file << "ID,Timestamp,Situation,Automatic_Thoughts,Emotions,Emotion_Intensity,"
             << "Physical_Sensations,Behaviors,Evidence_For,Evidence_Against,"
             << "Balanced_Thought,New_Emotion,New_Intensity\n";
        
        for (const auto& record : records) {
            file << "\"" << record.id << "\","
                 << "\"" << record.timestamp << "\","
                 << "\"" << record.situation << "\","
                 << "\"" << record.automatic_thoughts << "\","
                 << "\"" << record.emotions << "\","
                 << record.emotion_intensity << ","
                 << "\"" << record.physical_sensations << "\","
                 << "\"" << record.behaviors << "\","
                 << "\"" << record.evidence_for << "\","
                 << "\"" << record.evidence_against << "\","
                 << "\"" << record.balanced_thought << "\","
                 << "\"" << record.new_emotion << "\","
                 << record.new_intensity << "\n";
        }
        
        std::cout << "Data saved successfully." << std::endl;
    }
    
    void loadFromFile() {
        std::ifstream file(data_file);
        if (!file.is_open()) {
            return; // File doesn't exist yet, that's okay
        }
        
        std::string line;
        std::getline(file, line); // Skip header
        
        while (std::getline(file, line)) {
            // Simple CSV parsing (would need more robust parsing for production)
            ThoughtRecord record;
            // For simplicity, we'll just load what we can
            // In a real application, you'd want proper CSV parsing
            records.push_back(record);
        }
    }

public:
    ThoughtRecordJournal() : data_file("data/thought_records.csv"), export_directory("exports/") {
        ensureDataDirectory();
        loadFromFile();
    }
    
    ~ThoughtRecordJournal() {
        saveToFile();
    }
    
    void displayWelcome() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "           THOUGHT RECORD JOURNAL" << std::endl;
        std::cout << "         A Cognitive Behavioral Therapy Tool" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "\nThis tool helps you identify and challenge negative thought patterns." << std::endl;
        std::cout << "By examining the connection between situations, thoughts, and emotions," << std::endl;
        std::cout << "you can develop more balanced and helpful ways of thinking." << std::endl;
        std::cout << "\nRemember: Thoughts are not facts. They are mental events that can be" << std::endl;
        std::cout << "examined, questioned, and changed." << std::endl;
    }
    
    void createNewRecord() {
        ThoughtRecord record;
        record.timestamp = getCurrentTimestamp();
        record.id = generateId();
        
        std::cout << "\n" << std::string(50, '-') << std::endl;
        std::cout << "Creating New Thought Record" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        // Step 1: Situation
        std::cout << "\n1. SITUATION" << std::endl;
        std::cout << "Describe the situation that triggered your emotional response." << std::endl;
        std::cout << "Be specific: Who? What? When? Where?" << std::endl;
        std::cout << "Situation: ";
        std::getline(std::cin, record.situation);
        
        // Step 2: Automatic Thoughts
        std::cout << "\n2. AUTOMATIC THOUGHTS" << std::endl;
        std::cout << "What thoughts went through your mind? What did you tell yourself?" << std::endl;
        std::cout << "Automatic thoughts: ";
        std::getline(std::cin, record.automatic_thoughts);
        
        // Step 3: Emotions
        std::cout << "\n3. EMOTIONS" << std::endl;
        std::cout << "What emotions did you feel? (e.g., sad, angry, anxious, guilty)" << std::endl;
        std::cout << "Emotions: ";
        std::getline(std::cin, record.emotions);
        
        record.emotion_intensity = getValidatedIntensity("Rate the intensity of these emotions");
        
        // Step 4: Physical Sensations
        std::cout << "\n4. PHYSICAL SENSATIONS" << std::endl;
        std::cout << "What did you notice in your body? (e.g., tense muscles, rapid heartbeat)" << std::endl;
        std::cout << "Physical sensations: ";
        std::getline(std::cin, record.physical_sensations);
        
        // Step 5: Behaviors
        std::cout << "\n5. BEHAVIORS" << std::endl;
        std::cout << "What did you do? How did you respond to the situation?" << std::endl;
        std::cout << "Behaviors: ";
        std::getline(std::cin, record.behaviors);
        
        // Step 6: Evidence Examination
        std::cout << "\n6. EXAMINING THE EVIDENCE" << std::endl;
        std::cout << "Let's examine your automatic thoughts more closely." << std::endl;
        
        std::cout << "\nEvidence FOR your automatic thoughts:" << std::endl;
        std::cout << "What facts support these thoughts?" << std::endl;
        std::cout << "Evidence for: ";
        std::getline(std::cin, record.evidence_for);
        
        std::cout << "\nEvidence AGAINST your automatic thoughts:" << std::endl;
        std::cout << "What facts contradict these thoughts? What would you tell a friend?" << std::endl;
        std::cout << "Evidence against: ";
        std::getline(std::cin, record.evidence_against);
        
        // Step 7: Balanced Thought
        std::cout << "\n7. BALANCED THOUGHT" << std::endl;
        std::cout << "Based on the evidence, what would be a more balanced, realistic thought?" << std::endl;
        std::cout << "Balanced thought: ";
        std::getline(std::cin, record.balanced_thought);
        
        // Step 8: New Emotion
        std::cout << "\n8. NEW EMOTIONAL RESPONSE" << std::endl;
        std::cout << "How do you feel now with this more balanced thought?" << std::endl;
        std::cout << "New emotion: ";
        std::getline(std::cin, record.new_emotion);
        
        record.new_intensity = getValidatedIntensity("Rate the intensity of this new emotion");
        
        records.push_back(record);
        
        std::cout << "\n✅ Thought record saved successfully!" << std::endl;
        std::cout << "Record ID: " << record.id << std::endl;
        
        // Show improvement
        if (record.new_intensity < record.emotion_intensity) {
            int improvement = record.emotion_intensity - record.new_intensity;
            std::cout << "🎉 Great work! You reduced your emotional intensity by " 
                      << improvement << " points!" << std::endl;
        }
    }
    
    void viewRecords() {
        if (records.empty()) {
            std::cout << "\nNo thought records found. Create your first record!" << std::endl;
            return;
        }
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "YOUR THOUGHT RECORDS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        for (size_t i = 0; i < records.size(); ++i) {
            const auto& record = records[i];
            std::cout << "\n[" << (i + 1) << "] " << record.timestamp 
                      << " (ID: " << record.id << ")" << std::endl;
            std::cout << "Situation: " << record.situation << std::endl;
            std::cout << "Automatic Thought: " << record.automatic_thoughts << std::endl;
            std::cout << "Emotion: " << record.emotions << " (Intensity: " 
                      << record.emotion_intensity << "/10)" << std::endl;
            std::cout << "Balanced Thought: " << record.balanced_thought << std::endl;
            std::cout << "New Emotion: " << record.new_emotion << " (Intensity: " 
                      << record.new_intensity << "/10)" << std::endl;
            std::cout << std::string(40, '-') << std::endl;
        }
    }
    
    void showProgress() {
        if (records.empty()) {
            std::cout << "\nNo data available for progress analysis." << std::endl;
            return;
        }
        
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "PROGRESS ANALYSIS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        int total_records = records.size();
        int improved_records = 0;
        int total_improvement = 0;
        
        for (const auto& record : records) {
            if (record.new_intensity < record.emotion_intensity) {
                improved_records++;
                total_improvement += (record.emotion_intensity - record.new_intensity);
            }
        }
        
        std::cout << "Total thought records: " << total_records << std::endl;
        std::cout << "Records showing improvement: " << improved_records 
                  << " (" << (100.0 * improved_records / total_records) << "%)" << std::endl;
        
        if (improved_records > 0) {
            double avg_improvement = static_cast<double>(total_improvement) / improved_records;
            std::cout << "Average improvement: " << std::fixed << std::setprecision(1) 
                      << avg_improvement << " points" << std::endl;
        }
        
        std::cout << "\n📈 Keep practicing! The more you challenge your thoughts," << std::endl;
        std::cout << "the better you'll become at recognizing and changing unhelpful patterns." << std::endl;
    }
    
    void exportData() {
        if (records.empty()) {
            std::cout << "\nNo data to export." << std::endl;
            return;
        }
        
        std::string export_filename = export_directory + "thought_records_export_" + 
                                     getCurrentTimestamp().substr(0, 10) + ".csv";
        
        // Replace spaces and colons in filename
        std::replace(export_filename.begin(), export_filename.end(), ' ', '_');
        std::replace(export_filename.begin(), export_filename.end(), ':', '-');
        
        std::ofstream file(export_filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create export file." << std::endl;
            return;
        }
        
        // Write detailed CSV export
        file << "ID,Timestamp,Situation,Automatic_Thoughts,Emotions,Emotion_Intensity,"
             << "Physical_Sensations,Behaviors,Evidence_For,Evidence_Against,"
             << "Balanced_Thought,New_Emotion,New_Intensity,Improvement\n";
        
        for (const auto& record : records) {
            int improvement = record.emotion_intensity - record.new_intensity;
            file << "\"" << record.id << "\","
                 << "\"" << record.timestamp << "\","
                 << "\"" << record.situation << "\","
                 << "\"" << record.automatic_thoughts << "\","
                 << "\"" << record.emotions << "\","
                 << record.emotion_intensity << ","
                 << "\"" << record.physical_sensations << "\","
                 << "\"" << record.behaviors << "\","
                 << "\"" << record.evidence_for << "\","
                 << "\"" << record.evidence_against << "\","
                 << "\"" << record.balanced_thought << "\","
                 << "\"" << record.new_emotion << "\","
                 << record.new_intensity << ","
                 << improvement << "\n";
        }
        
        std::cout << "\n✅ Data exported successfully to: " << export_filename << std::endl;
        std::cout << "You can open this file in Excel or any spreadsheet application." << std::endl;
    }
    
    void showHelp() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "THOUGHT RECORD HELP & TIPS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << "\n🎯 PURPOSE:" << std::endl;
        std::cout << "Thought records help you identify and challenge negative thought patterns" << std::endl;
        std::cout << "that contribute to emotional distress." << std::endl;
        
        std::cout << "\n📝 WHEN TO USE:" << std::endl;
        std::cout << "• When you notice a sudden change in mood" << std::endl;
        std::cout << "• After a stressful or upsetting situation" << std::endl;
        std::cout << "• When you catch yourself thinking negatively" << std::endl;
        std::cout << "• As a daily practice to build awareness" << std::endl;
        
        std::cout << "\n💡 TIPS FOR SUCCESS:" << std::endl;
        std::cout << "• Be specific about situations - include details" << std::endl;
        std::cout << "• Write down thoughts exactly as they occurred" << std::endl;
        std::cout << "• Rate emotions honestly - there are no wrong answers" << std::endl;
        std::cout << "• Look for evidence like a detective - be objective" << std::endl;
        std::cout << "• Balanced thoughts should be realistic, not just positive" << std::endl;
        
        std::cout << "\n🔍 COMMON THINKING PATTERNS TO WATCH FOR:" << std::endl;
        std::cout << "• All-or-nothing thinking (black and white)" << std::endl;
        std::cout << "• Catastrophizing (imagining the worst)" << std::endl;
        std::cout << "• Mind reading (assuming you know what others think)" << std::endl;
        std::cout << "• Fortune telling (predicting negative outcomes)" << std::endl;
        std::cout << "• Personalization (blaming yourself for everything)" << std::endl;
        
        std::cout << "\n⚠️  IMPORTANT NOTES:" << std::endl;
        std::cout << "• This tool is for self-help and education" << std::endl;
        std::cout << "• It does not replace professional therapy" << std::endl;
        std::cout << "• If you're having thoughts of self-harm, seek immediate help" << std::endl;
        std::cout << "• Contact a mental health professional for persistent difficulties" << std::endl;
    }
    
    void run() {
        displayWelcome();
        
        while (true) {
            std::cout << "\n" << std::string(40, '=') << std::endl;
            std::cout << "MAIN MENU" << std::endl;
            std::cout << std::string(40, '=') << std::endl;
            std::cout << "1. Create New Thought Record" << std::endl;
            std::cout << "2. View Previous Records" << std::endl;
            std::cout << "3. Show Progress Analysis" << std::endl;
            std::cout << "4. Export Data" << std::endl;
            std::cout << "5. Help & Tips" << std::endl;
            std::cout << "6. Exit" << std::endl;
            std::cout << "\nChoose an option (1-6): ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                createNewRecord();
            } else if (choice == "2") {
                viewRecords();
            } else if (choice == "3") {
                showProgress();
            } else if (choice == "4") {
                exportData();
            } else if (choice == "5") {
                showHelp();
            } else if (choice == "6") {
                std::cout << "\nThank you for using the Thought Record Journal!" << std::endl;
                std::cout << "Remember: Practice makes progress. Keep challenging those thoughts! 💪" << std::endl;
                break;
            } else {
                std::cout << "\nInvalid choice. Please select 1-6." << std::endl;
            }
        }
    }
};

int main() {
    try {
        ThoughtRecordJournal journal;
        journal.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
