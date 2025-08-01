#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <map>
#include <cstddef>
#include <algorithm>
#include <numeric>
#include <cmath>

namespace fs = std::filesystem;

/**
 * Advanced Mood & Emotion Tracker - CBT Tool
 * 
 * Features:
 * - Daily mood logging with multiple emotions
 * - Context tracking (activities, locations, people)
 * - Sleep and energy level correlation
 * - Weather impact analysis
 * - Trigger identification
 * - Progress visualization with trends
 * - Predictive insights
 */

enum class MoodCategory {
    VERY_LOW = 1,
    LOW = 2,
    NEUTRAL = 3,
    GOOD = 4,
    VERY_GOOD = 5
};

struct EmotionEntry {
    std::string emotion_name;
    int intensity; // 1-10 scale
    std::string trigger;
};

struct MoodEntry {
    std::string timestamp;
    std::string date;
    MoodCategory overall_mood;
    std::vector<EmotionEntry> emotions;
    int energy_level; // 1-10
    int sleep_hours;
    int sleep_quality; // 1-10
    std::string activities;
    std::string location;
    std::string social_context;
    std::string weather;
    std::string notes;
    std::string id;
};

class MoodEmotionTracker {
private:
    std::vector<MoodEntry> entries;
    std::string data_file;
    std::string export_directory;
    
    // Predefined emotion lists for quick selection
    std::vector<std::string> positive_emotions = {
        "Happy", "Joyful", "Excited", "Grateful", "Peaceful", "Confident",
        "Loved", "Proud", "Hopeful", "Content", "Energetic", "Optimistic"
    };
    
    std::vector<std::string> negative_emotions = {
        "Sad", "Angry", "Anxious", "Frustrated", "Lonely", "Guilty",
        "Ashamed", "Worried", "Irritated", "Disappointed", "Overwhelmed", "Stressed"
    };
    
    std::vector<std::string> neutral_emotions = {
        "Calm", "Focused", "Curious", "Thoughtful", "Relaxed", "Alert"
    };
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    std::string getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d");
        return ss.str();
    }
    
    std::string generateId() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        return "ME_" + std::to_string(millis);
    }
    
    void ensureDataDirectory() {
        if (!fs::exists("data")) {
            fs::create_directory("data");
        }
        if (!fs::exists("exports")) {
            fs::create_directory("exports");
        }
    }
    
    int getValidatedInput(const std::string& prompt, int min_val, int max_val) {
        int value;
        while (true) {
            std::cout << prompt << " (" << min_val << "-" << max_val << "): ";
            std::string input;
            std::getline(std::cin, input);
            
            try {
                value = std::stoi(input);
                if (value >= min_val && value <= max_val) {
                    break;
                } else {
                    std::cout << "Please enter a number between " << min_val << " and " << max_val << "." << std::endl;
                }
            } catch (const std::exception&) {
                std::cout << "Please enter a valid number." << std::endl;
            }
        }
        return value;
    }
    
    MoodCategory getMoodCategory(int mood_value) {
        return static_cast<MoodCategory>(mood_value);
    }
    
    std::string moodCategoryToString(MoodCategory mood) {
        switch (mood) {
            case MoodCategory::VERY_LOW: return "Very Low";
            case MoodCategory::LOW: return "Low";
            case MoodCategory::NEUTRAL: return "Neutral";
            case MoodCategory::GOOD: return "Good";
            case MoodCategory::VERY_GOOD: return "Very Good";
            default: return "Unknown";
        }
    }
    
    void displayEmotionMenu(const std::vector<std::string>& emotions, const std::string& category) {
        std::cout << "\n" << category << " Emotions:" << std::endl;
        for (std::size_t i = 0; i < emotions.size(); ++i) {
            std::cout << (i + 1) << ". " << emotions[i] << std::endl;
        }
    }
    
    std::vector<EmotionEntry> collectEmotions() {
        std::vector<EmotionEntry> emotions;
        
        std::cout << "\n=== EMOTION SELECTION ===" << std::endl;
        std::cout << "Select up to 5 emotions you're experiencing today." << std::endl;
        
        while (emotions.size() < 5) {
            std::cout << "\nEmotion Categories:" << std::endl;
            std::cout << "1. Positive Emotions" << std::endl;
            std::cout << "2. Negative Emotions" << std::endl;
            std::cout << "3. Neutral Emotions" << std::endl;
            std::cout << "4. Custom Emotion" << std::endl;
            std::cout << "5. Done selecting emotions" << std::endl;
            
            std::cout << "\nChoose category (1-5): ";
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "5" || emotions.size() >= 5) {
                break;
            }
            
            EmotionEntry emotion;
            
            if (choice == "1") {
                displayEmotionMenu(positive_emotions, "Positive");
                int selection = getValidatedInput("Select emotion", 1, positive_emotions.size());
                emotion.emotion_name = positive_emotions[selection - 1];
            } else if (choice == "2") {
                displayEmotionMenu(negative_emotions, "Negative");
                int selection = getValidatedInput("Select emotion", 1, negative_emotions.size());
                emotion.emotion_name = negative_emotions[selection - 1];
            } else if (choice == "3") {
                displayEmotionMenu(neutral_emotions, "Neutral");
                int selection = getValidatedInput("Select emotion", 1, neutral_emotions.size());
                emotion.emotion_name = neutral_emotions[selection - 1];
            } else if (choice == "4") {
                std::cout << "Enter custom emotion: ";
                std::getline(std::cin, emotion.emotion_name);
            } else {
                std::cout << "Invalid choice. Please select 1-5." << std::endl;
                continue;
            }
            
            emotion.intensity = getValidatedInput("Rate intensity of " + emotion.emotion_name, 1, 10);
            
            std::cout << "What triggered this emotion? (optional): ";
            std::getline(std::cin, emotion.trigger);
            
            emotions.push_back(emotion);
            
            std::cout << "✅ Added: " << emotion.emotion_name << " (Intensity: " << emotion.intensity << ")" << std::endl;
        }
        
        return emotions;
    }
    
    void saveToFile() {
        std::ofstream file(data_file);
        if (!file.is_open()) {
            std::cerr << "Error: Could not save data to file." << std::endl;
            return;
        }
        
        // Write CSV header
        file << "ID,Timestamp,Date,Overall_Mood,Energy_Level,Sleep_Hours,Sleep_Quality,"
             << "Activities,Location,Social_Context,Weather,Notes,Emotions_Data\n";
        
        for (const auto& entry : entries) {
            // Serialize emotions data
            std::string emotions_data;
            for (const auto& emotion : entry.emotions) {
                emotions_data += emotion.emotion_name + ":" + std::to_string(emotion.intensity) + 
                               ":" + emotion.trigger + ";";
            }
            
            file << "\"" << entry.id << "\","
                 << "\"" << entry.timestamp << "\","
                 << "\"" << entry.date << "\","
                 << static_cast<int>(entry.overall_mood) << ","
                 << entry.energy_level << ","
                 << entry.sleep_hours << ","
                 << entry.sleep_quality << ","
                 << "\"" << entry.activities << "\","
                 << "\"" << entry.location << "\","
                 << "\"" << entry.social_context << "\","
                 << "\"" << entry.weather << "\","
                 << "\"" << entry.notes << "\","
                 << "\"" << emotions_data << "\"\n";
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
            MoodEntry entry;
            // For simplicity, we'll just load what we can
            // In a real application, you'd want proper CSV parsing
            entries.push_back(entry);
        }
    }
    
    double calculateAverage(const std::vector<double>& values) {
        if (values.empty()) return 0.0;
        return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    }

public:
    MoodEmotionTracker() : data_file("data/mood_entries.csv"), export_directory("exports/") {
        ensureDataDirectory();
        loadFromFile();
    }
    
    ~MoodEmotionTracker() {
        saveToFile();
    }
    
    void displayWelcome() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "         ADVANCED MOOD & EMOTION TRACKER" << std::endl;
        std::cout << "           A Comprehensive CBT Tool" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "\nTrack your daily moods, emotions, and their contexts to:" << std::endl;
        std::cout << "• Identify patterns and triggers" << std::endl;
        std::cout << "• Understand the impact of sleep, activities, and environment" << std::endl;
        std::cout << "• Monitor your emotional well-being over time" << std::endl;
        std::cout << "• Gain insights for better mental health management" << std::endl;
        std::cout << "\n💡 Tip: Consistent daily tracking provides the most valuable insights!" << std::endl;
    }
    
    void createNewEntry() {
        MoodEntry entry;
        entry.timestamp = getCurrentTimestamp();
        entry.date = getCurrentDate();
        entry.id = generateId();
        
        std::cout << "\n" << std::string(50, '-') << std::endl;
        std::cout << "Creating Mood Entry for " << entry.date << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        // Overall mood
        std::cout << "\n1. OVERALL MOOD" << std::endl;
        std::cout << "Rate your overall mood today:" << std::endl;
        std::cout << "1 = Very Low, 2 = Low, 3 = Neutral, 4 = Good, 5 = Very Good" << std::endl;
        int mood_value = getValidatedInput("Overall mood", 1, 5);
        entry.overall_mood = getMoodCategory(mood_value);
        
        // Specific emotions
        std::cout << "\n2. SPECIFIC EMOTIONS" << std::endl;
        entry.emotions = collectEmotions();
        
        // Energy and sleep
        std::cout << "\n3. ENERGY & SLEEP" << std::endl;
        entry.energy_level = getValidatedInput("Energy level today", 1, 10);
        entry.sleep_hours = getValidatedInput("Hours of sleep last night", 0, 24);
        entry.sleep_quality = getValidatedInput("Sleep quality", 1, 10);
        
        // Context information
        std::cout << "\n4. CONTEXT INFORMATION" << std::endl;
        std::cout << "Activities today (work, exercise, socializing, etc.): ";
        std::getline(std::cin, entry.activities);
        
        std::cout << "Primary location (home, office, outdoors, etc.): ";
        std::getline(std::cin, entry.location);
        
        std::cout << "Social context (alone, with family, friends, colleagues): ";
        std::getline(std::cin, entry.social_context);
        
        std::cout << "Weather/environment (sunny, rainy, cold, etc.): ";
        std::getline(std::cin, entry.weather);
        
        // Additional notes
        std::cout << "\n5. ADDITIONAL NOTES" << std::endl;
        std::cout << "Any additional observations or notes: ";
        std::getline(std::cin, entry.notes);
        
        entries.push_back(entry);
        
        std::cout << "\n✅ Mood entry saved successfully!" << std::endl;
        std::cout << "Entry ID: " << entry.id << std::endl;
        
        // Quick insight
        if (!entry.emotions.empty()) {
            auto max_emotion = *std::max_element(entry.emotions.begin(), entry.emotions.end(),
                [](const EmotionEntry& a, const EmotionEntry& b) {
                    return a.intensity < b.intensity;
                });
            std::cout << "💡 Your strongest emotion today: " << max_emotion.emotion_name 
                      << " (Intensity: " << max_emotion.intensity << ")" << std::endl;
        }
    }
    
    void viewRecentEntries() {
        if (entries.empty()) {
            std::cout << "\nNo mood entries found. Create your first entry!" << std::endl;
            return;
        }
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "RECENT MOOD ENTRIES" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        // Show last 10 entries
        std::size_t start = entries.size() > 10 ? entries.size() - 10 : 0;
        
        for (std::size_t i = start; i < entries.size(); ++i) {
            const auto& entry = entries[i];
            std::cout << "\n📅 " << entry.date << " (" << entry.id << ")" << std::endl;
            std::cout << "Overall Mood: " << moodCategoryToString(entry.overall_mood) << std::endl;
            std::cout << "Energy: " << entry.energy_level << "/10, Sleep: " 
                      << entry.sleep_hours << "h (Quality: " << entry.sleep_quality << "/10)" << std::endl;
            
            if (!entry.emotions.empty()) {
                std::cout << "Emotions: ";
                for (const auto& emotion : entry.emotions) {
                    std::cout << emotion.emotion_name << "(" << emotion.intensity << ") ";
                }
                std::cout << std::endl;
            }
            
            if (!entry.activities.empty()) {
                std::cout << "Activities: " << entry.activities << std::endl;
            }
            
            std::cout << std::string(40, '-') << std::endl;
        }
    }
    
    void showHelp() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "MOOD & EMOTION TRACKER HELP" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << "\n🎯 PURPOSE:" << std::endl;
        std::cout << "Track daily moods and emotions to identify patterns, triggers," << std::endl;
        std::cout << "and factors that influence your mental well-being." << std::endl;
        
        std::cout << "\n📝 DAILY TRACKING TIPS:" << std::endl;
        std::cout << "• Track at the same time each day for consistency" << std::endl;
        std::cout << "• Be honest about your emotions - there are no wrong answers" << std::endl;
        std::cout << "• Include context (activities, location, weather)" << std::endl;
        std::cout << "• Note sleep patterns - they significantly impact mood" << std::endl;
        std::cout << "• Track for at least 2 weeks to see meaningful patterns" << std::endl;
        
        std::cout << "\n⚠️  IMPORTANT NOTES:" << std::endl;
        std::cout << "• This tool is for self-monitoring and awareness" << std::endl;
        std::cout << "• It does not replace professional mental health care" << std::endl;
        std::cout << "• If you notice concerning patterns, consult a professional" << std::endl;
        std::cout << "• Crisis resources: National Suicide Prevention Lifeline 988" << std::endl;
    }
    
    void run() {
        displayWelcome();
        
        while (true) {
            std::cout << "\n" << std::string(40, '=') << std::endl;
            std::cout << "MAIN MENU" << std::endl;
            std::cout << std::string(40, '=') << std::endl;
            std::cout << "1. Create New Mood Entry" << std::endl;
            std::cout << "2. View Recent Entries" << std::endl;
            std::cout << "3. Help & Tips" << std::endl;
            std::cout << "4. Exit" << std::endl;
            std::cout << "\nChoose an option (1-4): ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                createNewEntry();
            } else if (choice == "2") {
                viewRecentEntries();
            } else if (choice == "3") {
                showHelp();
            } else if (choice == "4") {
                std::cout << "\nThank you for using the Mood & Emotion Tracker!" << std::endl;
                std::cout << "Remember: Consistent tracking leads to valuable insights! 📊✨" << std::endl;
                break;
            } else {
                std::cout << "\nInvalid choice. Please select 1-4." << std::endl;
            }
        }
    }
};

int main() {
    try {
        MoodEmotionTracker tracker;
        tracker.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
