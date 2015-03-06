# Text-Based-Blackjack
Text-based blackjack with nifty ASCII graphics ;)
## Why?
I stumbled across ASCII box-drawing characters one day and started playing around with them. The second thing I made was a full deck of playing cards, then I figured that I could use them to make a Blackjack game for my computer science class, in which I was studying linked-lists.
## Programming
This Blackjack game was developed for linux in C++ using some of the standard libraries. The main focus of this exercise was to practice using linked lists. At the time of publication, the game does not have any memory leaks (that valgrind detected during testing).
## How to Play
Before you play this, you will need to compile the source code. This project is not very complicated, so I did not feel the need to include a make file. On most linux operating systems, you should be able to compile simply by typing the following into a terminal:
    make blackjack
Or if you do not have make installed, but are using g++ (which is what I used), just type into a terminal:
    g++ blackjack.cpp -o blackjack
Either option produces the same effect. I presume MAC OS X has a similar/the same procedure, but I am unsure about Windows, although I am sure that there is a g++ version for it.
## Use
Feel free to download, play with, or modify this source code however you want. This software is licensed under the MIT license, so just make sure to tack that license on if you publish this anywhere.
