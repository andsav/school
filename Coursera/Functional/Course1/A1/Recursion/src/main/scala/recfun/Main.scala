package recfun

object Main {
  def main(args: Array[String]) {
    println("Pascal's Triangle")
    for (row <- 0 to 10) {
      for (col <- 0 to row)
        print(pascal(col, row) + " ")
      println()
    }
  }

  /**
   * Exercise 1
   */
    def pascal(c: Int, r: Int): Int = c match {
      case 0 => 1
      case `r` => 1
      case _ => pascal(c-1, r-1) + pascal(c, r-1)
    }
  
  /**
   * Exercise 2
   */
    def balance(chars: List[Char]): Boolean = {
      def open(c: List[Char], count: Int): Boolean = {
        if(count == 0) balance(c)
        else if(c.isEmpty) false
        else if(c.head == '(') open(c.tail, count+1)
        else if(c.head == ')') open(c.tail, count-1)
        else open(c.tail, count)
      }

      if(chars.isEmpty) true
      else if(chars.head == ')') false
      else if(chars.head == '(') open(chars.tail, 1)
      else balance(chars.tail)
    }
  
  /**
   * Exercise 3
   */
    def countChange(money: Int, coins: List[Int]): Int = {
      if(money == 0) 1
      else if(coins.isEmpty || money < 0) 0
      else countChange(money - coins.head, coins) + countChange(money, coins.tail)
    }
  }
