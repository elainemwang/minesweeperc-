//
// Created by Elaine Wang on 11/16/20.
//
#pragma once

#include "cinder/gl/gl.h"
#include "core/box.h"
#include "cinder/Timer.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

namespace minesweeper {

namespace visualizer {

enum class Mode { kBeginner = 0, kIntermediate = 1, kExpert = 2};

struct Container {
  Container(glm::vec2 top_left, glm::vec2 bottom_right, ci::Color color)
      : top_left_(top_left), bottom_right_(bottom_right), color_(color) {
    bounding_box_ = ci::Rectf(top_left, bottom_right);
  }
  glm::vec2 top_left_;
  glm::vec2 bottom_right_;
  ci::Color color_;
  ci::Rectf bounding_box_;
};

class Field {
 public:
  Field(size_t num_rows, size_t num_cols, double width, size_t num_mines);
  Field(const glm::vec2& top_left_corner, Mode mode, double width);
  void Draw();
  const std::vector<std::vector<Box>> GetBoard() const;
  const bool IsGameOver() const;
  const bool IsGameWon() const;

  size_t num_flagged_;
  size_t num_mines_;
  ci::Timer cinder_timer_;

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

  /**
   * The game is reset when the restart button is hit.
   */
  void RestartGame(Mode mode);

 private:
  const size_t kNumColsEx = 30;
  const size_t kNumRowsEx = 16;
  const size_t kNumMinesEx = 99;

  const size_t kNumColsIm = 16;
  const size_t kNumRowsIm = 16;
  const size_t kNumMinesIm = 40;

  const size_t kNumColsBg = 9;
  const size_t kNumRowsBg = 9;
  const size_t kNumMinesBg = 10;

  std::vector<std::vector<Box>> board_;
  glm::vec2 top_left_corner_;
  size_t num_rows_;
  size_t num_cols_;
  // Number of screen pixels in the width/height of one box
  double pixel_side_length_;

  size_t num_correct_unopened_;
  bool game_over_;
  bool win_;

  //Textures
  ci::gl::Texture2dRef closed_box_image_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/closed_box.png")));
  ci::gl::Texture2dRef flag_image_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/flag.png")));
  ci::gl::Texture2dRef mine_reg_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/mine_reg.png")));
  ci::gl::Texture2dRef mine_red_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/mine_red.png")));
  ci::gl::Texture2dRef mine_x_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/mine_x.png")));

  //Numbers
  ci::gl::Texture2dRef zero_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/0.png")));
  ci::gl::Texture2dRef one_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/1.png")));
  ci::gl::Texture2dRef two_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/2.png")));
  ci::gl::Texture2dRef three_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/3.png")));
  ci::gl::Texture2dRef four_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/4.png")));
  ci::gl::Texture2dRef five_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/5.png")));
  ci::gl::Texture2dRef six_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/6.png")));
  ci::gl::Texture2dRef seven_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/7.png")));
  ci::gl::Texture2dRef eight_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/8.png")));

  /**
   * Set all the mines in the field.
   * @param i The row position of the starting box.
   * @param j The col position of the starting box.
   */
  void SetMines(size_t i, size_t j);
};

}  // namespace visualizer

}  // namespace minesweeper
