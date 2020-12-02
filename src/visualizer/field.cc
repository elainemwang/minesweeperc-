//
// Created by Elaine Wang on 11/16/20.
//
#include "visualizer/field.h"

namespace minesweeper {

namespace visualizer {

using glm::vec2;

Field::Field(const glm::vec2& top_left_corner, size_t num_rows, size_t num_cols,
             double width, size_t num_mines)
    : top_left_corner_(top_left_corner),
      num_rows_(num_rows),
      num_cols_(num_cols),
      num_mines_(num_mines),
      pixel_side_length_(width / num_cols),
      restart_button_(
          top_left_corner_ + vec2(num_cols / 2 * pixel_side_length_, -37),
          top_left_corner_ + vec2(num_cols / 2 * pixel_side_length_, -37) +
              vec2(pixel_side_length_, pixel_side_length_),
          ci::Color("blue")),
      timer_(top_left_corner_ + vec2((num_cols - 2) * pixel_side_length_, -30),
             top_left_corner_ + vec2((num_cols - 2) * pixel_side_length_, -30) +
             vec2(pixel_side_length_, pixel_side_length_),
             ci::Color("black")),
      mines_left_(top_left_corner_ + vec2(pixel_side_length_, -30),
                  top_left_corner_ + vec2(pixel_side_length_, -30) +
                  vec2(pixel_side_length_, pixel_side_length_),
                  ci::Color("black")){
  RestartGame();
}

Field::Field(size_t num_rows, size_t num_cols, double width, size_t num_mines)
    : num_rows_(num_rows),
      num_cols_(num_cols),
      pixel_side_length_(width / num_cols),
      restart_button_(glm::vec2(0, 0),
                      glm::vec2(0, 0), ci::Color("blue")),
      timer_(glm::vec2(0, 0),
             glm::vec2(0, 0),
             ci::Color("black")),
      mines_left_(top_left_corner_ + vec2(pixel_side_length_, -30),
                  top_left_corner_ + vec2(pixel_side_length_, -30) +
                  vec2(pixel_side_length_, pixel_side_length_),
                  ci::Color("black")){
  RestartGame();
}

const bool Field::IsGameOver() const {
  return game_over_;
}

const bool Field::IsGameWon() const {
  return win_;
}

const std::vector<std::vector<Box>> Field::GetBoard() const {
  return board_;
}

void Field::Draw() const {
  for (size_t row = 0; row < num_rows_; ++row) {
    for (size_t col = 0; col < num_cols_; ++col) {
      Box b = board_[row][col];
      vec2 pixel_top_left = top_left_corner_ + vec2(col * pixel_side_length_,
                                                    row * pixel_side_length_);

      vec2 pixel_bottom_right =
          pixel_top_left + vec2(pixel_side_length_, pixel_side_length_);
      ci::Rectf pixel_bounding_box(pixel_top_left, pixel_bottom_right);
      if (b.IsOpen()) {
        // if the box is open
        if (b.IsMine()) {  // if the box is a mine
          ci::gl::color(ci::Color("black"));
          ci::gl::drawSolidRect(pixel_bounding_box);
        } else {  // otherwise draw the number (except zero)
          if (b.GetValue() != 0) {
            ci::gl::drawStringCentered(
                std::to_string(b.GetValue()),
                vec2((pixel_top_left.x + pixel_bottom_right.x) / 2,
                     (pixel_top_left.y + pixel_bottom_right.y) / 2),
                ci::Color(
                    "black"));  // TODO: different colors for different numbers
          }
        }
      } else if (b.IsFlagged()) {  // if the box is flagged
        ci::gl::color(ci::Color("red"));
        ci::gl::drawSolidRect(pixel_bounding_box);
      } else {  // if the box is closed
        ci::gl::color(ci::Color("gray"));
        ci::gl::drawSolidRect(pixel_bounding_box);
      }
      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);
    }
  }

  // Restart game button
  ci::gl::color(restart_button_.color_);
  ci::gl::drawSolidRect(restart_button_.bounding_box_);

  // Timer
  ci::gl::drawString(std::to_string((int) cinder_timer_.getSeconds()),
                     timer_.top_left_, timer_.color_);

  // Number of unflagged mines left
  ci::gl::drawString(std::to_string(num_mines_ - num_flagged_),
                     mines_left_.top_left_, mines_left_.color_);
}

vec2 Field::BoxRowColFromMousePos(const glm::vec2& mouse_screen_coords) {
  vec2 field_coords =
      (mouse_screen_coords - top_left_corner_) / (float)pixel_side_length_;
  size_t i = field_coords.y;
  size_t j = field_coords.x;
  return vec2(i, j);
}

void Field::SetUpField(size_t i, size_t j) {
  SetMines(i, j);
  SetAllBoxValues();
  cinder_timer_.start();
}

void Field::SetMines(size_t i, size_t j) {
  size_t mines_left = num_mines_;
  size_t row;
  size_t col;
  bool in_starting_box;
  while (mines_left > 0) {
    in_starting_box = false;
    row = rand() % num_rows_;
    col = rand() % num_cols_;
    // check if the mine tile is in the starting nine squares
    for (std::vector<size_t> box : board_[i][j].GetBoxesAround()) {
      if ((row == box[0] && col == box[1]) || (row == i && col == j)) {
        in_starting_box = true;
        break;
      }
    }
    if (!board_[row][col].IsMine() && !in_starting_box) {
      board_[row][col].SetMine();
      --mines_left;
    }
  }
}

void Field::SetAllBoxValues() {
  size_t current_box_value;
  for (std::vector<Box>& row : board_) {
    for (Box& b : row) {
      current_box_value = 0;
      // count all the mines in the boxes around
      for (const std::vector<size_t>& box_pos : b.GetBoxesAround()) {
        if (board_[box_pos[0]][box_pos[1]].IsMine()) {
          ++current_box_value;
        }
      }
      b.SetValue(current_box_value);
    }
  }
}

void Field::OpenBox(size_t i, size_t j) {
  Box& current_box = board_[i][j];
  if (!current_box.IsOpen() && !current_box.IsFlagged()) {
    if (current_box.OpenAndCheckGameOver()) {
      game_over_ = true;
      cinder_timer_.stop();
      // display after the game ends and a mine is opened
      restart_button_.color_ = ci::Color("red");
      for (std::vector<Box>& row : board_) {
        for (Box& b : row) {
          if (b.IsFlagged() && !b.IsMine()) {
            // TODO: if flagged incorrectly, set to bomb with an "x" over it
            // also need to add this to the draw method
          } else if (b.IsMine() && b.IsOpen()) {
            // TODO: if bomb is opened, set to bomb with a red background
          }
          b.OpenAndCheckGameOver();
        }
      }
    } else {
      num_correct_unopened_--;
      if (num_correct_unopened_ == 0) {
        game_over_ = true;
        win_ = true;
        restart_button_.color_ = ci::Color("green");
        cinder_timer_.stop();
      }
    }
    if (!current_box.IsMine() && current_box.GetValue() == 0 &&
        current_box.IsOpen()) {
      OpenBoxesAround(i, j);
    }
  }
}

void Field::OpenBoxesAround(size_t i, size_t j) {
  Box& current_box = board_[i][j];
  for (const std::vector<size_t>& box_pos : current_box.GetBoxesAround()) {
    if (!board_[box_pos[0]][box_pos[1]].IsOpen()) {
      OpenBox(box_pos[0], box_pos[1]);
    }
  }
}

void Field::SetBoxesAround() {
  for (int row = 0; row < num_rows_; ++row) {
    for (int col = 0; col < num_cols_; ++col) {
      for (int i = row - 1; i <= row + 1; ++i) {
        if (i >= 0 && i < num_rows_) {  // making sure the row is inbounds
          for (int j = col - 1; j <= col + 1; ++j) {
            if (j >= 0 && j < num_cols_) {    // making sure the col is inbounds
              if (!(i == row && j == col)) {  // don't add itself
                board_[row][col].AddBoxAround(i, j);
              }
            }
          }
        }
      }
    }
  }
}

void Field::FlagBox(size_t i, size_t j) {
  board_[i][j].Flag();
  ++num_flagged_;
}

void Field::UnflagBox(size_t i, size_t j) {
  board_[i][j].Unflag();
  --num_flagged_;
}

size_t Field::FlagsAroundBox(size_t i, size_t j) {
  size_t num_flags = 0;
  for (const std::vector<size_t>& box_pos : board_[i][j].GetBoxesAround()) {
    if (board_[box_pos[0]][box_pos[1]].IsFlagged()) {
      ++num_flags;
    }
  }
  return num_flags;
}

void Field::SetBoxAsMine(size_t i, size_t j) {
  board_[i][j].SetMine();
}

void Field::RestartGame() {
  board_.assign(num_rows_, std::vector<Box>(num_cols_, Box()));
  num_correct_unopened_ = num_rows_ * num_cols_ - num_mines_;
  num_flagged_ = 0;
  game_over_ = false;
  win_ = false;
  restart_button_.color_ = ci::Color("blue");
  SetBoxesAround();
  cinder_timer_ = ci::Timer();
}

const bool Field::IsRestartButtonHit(
    const glm::vec2& mouse_screen_coords) const {
  return ((mouse_screen_coords.x > restart_button_.top_left_.x &&
           mouse_screen_coords.x < restart_button_.bottom_right_.x) &&
          (mouse_screen_coords.y > restart_button_.top_left_.y &&
           mouse_screen_coords.y < restart_button_.bottom_right_.y));
}

}  // namespace visualizer
}  // namespace minesweeper