//
// Created by Elaine Wang on 11/16/20.
//

#pragma once
#include <vector>

namespace minesweeper{
class Box {
 public:
  Box();
  const size_t GetValue() const;
  const bool IsFlagged() const;
  void Flag();
  const bool IsMine() const;
  const bool IsOpen() const;
  /**
   * Opens the box if the box is not flagged
   * and returns true if the box is a bomb.
   */
  bool OpenAndCheckGameOver();
 private:
  size_t value_;
  bool flagged_;
  bool mine_;
  bool open_;
};
}
