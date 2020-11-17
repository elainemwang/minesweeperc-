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
}
const size_t Box::GetValue() const {
  return value_;
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
void Box::Flag() {
  flagged_ = true;
}
void Box::Unflag() {
  flagged_ = false;
}

const std::vector<std::vector<size_t>>& Box::GetBoxesAround() const{
  return boxes_around_;
}

bool Box::OpenAndCheckGameOver() {
  if(!flagged_){
    open_ = true;
  }
  if(open_ && mine_){
    return true;
  }
  return false;
}

}