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
}

const std::vector<std::vector<Box>> Field::GetBoard() const{
  return board_;
}

const Box Field::GetBox(size_t i, size_t j) const {
  return board_[i][j];
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
} // namespace visualizer
} // namespace minesweeper