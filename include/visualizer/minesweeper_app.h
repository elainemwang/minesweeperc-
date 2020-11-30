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

  void draw() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;
  void mouseMove(ci::app::MouseEvent event) override;

  const double kXWindowSize = 700;
  const double kYWindowSize = 500;
  const size_t kNumCols = 28;
  const size_t kNumRows = 20;

 private:
  Field field_;
  glm::vec2 mouse_pos_;
  bool game_start_ = false;
};
}
}