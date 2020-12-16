//
// Created by Elaine Wang on 11/17/20.
//

#include "visualizer/minesweeper_app.h"

namespace minesweeper {

namespace visualizer {

MinesweeperApp::MinesweeperApp()
    : field_(glm::vec2(0, kYBorder), Mode::kExpert, kXWindowSizeEx),
      restart_button_(
          glm::vec2(0, kYBorder) +
              glm::vec2((kXWindowSizeEx / 2 - 0.75 * kPixelsPerBox), -45),
          1.5 * kPixelsPerBox, 1.5 * kPixelsPerBox, ci::Color("white")),
      timer_(glm::vec2(0, kYBorder) +
                 glm::vec2(kXWindowSizeEx - 3 * kPixelsPerBox, -40),
             2 * kPixelsPerBox, kPixelsPerBox, ci::Color("red")),
      mines_left_(glm::vec2(0, kYBorder) + glm::vec2(kPixelsPerBox, -40),
                  2 * kPixelsPerBox, kPixelsPerBox, ci::Color("red")),
      bg_button_(
          glm::vec2(0, kYBorder) +
              glm::vec2((kXWindowSizeEx / 5 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder - 60 - kPixelsPerBox),
          2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white")),
      im_button_(
          glm::vec2(0, kYBorder) +
              glm::vec2((2 * kXWindowSizeEx / 5 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder - 60 - kPixelsPerBox),
          2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white")),
      ex_button_(
          glm::vec2(0, kYBorder) +
              glm::vec2((3 * kXWindowSizeEx / 5 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder - 60 - kPixelsPerBox),
          2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white")),
      scores_button_(
          glm::vec2(0, kYBorder) +
              glm::vec2((4 * kXWindowSizeEx / 5 - kPixelsPerBox),
                        kYWindowSizeEx + 2 * kYBorder - 60 - kPixelsPerBox),
          2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white")),
      mode_(Mode::kExpert),
      score_mode_(Mode::kExpert) {
  ci::app::setWindowSize((int)kXWindowSizeEx,
                         (int)kYWindowSizeEx + 2 * kYBorder);
  SetContainerPositions(kXWindowSizeEx, kYWindowSizeEx);
}
void MinesweeperApp::draw() {
  ci::Color8u background_color(190, 190, 190);  // light grey
  ci::gl::clear(background_color);
  if (!scores_page_) {
    field_.Draw();
  } else {
    scores_.DrawMode(score_mode_, glm::vec2(kXWindowSizeIm / 4, kYBorder));
  }
  // Restart game button
  ci::gl::color(ci::Color("white"));
  if (!field_.IsGameOver()) {
    ci::gl::draw(restart_button_reg_, restart_button_.bounding_box_);
  } else {
    if (!field_.IsGameWon()) {
      ci::gl::draw(restart_button_l_, restart_button_.bounding_box_);
    } else {
      ci::gl::draw(restart_button_w_, restart_button_.bounding_box_);
      if (scores_.CheckScore(mode_, field_.cinder_timer_.getSeconds()) &&
          !score_added_) {
        scores_.InsertScore(mode_, field_.cinder_timer_.getSeconds());
        score_added_ = true;
      }
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

  ci::gl::drawSolidRect(scores_button_.bounding_box_);
  if (!scores_page_) {
    ci::gl::drawStringCentered(
        "Scores",
        scores_button_.top_left_ + glm::vec2(kPixelsPerBox, kPixelsPerBox / 3),
        scores_button_.color_, ci::Font("Helvetica Neue", kPixelsPerBox / 2));
  } else {
    ci::gl::drawStringCentered(
        "Back",
        scores_button_.top_left_ + glm::vec2(kPixelsPerBox, kPixelsPerBox / 3),
        scores_button_.color_, ci::Font("Helvetica Neue", kPixelsPerBox / 2));
  }
}
void MinesweeperApp::mouseDown(ci::app::MouseEvent event) {
  if (!scores_page_) {
    if (!field_.IsGameOver()) {
      if (event.getPos().y > kYBorder &&
          event.getPos().y < YWindowSizeFromMode(mode_) + kYBorder) {
        glm::vec2 field_pos = field_.BoxRowColFromMousePos(event.getPos());
        if (!game_start_) {
          game_start_ = true;
          field_.SetUpField(field_pos.x, field_pos.y);
          score_added_ = false;
        }
        field_.OpenBox(field_pos.x, field_pos.y);
      }
    }
    if (IsButtonHit(event.getPos(), restart_button_)) {
      field_.RestartGame(mode_);
      game_start_ = false;
    }
  }

  if (IsButtonHit(event.getPos(), ex_button_)) {
    if (!scores_page_) {
      mode_ = Mode::kExpert;
      ci::app::setWindowSize((int)kXWindowSizeEx,
                             (int)kYWindowSizeEx + 2 * kYBorder);
      field_.RestartGame(Mode::kExpert);
      game_start_ = false;
      SetContainerPositions(kXWindowSizeEx, kYWindowSizeEx);
    } else {
      score_mode_ = Mode::kExpert;
    }
  } else if (IsButtonHit(event.getPos(), im_button_)) {
    if (!scores_page_) {
      mode_ = Mode::kIntermediate;
      ci::app::setWindowSize((int)kXWindowSizeIm,
                             (int)kYWindowSizeIm + 2 * kYBorder);
      field_.RestartGame(Mode::kIntermediate);
      game_start_ = false;
      SetContainerPositions(kXWindowSizeIm, kYWindowSizeIm);
    } else {
      score_mode_ = Mode::kIntermediate;
    }
  } else if (IsButtonHit(event.getPos(), bg_button_)) {
    if (!scores_page_) {
      mode_ = Mode::kBeginner;
      ci::app::setWindowSize((int)kXWindowSizeBg,
                             (int)kYWindowSizeBg + 2 * kYBorder);
      field_.RestartGame(Mode::kBeginner);
      game_start_ = false;
      SetContainerPositions(kXWindowSizeBg, kYWindowSizeBg);
    } else {
      score_mode_ = Mode::kBeginner;
    }
  } else if (IsButtonHit(event.getPos(), scores_button_)) {
    if (scores_page_) {
      field_.cinder_timer_.resume();
      ci::app::setWindowSize(XWindowSizeFromMode(mode_),
                             YWindowSizeFromMode(mode_) + 2 * kYBorder);
      SetContainerPositions(XWindowSizeFromMode(mode_),
                            YWindowSizeFromMode(mode_));
      scores_page_ = false;
    } else {
      field_.cinder_timer_.stop();
      ci::app::setWindowSize((int)kXWindowSizeIm,
                             (int)kYWindowSizeIm + 2 * kYBorder);
      SetContainerPositions(kXWindowSizeIm, kYWindowSizeIm);
      scores_page_ = true;
    }
  }
}

void MinesweeperApp::mouseMove(ci::app::MouseEvent event) {
  if (!field_.IsGameOver()) {
    mouse_pos_ = event.getPos();
  }
}

void MinesweeperApp::keyDown(ci::app::KeyEvent event) {
  if (!field_.IsGameOver() && !scores_page_) {
    if (mouse_pos_.y > kYBorder &&
        mouse_pos_.y < YWindowSizeFromMode(mode_) + kYBorder) {
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

void MinesweeperApp::SetContainerPositions(size_t x_window_size,
                                           size_t y_window_size) {
  restart_button_ =
      Container(glm::vec2(0, kYBorder) +
                    glm::vec2((x_window_size / 2 - 0.75 * kPixelsPerBox), -45),
                1.5 * kPixelsPerBox, 1.5 * kPixelsPerBox, ci::Color("white"));
  timer_ = Container(glm::vec2(0, kYBorder) +
                         glm::vec2(x_window_size - 3 * kPixelsPerBox, -40),
                     2 * kPixelsPerBox, kPixelsPerBox, ci::Color("red"));
  mines_left_ =
      Container(glm::vec2(0, kYBorder) + glm::vec2(kPixelsPerBox, -40),
                2 * kPixelsPerBox, kPixelsPerBox, ci::Color("red"));
  bg_button_ = Container(
      glm::vec2(0, kYBorder) +
          glm::vec2((x_window_size / 5 - kPixelsPerBox),
                    y_window_size + 2 * kYBorder - 60 - kPixelsPerBox),
      2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white"));
  im_button_ = Container(
      glm::vec2(0, kYBorder) +
          glm::vec2((2 * x_window_size / 5 - kPixelsPerBox),
                    y_window_size + 2 * kYBorder - 60 - kPixelsPerBox),
      2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white"));
  ex_button_ = Container(
      glm::vec2(0, kYBorder) +
          glm::vec2((3 * x_window_size / 5 - kPixelsPerBox),
                    y_window_size + 2 * kYBorder - 60 - kPixelsPerBox),
      2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white"));
  scores_button_ = Container(
      glm::vec2(0, kYBorder) +
          glm::vec2((4 * x_window_size / 5 - kPixelsPerBox),
                    y_window_size + 2 * kYBorder - 60 - kPixelsPerBox),
      2 * kPixelsPerBox, kPixelsPerBox, ci::Color("white"));
}

size_t MinesweeperApp::YWindowSizeFromMode(Mode mode) {
  if (mode == Mode::kBeginner) {
    return kYWindowSizeBg;
  } else if (mode == Mode::kIntermediate) {
    return kYWindowSizeIm;
  } else {
    return kYWindowSizeEx;
  }
}

size_t MinesweeperApp::XWindowSizeFromMode(Mode mode) {
  if (mode == Mode::kBeginner) {
    return kXWindowSizeBg;
  } else if (mode == Mode::kIntermediate) {
    return kXWindowSizeIm;
  } else {
    return kXWindowSizeEx;
  }
}

}  // namespace visualizer
}  // namespace minesweeper