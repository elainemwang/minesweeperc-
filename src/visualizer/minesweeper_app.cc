//
// Created by Elaine Wang on 11/17/20.
//

#include "visualizer/minesweeper_app.h"

namespace minesweeper {

namespace visualizer {

MinesweeperApp::MinesweeperApp()
    : field_(glm::vec2(0, 50), kNumRows, kNumCols, kXWindowSize,
             80) {  // TODO: make the num mines adjustable
  ci::app::setWindowSize((int)kXWindowSize, (int)kYWindowSize+(int)kTopBorder);
}
void MinesweeperApp::draw() {
  ci::Color8u background_color(255, 255, 255);
  ci::gl::clear(background_color);
  field_.Draw();
}
void MinesweeperApp::mouseDown(ci::app::MouseEvent event) {
  if(!field_.IsGameOver()) {
    if (event.getPos().y > kTopBorder) {
      glm::vec2 field_pos = field_.BoxRowColFromMousePos(event.getPos());
      if (!game_start_) {
        game_start_ = true;
        field_.SetUpField(field_pos.x, field_pos.y);
      }
      field_.OpenBox(field_pos.x, field_pos.y);
    }
  }
  if(field_.IsRestartButtonHit(event.getPos())){
    field_.RestartGame();
    game_start_ = false;
  }
}

void MinesweeperApp::mouseMove(ci::app::MouseEvent event) {
  if(!field_.IsGameOver()) {
    mouse_pos_ = event.getPos();
  }
}

void MinesweeperApp::keyDown(ci::app::KeyEvent event) {
  if(!field_.IsGameOver()) {
    glm::vec2 field_pos = field_.BoxRowColFromMousePos(mouse_pos_);
    switch (event.getCode()) {
      case ci::app::KeyEvent::KEY_SPACE:
        if (!field_.GetBoard()[field_pos.x][field_pos.y].IsOpen()) {
          // If the box is closed, flag/unflag it.
          if (field_.GetBoard()[field_pos.x][field_pos.y].IsFlagged()) {
            field_.UnflagBox(field_pos.x, field_pos.y);
          } else {
            field_.FlagBox(field_pos.x, field_pos.y);
          }
        } else {
          // If the box is open and
          // the number of flags around is equal to the value of the box,
          // open all the boxes around it.
          if (field_.FlagsAroundBox(field_pos.x, field_pos.y) ==
              field_.GetBoard()[field_pos.x][field_pos.y].GetValue()) {
            field_.OpenBoxesAround(field_pos.x, field_pos.y);
          }
        }
    }
  }
}

}  // namespace visualizer
}  // namespace minesweeper