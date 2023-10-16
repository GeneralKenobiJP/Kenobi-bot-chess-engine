# Kenobi-bot-chess-engine
Chess engine in C++, with simple MAC OS chess app in C++ and using SFML
[Abandoned due to poor performance]

There is some minor mistake in implementing the chess engine, which makes perft results not always correct, but it works reasonably well.
I've implemented most of the basic Search function that is the heart of a chess engine. It could be, however, greatly optimized.
This is the biggest project I had ever done (as of October 2023). I learnt a lot while working on it, it greatly increased my understaning of C++, compilers and linkers.
The code is messy, there are lots of commented out std::cout lines that should be erased, there is lots of code duplication.
Making all this mistakes, as well as engaging in an ambitious yet fun project, allowed me to progress as a coder, to see what should be avoided and what should be used.

If I were to retake the development of this project, I would have to introduce many optimizations that would give sense to this engine.
Further adjustments, which I did not get to yet, could also be implemented, as for now this is a really simple engine, lacking most of modern chess engine features.

My initial dream goal was to make a bot using this chess engine that coud be used on Lichess. Should I return to the project, I would aim for this goal to be achieved.
