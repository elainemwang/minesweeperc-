//
// Created by Elaine Wang on 12/9/20.
//

#pragma once

#include "cinder/gl/gl.h"
#include "field.h"

namespace minesweeper {

namespace visualizer {
class Scores {
 public:
  Scores();
  /**
   * Checks if the time is in the top 5 times.
   * @param mode The mode played.
   * @param time The time to check.
   * @return A boolean representing whether the time is in the top 5 of the
   * mode.
   */
  bool CheckScore(Mode mode, double time);
  /**
   * Inserts the score into the top 5 scores for the mode.
   * @param mode The mode to insert the score into.
   * @param time The score.
   */
  void InsertScore(Mode mode, double time);
  /**
   * Drawing for the given mode.
   * @param mode The mode to draw for.
   * @param top_left The top left position of the scoreboard.
   */
  void DrawMode(Mode mode, const glm::vec2& top_left);

 private:
  size_t scores_to_hold_ = 5;
  std::vector<size_t> beginner_scores_;
  std::vector<size_t> intermediate_scores_;
  std::vector<size_t> expert_scores_;

  const std::string kScoresFile = "assets/scores";
  const size_t kXWindowSizeIm = 400;
  const double kPixelsPerBox = 25;

  /**
   * Writes the scores to the given file.
   * @param file_path The file path of the file to write the scores to.
   */
  void WriteScoresToFile(const std::string& file_path);
  /**
   * Reads the scores from a given file.
   * @param file_path The file path of the file to read scores from.
   */
  void ReadScoresFromFile(const std::string& file_path);
  /**
   * Drawing method of the scores.
   * @param scores The scores vector to draw.
   * @param top_left The top left position of the scores.
   */
  void DrawScores(const std::vector<size_t>& scores, const glm::vec2& top_left);
  /**
   * Gets the scores vector for a mode.
   * @param mode The mode to get the scores vector for.
   * @return A scores vector for the given mode.
   */
  std::vector<size_t>& ScoresFromMode(Mode mode);
};

}  // namespace visualizer
}  // namespace minesweeper
