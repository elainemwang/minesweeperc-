#include <visualizer/field.h>

#include <catch2/catch.hpp>

TEST_CASE("Counting flags around") {
  SECTION("2x2 board") {
    minesweeper::visualizer::Field field(2, 2, 0);
    field.FlagBox(0, 0);
    field.FlagBox(1, 1);
    REQUIRE(field.FlagsAroundBox(0, 0) == 1);
    REQUIRE(field.FlagsAroundBox(0, 1) == 2);
    REQUIRE(field.FlagsAroundBox(1, 0) == 2);
    REQUIRE(field.FlagsAroundBox(1, 1) == 1);
  }
  SECTION("3x3 board") {
    minesweeper::visualizer::Field field(3, 3, 0);
    field.FlagBox(1, 0);
    field.FlagBox(2, 1);
    field.FlagBox(0, 2);
    REQUIRE(field.FlagsAroundBox(0, 0) == 1);
    REQUIRE(field.FlagsAroundBox(0, 1) == 2);
    REQUIRE(field.FlagsAroundBox(0, 2) == 0);
    REQUIRE(field.FlagsAroundBox(1, 0) == 1);
    REQUIRE(field.FlagsAroundBox(1, 1) == 3);
    REQUIRE(field.FlagsAroundBox(1, 2) == 2);
    REQUIRE(field.FlagsAroundBox(2, 0) == 2);
    REQUIRE(field.FlagsAroundBox(2, 1) == 1);
    REQUIRE(field.FlagsAroundBox(2, 2) == 1);
  }
}

TEST_CASE("Calculating box values") {
  SECTION("2x2 board") {
    minesweeper::visualizer::Field field(2, 2, 0);
    field.SetBoxAsMine(0, 0);
    field.SetBoxAsMine(1, 1);
    field.SetAllBoxValues();
    REQUIRE(field.GetBoard()[0][0].GetValue() == 1);
    REQUIRE(field.GetBoard()[0][1].GetValue() == 2);
    REQUIRE(field.GetBoard()[1][0].GetValue() == 2);
    REQUIRE(field.GetBoard()[1][1].GetValue() == 1);
  }
  SECTION("3x3 board") {
    minesweeper::visualizer::Field field(3, 3, 0);
    field.SetBoxAsMine(1, 0);
    field.SetBoxAsMine(2, 1);
    field.SetBoxAsMine(0, 2);
    field.SetAllBoxValues();
    REQUIRE(field.GetBoard()[0][0].GetValue() == 1);
    REQUIRE(field.GetBoard()[0][1].GetValue() == 2);
    REQUIRE(field.GetBoard()[0][2].GetValue() == 0);
    REQUIRE(field.GetBoard()[1][0].GetValue() == 1);
    REQUIRE(field.GetBoard()[1][1].GetValue() == 3);
    REQUIRE(field.GetBoard()[1][2].GetValue() == 2);
    REQUIRE(field.GetBoard()[2][0].GetValue() == 2);
    REQUIRE(field.GetBoard()[2][1].GetValue() == 1);
    REQUIRE(field.GetBoard()[2][2].GetValue() == 1);
  }
}

TEST_CASE("Getting box position from mouse coordinates") {
  minesweeper::visualizer::Field field(3, 3, 30);
  SECTION("Top left") {
    REQUIRE(field.BoxRowColFromMousePos(glm::vec2(5, 7)) == glm::vec2(0, 0));
  }
  SECTION("Top right") {
    REQUIRE(field.BoxRowColFromMousePos(glm::vec2(25, 8.6)) == glm::vec2(0, 2));
  }
  SECTION("Bottom left") {
    REQUIRE(field.BoxRowColFromMousePos(glm::vec2(5, 29)) == glm::vec2(2, 0));
  }
  SECTION("Bottom right") {
    REQUIRE(field.BoxRowColFromMousePos(glm::vec2(25, 28)) == glm::vec2(2, 2));
  }
  SECTION("Middle") {
    REQUIRE(field.BoxRowColFromMousePos(glm::vec2(12, 17)) == glm::vec2(1, 1));
  }
}

TEST_CASE("Setting the boxes around a box") {
  SECTION("2x2 board") {
    minesweeper::visualizer::Field field(2, 2, 0);
    REQUIRE(field.GetBoard()[0][0].GetBoxesAround() ==
            std::vector<std::vector<size_t>>{{0, 1}, {1, 0}, {1, 1}});
    REQUIRE(field.GetBoard()[0][1].GetBoxesAround() ==
            std::vector<std::vector<size_t>>{{0, 0}, {1, 0}, {1, 1}});
    REQUIRE(field.GetBoard()[1][0].GetBoxesAround() ==
            std::vector<std::vector<size_t>>{{0, 0}, {0, 1}, {1, 1}});
    REQUIRE(field.GetBoard()[1][1].GetBoxesAround() ==
            std::vector<std::vector<size_t>>{{0, 0}, {0, 1}, {1, 0}});
  }
}

// Game Over Testing
TEST_CASE("Game over from opening a mine") {
  SECTION("2x2 - Mine is opened -> Game Over"){
    minesweeper::visualizer::Field field(2, 2, 0);
    field.SetBoxAsMine(0, 0);
    field.SetBoxAsMine(1, 1);
    field.SetAllBoxValues();
    field.OpenBox(0,0);
    REQUIRE(field.IsGameOver());
    REQUIRE_FALSE(field.IsGameWon());
  }
  SECTION("2x2 - Non-Mine is opened -> Game not over"){
    minesweeper::visualizer::Field field(2, 2, 0);
    field.SetBoxAsMine(0, 0);
    field.SetBoxAsMine(1, 1);
    field.SetAllBoxValues();
    field.OpenBox(1,0);
    REQUIRE_FALSE(field.IsGameOver());
    REQUIRE_FALSE(field.IsGameWon());
  }
  SECTION("3x3 - Mine is opened -> Game Over"){
    minesweeper::visualizer::Field field(3, 3, 0);
    field.SetBoxAsMine(1, 0);
    field.SetBoxAsMine(2, 1);
    field.SetBoxAsMine(0, 2);
    field.SetAllBoxValues();
    field.OpenBox(2,1);
    REQUIRE(field.IsGameOver());
    REQUIRE_FALSE(field.IsGameWon());
  }
  SECTION("3x3 - Non-Mine is opened -> Game not over"){
    minesweeper::visualizer::Field field(3, 3, 0);
    field.SetBoxAsMine(1, 0);
    field.SetBoxAsMine(2, 1);
    field.SetBoxAsMine(0, 2);
    field.SetAllBoxValues();
    field.OpenBox(1,1);
    REQUIRE_FALSE(field.IsGameOver());
    REQUIRE_FALSE(field.IsGameWon());
  }
}

TEST_CASE("Game over from winning") {
  SECTION("2x2 - All non-mines are opened"){
    minesweeper::visualizer::Field field(2, 2, 0);
    field.SetBoxAsMine(0, 0);
    field.SetBoxAsMine(1, 1);
    field.SetAllBoxValues();
    field.OpenBox(0,1);
    field.OpenBox(1,0);
    REQUIRE(field.IsGameOver());
    REQUIRE(field.IsGameWon());
  }
  SECTION("3x3 - All non-mines are opened"){
    minesweeper::visualizer::Field field(3, 3, 0);
    field.SetBoxAsMine(1, 0);
    field.SetBoxAsMine(2, 1);
    field.SetBoxAsMine(0, 2);
    field.SetAllBoxValues();
    field.OpenBox(0,0);
    field.OpenBox(0,1);
    field.OpenBox(1,1);
    field.OpenBox(1,2);
    field.OpenBox(2,0);
    field.OpenBox(2,2);
    REQUIRE(field.IsGameOver());
    REQUIRE(field.IsGameWon());
  }
}
