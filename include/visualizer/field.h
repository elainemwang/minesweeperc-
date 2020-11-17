//
// Created by Elaine Wang on 11/16/20.
//
#pragma once

#include "cinder/gl/gl.h"
#include "core/box.h"

namespace minesweeper {

namespace visualizer {

class Field {
 public:
  Field(const glm::vec2& top_left_corner, size_t num_rows, size_t num_cols,
        double width);
  void Draw() const;
  void SetMines(size_t num_mines);
  void SetAllBoxValues();
  const std::vector<std::vector<Box>> GetBoard() const;
  /**
   * Opens the box at the location and any corresponding boxes
   * if the box value is zero.
   * @param i The row the box to open belongs to.
   * @param j The column the box to open belongs to.
   */
  void OpenBox(size_t i, size_t j);
  void FlagBox(size_t i, size_t j);
  const Box GetBox(size_t i, size_t j) const;
 private:
  std::vector<std::vector<Box>> board_;
  glm::vec2 top_left_corner_;
  size_t num_rows_;
  size_t num_cols_;
  size_t num_mines_;
  /** Number of screen pixels in the width/height of one sketchpad pixel */
  double pixel_side_length_;
};

}

}
