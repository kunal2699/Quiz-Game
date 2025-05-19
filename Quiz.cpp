#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Structures
struct Question {
    std::string text;
    std::vector<std::string> options;
    char correct_answer;
    int time_limit;
};

struct GameStats {
    int score = 0;
    int total_attempts = 0;
    std::vector<double> question_times;
    double total_time = 0.0;
};

struct LeaderboardEntry {
    std::string name;
    int total_attempts;
    double total_time;
    int score;
};

// Function to parse a question from JSON
Question parse_question(const json& j) {
    return {
        j["question"],
        j["options"].get<std::vector<std::string>>(),
        static_cast<char>(j["correct"].get<std::string>()[0]),
        j.value("time_limit", 30)
    };
}

// Ask questions and collect stats
GameStats ask_questions(const std::vector<Question>& questions) {
    GameStats stats;

    for (const auto& q : questions) {
        auto start = std::chrono::steady_clock::now();
        int attempts = 0;
        char answer;

        do {
            std::cout << "\n" << q.text << "\n";
            for (size_t i = 0; i < q.options.size(); i++) {
                std::cout << static_cast<char>('A' + i) << ") " 
                          << q.options[i] << "\n";
            }

            std::cout << "Your answer: ";
            std::cin >> answer;
            answer = toupper(answer);
            attempts++;
        } while (answer != q.correct_answer && attempts < 3);

        auto end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();

        if (answer == q.correct_answer) {
            stats.score++;
            std::cout << "Correct! ";
        } else {
            std::cout << "Correct answer: " << q.correct_answer << " ";
        }

        stats.total_attempts += attempts;
        stats.question_times.push_back(elapsed);
        stats.total_time += elapsed;
        std::cout << "(Time: " << elapsed << "s)\n";
    }

    return stats;
}

// Show game summary
void show_summary(const GameStats& stats) {
    std::cout << "\n=== FINAL RESULTS ===\n"
              << "Total Score: " << stats.score << "\n"
              << "Total Attempts: " << stats.total_attempts << "\n"
              << "Total Time: " << stats.total_time << "s\n"
              << "Average Time/Question: " 
              << (stats.question_times.empty() ? 0.0 :
                  std::accumulate(stats.question_times.begin(), 
                                  stats.question_times.end(), 0.0) 
                  / stats.question_times.size())
              << "s\n";
}

// Load leaderboard from file
std::vector<LeaderboardEntry> load_leaderboard(const std::string& filename) {
    std::vector<LeaderboardEntry> leaderboard;
    std::ifstream file(filename);
    if (!file.is_open()) return leaderboard;

    json data = json::parse(file, nullptr, false);
    if (!data.is_array()) return leaderboard;

    for (const auto& entry : data) {
        leaderboard.push_back({ 
            entry["name"], 
            entry["total_attempts"], 
            entry["total_time"], 
            entry["score"] 
        });
    }
    return leaderboard;
}

// Save leaderboard to file
void save_leaderboard(const std::string& filename, const std::vector<LeaderboardEntry>& leaderboard) {
    json data = json::array();
    for (const auto& entry : leaderboard) {
        data.push_back({ 
            {"name", entry.name}, 
            {"total_attempts", entry.total_attempts}, 
            {"total_time", entry.total_time},
            {"score", entry.score}
        });
    }
    std::ofstream file(filename);
    file << data.dump(4);
}

// Update and display leaderboard (fewest attempts, then least time)
void update_leaderboard(int total_attempts, double total_time, int score) {
    std::string name;
    std::cout << "Enter your name for the leaderboard: ";
    std::cin >> name;

    auto leaderboard = load_leaderboard("leaderboard.json");
    leaderboard.push_back({ name, total_attempts, total_time, score });

    // Sort: fewest attempts is best; if tie, least time is better; if still tie, higher score is better
    std::sort(leaderboard.begin(), leaderboard.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        if (a.total_attempts != b.total_attempts)
            return a.total_attempts < b.total_attempts;
        if (std::abs(a.total_time - b.total_time) > 1e-6)
            return a.total_time < b.total_time;
        return a.score > b.score;
    });
    if (leaderboard.size() > 10) leaderboard.resize(10);

    save_leaderboard("leaderboard.json", leaderboard);

    // Display leaderboard
    std::cout << "\n=== Leaderboard (Fewest Attempts, Least Time) ===\n";
    std::cout << "Rank | Name       | Attempts | Time(s)  | Score\n";
    std::cout << "-----------------------------------------------\n";
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        std::cout << std::setw(4) << (i+1) << " | "
                  << std::setw(10) << leaderboard[i].name << " | "
                  << std::setw(8) << leaderboard[i].total_attempts << " | "
                  << std::setw(8) << std::fixed << std::setprecision(2) << leaderboard[i].total_time << " | "
                  << std::setw(5) << leaderboard[i].score << "\n";
    }
}

int main() {
    try {
        std::ifstream file("questions.json");
        if (!file.is_open()) {
            std::cerr << "Error: Could not open questions.json\n";
            return 1;
        }
        json data = json::parse(file);

        std::vector<Question> questions;
        for (const auto& item : data["questions"]) {
            questions.push_back(parse_question(item));
        }

        GameStats stats = ask_questions(questions);
        show_summary(stats);
        update_leaderboard(stats.total_attempts, stats.total_time, stats.score);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
