//
// Created by Elaine Wang on 11/17/20.
//

#include "visualizer/minesweeper_app.h"

namespace minesweeper {

namespace visualizer {
MinesweeperApp::MinesweeperApp() : field_(glm::vec2(0,0), kNumRows, kNumCols,
                                              kXWindowSize){
  ci::app::setWindowSize((int)kXWindowSize, (int)kYWindowSize);
}
void MinesweeperApp::draw() {
  ci::Color8u background_color(255, 255, 255);  // light yellow
  ci::gl::clear(background_color);
  field_.Draw();
}

} //namespace visualizer
} //namespace minesweeper