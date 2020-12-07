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

class Field {
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

  /**
   * The game is reset when the restart button is hit.
   */
  void RestartGame();
  /**
   * Given the mouse's coordinates, it returns a boolean representing
   * whether or not the restart button is touched by the mouse.
   * @param mouse_screen_coords The mouse's screen coordinates
   * @return Whether or not the restart button is touched by the mouse.
   */
  const bool IsRestartButtonHit(const glm::vec2& mouse_screen_coords) const;

  Container GetRestartButton();


 private:
  std::vector<std::vector<Box>> board_;
  glm::vec2 top_left_corner_;
  size_t num_rows_;
  size_t num_cols_;
  size_t num_mines_;
  // Number of screen pixels in the width/height of one box
  double pixel_side_length_;

  size_t num_correct_unopened_;
  size_t num_flagged_;
  bool game_over_;
  bool win_;
  Container restart_button_;
  Container timer_;
  ci::Timer cinder_timer_;
  Container mines_left_;

  ci::gl::Texture2dRef closed_box_image_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/closed_box.png")));
  ci::gl::Texture2dRef restart_button_reg_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/restart_reg.png")));
  ci::gl::Texture2dRef restart_button_l_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/restart_l.png")));
  ci::gl::Texture2dRef restart_button_w_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/restart_w.png")));
  ci::gl::Texture2dRef flag_image_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/flag.png")));
  ci::gl::Texture2dRef mine_reg_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/mine_reg.png")));
  ci::gl::Texture2dRef mine_red_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/mine_red.png")));

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
