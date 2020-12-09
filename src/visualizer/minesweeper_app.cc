//
// Created by Elaine Wang on 11/17/20.
//

#include "visualizer/minesweeper_app.h"

namespace minesweeper {

namespace visualizer {

MinesweeperApp::MinesweeperApp()
    : field_(glm::vec2(0, kYBorder_), Mode::kExpert, kXWindowSizeEx),
      restart_button_(
          glm::vec2(0, kYBorder_) +
              glm::vec2((kXWindowSizeEx / 2 - 0.75 * kPixelsPerBox), -45),
          glm::vec2(0, kYBorder_) +
              glm::vec2((kXWindowSizeEx / 2 - 0.75 * kPixelsPerBox), -45) +
              glm::vec2(1.5 * kPixelsPerBox, 1.5 * kPixelsPerBox),
          ci::Color("white")),
      timer_(glm::vec2(0, kYBorder_) +
                 glm::vec2(kXWindowSizeEx - 3 * kPixelsPerBox, -40),
             glm::vec2(0, kYBorder_) +
                 glm::vec2(kXWindowSizeEx - 3 * kPixelsPerBox, -40) +
                 glm::vec2(2 * kPixelsPerBox, kPixelsPerBox),
             ci::Color("red")),
      mines_left_(glm::vec2(0, kYBorder_) + glm::vec2(kPixelsPerBox, -40),
                  glm::vec2(0, kYBorder_) + glm::vec2(kPixelsPerBox, -40) +
                      glm::vec2(2 * kPixelsPerBox, kPixelsPerBox),
                  ci::Color("red")),
      bg_button_(
          glm::vec2(0, kYBorder_) +
              glm::vec2((kXWindowSizeEx / 4 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder_ - 60 - kPixelsPerBox),
          glm::vec2(0, kYBorder_) +
              glm::vec2((kXWindowSizeEx / 4 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder_ - 60 - kPixelsPerBox) +
              glm::vec2(2 * kPixelsPerBox, kPixelsPerBox),
          ci::Color("white")),
      im_button_(
          glm::vec2(0, kYBorder_) +
              glm::vec2((kXWindowSizeEx / 2 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder_ - 60 - kPixelsPerBox),
          glm::vec2(0, kYBorder_) +
              glm::vec2((kXWindowSizeEx / 2 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder_ - 60 - kPixelsPerBox) +
              glm::vec2(2 * kPixelsPerBox, kPixelsPerBox),
          ci::Color("white")),
      ex_button_(
          glm::vec2(0, kYBorder_) +
              glm::vec2((3 * kXWindowSizeEx / 4 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder_ - 60 - kPixelsPerBox),
          glm::vec2(0, kYBorder_) +
              glm::vec2((3 * kXWindowSizeEx / 4 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder_ - 60 - kPixelsPerBox) +
              glm::vec2(2 * kPixelsPerBox, kPixelsPerBox),
          ci::Color("white")),
      mode_(Mode::kExpert) {
  ci::app::setWindowSize((int)kXWindowSizeEx,
                         (int)kYWindowSizeEx + 2 * kYBorder_);
}
void MinesweeperApp::draw() {
  ci::Color8u background_color(190, 190, 190);
  ci::gl::clear(background_color);
  field_.Draw();

  // Restart game button
  ci::gl::color(ci::Color("white"));
  if (!field_.IsGameOver()) {
    ci::gl::draw(restart_button_reg_, restart_button_.bounding_box_);
  } else {
    if (!field_.IsGameWon()) {
      ci::gl::draw(restart_button_l_, restart_button_.bounding_box_);
    } else {
      ci::gl::draw(restart_button_w_, restart_button_.bounding_box_);
    }
  }

  // Timer
  ci::gl::color(ci::Color("black"));
  ci::gl::drawSolidRect(timer_.bounding_box_);
  if (!field_.cinder_timer_.isStopped() &&
      field_.cinder_timer_.getSeconds() >= 999) {
    field_.cinder_timer_.stop();
  }
  ci::gl::drawStringCentered(
      std::to_string((int)field_.cinder_timer_.getSeconds()),
      timer_.top_left_ + glm::vec2(kPixelsPerBox, kPixelsPerBox / 5),
      timer_.color_, ci::Font("Helvetica Neue", kPixelsPerBox - 5));

  // Number of unflagged mines left
  ci::gl::drawSolidRect(mines_left_.bounding_box_);
  ci::gl::drawStringCentered(
      std::to_string(field_.num_mines_ - field_.num_flagged_),
      mines_left_.top_left_ + glm::vec2(kPixelsPerBox, kPixelsPerBox / 5),
      mines_left_.color_, ci::Font("Helvetica Neue", kPixelsPerBox - 5));

  // Mode buttons
  ci::gl::drawSolidRect(ex_button_.bounding_box_);
  ci::gl::drawStringCentered(
      "EX", ex_button_.top_left_ + glm::vec2(kPixelsPerBox, kPixelsPerBox / 5),
      ex_button_.color_, ci::Font("Helvetica Neue", kPixelsPerBox - 5));

  ci::gl::drawSolidRect(im_button_.bounding_box_);
  ci::gl::drawStringCentered(
      "IM", im_button_.top_left_ + glm::vec2(kPixelsPerBox, kPixelsPerBox / 5),
      im_button_.color_, ci::Font("Helvetica Neue", kPixelsPerBox - 5));

  ci::gl::drawSolidRect(bg_button_.bounding_box_);
  ci::gl::drawStringCentered(
      "BG", bg_button_.top_left_ + glm::vec2(kPixelsPerBox, kPixelsPerBox / 5),
      bg_button_.color_, ci::Font("Helvetica Neue", kPixelsPerBox - 5));
}
void MinesweeperApp::mouseDown(ci::app::MouseEvent event) {
  if (!field_.IsGameOver()) {
    if (event.getPos().y > kYBorder_ &&
        event.getPos().y < kYWindowSizeEx + kYBorder_) {
      glm::vec2 field_pos = field_.BoxRowColFromMousePos(event.getPos());
      if (!game_start_) {
        game_start_ = true;
        field_.SetUpField(field_pos.x, field_pos.y);
      }
      field_.OpenBox(field_pos.x, field_pos.y);
    }
  }
  if (IsButtonHit(event.getPos(), restart_button_)) {
    field_.RestartGame(mode_);
    game_start_ = false;
  } else if (IsButtonHit(event.getPos(), ex_button_)) {
    mode_ = Mode::kExpert;
    field_.RestartGame(Mode::kExpert);
    game_start_ = false;
    ci::app::setWindowSize((int)kXWindowSizeEx,
                           (int)kYWindowSizeEx + 2 * kYBorder_);
  } else if (IsButtonHit(event.getPos(), im_button_)) {
    mode_ = Mode::kIntermediate;
    field_.RestartGame(Mode::kIntermediate);
    game_start_ = false;
    ci::app::setWindowSize((int)kXWindowSizeIm,
                           (int)kYWindowSizeIm + 2 * kYBorder_);
  } else if (IsButtonHit(event.getPos(), bg_button_)) {
    mode_ = Mode::kBeginner;
    field_.RestartGame(Mode::kBeginner);
    game_start_ = false;
    ci::app::setWindowSize((int)kXWindowSizeBg,
                           (int)kYWindowSizeBg + 2 * kYBorder_);
  }
}

void MinesweeperApp::mouseMove(ci::app::MouseEvent event) {
  if (!field_.IsGameOver()) {
    mouse_pos_ = event.getPos();
  }
}

void MinesweeperApp::keyDown(ci::app::KeyEvent event) {
  if (!field_.IsGameOver()) {
    if (mouse_pos_.y > kYBorder_) {
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
}

const bool MinesweeperApp::IsButtonHit(const glm::vec2& mouse_screen_coords,
                                       Container button) const {
  return ((mouse_screen_coords.x > button.top_left_.x &&
           mouse_screen_coords.x < button.bottom_right_.x) &&
          (mouse_screen_coords.y > button.top_left_.y &&
           mouse_screen_coords.y < button.bottom_right_.y));
}

}  // namespace visualizer
}  // namespace minesweeper