//
// Created by Elaine Wang on 11/16/20.
//

#include "core/box.h"

namespace minesweeper{
Box::Box() {
  value_ = 0;
  flagged_ = false;
  open_ = false;
  mine_ = false;
  wrong_opened_ = false;
}
const size_t Box::GetValue() const {
  return value_;
}
void Box::SetValue(size_t value){
  value_ = value;
}
const bool Box::IsFlagged() const {
  return flagged_;
}
const bool Box::IsOpen() const {
  return open_;
}
const bool Box::IsMine() const {
  return mine_;
}
void Box::SetMine() {
  mine_ = true;
}
void Box::Flag() {
  flagged_ = true;
}
void Box::Unflag() {
  flagged_ = false;
}
void Box::SetWrongOpened() {
  wrong_opened_ = true;
}
const bool Box::IsWrongOpened() {
  return wrong_opened_;
}
const std::vector<std::vector<size_t>>& Box::GetBoxesAround() const{
  return boxes_around_;
}

void Box::AddBoxAround(size_t i, size_t j) {
  boxes_around_.push_back(std::vector<size_t>{i,j});
}

bool Box::OpenAndCheckGameOver() {
  open_ = true;
  if(mine_){
    return true;
  }
  return false;
}

}