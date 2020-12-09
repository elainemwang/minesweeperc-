//
// Created by Elaine Wang on 11/17/20.
//
#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "field.h"

namespace minesweeper {

namespace visualizer {

class MinesweeperApp : public ci::app::App {
 public:
  MinesweeperApp();

  const size_t kXWindowSizeEx = 750;
  const size_t kYWindowSizeEx = 400;

  const size_t kXWindowSizeIm = 400;
  const size_t kYWindowSizeIm = 400;

  const size_t kXWindowSizeBg = 225;
  const size_t kYWindowSizeBg = 225;

  const double kPixelsPerBox = 25;

  void draw() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;
  void mouseMove(ci::app::MouseEvent event) override;

  const double kYBorder_ = 2*kPixelsPerBox;


 private:
  Field field_;
  glm::vec2 mouse_pos_;
  bool game_start_ = false;

  Container restart_button_;
  Container timer_;
  Container mines_left_;

  Container bg_button_;
  Container im_button_;
  Container ex_button_;

  Mode mode_;

  ci::gl::Texture2dRef restart_button_reg_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/restart_reg.png")));
  ci::gl::Texture2dRef restart_button_l_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/restart_l.png")));
  ci::gl::Texture2dRef restart_button_w_ = ci::gl::Texture2d::create
      (ci::Surface(ci::loadImage("assets/restart_w.png")));

  /**
   * Given the mouse's coordinates and a button, it returns a
   * boolean representing whether or not the button is touched by the mouse.
   * @param mouse_screen_coords The mouse's screen coordinates
   * @param button The button to check
   * @return Whether or not the button is touched by the mouse.
   */
  const bool IsButtonHit(const glm::vec2& mouse_screen_coords, Container button) const;

  void SetContainerPositions(size_t x_window_size, size_t y_window_size);

  size_t YWindowSizeFromMode(Mode mode);

};
}
}