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
        double width, size_t num_mines);
  void Draw() const;
  const std::vector<std::vector<Box>> GetBoard() const;

  glm::vec2 BoxRowColFromMousePos(const glm::vec2& mouse_screen_coords);

  /**
   * Opens the box at the location and any corresponding boxes
   * if the box value is zero.
   * @param i The row the box to open belongs to.
   * @param j The column the box to open belongs to.
   */
  void OpenBox(size_t i, size_t j);

  void FlagBox(size_t i, size_t j);

  void UnflagBox(size_t i, size_t j);

  void OpenBoxesAround(size_t i, size_t j);

  size_t FlagsAroundBox(size_t i, size_t j);

 private:
  std::vector<std::vector<Box>> board_;
  glm::vec2 top_left_corner_;
  size_t num_rows_;
  size_t num_cols_;
  size_t num_mines_;
  /** Number of screen pixels in the width/height of one box*/
  double pixel_side_length_;
  bool game_over_;

  void SetMines(size_t i, size_t j);
  void SetAllBoxValues();

  /**
  * Sets the mines and box values.
  * @param i starting box row
  * @param j starting box column
  */
  void SetUpField(size_t i, size_t j);
  /**
   * Sets all boxes' boxes around.
   */
  void SetBoxesAround();
};

}

}
