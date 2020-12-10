//
// Created by Elaine Wang on 12/9/20.
//

#include "visualizer/scores.h"

namespace minesweeper {

namespace visualizer {

Scores::Scores() {
  ReadScoresFromFile(kScoresFile);
}

bool Scores::CheckScore(Mode mode, double time) {
  if (time < ScoresFromMode(mode).back()) {
    return true;
  } else {
    return false;
  }
}

void Scores::InsertScore(Mode mode, double time) {
  std::vector<size_t>& mode_scores = ScoresFromMode(mode);
  for (size_t i = 0; i < scores_to_hold_; ++i) {
    if (time < mode_scores[i]) {
      mode_scores.insert(mode_scores.begin() + i, time);
      mode_scores.pop_back();
      WriteScoresToFile(kScoresFile);
      break;
    }
  }
}

void Scores::ReadScoresFromFile(const std::string& file_path) {
  std::ifstream scoresfile(file_path);
  std::string line;
  for (size_t i = 0; i < scores_to_hold_; i++) {
    getline(scoresfile, line);
    ScoresFromMode(Mode::kBeginner).push_back(std::stoi(line));
  }
  for (size_t i = 0; i < scores_to_hold_; i++) {
    getline(scoresfile, line);
    ScoresFromMode(Mode::kIntermediate).push_back(std::stoi(line));
  }
  for (size_t i = 0; i < scores_to_hold_; i++) {
    getline(scoresfile, line);
    ScoresFromMode(Mode::kExpert).push_back(std::stoi(line));
  }
  scoresfile.close();
}

void Scores::WriteScoresToFile(const std::string& file_path) {
  std::ofstream scoresfile(file_path);
  for (size_t i = 0; i < scores_to_hold_; i++) {
    scoresfile << ScoresFromMode(Mode::kBeginner)[i];
    scoresfile << "\n";
  }
  for (size_t i = 0; i < scores_to_hold_; i++) {
    scoresfile << ScoresFromMode(Mode::kIntermediate)[i];
    scoresfile << "\n";
  }
  for (size_t i = 0; i < scores_to_hold_; i++) {
    scoresfile << ScoresFromMode(Mode::kExpert)[i];
    scoresfile << "\n";
  }
  scoresfile.close();
}

std::vector<size_t>& Scores::ScoresFromMode(Mode mode) {
  switch (mode) {
    case Mode::kBeginner:
      return beginner_scores_;
    case Mode::kIntermediate:
      return intermediate_scores_;
    case Mode::kExpert:
      return expert_scores_;
  }
}

void Scores::DrawMode(Mode mode, const glm::vec2& top_left) {
  ci::Rectf pixel_bounding_box(
      top_left, top_left + glm::vec2(kXWindowSizeIm / 2, 3 * kPixelsPerBox));
  ci::gl::color(ci::Color("black"));
  ci::gl::drawSolidRect(pixel_bounding_box);
  switch (mode) {
    case Mode::kBeginner:
      ci::gl::drawStringCentered(
          "BEGINNER", top_left + glm::vec2(kXWindowSizeIm / 4, kPixelsPerBox),
          ci::Color("white"), ci::Font("Helvetica Neue", kPixelsPerBox - 5));
      break;
    case Mode::kIntermediate:
      ci::gl::drawStringCentered(
          "INTERMEDIATE",
          top_left + glm::vec2(kXWindowSizeIm / 4, kPixelsPerBox),
          ci::Color("white"), ci::Font("Helvetica Neue", kPixelsPerBox - 5));
      break;
    case Mode::kExpert:
      ci::gl::drawStringCentered(
          "EXPERT", top_left + glm::vec2(kXWindowSizeIm / 4, kPixelsPerBox),
          ci::Color("white"), ci::Font("Helvetica Neue", kPixelsPerBox - 5));
      break;
  }
  DrawScores(ScoresFromMode(mode), top_left + glm::vec2(0, 3 * kPixelsPerBox));
}

void Scores::DrawScores(const std::vector<size_t>& scores,
                        const glm::vec2& top_left) {
  for (size_t i = 0; i < scores_to_hold_; ++i) {
    ci::gl::drawStringCentered(
        std::to_string(i + 1),
        top_left + glm::vec2(0, (2 * i + 1) * kPixelsPerBox),
        ci::Color("black"), ci::Font("Helvetica Neue", kPixelsPerBox - 5));
    ci::gl::drawStringCentered(
        std::to_string(scores[i]),
        top_left +
            glm::vec2(2 * kXWindowSizeIm / 4, (2 * i + 1) * kPixelsPerBox),
        ci::Color("black"), ci::Font("Helvetica Neue", kPixelsPerBox - 5));
  }
}

}  // namespace visualizer
}  // namespace minesweeper