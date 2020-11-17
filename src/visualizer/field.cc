//
// Created by Elaine Wang on 11/16/20.
//
#include "visualizer/field.h"

namespace minesweeper {

namespace visualizer {

using glm::vec2;

Field::Field(const glm::vec2 &top_left_corner, size_t num_rows, size_t num_cols, double width) :
      top_left_corner_(top_left_corner),
      num_rows_(num_rows),
      num_cols_(num_cols),
      pixel_side_length_(width / num_cols)
{
  board_.assign(num_rows, std::vector<Box>(num_cols, Box()));
  game_over_ = false;
}

const std::vector<std::vector<Box>> Field::GetBoard() const{
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
      if(b.IsOpen()){
        ci::gl::drawStringCentered(std::to_string(b.GetValue()),
                           vec2((pixel_top_left.x+pixel_bottom_right.x)/2,
                                (pixel_top_left.y+pixel_bottom_right.y)/2),
                           ci::Color("black")); //TODO: different colors for different numbers
      }
      else if(b.IsFlagged()){
        ci::gl::color(ci::Color("red"));
        ci::gl::drawSolidRect(pixel_bounding_box);
      }
      else{
        ci::gl::color(ci::Color("white"));
        ci::gl::drawSolidRect(pixel_bounding_box);
      }
      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);
    }
  }
}

vec2 Field::BoxRowColFromMousePos(const glm::vec2 &mouse_screen_coords) {
  vec2 field_coords =
      (mouse_screen_coords - top_left_corner_) / (float)pixel_side_length_;
  size_t i = field_coords.y;
  size_t j = field_coords.x;
  return vec2(i,j);
}

void Field::OpenBox(size_t i, size_t j) {
  Box& current_box = board_[i][j];
  if(current_box.OpenAndCheckGameOver()){
    game_over_ = true;
  }
  /*
  if(current_box.GetValue() == 0){
    OpenBoxesAround(i,j);
  }*/
}


void Field::OpenBoxesAround(size_t i, size_t j) {
  Box& current_box = board_[i][j];
  for(std::vector<size_t> box_pos : current_box.GetBoxesAround()){
    board_[box_pos[0]][box_pos[1]];
  }
}

void Field::FlagBox(size_t i, size_t j) {
  board_[i][j].Flag();
}

void Field::UnflagBox(size_t i, size_t j) {
  board_[i][j].Unflag();
}

} // namespace visualizer
} // namespace minesweeper