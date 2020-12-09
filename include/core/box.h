//
// Created by Elaine Wang on 11/16/20.
//

#pragma once
#include <vector>

namespace minesweeper {
class Box {
 public:
  Box();
  const size_t GetValue() const;
  void SetValue(size_t value);
  const bool IsFlagged() const;
  void Flag();
  void Unflag();
  const bool IsMine() const;
  void SetMine();
  const bool IsOpen() const;
  const bool IsWrongOpened();
  void SetWrongOpened();
  const std::vector<std::vector<size_t>>& GetBoxesAround() const;
  /**
   * Add the position to boxes_around_.
   * @param i The row position to add.
   * @param j The col position to add.
   */
  void AddBoxAround(size_t i, size_t j);
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
  bool wrong_opened_;

  // the (row,col) of all the boxes around the current box
  std::vector<std::vector<size_t>> boxes_around_;
};
}  // namespace minesweeper
