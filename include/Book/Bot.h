#ifndef BOT_H
#define BOT_H

#include"chess.hpp"
#include<map>
// ababa -> 
//abc -> 
class Bot
{
public:
	chess::Board board;
	int MAXDEPTH;
	std::map<char, int> VALUE;
	int oo = 1239872;
	Bot(int depth = 1)
	{
		MAXDEPTH = depth;
		VALUE['p'] = 1;
		VALUE['P'] = 1;
		VALUE['N'] = 3;
		VALUE['n'] = 3;
		VALUE['B'] = 3;
		VALUE['b'] = 3;
		VALUE['r'] = 5;
		VALUE['R'] = 5;
		VALUE['Q'] = 10;
		VALUE['q'] = 10;
		VALUE['K'] = oo;
		VALUE['k'] = oo;
	}
	int evaluate()
	{
		std::string s = board.getFen();
		//std::cout << s << std::endl;
		int sum = 0;
		for (auto x : s)
		{
			if (x == ' ')break;
			if ('a' <= x && x <= 'z')sum -= VALUE[x];
			else if ('A' <= x && x <= 'Z')sum += VALUE[x];
		}
		return sum;
	}
	chess::Move bestmove;

	int minimax(int depth, int alpha, int beta, int maxiPlayer)
	{
		auto [resultReason, result] = board.isGameOver();
		if (result != chess::GameResult::NONE)
		{
			if (result == chess::GameResult::DRAW)return -5 * oo;
			if (maxiPlayer)return -5 * oo;
			return 5 * oo;
		}
		chess::Movelist Move_list;
		chess::movegen::legalmoves(Move_list, board);
		if (MAXDEPTH == 0)
		{
			int t = rand() % Move_list.size();
			std::cout << rand() << std::endl;
			bestmove = Move_list[t];
		}
		if (depth == 0)
		{
			return evaluate();
		}
		if (maxiPlayer)
		{
			int val = -5 * oo;
			for (auto move : Move_list)
			{
				board.makeMove(move);
				int eval = minimax(depth - 1, alpha, beta, 0);
				board.unmakeMove(move);
				if(depth==MAXDEPTH)
				{
					if(val<eval)
					{
						val = eval;  
						bestmove = move ; 
					}
					else if(val==eval)
					{
						if(rand()%4==0)
						{
							bestmove = move ; 
						}
					}
				}
				else if(val<eval)val=eval; 
				if (alpha < eval)alpha = eval;
				if (beta < alpha)
				{
					break;
				}
			}
			return val;
		}
		else
		{
			int val = 5 * oo;
			for (auto move : Move_list)
			{
				board.makeMove(move);
				int eval = minimax(depth - 1, alpha, beta, 1);
				board.unmakeMove(move);
				if(depth==MAXDEPTH)
				{
					if(val>eval)
					{
						val=eval;  
						bestmove = move ; 
					}
					else if(val==eval)
					{
						if(rand()%4==0)
						{
							bestmove = move ; 
						}
					}
				}
				else if(val>eval)val=eval; 
				if (beta > eval)beta = eval;
				if (beta < alpha)
				{
					break;
				}
			}
			return val;
		}
	}
	chess::Move move(chess::Board Input)
	{
		board = Input;
		int turn = 0;
		if (board.sideToMove() == chess::Color::WHITE)
			turn = 1;
		bestmove = chess::Move();
		//int val = minimax2(MAXDEPTH, turn); // minimax2(MAXDEPTH, turn
		int val = minimax(MAXDEPTH, -5 * oo, 5 * oo, turn);
		std::cout << val << std::endl;
		return bestmove;
	}
	~Bot()
	{
		// Destructor logic if needed
	}
};
// khai bao Bot VD : Bot A(5) 5 la do sau tinh toan 
// usage : A.move(Board X ) tra ve nuoc di tot nhat cho luot di tiep theo 
#endif // BOT_H