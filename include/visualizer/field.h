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
  Field(size_t num_rows, size_t num_cols, double width, size_t num_mines);
  void Draw() const;
  const std::vector<std::vector<Box>> GetBoard() const;
  const bool IsGameOver() const;
  const bool IsGameWon() const;

  /**
   * Returns the box position that the mouse_screen_coords belongs to.
   * @param mouse_screen_coords The mouse_screen_coords to calculate
   * the box position from.
   * @return The box position the mouse_screen_coords belongs to.
   */
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

  void SetBoxAsMine(size_t i, size_t j);

  /**
   * Opens all the boxes around a box.
   * @param i The row the box to open around belongs to.
   * @param j The col the box to open around belongs to.
   */
  void OpenBoxesAround(size_t i, size_t j);

  /**
   * Returns the number of flags around a box.
   * @param i The row the box to count the flags around belongs to.
   * @param j The col the box to count the flags around belongs to.
   * @return The number of flags around the box.
   */
  size_t FlagsAroundBox(size_t i, size_t j);

  /**
 * Count all the mines around each box and set the box value.
 */
  void SetAllBoxValues();

  /**
   * Sets the mines and box values.
   * @param i The row position of the starting box.
   * @param j The col position of the starting box.
   */
  void SetUpField(size_t i, size_t j);

  /**
   * Sets all boxes' boxes around.
   */
  void SetBoxesAround();

 private:
  std::vector<std::vector<Box>> board_;
  glm::vec2 top_left_corner_;
  size_t num_rows_;
  size_t num_cols_;
  size_t num_mines_;
  size_t num_correct_unopened_;
  /** Number of screen pixels in the width/height of one box*/
  double pixel_side_length_;
  bool game_over_;
  bool win_;

  /**
   * Set all the mines in the field.
   * @param i The row position of the starting box.
   * @param j The col position of the starting box.
   */
  void SetMines(size_t i, size_t j);
};

}  // namespace visualizer

}  // namespace minesweeper
