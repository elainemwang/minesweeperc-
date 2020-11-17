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
void MinesweeperApp::mouseDown(ci::app::MouseEvent event) {
  glm::vec2 field_pos = field_.BoxRowColFromMousePos(event.getPos());
  field_.OpenBox(field_pos.x,field_pos.y);
}

void MinesweeperApp::mouseMove(ci::app::MouseEvent event) {
  mouse_pos_ = event.getPos();
}

void MinesweeperApp::keyDown(ci::app::KeyEvent event) {
  glm::vec2 field_pos = field_.BoxRowColFromMousePos(mouse_pos_);
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_SPACE:
      // If the box is closed, flag it. If the box is open and
      // the number of flags around is equal to the value of the box,
      // open all the boxes around it.
      if (!field_.GetBoard()[field_pos.x][field_pos.y].IsOpen()) {
        if(field_.GetBoard()[field_pos.x][field_pos.y].IsFlagged()){
          field_.UnflagBox(field_pos.x, field_pos.y);
        }
        else{
          field_.FlagBox(field_pos.x, field_pos.y);
        }
      } else {
        // TODO: If the box is open and
        //      the number of flags around is equal to the value of the box,
        //      open all the boxes around it.
      }
  }
}

} //namespace visualizer
} //namespace minesweeper