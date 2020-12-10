//
// Created by Elaine Wang on 12/9/20.
//
#include <visualizer/scores.h>

#include <catch2/catch.hpp>

TEST_CASE("Does a score belong in the top five") {
  minesweeper::visualizer::Scores scores;
  scores.ReadScoresFromFile("assets/test_scores");
  SECTION("Score belongs") {
    REQUIRE(scores.CheckScore(minesweeper::visualizer::Mode::kExpert, 998));
  }
  SECTION("Score does not belongs") {
    REQUIRE_FALSE(
        scores.CheckScore(minesweeper::visualizer::Mode::kExpert, 1000));
  }
}

TEST_CASE("Inserting/writing a score and reading from file") {
  minesweeper::visualizer::Scores scores;
  scores.ReadScoresFromFile("assets/test_scores");
  scores.InsertScore(minesweeper::visualizer::Mode::kBeginner, 1);
  REQUIRE(scores.ScoresFromMode(minesweeper::visualizer::Mode::kBeginner)[0] ==
          1);
  scores.ReadScoresFromFile("assets/test_scores");
  REQUIRE(scores.ScoresFromMode(minesweeper::visualizer::Mode::kBeginner)[0] ==
          1);
}