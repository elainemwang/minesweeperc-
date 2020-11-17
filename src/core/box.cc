//
// Created by Elaine Wang on 11/16/20.
//

#include "core/box.h"

namespace minesweeper{
Box::Box() {
  value_ = 0;
  flagged_ = false;
  open_ = true;
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
}