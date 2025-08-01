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
#include <random>

namespace fs = std::filesystem;

/**
 * Cognitive Distortion Identifier - Advanced CBT Tool
 * 
 * This application helps users identify and challenge cognitive distortions
 * (unhelpful thinking patterns) in their thoughts. It includes:
 * - Interactive distortion identification
 * - Educational content about each distortion
 * - Thought challenging exercises
 * - Progress tracking and pattern recognition
 * - Personalized insights and recommendations
 */

struct CognitiveDistortion {
    std::string name;
    std::string description;
    std::string example;
    std::vector<std::string> challenge_questions;
    std::vector<std::string> alternative_thoughts;
};

struct ThoughtAnalysis {
    std::string timestamp;
    std::string original_thought;
    std::vector<std::string> identified_distortions;
    std::string challenged_thought;
    int distress_before;
    int distress_after;
    std::string situation;
    std::string emotion;
    std::string id;
};

class CognitiveDistortionIdentifier {
private:
    std::vector<ThoughtAnalysis> analyses;
    std::map<std::string, CognitiveDistortion> distortions;
    std::string data_file;
    std::string export_directory;
    
    void initializeDistortions() {
        // All-or-Nothing Thinking
        distortions["all_or_nothing"] = {
            "All-or-Nothing Thinking",
            "Seeing things in black and white categories. If your performance falls short of perfect, you see yourself as a total failure.",
            "\"I made one mistake in my presentation, so I'm a terrible speaker.\"",
            {
                "Are there any gray areas or middle ground in this situation?",
                "What would you tell a friend who had this thought?",
                "Is this really an all-or-nothing situation?",
                "What evidence contradicts this extreme view?"
            },
            {
                "I made a mistake, but overall my presentation went well.",
                "Nobody is perfect, and one mistake doesn't define my abilities.",
                "I can learn from this mistake and improve next time."
            }
        };
        
        // Overgeneralization
        distortions["overgeneralization"] = {
            "Overgeneralization",
            "Seeing a single negative event as a never-ending pattern of defeat.",
            "\"I didn't get this job, so I'll never find employment.\"",
            {
                "Is this really always true, or just sometimes?",
                "What are some exceptions to this pattern?",
                "How many times has this actually happened?",
                "What evidence supports a more balanced view?"
            },
            {
                "This particular job wasn't the right fit, but there are other opportunities.",
                "One rejection doesn't predict all future outcomes.",
                "I can learn from this experience and improve my approach."
            }
        };
        
        // Mental Filter
        distortions["mental_filter"] = {
            "Mental Filter",
            "Picking out a single negative detail and dwelling on it exclusively so that your vision of all reality becomes darkened.",
            "\"My boss gave me mostly positive feedback, but mentioned one area for improvement. I'm doing terribly at work.\"",
            {
                "What positive aspects am I ignoring?",
                "Am I focusing only on the negative details?",
                "What would the complete picture look like?",
                "How much weight should this one detail really have?"
            },
            {
                "My boss gave me mostly positive feedback with one constructive suggestion.",
                "I can appreciate the positive feedback while working on the area for improvement.",
                "One area for growth doesn't negate all the positive aspects."
            }
        };
        
        // Disqualifying the Positive
        distortions["disqualifying_positive"] = {
            "Disqualifying the Positive",
            "Rejecting positive experiences by insisting they 'don't count' for some reason or other.",
            "\"My friends only invited me because they felt sorry for me.\"",
            {
                "Why am I dismissing this positive experience?",
                "What evidence do I have that this doesn't count?",
                "How would I interpret this if it happened to someone else?",
                "What if I accepted this positive experience at face value?"
            },
            {
                "My friends invited me because they enjoy my company.",
                "I deserve positive experiences and genuine friendships.",
                "I can accept compliments and positive gestures without questioning motives."
            }
        };
        
        // Jumping to Conclusions
        distortions["jumping_conclusions"] = {
            "Jumping to Conclusions",
            "Making negative interpretations even though there are no definite facts that convincingly support your conclusion.",
            "\"My friend didn't text me back immediately, so they must be angry with me.\"",
            {
                "What other explanations could there be?",
                "What facts do I actually have?",
                "Am I mind reading or fortune telling?",
                "What would I need to know to be certain?"
            },
            {
                "My friend might be busy, or their phone might be dead.",
                "There are many reasons why someone might not respond immediately.",
                "I can ask directly if I'm concerned, rather than assuming."
            }
        };
        
        // Magnification/Minimization
        distortions["magnification"] = {
            "Magnification (Catastrophizing) or Minimization",
            "Exaggerating the importance of things (such as your mistakes or someone else's achievement), or inappropriately shrinking things until they appear tiny.",
            "\"I forgot to call my mom back - I'm the worst daughter ever!\" or \"I got promoted, but it's no big deal.\"",
            {
                "Am I blowing this out of proportion?",
                "How important will this be in a week, month, or year?",
                "Am I minimizing something positive about myself?",
                "What's a more realistic perspective?"
            },
            {
                "I forgot to call my mom, but I can call her now and apologize.",
                "This mistake doesn't define me as a daughter.",
                "My promotion is an achievement I can be proud of."
            }
        };
        
        // Emotional Reasoning
        distortions["emotional_reasoning"] = {
            "Emotional Reasoning",
            "Assuming that your negative emotions necessarily reflect the way things really are: 'I feel it, therefore it must be true.'",
            "\"I feel guilty, so I must have done something wrong.\"",
            {
                "Are my feelings based on facts or assumptions?",
                "What evidence supports or contradicts this feeling?",
                "Could there be other reasons I feel this way?",
                "What would the facts say, regardless of how I feel?"
            },
            {
                "Feelings are valid but don't always reflect reality.",
                "I can feel guilty without having actually done something wrong.",
                "I can examine the facts separately from my emotions."
            }
        };
        
        // Should Statements
        distortions["should_statements"] = {
            "Should Statements",
            "Trying to motivate yourself with shoulds and shouldn'ts, as if you had to be whipped and punished before you could be expected to do anything.",
            "\"I should be able to handle this without getting stressed.\"",
            {
                "Who says I 'should' do this?",
                "What would happen if I replaced 'should' with 'could' or 'prefer'?",
                "Am I being realistic about human limitations?",
                "What would I tell a friend in this situation?"
            },
            {
                "It's normal to feel stressed in challenging situations.",
                "I prefer to handle things calmly, but it's okay if I sometimes feel overwhelmed.",
                "I can work on managing stress without demanding perfection from myself."
            }
        };
        
        // Labeling and Mislabeling
        distortions["labeling"] = {
            "Labeling and Mislabeling",
            "An extreme form of overgeneralization. Instead of describing your error, you attach a negative label to yourself or others.",
            "\"I made a mistake. I'm such an idiot.\"",
            {
                "Am I using harsh labels instead of describing specific behaviors?",
                "Would I call a friend this name for the same mistake?",
                "What's the difference between what I did and who I am?",
                "How can I describe this more accurately and kindly?"
            },
            {
                "I made a mistake, but that doesn't make me an idiot.",
                "Everyone makes mistakes - it's part of being human.",
                "I can learn from this error without attacking my character."
            }
        };
        
        // Personalization
        distortions["personalization"] = {
            "Personalization",
            "Seeing yourself as the cause of some negative external event which in fact you were not primarily responsible for.",
            "\"My team lost the game because I missed that one shot.\"",
            {
                "What other factors contributed to this outcome?",
                "Am I taking responsibility for things outside my control?",
                "What percentage of this outcome was actually due to my actions?",
                "How would I assign responsibility if I were an objective observer?"
            },
            {
                "The team's performance depends on many factors, not just my actions.",
                "I played my part, but the outcome wasn't solely my responsibility.",
                "I can take responsibility for my actions without blaming myself for everything."
            }
        };
    }
    
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
        return "CD_" + std::to_string(millis);
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
    
    void saveToFile() {
        std::ofstream file(data_file);
        if (!file.is_open()) {
            std::cerr << "Error: Could not save data to file." << std::endl;
            return;
        }
        
        // Write CSV header
        file << "ID,Timestamp,Original_Thought,Identified_Distortions,Challenged_Thought,"
             << "Distress_Before,Distress_After,Situation,Emotion\n";
        
        for (const auto& analysis : analyses) {
            // Serialize distortions
            std::string distortions_str;
            for (const auto& distortion : analysis.identified_distortions) {
                distortions_str += distortion + ";";
            }
            
            file << "\"" << analysis.id << "\","
                 << "\"" << analysis.timestamp << "\","
                 << "\"" << analysis.original_thought << "\","
                 << "\"" << distortions_str << "\","
                 << "\"" << analysis.challenged_thought << "\","
                 << analysis.distress_before << ","
                 << analysis.distress_after << ","
                 << "\"" << analysis.situation << "\","
                 << "\"" << analysis.emotion << "\"\n";
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
            ThoughtAnalysis analysis;
            // For simplicity, we'll just load what we can
            // In a real application, you'd want proper CSV parsing
            analyses.push_back(analysis);
        }
    }

public:
    CognitiveDistortionIdentifier() : data_file("data/distortion_analyses.csv"), export_directory("exports/") {
        ensureDataDirectory();
        initializeDistortions();
        loadFromFile();
    }
    
    ~CognitiveDistortionIdentifier() {
        saveToFile();
    }
    
    void displayWelcome() {
        std::cout << "\n" << std::string(70, '=') << std::endl;
        std::cout << "           COGNITIVE DISTORTION IDENTIFIER" << std::endl;
        std::cout << "              Advanced CBT Analysis Tool" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        std::cout << "\nThis tool helps you identify and challenge unhelpful thinking patterns" << std::endl;
        std::cout << "called cognitive distortions. By recognizing these patterns, you can:" << std::endl;
        std::cout << "• Develop more balanced and realistic thinking" << std::endl;
        std::cout << "• Reduce emotional distress" << std::endl;
        std::cout << "• Improve problem-solving abilities" << std::endl;
        std::cout << "• Build resilience and emotional well-being" << std::endl;
        std::cout << "\n💡 Remember: The goal isn't to think positively all the time," << std::endl;
        std::cout << "but to think more accurately and helpfully!" << std::endl;
    }
    
    void learnAboutDistortions() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "COGNITIVE DISTORTIONS REFERENCE GUIDE" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        int count = 1;
        for (const auto& pair : distortions) {
            const auto& distortion = pair.second;
            std::cout << "\n" << count << ". " << distortion.name << std::endl;
            std::cout << std::string(distortion.name.length() + 4, '-') << std::endl;
            std::cout << "Description: " << distortion.description << std::endl;
            std::cout << "Example: " << distortion.example << std::endl;
            std::cout << std::endl;
            count++;
        }
        
        std::cout << "💡 TIP: Keep this list handy when analyzing your thoughts!" << std::endl;
        std::cout << "The more familiar you become with these patterns, the easier" << std::endl;
        std::cout << "it becomes to spot them in your own thinking." << std::endl;
    }
    
    void analyzeThought() {
        ThoughtAnalysis analysis;
        analysis.timestamp = getCurrentTimestamp();
        analysis.id = generateId();
        
        std::cout << "\n" << std::string(60, '-') << std::endl;
        std::cout << "THOUGHT ANALYSIS SESSION" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        // Get the thought and context
        std::cout << "\n1. THOUGHT & CONTEXT" << std::endl;
        std::cout << "What thought would you like to analyze?" << std::endl;
        std::cout << "Thought: ";
        std::getline(std::cin, analysis.original_thought);
        
        std::cout << "\nWhat situation triggered this thought?" << std::endl;
        std::cout << "Situation: ";
        std::getline(std::cin, analysis.situation);
        
        std::cout << "\nWhat emotion are you feeling?" << std::endl;
        std::cout << "Emotion: ";
        std::getline(std::cin, analysis.emotion);
        
        analysis.distress_before = getValidatedInput("Rate your distress level before analysis", 1, 10);
        
        // Identify distortions
        std::cout << "\n2. DISTORTION IDENTIFICATION" << std::endl;
        std::cout << "Let's examine your thought for cognitive distortions." << std::endl;
        std::cout << "Your thought: \"" << analysis.original_thought << "\"" << std::endl;
        
        identifyDistortions(analysis);
        
        // Challenge the thought
        std::cout << "\n3. THOUGHT CHALLENGING" << std::endl;
        challengeThought(analysis);
        
        analysis.distress_after = getValidatedInput("Rate your distress level after analysis", 1, 10);
        
        analyses.push_back(analysis);
        
        // Show results
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "ANALYSIS COMPLETE" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        std::cout << "Original thought: \"" << analysis.original_thought << "\"" << std::endl;
        std::cout << "Challenged thought: \"" << analysis.challenged_thought << "\"" << std::endl;
        std::cout << "Distress reduction: " << (analysis.distress_before - analysis.distress_after) << " points" << std::endl;
        
        if (analysis.distress_after < analysis.distress_before) {
            std::cout << "🎉 Great work! You've successfully reduced your distress by challenging your thought!" << std::endl;
        } else if (analysis.distress_after == analysis.distress_before) {
            std::cout << "💭 Sometimes it takes time for new thoughts to feel natural. Keep practicing!" << std::endl;
        }
        
        std::cout << "Analysis ID: " << analysis.id << std::endl;
    }
    
    void identifyDistortions(ThoughtAnalysis& analysis) {
        std::cout << "\nI'll ask you about each type of cognitive distortion." << std::endl;
        std::cout << "Answer 'y' if you think your thought contains this distortion, 'n' if not." << std::endl;
        
        for (const auto& pair : distortions) {
            const auto& key = pair.first;
            const auto& distortion = pair.second;
            
            std::cout << "\n" << std::string(50, '-') << std::endl;
            std::cout << distortion.name << std::endl;
            std::cout << distortion.description << std::endl;
            std::cout << "Example: " << distortion.example << std::endl;
            std::cout << "\nYour thought: \"" << analysis.original_thought << "\"" << std::endl;
            
            std::cout << "\nDoes your thought contain " << distortion.name << "? (y/n): ";
            std::string response;
            std::getline(std::cin, response);
            
            if (response == "y" || response == "Y" || response == "yes") {
                analysis.identified_distortions.push_back(distortion.name);
                std::cout << "✅ Added: " << distortion.name << std::endl;
            }
        }
        
        if (analysis.identified_distortions.empty()) {
            std::cout << "\n🎯 Great! You didn't identify any cognitive distortions in this thought." << std::endl;
            std::cout << "This suggests your thinking is already quite balanced!" << std::endl;
        } else {
            std::cout << "\n📋 Identified distortions:" << std::endl;
            for (const auto& distortion : analysis.identified_distortions) {
                std::cout << "• " << distortion << std::endl;
            }
        }
    }
    
    void challengeThought(ThoughtAnalysis& analysis) {
        if (analysis.identified_distortions.empty()) {
            std::cout << "Since no distortions were identified, let's still explore if there's" << std::endl;
            std::cout << "a more helpful way to think about this situation." << std::endl;
        } else {
            std::cout << "Now let's challenge the distortions we identified." << std::endl;
        }
        
        // Collect challenge questions from identified distortions
        std::vector<std::string> all_questions;
        std::vector<std::string> all_alternatives;
        
        for (const auto& distortion_name : analysis.identified_distortions) {
            for (const auto& pair : distortions) {
                if (pair.second.name == distortion_name) {
                    const auto& distortion = pair.second;
                    all_questions.insert(all_questions.end(), 
                                       distortion.challenge_questions.begin(), 
                                       distortion.challenge_questions.end());
                    all_alternatives.insert(all_alternatives.end(),
                                          distortion.alternative_thoughts.begin(),
                                          distortion.alternative_thoughts.end());
                    break;
                }
            }
        }
        
        // If no distortions, use general questions
        if (all_questions.empty()) {
            all_questions = {
                "What evidence supports this thought?",
                "What evidence contradicts this thought?",
                "What would you tell a friend in this situation?",
                "How might you think about this differently?"
            };
        }
        
        // Ask challenge questions
        std::cout << "\nLet's explore some challenging questions:" << std::endl;
        
        // Randomly select 3-4 questions to avoid overwhelming the user
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(all_questions.begin(), all_questions.end(), gen);
        
        int num_questions = std::min(4, static_cast<int>(all_questions.size()));
        for (int i = 0; i < num_questions; ++i) {
            std::cout << "\n🤔 " << all_questions[i] << std::endl;
            std::cout << "Your response: ";
            std::string response;
            std::getline(std::cin, response);
            // We could store these responses for more detailed analysis
        }
        
        // Show alternative thoughts if available
        if (!all_alternatives.empty()) {
            std::cout << "\n💡 Here are some alternative ways to think about this:" << std::endl;
            for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(3), all_alternatives.size()); ++i) {
                std::cout << "• " << all_alternatives[i] << std::endl;
            }
        }
        
        // Get the challenged thought
        std::cout << "\nBased on this analysis, how would you rephrase your original thought?" << std::endl;
        std::cout << "Original: \"" << analysis.original_thought << "\"" << std::endl;
        std::cout << "Balanced thought: ";
        std::getline(std::cin, analysis.challenged_thought);
        
        if (analysis.challenged_thought.empty()) {
            analysis.challenged_thought = analysis.original_thought;
        }
    }
    
    void viewProgress() {
        if (analyses.empty()) {
            std::cout << "\nNo thought analyses found. Complete your first analysis!" << std::endl;
            return;
        }
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "PROGRESS ANALYSIS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        // Overall statistics
        int total_analyses = analyses.size();
        int total_distress_reduction = 0;
        std::map<std::string, int> distortion_frequency;
        
        for (const auto& analysis : analyses) {
            total_distress_reduction += (analysis.distress_before - analysis.distress_after);
            for (const auto& distortion : analysis.identified_distortions) {
                distortion_frequency[distortion]++;
            }
        }
        
        std::cout << "📊 OVERALL STATISTICS" << std::endl;
        std::cout << "Total thought analyses: " << total_analyses << std::endl;
        std::cout << "Average distress reduction: " << std::fixed << std::setprecision(1) 
                  << (static_cast<double>(total_distress_reduction) / total_analyses) << " points" << std::endl;
        
        // Most common distortions
        if (!distortion_frequency.empty()) {
            std::cout << "\n🎭 YOUR MOST COMMON DISTORTIONS" << std::endl;
            
            std::vector<std::pair<std::string, int>> sorted_distortions(
                distortion_frequency.begin(), distortion_frequency.end());
            std::sort(sorted_distortions.begin(), sorted_distortions.end(),
                      [](const auto& a, const auto& b) { return a.second > b.second; });
            
            for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(5), sorted_distortions.size()); ++i) {
                std::cout << (i + 1) << ". " << sorted_distortions[i].first 
                          << " (" << sorted_distortions[i].second << " times)" << std::endl;
            }
            
            std::cout << "\n💡 INSIGHT: Focus on recognizing your top distortions!" << std::endl;
            std::cout << "The more aware you become of your patterns, the easier" << std::endl;
            std::cout << "it becomes to catch and challenge them automatically." << std::endl;
        }
        
        // Recent analyses
        std::cout << "\n📝 RECENT ANALYSES (Last 5)" << std::endl;
        std::size_t start = analyses.size() > 5 ? analyses.size() - 5 : 0;
        
        for (std::size_t i = start; i < analyses.size(); ++i) {
            const auto& analysis = analyses[i];
            std::cout << "\n[" << (i + 1) << "] " << analysis.timestamp << std::endl;
            std::cout << "Thought: \"" << analysis.original_thought.substr(0, 50);
            if (analysis.original_thought.length() > 50) std::cout << "...";
            std::cout << "\"" << std::endl;
            std::cout << "Distortions: ";
            if (analysis.identified_distortions.empty()) {
                std::cout << "None identified";
            } else {
                for (std::size_t j = 0; j < analysis.identified_distortions.size(); ++j) {
                    if (j > 0) std::cout << ", ";
                    std::cout << analysis.identified_distortions[j];
                }
            }
            std::cout << std::endl;
            std::cout << "Distress: " << analysis.distress_before << " → " << analysis.distress_after;
            if (analysis.distress_after < analysis.distress_before) {
                std::cout << " ✅";
            }
            std::cout << std::endl;
        }
    }
    
    void practiceQuiz() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "COGNITIVE DISTORTION PRACTICE QUIZ" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::vector<std::pair<std::string, std::string>> quiz_examples = {
            {"I failed my driving test. I'll never be able to drive.", "Overgeneralization"},
            {"Everyone at the party thought I was boring.", "Mind Reading (Jumping to Conclusions)"},
            {"I got a B+ on my exam, but that's not good enough.", "All-or-Nothing Thinking"},
            {"My friend complimented my outfit, but she was just being nice.", "Disqualifying the Positive"},
            {"I feel anxious, so something bad must be about to happen.", "Emotional Reasoning"},
            {"I should be able to handle everything perfectly.", "Should Statements"},
            {"I made a mistake. I'm such an idiot.", "Labeling and Mislabeling"},
            {"The meeting went badly because I didn't speak up enough.", "Personalization"}
        };
        
        // Shuffle the examples
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(quiz_examples.begin(), quiz_examples.end(), gen);
        
        int correct = 0;
        int total = std::min(5, static_cast<int>(quiz_examples.size()));
        
        std::cout << "I'll show you some thoughts. Try to identify the main cognitive distortion!" << std::endl;
        
        for (int i = 0; i < total; ++i) {
            std::cout << "\n" << std::string(40, '-') << std::endl;
            std::cout << "Question " << (i + 1) << " of " << total << std::endl;
            std::cout << "Thought: \"" << quiz_examples[i].first << "\"" << std::endl;
            
            // Show options
            std::vector<std::string> distortion_names;
            for (const auto& pair : distortions) {
                distortion_names.push_back(pair.second.name);
            }
            std::shuffle(distortion_names.begin(), distortion_names.end(), gen);
            
            // Ensure correct answer is in the options
            std::string correct_answer = quiz_examples[i].second;
            if (std::find(distortion_names.begin(), distortion_names.end(), correct_answer) == distortion_names.end()) {
                distortion_names[0] = correct_answer;
            }
            
            // Show first 4 options
            for (int j = 0; j < 4 && j < static_cast<int>(distortion_names.size()); ++j) {
                std::cout << (j + 1) << ". " << distortion_names[j] << std::endl;
            }
            
            int choice = getValidatedInput("Your answer", 1, 4);
            std::string selected = distortion_names[choice - 1];
            
            if (selected == correct_answer) {
                std::cout << "✅ Correct! This is " << correct_answer << "." << std::endl;
                correct++;
            } else {
                std::cout << "❌ Not quite. This is " << correct_answer << "." << std::endl;
            }
            
            // Show explanation
            for (const auto& pair : distortions) {
                if (pair.second.name == correct_answer) {
                    std::cout << "💡 " << pair.second.description << std::endl;
                    break;
                }
            }
        }
        
        // Show results
        std::cout << "\n" << std::string(40, '=') << std::endl;
        std::cout << "QUIZ RESULTS" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        std::cout << "You got " << correct << " out of " << total << " correct!" << std::endl;
        
        if (correct == total) {
            std::cout << "🎉 Perfect score! You're becoming an expert at identifying distortions!" << std::endl;
        } else if (correct >= total * 0.8) {
            std::cout << "🌟 Great job! You have a solid understanding of cognitive distortions." << std::endl;
        } else if (correct >= total * 0.6) {
            std::cout << "👍 Good work! Keep practicing to improve your recognition skills." << std::endl;
        } else {
            std::cout << "📚 Keep studying! The more you practice, the better you'll get." << std::endl;
        }
        
        std::cout << "\n💡 TIP: Regular practice helps you automatically recognize these patterns in your own thinking!" << std::endl;
    }
    
    void showHelp() {
        std::cout << "\n" << std::string(70, '=') << std::endl;
        std::cout << "COGNITIVE DISTORTION IDENTIFIER HELP" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        std::cout << "\n🎯 PURPOSE:" << std::endl;
        std::cout << "This tool helps you identify and challenge cognitive distortions -" << std::endl;
        std::cout << "unhelpful thinking patterns that can increase emotional distress." << std::endl;
        
        std::cout << "\n📚 HOW TO USE:" << std::endl;
        std::cout << "1. Learn About Distortions - Study the 10 common types" << std::endl;
        std::cout << "2. Analyze Your Thoughts - Work through specific thoughts step-by-step" << std::endl;
        std::cout << "3. Practice with Quiz - Test your recognition skills" << std::endl;
        std::cout << "4. Track Progress - See patterns in your thinking over time" << std::endl;
        
        std::cout << "\n💡 TIPS FOR SUCCESS:" << std::endl;
        std::cout << "• Start with thoughts that cause moderate distress (not extreme)" << std::endl;
        std::cout << "• Be curious, not judgmental about your thinking patterns" << std::endl;
        std::cout << "• Practice regularly - recognition improves with repetition" << std::endl;
        std::cout << "• Focus on accuracy, not positivity in your balanced thoughts" << std::endl;
        std::cout << "• Remember that having distorted thoughts is normal and human" << std::endl;
        
        std::cout << "\n🧠 THE 10 COGNITIVE DISTORTIONS:" << std::endl;
        std::cout << "1. All-or-Nothing Thinking - Black and white, no middle ground" << std::endl;
        std::cout << "2. Overgeneralization - One event becomes a never-ending pattern" << std::endl;
        std::cout << "3. Mental Filter - Focusing only on negative details" << std::endl;
        std::cout << "4. Disqualifying Positive - Rejecting positive experiences" << std::endl;
        std::cout << "5. Jumping to Conclusions - Mind reading or fortune telling" << std::endl;
        std::cout << "6. Magnification/Minimization - Blowing things out of proportion" << std::endl;
        std::cout << "7. Emotional Reasoning - 'I feel it, so it must be true'" << std::endl;
        std::cout << "8. Should Statements - Unrealistic expectations and demands" << std::endl;
        std::cout << "9. Labeling - Harsh labels instead of describing behavior" << std::endl;
        std::cout << "10. Personalization - Taking responsibility for everything" << std::endl;
        
        std::cout << "\n⚠️  IMPORTANT NOTES:" << std::endl;
        std::cout << "• This tool is for education and self-awareness" << std::endl;
        std::cout << "• It does not replace professional therapy or counseling" << std::endl;
        std::cout << "• If thoughts are causing severe distress, seek professional help" << std::endl;
        std::cout << "• Crisis resources: National Suicide Prevention Lifeline 988" << std::endl;
    }
    
    void run() {
        displayWelcome();
        
        while (true) {
            std::cout << "\n" << std::string(50, '=') << std::endl;
            std::cout << "MAIN MENU" << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            std::cout << "1. Learn About Cognitive Distortions" << std::endl;
            std::cout << "2. Analyze a Thought" << std::endl;
            std::cout << "3. Practice Quiz" << std::endl;
            std::cout << "4. View Progress & Patterns" << std::endl;
            std::cout << "5. Help & Tips" << std::endl;
            std::cout << "6. Exit" << std::endl;
            std::cout << "\nChoose an option (1-6): ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                learnAboutDistortions();
            } else if (choice == "2") {
                analyzeThought();
            } else if (choice == "3") {
                practiceQuiz();
            } else if (choice == "4") {
                viewProgress();
            } else if (choice == "5") {
                showHelp();
            } else if (choice == "6") {
                std::cout << "\nThank you for using the Cognitive Distortion Identifier!" << std::endl;
                std::cout << "Remember: Awareness is the first step to changing unhelpful thinking patterns! 🧠✨" << std::endl;
                break;
            } else {
                std::cout << "\nInvalid choice. Please select 1-6." << std::endl;
            }
        }
    }
};

int main() {
    try {
        CognitiveDistortionIdentifier identifier;
        identifier.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
