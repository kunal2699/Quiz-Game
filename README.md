# Quiz Game in C++
This is a simple quiz game written in C++. The game asks the player multiple-choice questions from json file and calculates their score based on the number of correct answers. The Leaderboard will update on how many attempts are taken and time to answer the questions. Displays the final score at the end.
<br></br>

# How to Play
1. Clone the repository to your local machine.
2. Compile the quiz_game.cpp file using a C++ compiler.
3. Run the compiled executable.
4. Answer each question by entering the corresponding letter (a, b, c, or d).
5. Enter your name when prompted.
6. At the end of the quiz, your final score will be displayed on Leaderboard if you are in top 10.
<br></br>

# Features
- Simple console-based interface.
- Multiple-choice questions on C++ concepts.
- Calculates and displays the player's final score.
<br></br>

# Requirements
- C++ compiler
<br></br>

# Explaining the Code Structure

# Represents a single quiz question, including:
- text: The question itself.
- options: A vector of possible answers.
- correct_answer: The correct option (e.g., 'A', 'B', etc.).
- time_limit: (Unused in timing, but can be used for per-question limits).

# Tracks stats for the current quiz session:
- score: Number of correct answers.
- total_attempts: Total number of answer attempts made by the user.
- question_times: Vector of time (in seconds) spent on each question.
- total_time: Sum of all question times.

# Represents a leaderboard entry:
- name: Player’s name.
- total_attempts: Total attempts taken in the quiz.
- total_time: Total time taken in the quiz.
- score: Final score (number of correct answers).

# Future Improvements
- Add more questions and categories.
- Implement a scoring system based on question difficulty.
-  Add a timer for each question.

# AI tools
- Chatgpt to know how to read the json file in C++.
- Write some code to read the json.
